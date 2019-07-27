#ifndef _MISSION_DATA_H_
#define _MISSION_DATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "TargetPlaneData.h"
#include "TimeData.h"
#include "DataManager.h"
#include "CraftData.h"
#include "WeaponLibrary.h"

USING_NS_CC;
using namespace std;

class MissionData : public CCObject
{
public:
    MissionData(void);
	virtual ~MissionData(void);
    
    static MissionData* create(int mission_id);
    
    //加载自身的数据
	void loadData(TiXmlElement *elm);

    //加载时间轴
    void loadTimeline(TiXmlElement *elm);
    
    //获得key对应的时间片
    TimeData* getTimeData(string key);
    
    void init(int mission_id);
    int mission_id;
    
private:
    
    CC_SYNTHESIZE (string, id, ID);
    CC_SYNTHESIZE (bool, enable, Enable);
    CC_SYNTHESIZE (string, stage, Stage);
    CC_SYNTHESIZE (int, mode, Mode);
    CC_SYNTHESIZE (string, tip, Tip);
	CC_SYNTHESIZE (int, reward_gold, RewardGold);
	CC_SYNTHESIZE(int,reward_crystal,RewardCrystal);
	//火的上升的时间
	CC_SYNTHESIZE (float, time_fire, TimeFire);
	//火上升的百分比
	CC_SYNTHESIZE (float, percentageOfHigh, PercentageOfHigh);
    
    //Timeline中的start_at属性 表示Timeline从第start_at秒开始
	CC_SYNTHESIZE (int, start_at, StartAt);
    
	CC_SYNTHESIZE (CCDictionary*, timeline, Timeline);
    //地图属性ID
    CC_SYNTHESIZE(int, backgroundID, BackgroundID);
};

#endif