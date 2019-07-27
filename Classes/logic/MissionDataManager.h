//
//  MissionDataManager.h
//  planeGame
//
//  Created by mini01 on 14-9-24.
//
//

#ifndef __planeGame__MissionDataManager__
#define __planeGame__MissionDataManager__

#include "cocos2d.h"
#include "MissionData.h"
#include "tinyxml.h"

USING_NS_CC;

class MissionDataManager
{
public:
    MissionDataManager();
    ~MissionDataManager();
    
    static MissionDataManager* shared();
    
    CCDictionary* dic_MissionData;
    
    bool loadAllMissionData();
    
    MissionData* getRandomMission();
    
    MissionData* getMissionById(int value);
    
    MissionData* getMissionByWave(int wave);
    
private:
    static MissionDataManager* s_shareManager;
    
};

#endif /* defined(__planeGame__MissionDataManager__) */
