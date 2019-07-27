#include "BatteryData.h"


BatteryData::BatteryData()
{
}

BatteryData::~BatteryData()
{
}

BatteryData* BatteryData::create(TiXmlElement *elm)
{
	BatteryData *data = new BatteryData();

	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}

bool BatteryData::loadData(TiXmlElement *elm)
{
	//属性
	this->setID(elm->Attribute("id"));
	this->setType("101");
	this->setAutomaticRotate(false);
	this->setRecoil(false);
	this->setX(0.0f);
	this->setY(0.0f);
	this->setScale(1.0f);
	this->setSpeed(90.0f);

	//id
	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}
	
	//类型
	if(elm->Attribute("type"))
	{
		this->setType(elm->Attribute("type"));
	}

	//自动转向
	if(elm->Attribute("is_rotate"))
	{
		this->setAutomaticRotate(atoi(elm->Attribute("is_rotate")) == 0 ? false : true);
	}

	//是否有后坐力
	if(elm->Attribute("recoil"))
	{
		this->setRecoil(atoi(elm->Attribute("recoil")) == 0 ? false : true);
	}

	//相对位置
	if(elm->Attribute("x"))
	{
		this->setX(atof(elm->Attribute("x")));
	}
	if(elm->Attribute("y"))
	{
		this->setY(atof(elm->Attribute("y")));
	}

	//炮台缩放
	if(elm->Attribute("scale"))
	{
		this->setScale(atof(elm->Attribute("scale")));
	}

	//炮台角速度
	if(elm->Attribute("speed"))
	{
		this->setSpeed(atof(elm->Attribute("speed")));
	}

	return true;
}