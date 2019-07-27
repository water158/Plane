#include "RemoveTargetData.h"



RemoveTargetData::RemoveTargetData(void)
{

}


RemoveTargetData::~RemoveTargetData(void)
{

}

RemoveTargetData* RemoveTargetData::create(TiXmlElement *elm)
{
	RemoveTargetData *data = new RemoveTargetData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}


bool RemoveTargetData::loadData(TiXmlElement *elm)
{
	this->setID("");
	this->setTargetPlaneID(-1);


	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}

	if(elm->Attribute("targetplane_id"))
	{
		this->setTargetPlaneID(atoi(elm->Attribute("targetplane_id")));
	}

	return true;
}