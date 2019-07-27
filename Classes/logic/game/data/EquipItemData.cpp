#include "EquipItemData.h"

EquipItemData::EquipItemData()
{

}

EquipItemData::~EquipItemData()
{

}

EquipItemData* EquipItemData::create(TiXmlElement *elm)
{
	EquipItemData* data = new EquipItemData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载属性数据
bool EquipItemData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{

	}

	this->setID("1");
	this->setPlaneID("101");
	this->setAttrackValue(0.0f);
	this->setBloodValue(0.0f);
	this->setAttrackPercentage(0.0f);
	this->setBloodPercentage(0.0f);
	this->setPropExplodeRate(0.0f);
	this->setBloodExplodeRate(0.0f);
	this->setLevelCap(10);
	this->setQuality(EQUIP_QUALITY_WHITE);
	this->setGunType(EQUIPMENT_TYPE_MAIN_GUN);
	this->setMainGunType(MAINGUN_SKILL_TYPE_SANDS_OF_TIME);
	this->setSecondaryLeftGunType(-1);
	this->setSecondaryRightGunType(-1);
	this->setWingType(-1);


	//id
	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}

	//所持这个属性的飞机ID
	if(elm->Attribute("plane_id"))
	{
		this->setPlaneID(elm->Attribute("plane_id"));
	}

	//增加的攻击值
	if(elm->Attribute("attrack_value"))
	{
		this->setAttrackValue(atof(elm->Attribute("attrack_value")));
	}

	//增加的血量
	if(elm->Attribute("blood_value"))
	{
		this->setBloodValue(atof(elm->Attribute("blood_value")));
	}

	//增加的攻击百分比
	if(elm->Attribute("attrack_percentage"))
	{
		this->setAttrackPercentage(atof(elm->Attribute("attrack_percentage")));
	}

	//增加的血百分比
	if(elm->Attribute("blood_percentage"))
	{
		this->setBloodPercentage(atof(elm->Attribute("blood_percentage")));
	}

	//道具爆率
	if(elm->Attribute("props_explode_rate"))
	{
		this->setPropExplodeRate(atof(elm->Attribute("props_explode_rate")));
	}

	//血量爆率
	if(elm->Attribute("blood_explode_rate"))
	{
		this->setBloodExplodeRate(atof(elm->Attribute("blood_explode_rate")));
	}

	//等级限制
	if(elm->Attribute("level_cap"))
	{
		this->setLevelCap(atoi(elm->Attribute("level_cap")));
	}

	//品级
	if(elm->Attribute("quality"))
	{
		this->setQuality(atoi(elm->Attribute("quality")));
	}

	//炮管类型(四个装备槽的类型)
	if(elm->Attribute("gun_type"))
	{
		this->setGunType(atoi(elm->Attribute("gun_type")));
	}

	//主炮位置的具体类型
	if(elm->Attribute("main_gun_type"))
	{
		this->setMainGunType(atoi(elm->Attribute("main_gun_type")));
	}

	//副炮1的类型
	if(elm->Attribute("secondary_left_gun_type"))
	{
		this->setSecondaryLeftGunType(atoi(elm->Attribute("secondary_left_gun_type")));
	}

	//副炮2的类型
	if(elm->Attribute("secondary_right_gun_type"))
	{
		this->setSecondaryRightGunType(atoi(elm->Attribute("secondary_right_gun_type")));
	}

	//僚机的类型
	if(elm->Attribute("wing_type"))
	{
		this->setWingType(atoi(elm->Attribute("wing_type")));
	}

	return true;
}