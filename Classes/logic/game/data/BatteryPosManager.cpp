//
//  BatteryPosManager.cpp
//  ambition
//
//  Created by mini01 on 14-12-16.
//
//

#include "BatteryPosManager.h"

BatteryPosManager::BatteryPosManager()
{

}

BatteryPosManager::~BatteryPosManager()
{
    CC_SAFE_DELETE(m_sharedInstance);
    CC_SAFE_DELETE(m_dicBatteryPosData);
}

BatteryPosManager* BatteryPosManager::m_sharedInstance = NULL;

BatteryPosManager* BatteryPosManager::shared()
{
    if (!m_sharedInstance)
    {
        m_sharedInstance = new BatteryPosManager();
        
        m_sharedInstance->loadData();
    }
    
    return m_sharedInstance;
}

void BatteryPosManager::loadData()
{
    m_dicBatteryPosData = CCDictionary::create();
    
    m_dicBatteryPosData->retain();
    
    string strPath = "game/enemy/BatteryPosData.xml";
    
    unsigned long nLength = 0;
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "rt", &nLength);
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获取根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
    {
        BatteryPositionData* posData = BatteryPositionData::create(elmCursor);
        
        m_dicBatteryPosData->setObject(posData, posData->getID());
        
        elmCursor = elmCursor->NextSiblingElement();
    }

}

BatteryPositionData* BatteryPosManager::getBatteryPosData(string plane_id)
{
    return (BatteryPositionData*)m_dicBatteryPosData->objectForKey(plane_id);
}

PositionData* BatteryPosManager::getBatteryPos(string plane_id, string battery_id)
{
    BatteryPositionData* planeBatteryData = (BatteryPositionData*)m_dicBatteryPosData->objectForKey(plane_id);
    
    CCLog("planeBatteryData id %s battery_id %s",planeBatteryData->getID().c_str(),battery_id.c_str());
    
    return (PositionData*)planeBatteryData->getDictPositionsData()->objectForKey(battery_id);
}

