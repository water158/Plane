#ifndef __BULLET_DATA_H__
#define __BULLET_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"

#include "ObjectData.h"
#include "ShapesData.h"
#include "CollisionsData.h"
#include "MiscData.h"

USING_NS_CC;
using namespace std;


class BulletData : public ObjectData
{
public:
	virtual ~BulletData();
    
    static BulletData* create(TiXmlElement *elm);
    
	virtual bool loadData(TiXmlElement *elm);
    
	//加载子弹形态数据
	void loadShapesData(TiXmlElement *elm);

	//加载子弹碰撞数据
	void loadCollisionsData(TiXmlElement *elm);

    //加载附加数据
    void loadMiscData(TiXmlElement *elm);
    
private:
    BulletData();

	//id
	CC_SYNTHESIZE(string, m_strID, ID);
    
	//形态数据
	CC_SYNTHESIZE(ShapesData*, m_pShapesData, ShapesData);

	//碰撞数据
	CC_SYNTHESIZE(CollisionsData*, m_pCollisionsData, CollisionsData);
    
    //附加数据
    CC_SYNTHESIZE(MiscData*, m_pMiscData, MiscData);
};

#endif