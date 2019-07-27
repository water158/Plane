//
//  DailyCountData.cpp
//  phoenix
//
//  Created by mini01 on 14-11-11.
//
//

#include "DailyCountData.h"

DailyCountData::DailyCountData()
{
    m_dailyTotalScore = 0;
    m_oneMissionTotalScore = 0;
    m_dailyTotalWaves = 0;
    m_oneMissionTotalWaves = 0;
    m_strengthEquip = 0;
    m_dailyStrengthTimes = 0;
    m_dailyGetEquip = 0;
    m_dailyGetGold = 0;
    m_oneMissionGetGold = 0;
    m_dailyDestoryEnemy = 0;
    m_oneMissionDestoryEnemy = 0;
    m_dailyAttackTimes = 0;
    m_dailyLotteryTimes = 0;
    m_dailySellEquip = 0;
    m_dailyShareEquip = 0;
    m_dailyShareAchieve = 0;
    m_dailyShareAcount = 0;
}

DailyCountData::~DailyCountData()
{

}

DailyCountData* DailyCountData::m_sharedInstance = NULL;

DailyCountData* DailyCountData::sharedInstance()
{
    if (!m_sharedInstance)
    {
        m_sharedInstance = new DailyCountData();
        
        m_sharedInstance->init();
    }
    
    return m_sharedInstance;
}

void DailyCountData::init()
{
    this->check();
    
    this->load();
}

void DailyCountData::check()
{
    m_sDailyCountDataPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "DailyData_out.xml";
    
    bool exsit = isFileExist(m_sDailyCountDataPath.c_str());
    
    if (!exsit)
    {
        copyData("common/DailyData.xml", "DailyData_out.xml");
    }
}

void DailyCountData::checkVersion()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLongth = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sDailyCountDataPath.c_str(), "rt", &nLongth);
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

void DailyCountData::loadDataFromVer(int version)
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

void DailyCountData::load()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sDailyCountDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获取数据的根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    //当日分数累积
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    this->setDailyTotalScore(atoi(elmCursor->FirstChild()->Value()));
    
    //一局分数累积
    elmCursor = elmCursor->NextSiblingElement();
    this->setOneMissionTotalScore(atoi(elmCursor->FirstChild()->Value()));
    
    //当日波数累积
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyTotalWaves(atoi(elmCursor->FirstChild()->Value()));
    
    //一局波数累积
    elmCursor = elmCursor->NextSiblingElement();
    this->setOneMissionTotalWaves(atoi(elmCursor->FirstChild()->Value()));
    
    //强化任一装备到级数
    elmCursor = elmCursor->NextSiblingElement();
    this->setStrengthEquip(atoi(elmCursor->FirstChild()->Value()));
    
    //当日累积强化次数
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyStrengthTimes(atoi(elmCursor->FirstChild()->Value()));
    
    //当日累积获得装备
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyGetEquip(atoi(elmCursor->FirstChild()->Value()));
    
    //当日累积获得金币
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyGetGold(atoi(elmCursor->FirstChild()->Value()));
    
    //一局累积获得金币
    elmCursor = elmCursor->NextSiblingElement();
    this->setOneMissionGetGold(atoi(elmCursor->FirstChild()->Value()));
    
    //当日累积摧毁敌机
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyDestoryEnemy(atoi(elmCursor->FirstChild()->Value()));
    
    //一局累积摧毁敌机
    elmCursor = elmCursor->NextSiblingElement();
    this->setOneMissionDestoryEnemy(atoi(elmCursor->FirstChild()->Value()));
    
    //当日累积出击次数
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyAttackTimes(atoi(elmCursor->FirstChild()->Value()));
    
    //当日抽奖
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyLotteryTimes(atoi(elmCursor->FirstChild()->Value()));
    
    //当日出售装备
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailySellEquip(atoi(elmCursor->FirstChild()->Value()));
    
    //当日分享装备
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyShareEquip(atoi(elmCursor->FirstChild()->Value()));
    
    //当日分享成就
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyShareAchieve(atoi(elmCursor->FirstChild()->Value()));
    
    //当日分享成绩（结算界面）
    elmCursor = elmCursor->NextSiblingElement();
    this->setDailyShareAcount(atoi(elmCursor->FirstChild()->Value()));
    
    delete xmlDoc;
    
}

void DailyCountData::save()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sDailyCountDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获取数据根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    //当日分数累积
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyTotalScore()));
    
    //一局分数累积
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getOneMissionTotalScore()));
    
    //当日波数累积
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyTotalWaves()));
    
    //一局累积波数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getOneMissionTotalWaves()));
    
    //强化任一装备到级数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getStrengthEquip()));
    
    //当日累积强化次数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyStrengthTimes()));
    
    //当日累积获得装备
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyGetEquip()));
    
    //当日累积获得金币
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyGetGold()));
    
    //一局累积获得金币
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getOneMissionGetGold()));
    
    //当日累积摧毁敌机
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyDestoryEnemy()));
    
    //一局累积摧毁敌机
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getOneMissionDestoryEnemy()));
    
    //当日累积出击次数
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyAttackTimes()));
    
    //当日抽奖
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyLotteryTimes()));
    
    //当日出售装备
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailySellEquip()));
    
    //当日分享装备
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyShareEquip()));
    
    //当日分享成就
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyShareAchieve()));
    
    //当日分享成绩 （结算界面）
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDailyShareAcount()));
    
    
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sDailyCountDataPath);
    
    delete xmlDoc;
}

void DailyCountData::clearData()
{
    bool isExist = isFileExist(m_sDailyCountDataPath.c_str());
    
    if (isExist)
    {
        remove(m_sDailyCountDataPath.c_str());
    }
}

/************************* 操作 *******************************/

//增加当日波数
void DailyCountData::addDailayTotalWaves()
{
    m_dailyTotalWaves++ ;
    
    hadAchieveComplete();
}

//增加当日累积摧毁敌机数
void DailyCountData::addDailyDestrotEnemy()
{
    m_dailyDestoryEnemy++ ;
    
    hadAchieveComplete();
}

void DailyCountData::addDailyGetEquip()
{
    m_dailyGetEquip++ ;
    
    hadAchieveComplete();
}

void DailyCountData::addDailyGetGold()
{
    m_dailyGetGold++;
    
    hadAchieveComplete();
}

void DailyCountData::addAttackTime()
{
    m_dailyAttackTimes++;
    
    hadAchieveComplete();
}

void DailyCountData::addDailyTotalScore(int score)
{
    m_dailyTotalScore += score;
    
    hadAchieveComplete();
}

void DailyCountData::addDailySellEquip()
{
    m_dailySellEquip++;
    
    hadAchieveComplete();
}

void DailyCountData::addDailyLotteryTime()
{
    m_dailyLotteryTimes++;
    
    hadAchieveComplete();
}


/************************ 是否有成就达成 **************************/

bool DailyCountData::hadAchieveComplete()
{
    
    CCArray* arr = DailyTask::sharedInstance()->arr_dailyTask;
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (!item->getIsFinish())
        {
           checkAchieve(item);
        }
    }
    
    return true;
}

void DailyCountData::checkAchieve(AchieveItem *item)
{
    
    bool hadComplete = false;
    
    switch (item->getType())
    {
        case task_day_score:  //日累积分数
            if (m_dailyTotalScore >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;

        case task_one_score:      //一局累积分数
            if (m_oneMissionTotalScore >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_day_wave:       //日累积波数
            if (m_dailyTotalWaves >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_one_wave:       //一局波数累积
            if (m_oneMissionTotalWaves >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_streng_level:   //强化任一装备到等级
            if (m_strengthEquip >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_streng_time:    //强化次数
            if (m_dailyStrengthTimes >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_get_equip:      //累积获得装备
            if (m_dailyGetEquip >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_get_gold:       //累积获得金币
            if (m_dailyGetGold >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
            
        case task_get_goldOneTime:      //一局累积获得金币
            if (m_oneMissionGetGold >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_day_shatter:    //累积摧毁敌机
            if (m_dailyDestoryEnemy >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_one_shatter:    //一局摧毁敌机
            if (m_oneMissionDestoryEnemy >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_day_attack:     //累积出击次数
            if (m_dailyAttackTimes >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_day_lottery:    //累积抽奖次数
            if (m_dailyLotteryTimes >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_sell_equip:     //累积出售装备
            if (m_dailySellEquip >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_share_equip:    //分享装备
            if (m_dailyShareEquip >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_share_achieve:  //分享成就
            if (m_dailyShareAchieve >= item->getRequestCount())
            {
                hadComplete = true;
            }
            break;
            
        case task_share_account:  //分享成绩
            if (m_dailyShareAcount >= item->getRequestCount())
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
        
        GameCountData::sharedInstance()->addTotalTask();
        
        //GameCountData::sharedInstance()->save();
        
        //GameDelegate::shared()->openNotification(item->getDescribe().c_str());
        GameDelegate::shared()->openNotification(item, achieve_daily);
    }
}

bool DailyCountData::hadAchieveUnReceive()
{
    CCArray* arr = DailyTask::sharedInstance()->arr_dailyTask;
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            return true;
        }
    }
    return false;
}

