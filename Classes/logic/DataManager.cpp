#include "DataManager.h"

using namespace std;
USING_NS_CC;


DataManager::DataManager()
{

}

DataManager::~DataManager()
{
    CC_SAFE_DELETE(m_dicTarplaneData);
    
    CC_SAFE_DELETE(m_dicTarConnonData);
    
    CC_SAFE_DELETE(m_dicTarToolsData);
}

DataManager* DataManager::m_pSharedDataManager = NULL;

DataManager* DataManager::sharedDataManager()
{
	if(!m_pSharedDataManager)
	{
		m_pSharedDataManager = new DataManager();
		m_pSharedDataManager->init();
	}

	return m_pSharedDataManager;
}

//初始化
bool DataManager::init()
{
    m_dicTarplaneData = CCDictionary::create();
    
    m_dicTarplaneData->retain();
    
    m_dicTarConnonData = CCDictionary::create();
    
    m_dicTarConnonData->retain();
    
    m_dicTarToolsData = CCDictionary::create();
    
    m_dicTarToolsData->retain();

	return true;
}

/**********加载区********/
//飞机数据（主机、敌机、炮台、金币、道具）
void DataManager::loadCraftData(int data_type, int craft_id)
{
	string strPath = "";
	string strName = "";
	//主机数据
	if(data_type == CRAFT_DATA_TYPE_HERO)
	{
		strName = "hero";
        
        strPath.append("game/")
		.append(strName)
		.append("/")
		.append(strName)
		.append("_")
		.append(getStringFromInt(craft_id))
		.append(".xml");
	}//僚机数据
    else if(data_type == CRAFT_DATA_TYPE_WING)
    {
        strName = "wing";
        
        
        strPath.append("game/")
        .append(strName)
        .append("/")
        .append(strName)
        .append(".xml");
    }
	//敌机数据
	else if(data_type == CRAFT_DATA_TYPE_ENEMY)
	{
		strName = "enemy";
        
        
        strPath.append("game/")
		.append(strName)
		.append("/")
		.append(strName)
		.append("_")
		.append(getStringFromInt(craft_id))
		.append(".xml");
	}
    //炮台数据
    else if (data_type == CRAFT_DATA_TYPE_CONNON)
    {
        strName = "cannon";

        strPath.append("game/")
		.append("enemy")
		.append("/")
		.append(strName)
		.append("_")
		.append(getStringFromInt(craft_id))
		.append(".xml");
    }else if (data_type == CRAFT_DATA_TYPE_DROP)
    {
        strName = "drops";
        
        strPath.append("game/")
        .append(strName)
        .append("/")
        .append(strName)
        .append("_")
        .append(getStringFromInt(craft_id))
        .append(".xml");
    }
	

	CCLog("-----load---------%s-----", strPath.c_str());
	//已经导入了就直接返回
	if ((CraftData *)gCache->getCraftDataInCache(strPath))
	{
		return;
	}

	CraftData* data = NULL;

	unsigned long nLength = 0;
	char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"rt", &nLength);

	////CCLog("CraftData %s", pBuff);

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory(pBuff, nLength);

	//获得根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();

	if (elmRoot)
	{
		data = CraftData::create(elmRoot);
		data->setType(data_type);
	}

	delete xmlDoc;

	gCache->addCraftDataToCache(strPath, data);

}
//加载子弹数据
void DataManager::loadAllBulletData()
{
    for (int bullet_id = 1; bullet_id <= 24; bullet_id++)
    {
        string strPath = "game/ammo/bullet_";
        strPath.append(getStringFromInt(bullet_id)).append(".xml");
        
        //已经导入了就直接返回
        if ((BulletData *)gCache->getBulletDataInCache(strPath))
        {
            return;
        }
        
        BulletData* data = NULL;
        
        unsigned long nLength = 0;
        char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"rt", &nLength);
        
        if (!pBuff)
        {
            continue;
        }
        
        //CCLog("bullet data %s",pBuff);
        
        TiXmlDocument *xmlDoc = new TiXmlDocument();
        xmlDoc->LoadMemory(pBuff, nLength);
        
        //获得根元素
        TiXmlElement *elmRoot = xmlDoc->RootElement();
        
        if (elmRoot)
        {
            data = BulletData::create(elmRoot);
            data->setID(getStringFromInt(bullet_id));
        }
        
        delete xmlDoc;
        
        gCache->addBulletDataToCache(strPath, data);
        CCLog("bullet id  == %d",bullet_id);
    }
}

//加载所有炮管数据
void DataManager::loadAllConnonData()
{
    
    
    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 15; j++)
        {
            string strPath = "";
            string strName = "";
            
            strName = "cannon";
            
            int craft_id = i * 100 + j;
            
            strPath.append("game/")
            .append("enemy")
            .append("/")
            .append(strName)
            .append("_")
            .append(getStringFromInt(craft_id))
            .append(".xml");
            
            //已经导入了就直接返回
            if ((CraftData *)gCache->getCraftDataInCache(strPath))
            {
                return;
            }
            
            CraftData* data = NULL;
            
            unsigned long nLength = 0;
            char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"rt", &nLength);
            
            if (!pBuff)
            {
                continue;
            }
            
            ////CCLog("CraftData %s", pBuff);
            
            TiXmlDocument *xmlDoc = new TiXmlDocument();
            xmlDoc->LoadMemory(pBuff, nLength);
            
            //获得根元素
            TiXmlElement *elmRoot = xmlDoc->RootElement();
            
            if (elmRoot)
            {
                data = CraftData::create(elmRoot);
                data->setType(CRAFT_DATA_TYPE_CONNON);
            }
            
            delete xmlDoc;
            
            gCache->addCraftDataToCache(strPath, data);
        }
    }
        
    
}

//加载所有敌机数据
void DataManager::loadAllEnemyData()
{
    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            string strPath = "";
            string strName = "";

            int craft_id = i * 100 + j;
            
            //敌机数据

                strName = "enemy";
                
                
                strPath.append("game/")
                .append(strName)
                .append("/")
                .append(strName)
                .append("_")
                .append(getStringFromInt(craft_id))
                .append(".xml");


            
            
            CCLog("-----load---------%s-----", strPath.c_str());
            //已经导入了就直接返回
            if ((CraftData *)gCache->getCraftDataInCache(strPath))
            {
                return;
            }
            
            CraftData* data = NULL;
            
            unsigned long nLength = 0;
            char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"rt", &nLength);
            
            if (!pBuff)
            {
                continue;
            }
            
            ////CCLog("CraftData %s", pBuff);
            
            TiXmlDocument *xmlDoc = new TiXmlDocument();
            xmlDoc->LoadMemory(pBuff, nLength);
            
            //获得根元素
            TiXmlElement *elmRoot = xmlDoc->RootElement();
            
            if (elmRoot)
            {
                data = CraftData::create(elmRoot);
                data->setType(CRAFT_TYPE_ENEMY);
            }
            
            delete xmlDoc;
            
            gCache->addCraftDataToCache(strPath, data);
            
            CCDictionary* dict_parts = data->getShapesData()->getDictShapesData();
            
            CCLog("dict_parts count is %d",dict_parts->count());
            
            CCDictElement* ele = NULL;
            
            CCDICT_FOREACH(dict_parts, ele)
            {
                ShapePartData* data_part = (ShapePartData*)ele->getObject();
                
                //预加载Enemy飞机的图片
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(data_part->getPlistPath().c_str());
                
                CCLog("************ load enemy: %s **************",data_part->getPlistPath().c_str());
            }

        }
    }
}

//加载所有targetConnon数据
void DataManager::loadAllTargetConnonData()
{
    string strPath = "game/enemy/";
    
    //加载所有的敌机数据
    string strPlaneData = strPath.append("ConnonData.xml");
    
    unsigned long nLength = 0;
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPlaneData.c_str(), "rt", &nLength);
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //tarplanedata
    TiXmlElement *elmPlane = elmRoot->FirstChildElement();
    
    while (elmPlane)
    {
        TargetPlaneData* connon = TargetPlaneData::create(elmPlane);
        
        m_dicTarConnonData->setObject(connon, connon->getID());
        
        elmPlane = elmPlane->NextSiblingElement();
    }
}

//加载所有targetplane数据
void DataManager::loadAllTargetPlaneData()
{
    string strPath = "game/enemy/";
    
    //加载所有的敌机数据
    string strPlaneData = strPath.append("EnemyData.xml");
    
    unsigned long nLength = 0;
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPlaneData.c_str(), "rt", &nLength);
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //tarplanedata
    TiXmlElement *elmPlane = elmRoot->FirstChildElement();
    
    int i = 0;
    
    while (elmPlane)
    {
        i++;
        
        TargetPlaneData* plane = TargetPlaneData::create(elmPlane);
        
        m_dicTarplaneData->setObject(plane, plane->getID());

        elmPlane = elmPlane->NextSiblingElement();
    }
    
}

//加载所有道具的数据
void DataManager::loadAllToolsData()
{
    string strPath = "common/ToolsData.xml";
    
    unsigned long nLength = 0;
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "rt", &nLength);
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //tartoolsdata
    TiXmlElement* elmTool = elmRoot->FirstChildElement();
    
    while (elmTool)
    {
        TargetPlaneData* tool = TargetPlaneData::create(elmTool);
        
        m_dicTarToolsData->setObject(tool, tool->getID());
        
        elmTool = elmTool->NextSiblingElement();
    }
}
//加载所有掉落数据
void DataManager::loadDropData(int drop_id)
{
    string strName = "drops";
    string strPath= "game/";
    
    strPath.append(strName)
    .append("/")
    .append(strName)
    .append("_")
    .append(getStringFromInt(drop_id))
    .append(".xml");
    
    
    CCLog("-----load---------%s-----", strPath.c_str());
    //已经导入了就直接返回
    if ((CraftData *)gCache->getDropDataInCache(strPath))
    {
        return;
    }
    
    DropData* data = NULL;
    
    unsigned long nLength = 0;
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"rt", &nLength);
    
    ////CCLog("DropData %s", pBuff);
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    if (elmRoot)
    {
        data = DropData::create(elmRoot);
        //data->setType(data_type);
    }
    
    delete xmlDoc;
    
    gCache->addDropDataToCache(strPath, data);
}

/**********获取区********/
CraftData* DataManager::getCraftData(int data_type, int craft_id)
{
	string strPath = "";
	string strName = "";
	//主机数据
	if(data_type == CRAFT_DATA_TYPE_HERO)
	{
		strName = "hero";
        
        strPath.append("game/")
		.append(strName)
		.append("/")
		.append(strName)
		.append("_")
		.append(getStringFromInt(craft_id))
		.append(".xml");
	}
    //僚机数据
    else if(data_type == CRAFT_DATA_TYPE_WING)
    {
        strName = "wing";
        
        strPath.append("game/")
        .append(strName)
        .append("/")
        .append(strName)
        .append(".xml");
    }
	//敌机数据
	else if(data_type == CRAFT_DATA_TYPE_ENEMY)
	{
		strName = "enemy";
        
        strPath.append("game/")
		.append(strName)
		.append("/")
		.append(strName)
		.append("_")
		.append(getStringFromInt(craft_id))
		.append(".xml");
	}
    //炮台数据
    else if (data_type == CRAFT_DATA_TYPE_CONNON)
    {
        strName = "cannon";
        
        strPath.append("game/")
		.append("enemy")
		.append("/")
		.append(strName)
		.append("_")
		.append(getStringFromInt(craft_id))
		.append(".xml");
    }else if (data_type == CRAFT_DATA_TYPE_DROP)
    {
        strName = "drops";
        
        strPath.append("game/")
        .append(strName)
        .append("/")
        .append(strName)
        .append("_")
        .append(getStringFromInt(craft_id))
        .append(".xml");
    }

	//CCLog("-----get---------%s-----", strPath.c_str());
	//已经导入了就直接返回
	if ((CraftData *)gCache->getCraftDataInCache(strPath))
	{
		return (CraftData *)gCache->getCraftDataInCache(strPath);
	}

	return NULL;
}
//获取子弹数据
BulletData* DataManager::getBulletData(int bullet_id)
{
	string strPath = "game/ammo/bullet_";
	strPath.append(getStringFromInt(bullet_id)).append(".xml");

	//已经导入了就直接返回
	if ((BulletData*)gCache->getBulletDataInCache(strPath))
	{
		return (BulletData*)gCache->getBulletDataInCache(strPath);
	}

	return NULL;
}
//获取掉落数据
DropData* DataManager::getDropData(int drop_id)
{
    string strPath = "game/drops/drops_";
    strPath.append(getStringFromInt(drop_id)).append(".xml");
    
    //已经导入了就直接返回
    if ((BulletData*)gCache->getDropDataInCache(strPath))
    {
        return (DropData*)gCache->getDropDataInCache(strPath);
    }
    
    return NULL;
}

//获取targetPlane
TargetPlaneData* DataManager::getTargetPlane(string plane_id)
{
    return (TargetPlaneData*)m_dicTarplaneData->objectForKey(plane_id);
}

//获取targetConnon
TargetPlaneData* DataManager::getTargetConnon(string connon_id)
{
    return (TargetPlaneData*)m_dicTarConnonData->objectForKey(connon_id);
}

//获取targetTools
TargetPlaneData* DataManager::getTargetTool(string tool_id)
{
    return (TargetPlaneData*)m_dicTarToolsData->objectForKey(tool_id);
}