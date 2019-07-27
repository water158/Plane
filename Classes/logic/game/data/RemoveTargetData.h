#ifndef _REMOVETARGETDATA_H_
#define _REMOVETARGETDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"

USING_NS_CC;
using namespace std;

class RemoveTargetData : public CCObject
{
public:
	virtual ~RemoveTargetData(void);

	static RemoveTargetData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

private:
	RemoveTargetData(void);

	CC_SYNTHESIZE (string, id, ID);
	CC_SYNTHESIZE (int, targetPlaneID, TargetPlaneID);
};

#endif