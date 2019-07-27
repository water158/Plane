#ifndef _BATTERYDATA_H_
#define _BATTERYDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"

#include "TimePlaneData.h"

USING_NS_CC;
using namespace std;

class BatteryData : public CCObject
{
public:
	virtual ~BatteryData(void);

	static BatteryData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

private:
	BatteryData(void);

	CC_SYNTHESIZE(string, id, ID);

	//对应TargetPlane里的id
	CC_SYNTHESIZE(string, type, Type);

	//是否自动旋转
	CC_SYNTHESIZE(bool, m_bAutomaticRotate, AutomaticRotate);

	//是否有后坐力
	CC_SYNTHESIZE(bool, m_bRecoil, Recoil);

	//相对位置(所在飞机的相对位置)
	CC_SYNTHESIZE(float, x, X);
	CC_SYNTHESIZE(float, y, Y);

	//炮台缩放
	CC_SYNTHESIZE(float, m_fScale, Scale);

	//炮台角速度
	CC_SYNTHESIZE(float, m_fSpeed, Speed);
};

#endif