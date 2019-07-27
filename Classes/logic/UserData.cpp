#include "UserData.h"


UserData::UserData()
{
	////CCLog("UserData Constructed");
    
    
}

UserData::~UserData()
{
	////CCLog("UserData Destroyed");
}

UserData* UserData::s_sharedUserData = NULL;

UserData* UserData::shared(void)
{
	if(!s_sharedUserData)
	{
		s_sharedUserData = new UserData();
		s_sharedUserData->init();
	}

	return s_sharedUserData;
}

void UserData::init()
{
	this->checkVersion();
    
    //内存数据初始化
    m_heroHp = 0.0f;
    
    m_nReviveTimes = 0;
	//load();
}

void UserData::check()
{
	m_sUserDataPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "userdata_out.xml";
    
    CCLog("%s",m_sUserDataPath.c_str());

	bool exsit = isFileExist(m_sUserDataPath.c_str());

	if(!exsit)
	{
		copyData("common/userdata.xml", "userdata_out.xml");
	}
}

void UserData::checkVersion()
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLongth = 0;

	char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLongth);
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

void UserData::loadDataFromVer(int version)
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

void UserData::load()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //UserLevel
    TiXmlElement *elmUserLevel = elmRoot->FirstChildElement();
    
    //UserLevel 体力
    TiXmlElement *elmCursor = elmUserLevel->FirstChildElement();
    CCLog("Element Name %s", elmCursor->FirstChild()->Value());
    this->setEnergyNum(atoi(elmCursor->FirstChild()->Value()));
    
    
    //UserLevel 金币
    elmCursor = elmCursor->NextSiblingElement();
    CCLog("Element Name %s", elmCursor->FirstChild()->Value());
    this->setGoldNum(atoi(elmCursor->FirstChild()->Value()));
    CCLog("--load--GoldNum----(%d)------------------------------------------------------------------------------------", this->getGoldNum());
    
    //UserLevel 水晶
    elmCursor = elmCursor->NextSiblingElement();
    CCLog("Element Name %s", elmCursor->FirstChild()->Value());
    this->setGemNum(atoi(elmCursor->FirstChild()->Value()));
    
    //UserLevel 主炮
    elmCursor = elmCursor->NextSiblingElement();
    CCLog("Element Name %s", elmCursor->FirstChild()->Value());
    this->setSelectedMainGun(atoi(elmCursor->FirstChild()->Value()));
    
    //UserLevel 左副炮
    elmCursor = elmCursor->NextSiblingElement();
    CCLog("Element Name %s", elmCursor->FirstChild()->Value());
    this->setSelectedLeftGun(atoi(elmCursor->FirstChild()->Value()));
    
    //UserLevel 右副炮
    elmCursor = elmCursor->NextSiblingElement();
    CCLog("Element Name %s", elmCursor->FirstChild()->Value());
    this->setSelectedRightGun(atoi(elmCursor->FirstChild()->Value()));
    
    //UserLevel 左僚机
    elmCursor = elmCursor->NextSiblingElement();
    CCLog("Element Name %s", elmCursor->FirstChild()->Value());
    this->setSelectedWing(atoi(elmCursor->FirstChild()->Value()));
    
    //UserLevel 道具
    elmCursor = elmCursor->NextSiblingElement();
    if (elmCursor)
    {
        this->setNoTool_1(atoi(elmCursor->Attribute("tool_1")));
        this->setNoTool_2(atoi(elmCursor->Attribute("tool_2")));
        this->setNoTool_3(atoi(elmCursor->Attribute("tool_3")));
        this->setNoTool_4(atoi(elmCursor->Attribute("tool_4")));
        this->setNoTool_5(atoi(elmCursor->Attribute("tool_5")));
        this->setNoTool_6(atoi(elmCursor->Attribute("tool_6")));
    }
    
    //Equipment
    TiXmlElement *elmEquipment = elmUserLevel->NextSiblingElement();
    
    this->loadAllEquipments(elmEquipment);
    ////UserMission
    //TiXmlElement *elmUserMission = elmUserLevel->NextSiblingElement();
    //CCLog("----------------2-----");
    //elmCursor = elmUserMission->FirstChildElement();
    
    //while(elmCursor)
    //{
    //	UserDataMission data;
    //	data.loadData(elmCursor);
    
    //	//加入到map中
    //	user_missions.insert(MapUserDataMession::value_type(data.getID(), data));
    
    //	//下一个
    //	elmCursor = elmCursor->NextSiblingElement();
    //}
    //UserStatus
    TiXmlElement *elmUserStatus = elmEquipment->NextSiblingElement();
    
    //UserStatus 是否第一次
    elmCursor = elmUserStatus->FirstChildElement();
    this->setFirstTime(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 是否是第一次充值
    elmCursor = elmCursor->NextSiblingElement();
    this->setFirstShop(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 是否是第一次金币抽奖
    elmCursor = elmCursor->NextSiblingElement();
    this->setIsFirstLotteryUseCoin(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 是否是第一次10钻抽奖
    elmCursor = elmCursor->NextSiblingElement();
    this->setIsFirstLotteryUseTenCrystal(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 是否是第一次40钻抽奖
    elmCursor = elmCursor->NextSiblingElement();
    this->setIsFirstLotteryUseFourtyCrystal(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 上次金币抽奖免费的时间记录
    elmCursor = elmCursor->NextSiblingElement();
    if(strcmp(elmCursor->FirstChild()->Value(), "0") == 0)
    {
        this->setLastFreeTimeForCoin(getCurrentSystemTime());
    }
    else
    {
        this->setLastFreeTimeForCoin(atoi(elmCursor->FirstChild()->Value()));
    }
    
    //UserStatus 上次10钻抽奖免费的时间记录
    elmCursor = elmCursor->NextSiblingElement();
    if(strcmp(elmCursor->FirstChild()->Value(), "0") == 0)
    {
        this->setLastFreeTimeForTenCrystal(getCurrentSystemTime());
    }
    else
    {
        this->setLastFreeTimeForTenCrystal(atoi(elmCursor->FirstChild()->Value()));
    }
    
    //UserStatus 上次40钻抽奖免费的时间记录
    elmCursor = elmCursor->NextSiblingElement();
    if(strcmp(elmCursor->FirstChild()->Value(), "0") == 0)
    {
        this->setLastFreeTimeForFourtyCrystal(getCurrentSystemTime());
    }
    else
    {
        this->setLastFreeTimeForFourtyCrystal(atoi(elmCursor->FirstChild()->Value()));
    }
    
    
    //是否有未完成的申诉
    elmCursor = elmCursor->NextSiblingElement();
    this->setFinishComplain(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //时间记录 用于恢复体力
    elmCursor = elmCursor->NextSiblingElement();
    if (elmCursor)
    {
        this->setTimeRecord(atoi(elmCursor->FirstChild()->Value()));
    }
    
    //第一次登陆的时间记录 精确到毫秒
    elmCursor = elmCursor->NextSiblingElement();
    if (strcmp(elmCursor->FirstChild()->Value(), "0") == 0)
    {
        this->setFirstTimeRecord(getCurrentSystemTime());
    }
    else
    {
        this->setFirstTimeRecord(atoi(elmCursor->FirstChild()->Value()));
    }
    
    //第一次登录游戏的时间 精确到年月日
    elmCursor = elmCursor->NextSiblingElement();
    if(strcmp(elmCursor->Attribute("year"), "0") == 0)
    {
        this->setLastLoginYear(0);
    }
    else
    {
        this->setLastLoginYear(atoi(elmCursor->Attribute("year")));
    }
    
    if(strcmp(elmCursor->Attribute("mon"), "0") == 0)
    {
        this->setLastLoginMon(0);
    }
    else
    {
        this->setLastLoginMon(atoi(elmCursor->Attribute("mon")));
    }
    
    if(strcmp(elmCursor->Attribute("day"), "0") == 0)
    {
        this->setLastLoginDay(0);
    }
    else
    {
        this->setLastLoginDay(atoi(elmCursor->Attribute("day")));
    }
    
    //上次日常任务刷新的时间 精确到年月日
    elmCursor = elmCursor->NextSiblingElement();
    if(strcmp(elmCursor->Attribute("year"), "0") == 0)
    {
        this->setLastDailyRefreshYear(0);
    }
    else
    {
        this->setLastDailyRefreshYear(atoi(elmCursor->Attribute("year")));
    }
    
    if(strcmp(elmCursor->Attribute("mon"), "0") == 0)
    {
        this->setLastDailyRefreshMon(0);
    }
    else
    {
        this->setLastDailyRefreshMon(atoi(elmCursor->Attribute("mon")));
    }
    
    if(strcmp(elmCursor->Attribute("day"), "0") == 0)
    {
        this->setLastDailyRefreshDay(0);
    }
    else
    {
        this->setLastDailyRefreshDay(atoi(elmCursor->Attribute("day")));
    }
    
    //已签到的天数
    elmCursor = elmCursor->NextSiblingElement();
    this->setHadeSigned(atoi(elmCursor->FirstChild()->Value()));
    
    //当前使用的签到奖励编号
    elmCursor = elmCursor->NextSiblingElement();
    this->setCurSignReward(atoi(elmCursor->FirstChild()->Value()));
    
    //签到奖励的更新记录
    elmCursor = elmCursor->NextSiblingElement();
    this->setSignRewardUpdateRecord(atoi(elmCursor->FirstChild()->Value()));
    
    //UserSetting
    TiXmlElement *elmUserSetting = elmUserStatus->NextSiblingElement();
    //UserStatus 是否允许音效
    elmCursor = elmUserSetting->FirstChildElement();
    this->setSoundEnable(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 是否允许音乐
    elmCursor = elmCursor->NextSiblingElement();
    this->setMusicEnable(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 是否允许震动
    elmCursor = elmCursor->NextSiblingElement();
    this->setVibraEnable(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //UserStatus 是否开启特效
    elmCursor = elmCursor->NextSiblingElement();
    this->setParticleEnable(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    delete xmlDoc;
}

void UserData::save()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    elmRoot->SetAttribute("version", this->getVersion());//userdata版本
    
    //UserLevel
    TiXmlElement *elmUserLevel = elmRoot->FirstChildElement();
    
    //UserLevel 体力
    TiXmlElement *elmCursor = elmUserLevel->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getEnergyNum()).c_str());
    
    //UserLevel 金币
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getGoldNum()).c_str());
    
    //UserLevel 水晶
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getGemNum()).c_str());
    
    //UserLevel 主炮
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSelectedMainGun()).c_str());
    
    //UserLevel 左副炮
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSelectedLeftGun()).c_str());
    
    //UserLevel 右副炮
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSelectedRightGun()).c_str());
    
    //UserLevel 僚机
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSelectedWing()).c_str());
    
    //UserLevel 道具
    elmCursor = elmCursor->NextSiblingElement();
    if (elmCursor)
    {
        elmCursor->SetAttribute("tool_1", getStringFromInt(this->getNoTool_1()));
        elmCursor->SetAttribute("tool_2", getStringFromInt(this->getNoTool_2()));
        elmCursor->SetAttribute("tool_3", getStringFromInt(this->getNoTool_3()));
        elmCursor->SetAttribute("tool_4", getStringFromInt(this->getNoTool_4()));
        elmCursor->SetAttribute("tool_5", getStringFromInt(this->getNoTool_5()));
        elmCursor->SetAttribute("tool_6", getStringFromInt(this->getNoTool_6()));
    }
    
    //UserMission 设定关卡信息
    TiXmlElement *elmUserMission = elmUserLevel->NextSiblingElement();
    
    elmCursor = elmUserMission->FirstChildElement();
    
    while(elmCursor)
    {
        //获得数据
        int id = atoi(elmCursor->Attribute("id"));
        UserDataMission* data = getUserDataMissionByID(id);
        
        if(data)
        {
            elmCursor->SetAttribute("enable", data->getEnable() ? 1 : 0);
            elmCursor->SetAttribute("score", data->getScore());
            elmCursor->SetAttribute("grade", data->getGrade());
        }
        
        //下一个
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //UserStatus
    TiXmlElement *elmUserStatus = elmUserMission->NextSiblingElement();
    
    //UserStatus 是否第一次
    elmCursor = elmUserStatus->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getFirstTime() ? 1 : 0).c_str());
    
    //UserStatus 是否是第一次购买钻石
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getFirstShop() ? 1 : 0).c_str());
    
    //UserStatus 是否是第一次金币抽奖
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getIsFirstLotteryUseCoin() ? 1 : 0).c_str());
    
    //UserStatus 是否是第一次10钻抽奖
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getIsFirstLotteryUseTenCrystal() ? 1 : 0).c_str());
    
    //UserStatus 是否是第一次40钻抽奖
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getIsFirstLotteryUseFourtyCrystal() ? 1 : 0).c_str());
    
    //UserStatus 上次金币抽奖免费的时间
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getLastFreeTimeForCoin()));
    
    //UserStatus 上次10钻抽奖免费的时间
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getLastFreeTimeForTenCrystal()));
    
    //UserStatus 上次40钻抽奖免费的时间
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getLastFreeTimeForFourtyCrystal()));
    
    //申诉是否完成
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getFinishComplain() ? 1 : 0).c_str());
    
    //时间记录 用于恢复体力
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getTimeRecord()));
    
    //第一次登录的时间记录  精确到毫秒
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getFirstTimeRecord()));
    
    //第一次登录游戏的时间 精确到年月日
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->SetAttribute("year", getStringFromInt(this->getLastLoginYear()).c_str());
    elmCursor->SetAttribute("mon", getStringFromInt(this->getLastLoginMon()).c_str());
    elmCursor->SetAttribute("day", getStringFromInt(this->getLastLoginDay()).c_str());
    
    //上次日常任务刷新的时间 精确到年月日
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->SetAttribute("year", getStringFromInt(this->getLastDailyRefreshYear()).c_str());
    elmCursor->SetAttribute("mon", getStringFromInt(this->getLastDailyRefreshMon()).c_str());
    elmCursor->SetAttribute("day", getStringFromInt(this->getLastDailyRefreshDay()).c_str());
    
    //已签到的天数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getHadeSigned()));
    
    //当前使用的签到奖励表编号
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCurSignReward()));
    
    //签到奖励的更新记录
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSignRewardUpdateRecord()));
    
    //UserSetting
    TiXmlElement *elmUserSetting = elmUserStatus->NextSiblingElement();
    
    //UserStatus 是否允许音效
    elmCursor = elmUserSetting->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSoundEnable() ? 1 : 0).c_str());
    
    //UserStatus 是否允许音乐
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getMusicEnable() ? 1 : 0).c_str());
    
    //UserStatus 是否允许震动
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getVibraEnable() ? 1 : 0).c_str());
    
    //UserStatus 是否开启粒子特效
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getParticleEnable() ? 1 : 0).c_str());
    
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sUserDataPath);
    
    delete xmlDoc;
}

//增加体力
void UserData::increaseEnergy(int energy)
{
    this->m_nEnergy += energy;
}

void UserData::decreaseEnergy(int energy)
{
    this->m_nEnergy -= energy;
}


//钱是否够
bool UserData::isGoldEnough(int gold)
{
	if(this->getGoldNum() >= gold)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//增加钱
void UserData::increaseGoldNum(int gold)
{
	this->m_nGoldNum += gold;
}

//减少钱
void UserData::decreaseGoldNum(int gold)
{
	this->m_nGoldNum -= gold;
}

//水晶是否够
bool UserData::isGemEnough(int gem)
{
	if(this->getGemNum() >= gem)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//增加水晶
void UserData::increaseGemNum(int gem)
{
	this->m_nGemNum += gem;
}

void UserData::increaseGemNumByProductID(int product_id)
{
    int nGemNumBuyed = getGemNumByProductID(product_id);
    int nGemNumBonus = 0;
    
    bool bFirstShopping = UserData::shared()->getFirstShop();
    
    if(bFirstShopping)
    {
        nGemNumBonus = nGemNumBuyed;
    }
    
    UserData::shared()->setFirstShop(false);
    
    UserData::shared()->increaseGemNum(nGemNumBuyed + nGemNumBonus);
    
    UserData::shared()->save();
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    
    //统计钻石来源 ID为6
    //MobClickCpp::bonus(nGemNumBonus, ANALYTICS_GEM_GETBY_PAY);
    
    if (bFirstShopping)
    {
        //统计首次充值
        //MobClickCpp::event(AnalyticsStatID::getGemID(nGemNumBuyed));
    }
    
    //统计购买钻石
    //MobClickCpp::pay(getPriceByProductID(product_id),10,AnalyticsStatID::getGemID(nGemNumBuyed), 1, 0);
    
#endif

}

//减少水晶
void UserData::decreaseGemNum(int gem)
{
	this->m_nGemNum -= gem;
}

//获得当前关卡的是否解锁，分数和等级
UserDataMission* UserData::getUserDataMissionByID(int id)
{
	MapUserDataMession::iterator it = m_mapUserMissions.find(id);

	if(it == m_mapUserMissions.end())
	{
		return NULL;
	}
	else
	{
		return &it->second;
	}
}

//根据ID，设定关卡数据
void UserData::setUserDataMissionByID(int id, UserDataMission *&data)
{
	UserDataMission* targetData = getUserDataMissionByID(id);

	if(targetData)
	{
		targetData->setEnable(data->getEnable());
		targetData->setScore(data->getScore());
		targetData->setGrade(data->getGrade());
	}
}

//导入用户装备数据
void UserData::loadAllEquipments(TiXmlElement* elm)
{
    if(!elm)
    {
        CCLog("UserData::loadAllEquipments   elm is null");
        return;
    }
    
}

//更改使用中的装备
void UserData::setEquippedEquipmentsID(int equip_id, int new_id)
{
    if(equip_id == kTagMainGun)
    {
        this->setSelectedMainGun(new_id);
    }
    else if(equip_id == kTagSencodaryGun1)
    {
        this->setSelectedLeftGun(new_id);
    }
    else if(equip_id == kTagSencodaryGun2)
    {
        this->setSelectedRightGun(new_id);
    }
    else if(equip_id == kTagWing)
    {
        this->setSelectedWing(new_id);
    }
}

//增加道具
void UserData::increaseTool(int tool_id,int num)
{
    switch (tool_id)
    {
        case 1:
            m_tool_1 += num;
            break;
        
        case 2:
            m_tool_2 += num;
            break;
            
        case 3:
            m_tool_3 += num;
            break;
            
        case 4:
            m_tool_4 += num;
            break;
            
        case 5:
            m_tool_5 += num;
            break;
            
        case 6:
            m_tool_6 += num;
            break;
            
        default:
            break;
    }
}

//减少道具
void UserData::decreaseTool()
{
    
    //道具是在进入关卡中的时候 如果身上有这种道具  就会直接使用掉 逻辑暂定如下
    
    if (m_tool_1 > 0)
    {
        m_tool_1--;
    }
    
    if (m_tool_2 > 0)
    {
        m_tool_2--;
    }
    
    if (m_tool_3 > 0)
    {
        m_tool_3--;
    }
    
    if (m_tool_4 > 0)
    {
        m_tool_4--;
    }
    
    if (m_tool_5 > 0)
    {
        m_tool_5--;
    }
    
    if (m_tool_6 > 0)
    {
        m_tool_6--;
    }
}

//根据id获取道具的数量
int UserData::getToolNum(int tool_id)
{
    switch (tool_id)
    {
        case 1:
            return m_tool_1;
            break;
            
        case 2:
            return m_tool_2;
            break;
            
        case 3:
            return m_tool_3;
            break;
            
        case 4:
            return m_tool_4;
            break;
            
        case 5:
            return m_tool_5;
            break;
            
        case 6:
            return m_tool_6;
            break;
            
        default:
            break;
    }
    
    return 0;
}

/*************消费记录*****************/
/*
//保存消费记录
void UserData::savePurchraseHistory1(int price, int tradeId)
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;

	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
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

	//如果没有则创建
	if(!elmUserLevel)
	{
		TiXmlElement *pItemPurchraseHistory = new TiXmlElement("PurchraseHistory");
		elmRoot->LinkEndChild(pItemPurchraseHistory);

		elmUserLevel = pItemPurchraseHistory;
	}

	TiXmlElement *pItem = new TiXmlElement("Item");

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

	std::string strTime = "";
	strTime += convertIntToString(year);
	strTime += ".";
	strTime += convertIntToString(month);
	strTime += ".";
	strTime += convertIntToString(day);
	strTime += " ";
	strTime += convertIntToString(hour);
	strTime += ":";
	strTime += convertIntToString(min);
	strTime += ":";
	strTime += convertIntToString(second);


	pItem->SetAttribute("price", price);
	pItem->SetAttribute("tid", tradeId);
	pItem->SetAttribute("time", strTime);
	pItem->SetAttribute("finish", 0);

	elmUserLevel->LinkEndChild(pItem);

	//为加密做准备
	string strContent;
	xmlDoc->Serialize(strContent);

	CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sUserDataPath);
}

//检测未返还的消费记录下标并返回（检测到的第一条）
int UserData::getUnFinishedPurchraseIndex1(std::string& bid)
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;

	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
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
		bool bFinish = atoi(pItem->Attribute("finish")) == 0 ? false : true;
		bid = pItem->Attribute("tid");
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
int UserData::changePurchraseHistory1(std::string bid)
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;

	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
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
		if(pItem->Attribute("tid") == bid)
		{
			pItem->SetAttribute("finish", 1);
			nNum = atoi(pItem->Attribute("price"));
			break;
		}
		pItem = pItem->NextSiblingElement();
	}

	//为加密做准备
	string strContent;
	xmlDoc->Serialize(strContent);

	CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sUserDataPath);

	CCLog("save xml finished");

	return nNum;
}

//获取所有的消费信息
string UserData::getAllPurchraseHistory1()
{
	check();

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;

	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
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

	//如果没有则创建
	if(!elmUserLevel)
	{
		TiXmlElement *pItemPurchraseHistory = new TiXmlElement("PurchraseHistory");
		elmRoot->LinkEndChild(pItemPurchraseHistory);

		elmUserLevel = pItemPurchraseHistory;
	}

	//消费记录
	TiXmlElement* pItem = elmUserLevel->FirstChildElement();
	string strHistory = "gem=";
	strHistory += convertIntToString(this->getGemNum());
	strHistory += "&";
	strHistory += "paylog=";
	while (pItem)
	{
		strHistory += "{\"price\":\"";
		strHistory += pItem->Attribute("price");
		strHistory += "\",";

		strHistory += "\"tid\":\"";
		strHistory += pItem->Attribute("tid");
		strHistory += "\",";

		strHistory += "\"time\":\"";
		strHistory += pItem->Attribute("time");
		strHistory += "\",";

		strHistory += "\"finish\":\"";
		strHistory += pItem->Attribute("finish");
		strHistory += "\"},";

		pItem = pItem->NextSiblingElement();
	}

	return strHistory;
}*/
