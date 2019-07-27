//
//  MissionDataManager.cpp
//  planeGame
//
//  Created by mini01 on 14-9-24.
//
//

#include "MissionDataManager.h"


MissionDataManager::MissionDataManager()
{

}

MissionDataManager::~MissionDataManager()
{
    CCLog("MissionData Destroy.");
}

MissionDataManager* MissionDataManager::s_shareManager = NULL;

MissionDataManager* MissionDataManager::shared()
{
    if (!s_shareManager)
    {
        s_shareManager = new MissionDataManager();
    }
    
    return s_shareManager;
}

bool MissionDataManager::loadAllMissionData()
{
    //创建关卡数据字典
    dic_MissionData = CCDictionary::create();
    
    dic_MissionData->retain();
    
    for (int i = 1; i <= 25; i++)
    {
        MissionData* mission = MissionData::create(i);

        dic_MissionData->setObject(mission, i);
    }
    
    return true;
}

MissionData* MissionDataManager::getRandomMission()
{
    MissionData* mission = (MissionData*)dic_MissionData->randomObject();
    
    return mission;
}

MissionData* MissionDataManager::getMissionById(int value)
{
    MissionData* mission = (MissionData*)dic_MissionData->objectForKey(value);
    
    return mission;
}

MissionData* MissionDataManager::getMissionByWave(int wave)
{
    int randnum = 0;
    
    if (wave <= 20)
    {
        randnum = getRandom(1, 5);
    }
    else if (wave <= 50)
    {
        randnum = getRandom(6, 10);
    }
    else if (wave <= 80)
    {
        randnum = getRandom(11, 15);
    }
    else
    {
        randnum = getRandom(16, 25);
    }
    
    MissionData* mission = (MissionData*)dic_MissionData->objectForKey(randnum);
    
    return mission;
}