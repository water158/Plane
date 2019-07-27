#ifndef __PATHDATA_H__
#define __PATHDATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "SmartRes.h"
#include "FerrisTools.h"
#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class PathData : public CCObject
{
public:
    virtual ~PathData();
    
    static PathData* create(TiXmlElement *elm);

	bool loadData(TiXmlElement *elm);

	//获取数据行为类型
	int getDataType(TiXmlElement *elm);

private:
	PathData();

	//行为ID
	CC_SYNTHESIZE(string, id, ID);
	//x
	CC_SYNTHESIZE(float, bx, BX);
	//y
	CC_SYNTHESIZE(float, by, BY);
	//目标x
	CC_SYNTHESIZE(float, bx1, BX1);
	//目标y
	CC_SYNTHESIZE(float, by1, BY1);
	//行为时间
	CC_SYNTHESIZE(float, time, Time);
	//圆弧运动半径
	CC_SYNTHESIZE(float, radius, Radius);
	//角度
	CC_SYNTHESIZE(float, angle, Angle);
	//相对移动x
	CC_SYNTHESIZE(float, moveBy_x, MoveBy_X);
	//相对移动Y
	CC_SYNTHESIZE(float, moveBy_y, MoveBy_Y);
	//播放加血动画特效
	CC_SYNTHESIZE(bool, isPlayEffect, IsPlayEffect);
    //是否自动转向 默认是自动的
    CC_SYNTHESIZE(bool, is_auto_rotation, IsAutoRotation);
    
	//重复事件(起始ID和结束ID)
	CC_SYNTHESIZE(int, bID, BID);
	CC_SYNTHESIZE(int, eID, EID);
	CC_SYNTHESIZE (int, totalRepeatTimes, TotalRepeatTimes);


	//数据所属的动作类型
	CC_SYNTHESIZE(int, type, Type);

	float getBeginX();
	float getBeginY();
	float getEndX();
	float getEndY();
	float getMoveByX();
	float getMoveByY();
};

#endif