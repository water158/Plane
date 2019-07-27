//
//  DailyTask.cpp
//  phoenix
//
//  Created by mini01 on 14-11-10.
//
//

#include "DailyTask.h"

DailyTask::DailyTask()
{
    m_firstTaskID = "0";
    m_secTaskID = "0";
    m_thirdTaskID = "0";
    m_fourTaskID = "0";
    
    m_shareTaskID = "0";
    
    arr_dailyTask = CCArray::create();
    
    arr_dailyTask->retain();
}

DailyTask::~DailyTask()
{
    CC_SAFE_DELETE_ARRAY(arr_dailyTask);
}

DailyTask* DailyTask::s_sharedInstance = NULL;

DailyTask* DailyTask::sharedInstance()
{
    if (!s_sharedInstance)
    {
        s_sharedInstance = new DailyTask();
        
        s_sharedInstance->init();
    }
    
    return s_sharedInstance;
}

void DailyTask::init()
{
    this->checkVersion();
}

void DailyTask::checkVersion()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLongth = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sDailyTaskDataPath.c_str(), "rt", &nLongth);
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
    
    
    
    if (getCurrentTime(kGetTimeMonth) != UserData::shared()->getLastDailyRefreshMon() || getCurrentTime(kGetTimeDay) != UserData::shared()->getLastDailyRefreshDay())
    {
        refreshTask();
    }
    else
    {
        this->loadDataFromVer(m_nVersion);
    }
    
    //CCLOG("version==%f", this->getVersion());
    
    delete xmlDoc;
}

void DailyTask::loadDataFromVer(int version)
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

void DailyTask::check()
{
    m_sDailyTaskDataPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "dailyTask_out.xml";
    
    bool exsit = isFileExist(m_sDailyTaskDataPath.c_str());
    
    if (!exsit)
    {
        copyData("common/dailyTask.xml", "dailyTask_out.xml");
    }
}

void DailyTask::load()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sDailyTaskDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    CCLog("%s",pBuff);
    
    //获取数据根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    //CurState
    TiXmlElement* elmState = elmRoot->FirstChildElement();
    
    TiXmlElement* elmCursor = elmState->FirstChildElement();
    
    if (atoi(elmCursor->FirstChild()->Value()) == 0)
    {
        this->setDate(time(NULL));
    }
    else
    {
        if (this->getDate() == 0)
        {
            this->setDate(atoi(elmCursor->FirstChild()->Value()));
        }
    }
    
    
    //task_daily
    elmCursor = elmCursor->NextSiblingElement();
    
    
    //1
    TiXmlElement* elmItem = elmCursor->FirstChildElement();
    if (m_firstTaskID.compare("0") == 0)
    {
        this->setFirstTaskID(elmItem->FirstChild()->Value());
    }
    
    //2
    elmItem = elmItem->NextSiblingElement();
    if (m_secTaskID.compare("0") == 0)
    {
        this->setSecTaskID(elmItem->FirstChild()->Value());
    }
    
    //3
    elmItem = elmItem->NextSiblingElement();
    if (m_thirdTaskID.compare("0") == 0)
    {
        this->setThirdTaskID(elmItem->FirstChild()->Value());
    }
    
    //4
    elmItem = elmItem->NextSiblingElement();
    if (m_fourTaskID.compare("0") == 0)
    {
        this->setFourTaskID(elmItem->FirstChild()->Value());
    }
    
    
    //task_share
    elmCursor = elmCursor->NextSiblingElement();
    this->setShareTaskID(elmCursor->FirstChild()->Value());
    
    //tasklist
    TiXmlElement* elmTask = elmState->NextSiblingElement();
    //normaltask
    TiXmlElement* normalTask = elmTask->FirstChildElement();
    elmCursor = normalTask->FirstChildElement();
    
    while (elmCursor)
    {
        string id = elmCursor->Attribute("id");
        
        CCLog("");
        
        if (id.compare(m_firstTaskID) == 0 || id.compare(m_secTaskID) == 0 || id.compare(m_thirdTaskID) == 0 || id.compare(m_fourTaskID) == 0)
        {
            AchieveItem* achieve = AchieveItem::create(elmCursor);
            
            arr_dailyTask->addObject(achieve);
        }
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //shareTask
//    TiXmlElement* shareTask = normalTask->NextSiblingElement();
//    elmCursor = shareTask->FirstChildElement();
//    
//    while (elmCursor)
//    {
//        string id = elmCursor->Attribute("id");
//        if (id.compare(m_shareTaskID) == 0)
//        {
//            //AchieveItem* achieve = AchieveItem::create(elmCursor);
//            
//            //arr_dailyTask->addObject(achieve);
//        }
//        
//        elmCursor = elmCursor->NextSiblingElement();
//    }
    
    delete xmlDoc;
}

void DailyTask::save()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sDailyTaskDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    //curstate
    TiXmlElement* elmState = elmRoot->FirstChildElement();
    //date
    TiXmlElement* elmCursor = elmState->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDate()));
    
    //task_daily
    elmCursor = elmCursor->NextSiblingElement();
    //1
    TiXmlElement* elmItem = elmCursor->FirstChildElement();
    elmItem->FirstChild()->SetValue(m_firstTaskID);
    //2
    elmItem = elmItem->NextSiblingElement();
    elmItem->FirstChild()->SetValue(m_secTaskID);
    //3
    elmItem = elmItem->NextSiblingElement();
    elmItem->FirstChild()->SetValue(m_thirdTaskID);
    //4
    elmItem = elmItem->NextSiblingElement();
    elmItem->FirstChild()->SetValue(m_fourTaskID);
    
    //task_share
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(m_shareTaskID);
    
    //taskList
    TiXmlElement* elmTask = elmState->NextSiblingElement();
    //normalTask
    TiXmlElement* normalTask = elmTask->FirstChildElement();
    elmCursor = normalTask->FirstChildElement();
    while (elmCursor)
    {
        if (strcmp(elmCursor->Attribute("id"),m_firstTaskID.c_str()) == 0)
        {
            AchieveItem* achieve = (AchieveItem*)arr_dailyTask->objectAtIndex(0);
            
            saveElm(elmCursor, achieve);
        }
        
        if (strcmp(elmCursor->Attribute("id"),m_secTaskID.c_str()) == 0)
        {
            AchieveItem* achieve = (AchieveItem*)arr_dailyTask->objectAtIndex(1);
            
            saveElm(elmCursor, achieve);
        }
        
        if (strcmp(elmCursor->Attribute("id"),m_thirdTaskID.c_str()) == 0)
        {
            AchieveItem* achieve = (AchieveItem*)arr_dailyTask->objectAtIndex(2);
            
            saveElm(elmCursor, achieve);
        }
        
        if (strcmp(elmCursor->Attribute("id"),m_fourTaskID.c_str()) == 0)
        {
            AchieveItem* achieve = (AchieveItem*)arr_dailyTask->objectAtIndex(3);
            
            saveElm(elmCursor, achieve);
        }
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //shareTask
//    TiXmlElement* shareTask = normalTask->NextSiblingElement();
//    elmCursor = shareTask->FirstChildElement();
//    
//    while (elmCursor)
//    {
//        if (strcmp(elmCursor->Attribute("id"),m_shareTaskID.c_str()) == 0)
//        {
//            AchieveItem* achieve = (AchieveItem*)arr_dailyTask->objectAtIndex(4);
//            
//            TiXmlElement* item = elmCursor->FirstChildElement();
//            item->FirstChild()->SetValue(getStringFromInt(achieve->getIsFinish() ? 1 : 0));
//            
//            item = item->NextSiblingElement();
//            item->FirstChild()->SetValue(getStringFromInt(achieve->getIsGet() ? 1 : 0));
//        }
//        
//        elmCursor = elmCursor->NextSiblingElement();
//    }
    
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sDailyTaskDataPath);
    
    delete xmlDoc;
    
}

void DailyTask::saveElm(TiXmlElement *elmCursor, AchieveItem *item)
{
    TiXmlElement* elm = elmCursor->FirstChildElement();  //名字
    elm = elm->NextSiblingElement();  //描述
    elm = elm->NextSiblingElement();  //奖励
    elm = elm->NextSiblingElement(); //奖励1
    elm = elm->NextSiblingElement();  //奖励2
    elm = elm->NextSiblingElement(); //是否完成
    elm->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0));
    
    elm = elm->NextSiblingElement();  //需求的数量
    elm = elm->NextSiblingElement();  //类型
    elm = elm->NextSiblingElement();   //是否领取
    elm->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0));
}

void DailyTask::clearData()
{
    bool isExist = isFileExist(m_sDailyTaskDataPath.c_str());
    
    if (isExist)
    {
        remove(m_sDailyTaskDataPath.c_str());
    }
}

void DailyTask::refreshTask()
{
    this->clearData();
    
    int randnum1 = getRandom(1, 10);
    this->setFirstTaskID(getStringFromInt(randnum1));
    
    int randnum2 = getRandom(1, 10);
    while (randnum2 == randnum1)
    {
        randnum2 = getRandom(1, 10);
    }
    this->setSecTaskID(getStringFromInt(randnum2));
    
    int randnum3 = getRandom(1, 10);
    while (randnum3 == randnum2 || randnum3 == randnum1)
    {
        randnum3 = getRandom(1, 10);
    }
    this->setThirdTaskID(getStringFromInt(randnum3));
    
    int randnum4 = getRandom(1, 10);
    while (randnum4 == randnum3 || randnum4 == randnum2 || randnum4 == randnum1)
    {
        randnum4 = getRandom(1, 10);
    }
    this->setFourTaskID(getStringFromInt(randnum4));
    
    load();
    
    save();
    
    UserData::shared()->setLastDailyRefreshDay(getCurrentTime(kGetTimeDay));
    
    UserData::shared()->setLastDailyRefreshMon(getCurrentTime(kGetTimeMonth));
    
    UserData::shared()->save();
    
}

AchieveItem* DailyTask::getTaskByID(int nId)
{
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr_dailyTask, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (atoi(item->getID().c_str()) == nId)
        {
            return item;
        }
    }
    
    return NULL;
}



