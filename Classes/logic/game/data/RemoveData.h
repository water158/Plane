#ifndef _REMOVEDATA_H_
#define _REMOVEDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "GlobalEnum.h"
#include "RemoveTargetData.h"

USING_NS_CC;
using namespace std;

class RemoveData : public CCObject
{
public:
	virtual ~RemoveData(void);

	static RemoveData* create1(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

	RemoveTargetData* getRemoveTargetData(string key);

private:
	RemoveData(void);
	bool init(TiXmlElement *elm);

	CC_SYNTHESIZE (string, id, ID);
	CC_SYNTHESIZE (CCDictionary*, targetPlanes, TargetPlanes);
};

#endif