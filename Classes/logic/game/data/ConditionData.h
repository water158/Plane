#ifndef _CONDITIONDATA_H_
#define _CONDITIONDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "ConditionTargetData.h"
#include "GlobalEnum.h"
#include "TargetGunGroupData.h"

USING_NS_CC;
using namespace std;

class ConditionData : public CCObject
{
public:
	virtual ~ConditionData(void);

	static ConditionData* create(TiXmlElement *elm);

	    //加载自身的数据
	bool loadData(TiXmlElement *elm);

	//ConditionTargetData* getConditionTargetData(string key);
    
    //TargetGunGroupData* getTargetGunGroupData(string key);

private:
	ConditionData(void);
	bool init(TiXmlElement *elm);

	CC_SYNTHESIZE (string, id, ID);
	//CC_SYNTHESIZE (CCDictionary*, targetPlanes, TargetPlanes);
    
    CC_SYNTHESIZE (CCArray*, target_gungroups, TargetGunGroup);
};

#endif