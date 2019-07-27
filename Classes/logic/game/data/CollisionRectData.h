#ifndef __COLLISION_RECT_DATA_H__
#define __COLLISION_RECT_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "FerrisTools.h"


USING_NS_CC;
using namespace std;

class CollisionRectData : public CCObject
{
public:
	virtual ~CollisionRectData();

	static CollisionRectData* create(TiXmlElement *elm);

	//加载属性数据
	bool loadData(TiXmlElement *elm);

private:
	CollisionRectData();

	//////数据属性
	//id
	CC_SYNTHESIZE (string , m_strID, ID);
	//x
	CC_SYNTHESIZE (float , m_fX, X);
	//y
	CC_SYNTHESIZE (float , m_fY, Y);
	//w
	CC_SYNTHESIZE (float , m_fWidth, Width);
	//h
	CC_SYNTHESIZE (float , m_fHeight, Height);
};

#endif