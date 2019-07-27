#include "ConditionTargetData.h"



ConditionTargetData::ConditionTargetData(void)
{

}


ConditionTargetData::~ConditionTargetData(void)
{

}

ConditionTargetData* ConditionTargetData::create(TiXmlElement *elm)
{
	ConditionTargetData *data = new ConditionTargetData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}


bool ConditionTargetData::loadData(TiXmlElement *elm)
{
	this->setID("");
	this->setTargetPlaneID(-1);
	this->setHP(0);
	this->setRepeatTimes(0);


	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}

	if(elm->Attribute("hp"))
	{
		this->setHP(atoi(elm->Attribute("hp")));
	}

	if(elm->Attribute("targetplane_id"))
	{
		this->setTargetPlaneID(atoi(elm->Attribute("targetplane_id")));
	}

	if(elm->Attribute("repeat_times"))
	{
		this->setRepeatTimes(atoi(elm->Attribute("repeat_times")));
	}

	return true;
}