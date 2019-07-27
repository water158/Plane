#ifndef _TIMEDATA_H_
#define _TIMEDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"

#include "TimePlaneData.h"

USING_NS_CC;
using namespace std;

//typedef map<string, TimePlaneData> MapTimePlaneData;

class TimeData : public CCObject
{
public:
	virtual ~TimeData(void);
    
    static TimeData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

	//加载TargetGunGroup数据
	void loadTimePlaneData(TiXmlElement *elm);

private:
    TimeData(void);
    
	CC_SYNTHESIZE(string, id, ID);
    
    //是否结束
	CC_SYNTHESIZE(bool, m_bIsEnd, IsEnd);
    
	
	//背景地图滚动的速度变化
	CC_SYNTHESIZE(float, mapSpeedOffset, MapSpeedOffset);
    
	//速度变化时间
	CC_SYNTHESIZE(float, time, Time);
    
    //是否消灭完当前所有飞机 才进入下一个时间片
	CC_SYNTHESIZE(bool, m_bIsPause, IsPause);

	//标记是否有boss警告提示
	CC_SYNTHESIZE(bool, m_bWarning, Warning);

	//警告的提示时间
	CC_SYNTHESIZE(float, m_fWarningTime, WarningTime);

    CC_SYNTHESIZE(CCArray*, time_planes, TimePlanes);
};

#endif