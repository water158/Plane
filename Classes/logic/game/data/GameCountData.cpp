//
//  GameCountData.cpp
//  phoenix
//
//  Created by mini01 on 14-11-11.
//
//

#include "GameCountData.h"

GameCountData::GameCountData()
{
    m_mileage = 0.0f;
    m_shatter = 0;
    m_useTools = 0;
    m_greenEquip = 0;
    m_blueEquip = 0;
    m_pupleEquip = 0;
    m_orangeEquip = 0;
    m_lotteryTimes = 0;
    m_recharge = 0;
    m_consumeDamond = 0;
    m_attackTimes = 0;
    m_bestScore = 0;
    m_totalGold = 0;
    m_totalGem = 0;
    m_totalTask = 0;
}

GameCountData::~GameCountData()
{

}

GameCountData* GameCountData::m_sharedInstance = NULL;

GameCountData* GameCountData::sharedInstance()
{
    if (!m_sharedInstance)
    {
        m_sharedInstance = new GameCountData();
        
        m_sharedInstance->init();
    }
    
    return m_sharedInstance;
}

void GameCountData::init()
{
    checkVersion();
}

void GameCountData::checkVersion()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLongth = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sGameCountDataPath.c_str(), "rt", &nLongth);
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

void GameCountData::loadDataFromVer(int version)
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

void GameCountData::check()
{
    m_sGameCountDataPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "AllGameData_out.xml";
    
    bool exsit = isFileExist(m_sGameCountDataPath.c_str());
    
    if (!exsit)
    {
        copyData("common/AllGameData.xml", "AllGameData_out.xml");
    }
}

void GameCountData::load()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sGameCountDataPath.c_str(), "rt", &nLength);
    
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //todo的类型为之前策划策划的装备类型成就  现在已经取消了
    
    //获取数据根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    //里程
    this->setMileage(atoi(elmCursor->FirstChild()->Value()));
    //击毁数
    elmCursor = elmCursor->NextSiblingElement();
    this->setShatter(atoi(elmCursor->FirstChild()->Value()));
    //释放道具
    elmCursor = elmCursor->NextSiblingElement();
    this->setUseTools(atoi(elmCursor->FirstChild()->Value()));
    //todo:释放道具类型处理
    elmCursor = elmCursor->NextSiblingElement();
    //获得优秀装备
    elmCursor = elmCursor->NextSiblingElement();
    this->setGreenEquip(atoi(elmCursor->FirstChild()->Value()));
    //todo::获得优秀装备类型处理
    elmCursor = elmCursor->NextSiblingElement();
    //获得精良
    elmCursor = elmCursor->NextSiblingElement();
    this->setBlueEquip(atoi(elmCursor->FirstChild()->Value()));
    //todo::获得精良装备类型处理
    elmCursor = elmCursor->NextSiblingElement();
    //获得史诗装备
    elmCursor = elmCursor->NextSiblingElement();
    this->setPupleEquip(atoi(elmCursor->FirstChild()->Value()));
    //todo::获得史诗装备类型处理
    elmCursor = elmCursor->NextSiblingElement();
    //获得传说装备
    elmCursor = elmCursor->NextSiblingElement();
    this->setOrangeEquip(atoi(elmCursor->FirstChild()->Value()));
    //todo:获得传说装备类型处理
    elmCursor = elmCursor->NextSiblingElement();
    //抽奖次数
    elmCursor = elmCursor->NextSiblingElement();
    this->setLotteryTimes(atoi(elmCursor->FirstChild()->Value()));
    //装备强化等级
    elmCursor = elmCursor->NextSiblingElement();
    this->setEquipStrength(atoi(elmCursor->FirstChild()->Value()));
    //充值数
    elmCursor = elmCursor->NextSiblingElement();
    this->setRecharge(atoi(elmCursor->FirstChild()->Value()));
    //累积消耗钻石
    elmCursor = elmCursor->NextSiblingElement();
    this->setConsumeDamond(atoi(elmCursor->FirstChild()->Value()));
    //累积出击次数
    elmCursor = elmCursor->NextSiblingElement();
    this->setAttackTimes(atoi(elmCursor->FirstChild()->Value()));
    //最高分数
    elmCursor = elmCursor->NextSiblingElement();
    this->setBestScore(atoi(elmCursor->FirstChild()->Value()));
    //累积获得金币
    elmCursor = elmCursor->NextSiblingElement();
    this->setTotalGold(atoi(elmCursor->FirstChild()->Value()));
    //累积获得钻石
    elmCursor = elmCursor->NextSiblingElement();
    this->setTotalGem(atoi(elmCursor->FirstChild()->Value()));
    //累积完成任务
    elmCursor = elmCursor->NextSiblingElement();
    this->setTotalTask(atoi(elmCursor->FirstChild()->Value()));
    
    
    delete xmlDoc;
    
}

void GameCountData::save()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sGameCountDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    
    //获取数据根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    //里程
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getMileage()));
    //击毁
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getShatter()));
    //释放道具
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getUseTools()));
    //释放道具类型
    elmCursor = elmCursor->NextSiblingElement();
    //获得优秀装备
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getGreenEquip()));
    //获得优秀装备类型
    elmCursor = elmCursor->NextSiblingElement();
    //获得精良装备
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getBlueEquip()));
    //获得精良装备类型
    elmCursor = elmCursor->NextSiblingElement();
    //获得史诗装备
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getPupleEquip()));
    //获得史诗装备类型
    elmCursor = elmCursor->NextSiblingElement();
    //获得传说装备
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getOrangeEquip()));
    //获得传说装备类型
    elmCursor = elmCursor->NextSiblingElement();
    //抽奖次数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getLotteryTimes()));
    //装备强化等级
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getEquipStrength()));
    //充值数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getRecharge()));
    //累积消耗钻石
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getConsumeDamond()));
    //累积出击次数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getAttackTimes()));
    //最高分数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getBestScore()));
    //累积获得金币
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getTotalGold()));
    //累积获得钻石
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getTotalGem()));
    //累积完成任务
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getTotalTask()));
    
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sGameCountDataPath);
    
    delete xmlDoc;

}


/***************** 操作 *******************/

void GameCountData::addShatter()
{
    m_shatter++;
    
    hadAchieveComplete();
}

void GameCountData::addUseTools()
{
    m_useTools++;
    
    hadAchieveComplete();
}

void GameCountData::addGreenEquip()
{
    m_greenEquip++;
    
    hadAchieveComplete();
}

void GameCountData::addBlueEquip()
{
    m_blueEquip++;
    
    hadAchieveComplete();
}

void GameCountData::addPupleEquip()
{
    m_pupleEquip++;
    
    hadAchieveComplete();
}

void GameCountData::addOrangeEquip()
{
    m_orangeEquip++;
    
    hadAchieveComplete();
}

void GameCountData::addAttackTime()
{
    m_attackTimes++;
    
    hadAchieveComplete();
}

void GameCountData::addTotalGold(int num)
{
    m_totalGold += num;
    
    hadAchieveComplete();
}

void GameCountData::addTotalGem(int num)
{
    m_totalGem += num;
    
    hadAchieveComplete();
}

void GameCountData::addTotalTask()
{
    m_totalTask++;
    
    hadAchieveComplete();
}

void GameCountData::addConsumeGem(int num)
{
    m_consumeDamond += num;
    
    hadAchieveComplete();
}


/***************** 检查是否有成就完成  ********************/

bool GameCountData::hadAchieveComplete()
{
    
    //检查里程数组
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_mileage, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
            checkAchieve(item);
        }
    }
    
    //检查击毁数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_shatter, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
            checkAchieve(item);
        }
    }
    
    //检查释放道具数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_tools, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
            checkAchieve(item);
        }
    }
    
    //检查装备数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_equipment, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
            checkAchieve(item);
        }
    }
    
    //检查充值数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_pay, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
            checkAchieve(item);
        }
    }
    
    //检查等级数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_level, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
            checkAchieve(item);
        }
    }
    
    //检查消费数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_consume, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
            checkAchieve(item);
        }
    }
    
    return true;
}

void GameCountData::checkAchieve(AchieveItem *item)
{
    bool hadComplete = false;
    
    switch (item->getType())
    {
        //CCLog("item->getType() : %d",item->getType());
            
        case ach_mileage:      //里程成就
            //CCLog("m_mileage : %d",m_mileage);
            if (m_mileage >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case ach_shatter:          //击毁成就
            //CCLog("m_shatter : %d",m_shatter);
            if (m_shatter >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case ach_tools:            //道具成就
            //CCLog("m_useTools : %d",m_useTools);
            if (m_useTools >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case ach_equipment:        //装备成就
            ////CCLog("m_mileage : %d",m_useTools);
        {
            //根据id  确定是什么成就
            int n_id = atoi(item->getID().c_str());
            
            if (n_id == 1 || n_id == 5)  //绿色装备成就
            {
                if (m_greenEquip >= item->getRequestCount())
                {
                    hadComplete = true;
                }
            }
            
            if (n_id == 2 || n_id == 6)  //蓝色装备成就
            {
                if (m_blueEquip >= item->getRequestCount())
                {
                    hadComplete = true;
                }
            }
            
            if (n_id == 3 || n_id == 7)   //紫色装备成就
            {
                if (m_pupleEquip >= item->getRequestCount())
                {
                    hadComplete = true;
                }
            }
            
            if (n_id == 4 || n_id == 8)   //橙色装备成就
            {
                if (m_orangeEquip >= item->getRequestCount())
                {
                    hadComplete = true;
                }
            }
            
            break;
        }
            
        case ach_level:            //等级成就
            //CCLog("m_equipStrength : %d",m_equipStrength);
            if (m_equipStrength >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case ach_pay:             //充值成就
            //CCLog("m_recharge : %d",m_recharge);
            if (m_recharge >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;

            
        case ach_consume:          //消费成就
            //CCLog("m_consumeDamond : %d",m_consumeDamond);
            if (m_consumeDamond >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        default:
            break;
    }
    

    if (hadComplete)
    {
        item->setIsFinish(true);
        
        //Achieve::getInstance()->save();
        
        //GameDelegate::shared()->openNotification(item->getDescribe().c_str());
        GameDelegate::shared()->openNotification(item,achieve_game);
    }
    
}

bool GameCountData::hadAchieveUnReceive()
{
    //检查里程数组
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_mileage, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }
    
    //检查击毁数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_shatter, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }
    
    //检查释放道具数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_tools, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }
    
    //检查装备数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_equipment, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }
    
    //检查充值数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_pay, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }
    
    //检查等级数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_level, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }
    
    //检查消费数组
    obj = NULL;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_consume, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }

    return false;
}





















