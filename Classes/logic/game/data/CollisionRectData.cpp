#include "CollisionRectData.h"

CollisionRectData::CollisionRectData()
{

}

CollisionRectData::~CollisionRectData()
{

}

CollisionRectData* CollisionRectData::create(TiXmlElement *elm)
{
	CollisionRectData* data = new CollisionRectData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载属性数据
bool CollisionRectData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("CollisonRectData::loadData  elm is null", "");
	}

	this->setID("1");
	this->setX(0.0f);
	this->setY(0.0f);
	this->setWidth(0.0f);
	this->setHeight(0.0f);

	//id
	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}

	//X
	if(elm->Attribute("x"))
	{
		this->setX(getFloatFromS(elm->Attribute("x")));
	}

	//Y
	if(elm->Attribute("y"))
	{
		this->setY(getFloatFromS(elm->Attribute("y")));
	}

	//w
	if(elm->Attribute("w"))
	{
		this->setWidth(getFloatFromS(elm->Attribute("w")));
	}

	//h
	if(elm->Attribute("h"))
	{
		this->setHeight(getFloatFromS(elm->Attribute("h")));
	}

	return true;
}