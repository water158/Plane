#include "PurchaseLogManager.h"

PurchaseLogManagerItem* PurchaseLogManagerItem::create()
{
    PurchaseLogManagerItem *item = new PurchaseLogManagerItem();
	if(item)
	{
		item->autorelease();
		return item;
	}
    
	return NULL;
}

void PurchaseLogManagerItem::loadData(TiXmlElement *elm)
{
    //属性
	this->setTradeID(elm->Attribute(ATTRIBUTE_TRADE_ID));
    this->setProductID(atoi(elm->Attribute(ATTRIBUTE_PRODUCT_ID)));
    this->setTime(elm->Attribute(ATTRIBUTE_TIME));
    this->setRetryTimes(atoi(elm->Attribute(ATTRIBUTE_RETRY)));
    this->setFinished(strncmp(elm->Attribute(ATTRIBUTE_FINISH), "1", 1) == 0 ? true : false);
    this->setGemHaved(elm->Attribute(ATTRIBUTE_GEM_HAVED));
}

void PurchaseLogManagerItem::decreaseRetryTimes()
{
    m_nRetryTimes --;
}

PurchaseLogManager::PurchaseLogManager()
{
	////CCLog("PurchaseLogManager Constructed");
    
    m_dictItems = CCDictionary::create();
    m_dictItems->retain();
}

PurchaseLogManager::~PurchaseLogManager()
{
	////CCLog("PurchaseLogManager Destroyed");
    
    CC_SAFE_RELEASE(m_dictItems);
}

PurchaseLogManager* PurchaseLogManager::s_sharedPurchaseLogManager = NULL;

PurchaseLogManager* PurchaseLogManager::shared(void)
{
	if(!s_sharedPurchaseLogManager)
	{
		s_sharedPurchaseLogManager = new PurchaseLogManager();
		s_sharedPurchaseLogManager->init();
	}

	return s_sharedPurchaseLogManager;
}

void PurchaseLogManager::init()
{
	load();
}

void PurchaseLogManager::check()
{
	m_sPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "history_out.xml";
    
	bool exsit = isFileExist(m_sPath.c_str());
    
	if(!exsit)
	{
		copyData("common/history.xml", "history_out.xml");
	}
}

void PurchaseLogManager::load()
{
	check();
    
	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;
    
	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sPath.c_str(), "rt", &nLength);
	xmlDoc->LoadMemory(pBuff, nLength);
    
	//获得数据的根元素 <PurchaseLogManager>
	TiXmlElement *elmRoot = xmlDoc->RootElement();
    
	//Purchase History Item
	TiXmlElement *elmCursor = elmRoot->FirstChildElement();
    
    while(elmCursor)
	{
		PurchaseLogManagerItem *item = PurchaseLogManagerItem::create();
		item->loadData(elmCursor);
        
		//加入到array
        m_dictItems->setObject(item, item->getTradeID());
		
        //下一个
		elmCursor = elmCursor->NextSiblingElement();
	}
    
	delete xmlDoc;
}

void PurchaseLogManager::save()
{
    check();
    
	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;
    
	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sPath.c_str(), "rt", &nLength);
	xmlDoc->LoadMemory(pBuff, nLength);
    
	//获得数据的根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();
    elmRoot->Clear();
    
    CCDictElement *obj;
    CCDICT_FOREACH(m_dictItems, obj)
    {
        PurchaseLogManagerItem *pItemData = (PurchaseLogManagerItem *)obj->getObject();
        
        TiXmlElement *pItemXMLNode = new TiXmlElement("Item");
        
        //设定到<Item>
        pItemXMLNode->SetAttribute(ATTRIBUTE_TRADE_ID, pItemData->getTradeID());
        pItemXMLNode->SetAttribute(ATTRIBUTE_PRODUCT_ID, pItemData->getProductID());
        pItemXMLNode->SetAttribute(ATTRIBUTE_TIME, pItemData->getTime());
        pItemXMLNode->SetAttribute(ATTRIBUTE_RETRY, pItemData->getRetryTimes());
        pItemXMLNode->SetAttribute(ATTRIBUTE_FINISH, pItemData->getFinished() ? 1 : 0);
        pItemXMLNode->SetAttribute(ATTRIBUTE_GEM_HAVED, pItemData->getGemHaved());
        
        //将<Item>增加到<PurchaseLogManager>
        elmRoot->LinkEndChild(pItemXMLNode);
    }
    
    //为加密做准备
	string strContent;
	xmlDoc->Serialize(strContent);
    
	CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sPath);
    
	delete xmlDoc;
}

//增加消费记录
void PurchaseLogManager::insert(string tradeId, int product_id)
{
    if (m_dictItems->objectForKey(tradeId))
    {
        return;
    }
    
	//获取年月日时秒
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	timep = now.tv_sec;
#endif
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour=tm->tm_hour;
	int min=tm->tm_min;
	int second=tm->tm_sec;
    
    //格式化时间
	std::string strTime = "";
	strTime += getStringFromInt(year);
	strTime += ".";
	strTime += getStringFromInt(month);
	strTime += ".";
	strTime += getStringFromInt(day);
	strTime += " ";
	strTime += getStringFromInt(hour);
	strTime += ":";
	strTime += getStringFromInt(min);
	strTime += ":";
	strTime += getStringFromInt(second);
    
    PurchaseLogManagerItem *item = PurchaseLogManagerItem::create();
    
    //设定到<Item>
    item->setTradeID(tradeId);
	item->setProductID(getPriceByProductID(product_id));
    item->setTime(strTime);
    item->setRetryTimes(TOTAL_RETRY_TIMES);
    item->setFinished(false);
    item->setGemHaved(getStringFromInt(UserData::shared()->getGemNum()));
    
    
    //加入到array
    m_dictItems->setObject(item, item->getTradeID());
}

void PurchaseLogManager::changeToFinished(string tradeId)
{
    if (!m_dictItems->objectForKey(tradeId))
    {
        return;
    }
    
    PurchaseLogManagerItem *pItemData = (PurchaseLogManagerItem *)m_dictItems->objectForKey(tradeId);
    
    pItemData->setFinished(true);
}

int PurchaseLogManager::getProductID(string tradeId)
{
    if (!m_dictItems->objectForKey(tradeId))
    {
        return 0;
    }
    
    PurchaseLogManagerItem *pItemData = (PurchaseLogManagerItem *)m_dictItems->objectForKey(tradeId);
    
    return pItemData->getProductID();
}

void PurchaseLogManager::syncPurchase()
{
    CCDictElement *obj;
    CCDICT_FOREACH(m_dictItems, obj)
    {
        PurchaseLogManagerItem *pItemData = (PurchaseLogManagerItem *)obj->getObject();
        
        if (!pItemData->getFinished())
        {
            if (pItemData->getRetryTimes() > 0)
            {
                //verify
                VerificationManager::shared()->requestPurchase(pItemData->getTradeID());
                
                //CCFileUtils::sharedFileUtils()->writeToFile(NULL, "");
                
                pItemData->decreaseRetryTimes();
            }
        }
    }
}

//获取所有的消费信息
string PurchaseLogManager::toString()
{
    string strHistory = "gem=";
	strHistory += getStringFromInt(UserData::shared()->getGemNum());
	strHistory += "&";
	strHistory += "paylog=";
    
    CCDictElement *obj;
    CCDICT_FOREACH(m_dictItems, obj)
    {
        PurchaseLogManagerItem *pItemData = (PurchaseLogManagerItem *)obj->getObject();
        
        strHistory += "{";
        strHistory += "\"tid\":\"";
		strHistory += pItemData->getTradeID();
		strHistory += "\",";
        
        strHistory += "\"product\":\"";
		strHistory += pItemData->getProductID();
		strHistory += "\",";
        
		strHistory += "\"time\":\"";
		strHistory += pItemData->getTime();
		strHistory += "\",";
        
		strHistory += "\"finish\":\"";
		strHistory += pItemData->getFinished() ? "1" : "0";
		strHistory += "\",";
        
        strHistory += "\"haved\":\"";
		strHistory += pItemData->getGemHaved();
		strHistory += "\"}";
    }
    
	return strHistory;
}
/*


void PurchaseLogManager::checkVersion()
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLongth = 0;

	char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sPath.c_str(), "rt", &nLongth);
	xmlDoc->LoadMemory(pBuff, nLongth);

	//获取根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();

	if(elmRoot->Attribute("version"))
	{
		this->setVersion(atoi(elmRoot->Attribute("version")));
	}
	else
	{
		//如果不存在version属性，默认为1.0版本
		this->setVersion(1);
	}

	this->loadDataFromVer(m_nVersion);

	//CCLOG("version==%f", this->getVersion());

	delete xmlDoc;
}

void PurchaseLogManager::loadDataFromVer(int version)
{
	int ver = version - 1;

	switch(ver)
	{
	case USERDATA_VERSION_1:
		CCLOG("version 1");
		load();
		break;

	case USERDATA_VERSION_2:
		CCLOG("version 2");
		break;

	case USERDATA_VERSION_3:
		CCLOG("version 3");
		break;

	case USERDATA_VERSION_4:
		CCLOG("version 4");
		break;

	case USERDATA_VERSION_5:
		CCLOG("version 5");
		break;

	default:
		load();
		break;
	}
}
*/


/*





//检测未返还的消费记录下标并返回（检测到的第一条）
int PurchaseLogManager::getUnFinishedPurchraseIndex(std::string& bid)
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;

	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sPath.c_str(), "rt", &nLength);
	xmlDoc->LoadMemory(pBuff, nLength);

	//获得数据的根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();

	CCLog("getUnFinishedPurchraseIndex     1");
	//UserLevel
	TiXmlElement *elmUserLevel = elmRoot->FirstChildElement();

	//找到PurchraseHistory
	for (int i = 0; i < 4; i++)
	{
		elmUserLevel = elmUserLevel->NextSiblingElement();
	}
	CCLog("getUnFinishedPurchraseIndex     2");
	//消费记录
	//如果没有则创建
	if(!elmUserLevel)
	{
		TiXmlElement *pItemPurchraseHistory = new TiXmlElement("PurchraseHistory");
		elmRoot->LinkEndChild(pItemPurchraseHistory);

		elmUserLevel = pItemPurchraseHistory;
	}
	TiXmlElement* pItem = elmUserLevel->FirstChildElement();
	CCLog("getUnFinishedPurchraseIndex     3");
	//获取消费记录条数
	int nTotalNum = 0;
	TiXmlElement* pItem1 = pItem;
	while(pItem1)
	{
		nTotalNum ++;

		pItem1 = pItem1->NextSiblingElement();
	}
	CCLog("getUnFinishedPurchraseIndex     4");
	int nIndex = 0;

	while (pItem)
	{
		//查看返还状态，有未完成的就终止，返回下标
		bool bFinish = atoi(pItem->Attribute(ATTRIBUTE_FINISH)) == 0 ? false : true;
		bid = pItem->Attribute(ATTRIBUTE_TRADE_ID);
		if(!bFinish)
		{
			break;
		}
		nIndex ++;

		pItem = pItem->NextSiblingElement();
	}
	CCLog("getUnFinishedPurchraseIndex     5");
	//未到结尾就检测到了
	if(nIndex < nTotalNum)
	{
		CCLog("nIndex   =======     (%d)", nIndex);
		return nIndex;
	}

	//没有就返回-1
	return -1;
}

//完成返还后更改消费记录
int PurchaseLogManager::changePurchraseHistory(std::string bid)
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;

	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sPath.c_str(), "rt", &nLength);
	xmlDoc->LoadMemory(pBuff, nLength);

	//获得数据的根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();

	//UserLevel
	TiXmlElement *elmUserLevel = elmRoot->FirstChildElement();

	//找到PurchraseHistory
	for (int i = 0; i < 4; i++)
	{
		elmUserLevel = elmUserLevel->NextSiblingElement();
	}
	//消费记录
	TiXmlElement* pItem = elmUserLevel->FirstChildElement();
	int nNum = 0;
	while (pItem)
	{
		if(pItem->Attribute(ATTRIBUTE_TRADE_ID) == bid)
		{
			pItem->SetAttribute(ATTRIBUTE_FINISH, 1);
			nNum = atoi(pItem->Attribute(ATTRIBUTE_PRODUCT_ID));
			break;
		}
		pItem = pItem->NextSiblingElement();
	}

	//为加密做准备
	string strContent;
	xmlDoc->Serialize(strContent);

	CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sPath);

	CCLog("save xml finished");

	return nNum;
}


*/