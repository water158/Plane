//
//  Achieve.cpp
//  phoenix
//
//  Created by mini01 on 14-11-6.
//
//

#include "Achieve.h"

Achieve::Achieve()
{
    achieveArr_mileage = CCArray::create();
    achieveArr_mileage->retain();
    
    achieveArr_shatter = CCArray::create();
    achieveArr_shatter->retain();
    
    achieveArr_tools = CCArray::create();
    achieveArr_tools->retain();
    
    achieveArr_equipment = CCArray::create();
    achieveArr_equipment->retain();
    
    achieveArr_pay = CCArray::create();
    achieveArr_pay->retain();
    
    achieveArr_level = CCArray::create();
    achieveArr_level->retain();
    
    achieveArr_consume = CCArray::create();
    achieveArr_consume->retain();
}

Achieve::~Achieve()
{
    CC_SAFE_DELETE_ARRAY(achieveArr_mileage);
    CC_SAFE_DELETE_ARRAY(achieveArr_shatter);
    CC_SAFE_DELETE_ARRAY(achieveArr_tools);
    CC_SAFE_DELETE_ARRAY(achieveArr_equipment);
    CC_SAFE_DELETE_ARRAY(achieveArr_pay);
    CC_SAFE_DELETE_ARRAY(achieveArr_level);
    CC_SAFE_DELETE_ARRAY(achieveArr_consume);
}

Achieve* Achieve::s_sharedInstance = NULL;

Achieve* Achieve::getInstance()
{
    if (!s_sharedInstance)
    {
        s_sharedInstance = new Achieve();
        
        s_sharedInstance->init();
    }
    
    return s_sharedInstance;
}

void Achieve::init()
{
    this->checkVersion();
}

void Achieve::checkVersion()
{
    this->check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    
    unsigned long nLongth = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sAchieveDataPath.c_str(), "rt", &nLongth);
    xmlDoc->LoadMemory(pBuff, nLongth);
    
    //获取根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    if (elmRoot->Attribute("version"))
    {
        this->setDataVersion(atoi(elmRoot->Attribute("version")));
    }
    else
    {
        //如果不存在version属性，默认为1.0版本
        this->setDataVersion(1);
    }
    
    this->loadDataFromVer(m_dataVersion);
    
    delete xmlDoc;
}

void Achieve::check()
{
    m_sAchieveDataPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "achieve_out.xml";
    
    bool exsit = isFileExist(m_sAchieveDataPath.c_str());
    
    if (!exsit)
    {
        copyData("common/achieve.xml", "achieve_out.xml");
    }
}

void Achieve::loadDataFromVer(int version)
{
    int ver = version - 1;
    
    switch (ver)
    {
        case USERDATA_VERSION_1:
            CCLog("version 1");
            load();
            break;
            
        case USERDATA_VERSION_2:
            CCLog("version 2");
            load();
            break;
        
        case USERDATA_VERSION_3:
            CCLog("version 3");
            load();
            break;
            
        case USERDATA_VERSION_4:
            CCLog("version 4");
            load();
            break;
            
        case USERDATA_VERSION_5:
            CCLog("version 5");
            load();
            break;
            
        default:
            load();
            break;
    }
}

void Achieve::load()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sAchieveDataPath.c_str(), "rt", &nLength);
    
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //CCLog("%s",pBuff);
    
    //获取数据的根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    //里程成就
    TiXmlElement* elmAchMileage = elmRoot->FirstChildElement();
    
    TiXmlElement* elmCursor = elmAchMileage->FirstChildElement();
    
    while (elmCursor)
    {
        AchieveItem* achieve_mileage = AchieveItem::create(elmCursor);
        
        achieveArr_mileage->addObject(achieve_mileage);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //击毁成就
    TiXmlElement* elmAchShatter = elmAchMileage->NextSiblingElement();
    
    elmCursor = elmAchShatter->FirstChildElement();
    
    while (elmCursor)
    {
        AchieveItem* achieve_shatter = AchieveItem::create(elmCursor);
        
        achieveArr_shatter->addObject(achieve_shatter);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //道具成就
    TiXmlElement* elmAchTools = elmAchShatter->NextSiblingElement();
    
    elmCursor = elmAchTools->FirstChildElement();
    
    while (elmCursor)
    {
        AchieveItem* achieve_tools = AchieveItem::create(elmCursor);
        
        achieveArr_tools->addObject(achieve_tools);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //装备成就
    TiXmlElement* elmAchEquipment = elmAchTools->NextSiblingElement();
    
    elmCursor = elmAchEquipment->FirstChildElement();
    
    while (elmCursor)
    {
        AchieveItem* achieve_equipment = AchieveItem::create(elmCursor);
        
        achieveArr_equipment->addObject(achieve_equipment);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //等级成就
    TiXmlElement* elmAchLevel = elmAchEquipment->NextSiblingElement();
    
    elmCursor = elmAchLevel->FirstChildElement();
    
    while (elmCursor)
    {
        AchieveItem* achieve_level = AchieveItem::create(elmCursor);
        
        achieveArr_level->addObject(achieve_level);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //充值成就
    TiXmlElement* elmAchPay = elmAchLevel->NextSiblingElement();
    
    elmCursor = elmAchPay->FirstChildElement();
    
    while (elmCursor)
    {
        AchieveItem* achieve_pay = AchieveItem::create(elmCursor);
        
        achieveArr_pay->addObject(achieve_pay);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //消费成就
    TiXmlElement* elmAchConsume = elmAchPay->NextSiblingElement();
    
    elmCursor = elmAchConsume->FirstChildElement();
    
    while (elmCursor)
    {
        AchieveItem* achieve_consume = AchieveItem::create(elmCursor);
        
        achieveArr_consume->addObject(achieve_consume);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    delete xmlDoc;
}

void Achieve::save()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sAchieveDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    elmRoot->SetAttribute("version", this->getDataVersion());//achieve版本
    
    //里程成就
    TiXmlElement* elmAchMileage = elmRoot->FirstChildElement();
    
    TiXmlElement* elmCursor = elmAchMileage->FirstChildElement();
    
    int i = 0;
    
    while (elmCursor)
    {
        AchieveItem* item = (AchieveItem*)achieveArr_mileage->objectAtIndex(i);
        
        TiXmlElement* elmName = elmCursor->FirstChildElement();
        
        TiXmlElement* elmDes = elmName->NextSiblingElement();
        
        TiXmlElement* elmReward = elmDes->NextSiblingElement();
        
        TiXmlElement* elmRewardType1 = elmReward->NextSiblingElement();
        
        TiXmlElement* elmRewardType2 = elmRewardType1->NextSiblingElement();
        
        TiXmlElement* elmIsFinish = elmRewardType2->NextSiblingElement();
        
        elmIsFinish->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0).c_str());
        
        TiXmlElement* elmRequest = elmIsFinish->NextSiblingElement();
        
        TiXmlElement* elmType = elmRequest->NextSiblingElement();
        
        TiXmlElement* elmIsGet = elmType->NextSiblingElement();
        
        elmIsGet->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0).c_str());
        
        elmCursor = elmCursor->NextSiblingElement();
        
        i++;
    }
    
    //击毁成就
    TiXmlElement* elmAchShatter = elmAchMileage->NextSiblingElement();
    
    elmCursor = elmAchShatter->FirstChildElement();
    
    i = 0;
    
    while (elmCursor)
    {
        AchieveItem* item = (AchieveItem*)achieveArr_shatter->objectAtIndex(i);
        
        TiXmlElement* elmName = elmCursor->FirstChildElement();
        
        TiXmlElement* elmDes = elmName->NextSiblingElement();
        
        TiXmlElement* elmReward = elmDes->NextSiblingElement();
        
        TiXmlElement* elmRewardType1 = elmReward->NextSiblingElement();
        
        TiXmlElement* elmRewardType2 = elmRewardType1->NextSiblingElement();
        
        TiXmlElement* elmIsFinish = elmRewardType2->NextSiblingElement();
        
        elmIsFinish->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0).c_str());
        
        TiXmlElement* elmRequest = elmIsFinish->NextSiblingElement();
        
        TiXmlElement* elmType = elmRequest->NextSiblingElement();
        
        TiXmlElement* elmIsGet = elmType->NextSiblingElement();
        
        elmIsGet->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0).c_str());
        
        elmCursor = elmCursor->NextSiblingElement();
        
        i++;
    }
    
    //道具成就
    TiXmlElement* elmAchTools = elmAchShatter->NextSiblingElement();
    
    elmCursor = elmAchTools->FirstChildElement();
    
    i = 0;
    
    while (elmCursor)
    {
        AchieveItem* item = (AchieveItem*)achieveArr_tools->objectAtIndex(i);
        
        TiXmlElement* elmName = elmCursor->FirstChildElement();
        
        TiXmlElement* elmDes = elmName->NextSiblingElement();
        
        TiXmlElement* elmReward = elmDes->NextSiblingElement();
        
        TiXmlElement* elmRewardType1 = elmReward->NextSiblingElement();
        
        TiXmlElement* elmRewardType2 = elmRewardType1->NextSiblingElement();
        
        TiXmlElement* elmIsFinish = elmRewardType2->NextSiblingElement();
        
        elmIsFinish->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0).c_str());
        
        TiXmlElement* elmRequest = elmIsFinish->NextSiblingElement();
        
        TiXmlElement* elmType = elmRequest->NextSiblingElement();
        
        TiXmlElement* elmIsGet = elmType->NextSiblingElement();
        
        elmIsGet->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0).c_str());
        
        elmCursor = elmCursor->NextSiblingElement();
        
        i++;
    }
    
    
    //装备成就
    TiXmlElement* elmAchEquipment = elmAchTools->NextSiblingElement();
    
    elmCursor = elmAchEquipment->FirstChildElement();
    
    i = 0;
    
    while (elmCursor)
    {
        AchieveItem* item = (AchieveItem*)achieveArr_equipment->objectAtIndex(i);
        
        TiXmlElement* elmName = elmCursor->FirstChildElement();
        
        TiXmlElement* elmDes = elmName->NextSiblingElement();
        
        TiXmlElement* elmReward = elmDes->NextSiblingElement();
        
        TiXmlElement* elmRewardType1 = elmReward->NextSiblingElement();
        
        TiXmlElement* elmRewardType2 = elmRewardType1->NextSiblingElement();
        
        TiXmlElement* elmIsFinish = elmRewardType2->NextSiblingElement();
        
        elmIsFinish->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0).c_str());
        
        TiXmlElement* elmRequest = elmIsFinish->NextSiblingElement();
        
        TiXmlElement* elmType = elmRequest->NextSiblingElement();
        
        TiXmlElement* elmIsGet = elmType->NextSiblingElement();
        
        elmIsGet->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0).c_str());
        
        elmCursor = elmCursor->NextSiblingElement();
        
        elmCursor = elmCursor->NextSiblingElement();
        
        i++;
    }
    
    //等级成就
    TiXmlElement* elmAchLevel = elmAchEquipment->NextSiblingElement();
    
    elmCursor = elmAchLevel->FirstChildElement();
    
    i = 0;
    
    while (elmCursor)
    {
        AchieveItem* item = (AchieveItem*)achieveArr_level->objectAtIndex(i);
        
        TiXmlElement* elmName = elmCursor->FirstChildElement();
        
        TiXmlElement* elmDes = elmName->NextSiblingElement();
        
        TiXmlElement* elmReward = elmDes->NextSiblingElement();
        
        TiXmlElement* elmRewardType1 = elmReward->NextSiblingElement();
        
        TiXmlElement* elmRewardType2 = elmRewardType1->NextSiblingElement();
        
        TiXmlElement* elmIsFinish = elmRewardType2->NextSiblingElement();
        
        elmIsFinish->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0).c_str());
        
        TiXmlElement* elmRequest = elmIsFinish->NextSiblingElement();
        
        TiXmlElement* elmType = elmRequest->NextSiblingElement();
        
        TiXmlElement* elmIsGet = elmType->NextSiblingElement();
        
        elmIsGet->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0).c_str());
        
        elmCursor = elmCursor->NextSiblingElement();
        
        i++;
    }
    
    //赌博成就
    TiXmlElement* elmAchPay = elmAchLevel->NextSiblingElement();
    
    elmCursor = elmAchPay->FirstChildElement();
    
    i = 0;
    
    while (elmCursor)
    {
        AchieveItem* item = (AchieveItem*)achieveArr_pay->objectAtIndex(i);
        
        TiXmlElement* elmName = elmCursor->FirstChildElement();
        
        TiXmlElement* elmDes = elmName->NextSiblingElement();
        
        TiXmlElement* elmReward = elmDes->NextSiblingElement();
        
        TiXmlElement* elmRewardType1 = elmReward->NextSiblingElement();
        
        TiXmlElement* elmRewardType2 = elmRewardType1->NextSiblingElement();
        
        TiXmlElement* elmIsFinish = elmRewardType2->NextSiblingElement();
        
        elmIsFinish->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0).c_str());
        
        TiXmlElement* elmRequest = elmIsFinish->NextSiblingElement();
        
        TiXmlElement* elmType = elmRequest->NextSiblingElement();
        
        TiXmlElement* elmIsGet = elmType->NextSiblingElement();
        
        elmIsGet->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0).c_str());
        
        elmCursor = elmCursor->NextSiblingElement();
        i++;
    }
    
    //消费成就
    TiXmlElement* elmAchConsume = elmAchPay->NextSiblingElement();
    
    elmCursor = elmAchConsume->FirstChildElement();
    
    i = 0;
    
    while (elmCursor)
    {
        AchieveItem* item = (AchieveItem*)achieveArr_consume->objectAtIndex(i);
        
        TiXmlElement* elmName = elmCursor->FirstChildElement();
        
        TiXmlElement* elmDes = elmName->NextSiblingElement();
        
        TiXmlElement* elmReward = elmDes->NextSiblingElement();
        
        TiXmlElement* elmRewardType1 = elmReward->NextSiblingElement();
        
        TiXmlElement* elmRewardType2 = elmRewardType1->NextSiblingElement();
        
        TiXmlElement* elmIsFinish = elmRewardType2->NextSiblingElement();
        
        elmIsFinish->FirstChild()->SetValue(getStringFromInt(item->getIsFinish() ? 1 : 0).c_str());
        
        TiXmlElement* elmRequest = elmIsFinish->NextSiblingElement();
        
        TiXmlElement* elmType = elmRequest->NextSiblingElement();
        
        TiXmlElement* elmIsGet = elmType->NextSiblingElement();
        
        elmIsGet->FirstChild()->SetValue(getStringFromInt(item->getIsGet() ? 1 : 0).c_str());
        
        elmCursor = elmCursor->NextSiblingElement();
        
        i++;
    }
    
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sAchieveDataPath);
    
    delete xmlDoc;

}

AchieveItem* Achieve::getAchieve(AchievType type, int Id)
{
    Id -= 1;
    
    switch (type)
    {
        case ach_mileage:
            return (AchieveItem*)achieveArr_mileage->objectAtIndex(Id);
            break;
        case ach_shatter:
            return (AchieveItem*)achieveArr_shatter->objectAtIndex(Id);
            break;
        case ach_tools:
            return (AchieveItem*)achieveArr_tools->objectAtIndex(Id);
            break;
        case ach_equipment:
            return (AchieveItem*)achieveArr_equipment->objectAtIndex(Id);
            break;
        case ach_level:
            return (AchieveItem*)achieveArr_level->objectAtIndex(Id);
            break;
        case ach_pay:
            return (AchieveItem*)achieveArr_pay->objectAtIndex(Id);
            break;
        case ach_consume:
            return (AchieveItem*)achieveArr_consume->objectAtIndex(Id);
            break;
        default:
            break;
    }
    
    return NULL;
}








