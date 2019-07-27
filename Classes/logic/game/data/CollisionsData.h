#ifndef __COLLISIONS_DATA_H__
#define __COLLISIONS_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "CollisionGroupData.h"

USING_NS_CC;
using namespace std;

class CollisionLineData : public CCObject
{
public:
    
    virtual ~CollisionLineData();
    
    CollisionLineData();
    
    static CollisionLineData* create(TiXmlElement* elm);
    
    virtual bool loadData(TiXmlElement *elm);
    
    CC_SYNTHESIZE(float,m_fLeftX,LeftX);
    
    CC_SYNTHESIZE(float,m_fLeftY,LeftY);
    
    CC_SYNTHESIZE(float,m_fRightX,RightX);
    
    CC_SYNTHESIZE(float,m_fRightY,RightY);
    
};

class CollisionsData : public CCObject
{
public:
	virtual ~CollisionsData();

	static CollisionsData* create(TiXmlElement *elm);

	//加载CollisionGroupData
	bool loadData(TiXmlElement *elm);

	//通过ID获取数据CollisionGroupData
	CollisionGroupData* getCollisionGroupDataByKeys(string key);

private:
	CollisionsData();
    
    //碰撞线数据
    CC_SYNTHESIZE(CollisionLineData*,m_pCollisionsLineData,CollisionLineData);


	CC_SYNTHESIZE (CCDictionary*, m_pDictCollisionsData, DictCollisionsData);

};

#endif