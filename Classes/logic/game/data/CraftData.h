#ifndef __CRAFT_DATA_H__
#define __CRAFT_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "ObjectData.h"
#include "ShapesData.h"
#include "CollisionsData.h"
#include "TargetPlaneData.h"
#include "TargetGunGroupData.h"
#include "BatteryPositionData.h"
#include "MiscData.h"

USING_NS_CC;
using namespace std;





class CraftData : public ObjectData
{
public:
	virtual ~CraftData();
    
    static CraftData* create(TiXmlElement *elm);
    
	virtual bool loadData(TiXmlElement *elm);

	//加载飞机形态数据
	void loadShapesData(TiXmlElement *elm);
	
	//加载飞机碰撞数据
	void loadCollisionsData(TiXmlElement *elm);

	//加载飞机武器数据
	void loadWeaponData(TiXmlElement *elm);

	//加载炮管位置数据
	void loadBatteryPositionData(TiXmlElement *elm);
    
    //加载附加数据
    void loadMisc(TiXmlElement *elm);
    
private:
    CraftData();
    
    //Type
    CC_SYNTHESIZE(int, m_nType, Type);

	//形态数据
	CC_SYNTHESIZE(ShapesData*, m_pShapesData, ShapesData);

	//碰撞数据
	CC_SYNTHESIZE(CollisionsData*, m_pCollisionsData, CollisionsData);

	//武器数据
	CC_SYNTHESIZE(CCArray*, m_pArrayWeaponData, ArrayWeaponData);
    
    

	//炮管位置数据
	CC_SYNTHESIZE(BatteryPositionData*, m_pBatteryPositionData, BatteryPositionData);
    
    //附加数据
    CC_SYNTHESIZE(MiscData*, m_pMisc, MiscData);
    
    //炮类型 用于区分 主机炮和敌机炮  以及主机的具体哪个炮
    CC_SYNTHESIZE(int, m_connonType, ConnonType);
};

#endif