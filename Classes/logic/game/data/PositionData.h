#ifndef __POSITION_DATA_H__
#define __POSITION_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class PositionData : public CCObject
{
public:
	virtual ~PositionData();

	static PositionData* create(TiXmlElement *elm);

	//加载自身数据
	bool loadData(TiXmlElement *elm);

private:
	PositionData();

	//id
	CC_SYNTHESIZE (string, m_strID, ID);

	//x
	CC_SYNTHESIZE (float, m_fX, X);

	//y
	CC_SYNTHESIZE (float, m_fY, Y);

};

#endif