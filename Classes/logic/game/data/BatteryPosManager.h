//
//  BatteryPosManager.h
//  ambition
//
//  Created by mini01 on 14-12-16.
//
//

#ifndef __ambition__BatteryPosManager__
#define __ambition__BatteryPosManager__

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"
#include "BatteryPositionData.h"

USING_NS_CC;
USING_NS_STD;

class BatteryPosManager
{
public:
    
    static BatteryPosManager* shared();
    
    PositionData* getBatteryPos(string plane_id,string battery_id);
    
    BatteryPositionData* getBatteryPosData(string plane_id);
    
private:
    BatteryPosManager();
    ~BatteryPosManager();
    
    void loadData();
    
    static BatteryPosManager* m_sharedInstance;
    
    CCDictionary* m_dicBatteryPosData;
};

#endif /* defined(__ambition__BatteryPosManager__) */
