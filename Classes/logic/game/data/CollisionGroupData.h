#ifndef __COLLISION_GROUP_DATA_H__
#define __COLLISION_GROUP_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "FerrisTools.h"
#include "CollisionRectData.h"


USING_NS_CC;
using namespace std;

class CollisionGroupData : public CCObject
{
public:
	virtual ~CollisionGroupData();

	static CollisionGroupData* create(TiXmlElement *elm);

	//加载属性数据
	bool loadData(TiXmlElement *elm);

	//获取次碰撞组下的某个碰撞矩形数据
	CollisionRectData* getCollisionRectDataByKey(string key);

private:
	CollisionGroupData();

	//////数据属性
	//id
	CC_SYNTHESIZE (string , m_strID, ID);

	//碰撞组的所有矩形
	CC_SYNTHESIZE (CCDictionary* , m_pDictCollisionGroupRects, DictCollisionGroupRects);
	
};

#endif