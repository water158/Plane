#ifndef _CONDITIONTARGETDATA_H_
#define _CONDITIONTARGETDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"

USING_NS_CC;
using namespace std;

class ConditionTargetData : public CCObject
{
public:
	virtual ~ConditionTargetData(void);

	static ConditionTargetData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

private:
	ConditionTargetData(void);

	CC_SYNTHESIZE (string, id, ID);
	CC_SYNTHESIZE (int, hp, HP);
	CC_SYNTHESIZE (int, targetPlaneID, TargetPlaneID);
	CC_SYNTHESIZE (int, repeatTimes, RepeatTimes);
};

#endif