#ifndef __EQUIP_ITEM_DATA_H__
#define __EQUIP_ITEM_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "FerrisTools.h"


USING_NS_CC;
using namespace std;

class EquipItemData : public CCObject
{
public:
	virtual ~EquipItemData();

	static EquipItemData* create(TiXmlElement *elm);

	//加载属性数据
	bool loadData(TiXmlElement *elm);

private:
	EquipItemData();

	//////数据属性
	//id
	CC_SYNTHESIZE (string , m_strID, ID);
	//所持这个属性的飞机ID
	CC_SYNTHESIZE (string , m_strPlaneID, PlaneID);
	//增加的攻击值
	CC_SYNTHESIZE (float , m_fAttrackValue, AttrackValue);
	//增加的血量
	CC_SYNTHESIZE (float , m_fBloodValue, BloodValue);
	//增加的攻击百分比
	CC_SYNTHESIZE (float , m_fAttrackPercentage, AttrackPercentage);
	//增加的血百分比
	CC_SYNTHESIZE (float , m_fBloodPercentage, BloodPercentage);
	//道具爆率
	CC_SYNTHESIZE (float , m_fPropExplodeRate, PropExplodeRate);
	//血量爆率
	CC_SYNTHESIZE (float , m_fBloodExplodeRate, BloodExplodeRate);
	//等级限制
	CC_SYNTHESIZE (int , m_nLevelCap, LevelCap);
	//品级
	CC_SYNTHESIZE (int , m_nQuality, Quality);
	//炮管类型(四个装备槽的类型)
	CC_SYNTHESIZE (int , m_nGunType, GunType);
	//主炮位置的具体类型
	CC_SYNTHESIZE (int , m_nMainGunType, MainGunType);
	//副炮1的类型
	CC_SYNTHESIZE (int , m_nSecondaryLeftGunType, SecondaryLeftGunType);
	//副炮2的类型
	CC_SYNTHESIZE (int , m_nSecondaryRightGunType, SecondaryRightGunType);
	//僚机的类型
	CC_SYNTHESIZE (int , m_nWingType, WingType);
};

#endif