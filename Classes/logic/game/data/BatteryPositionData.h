#ifndef __BATTERYPOSITION_DATA_H__
#define __BATTERYPOSITION_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "PositionData.h"

USING_NS_CC;
using namespace std;

class BatteryPositionData : public CCObject
{
public:
	virtual ~BatteryPositionData();

	static BatteryPositionData* create(TiXmlElement *elm);

	//加载自身数据
	bool loadData(TiXmlElement *elm);

private:
	BatteryPositionData();


	CC_SYNTHESIZE (CCDictionary*, m_pDictPositionsData, DictPositionsData);
    
    CC_SYNTHESIZE(string, m_id, ID);
    
    CC_SYNTHESIZE(int , m_nConnon, ConnonCount);

};

#endif