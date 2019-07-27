#ifndef _TIMEPLANEDATA_H_
#define _TIMEPLANEDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"
#include "SmartRes.h"
#include "BatteryData.h"

#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class TimePlaneData : public CCObject
{
public:
	virtual ~TimePlaneData(void);
    
    static TimePlaneData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

	//加载炮台
	bool loadBatterys(TiXmlElement *elm);

private:
    TimePlaneData(void);

	CC_SYNTHESIZE(string, id, ID);
	CC_SYNTHESIZE(string, route, Route);

	//是否随机轨迹
	CC_SYNTHESIZE(bool, m_bIsRandRoute, IsRandRoute);

	//对应TargetPlane里的id
	CC_SYNTHESIZE(string, type, Type);  
	CC_SYNTHESIZE(float, speed, Speed);
	CC_SYNTHESIZE(float, first_delay, FirstDelay);
	CC_SYNTHESIZE(float, repeat_delay, RepeatDelay);
	CC_SYNTHESIZE(int, repeat_times, RepeatTimes);

	//附属飞机是否加到boss身上
	CC_SYNTHESIZE(int, isBelongToBoss, IsBelongToBoss);

	//是否可以回弹
	CC_SYNTHESIZE(bool, m_bIsReboundCraft, IsReboundCraft);
	//可以回弹的飞机是否是第一次移动
	CC_SYNTHESIZE(CCPoint, m_posFirstMove, FirstMove);

	//是否可以回弹
	CC_SYNTHESIZE(bool, m_bIsDownBoss, IsDownBoss);

	//炮台是否可以旋转
	CC_SYNTHESIZE(bool, m_bIsCanRotate, IsCanRotate);

	//标记是否有导弹飞机提示
	CC_SYNTHESIZE(bool,m_isMissile,IsMissile);

	//导弹飞机飞过时间
	CC_SYNTHESIZE(float,m_fMissTime,MissileTime);

	//导弹飞机出现时间
	CC_SYNTHESIZE(float,m_AppearTime,MissileAppear);

	//飞机的所有炮台
	CC_SYNTHESIZE(CCArray*, m_pArrBatterys, ArrBatterys);
};

#endif