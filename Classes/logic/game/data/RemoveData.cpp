#include "RemoveData.h"



RemoveData::RemoveData(void)
{

}


RemoveData::~RemoveData(void)
{
	CC_SAFE_RELEASE(targetPlanes);
}

RemoveData* RemoveData::create1(TiXmlElement *elm)
{
	RemoveData *data = new RemoveData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}

bool RemoveData::init(TiXmlElement *elm)
{
	this->setID("");

	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}

	loadData(elm);

	return true;
}


bool RemoveData::loadData(TiXmlElement *elm)
{
	targetPlanes = CCDictionary::create();
	targetPlanes->retain();

	TiXmlElement *elmPlane = elm->FirstChildElement();
	while (elmPlane)
	{
		RemoveTargetData* data = RemoveTargetData::create(elmPlane);

		targetPlanes->setObject(data,data->getID());

		elmPlane = elmPlane->NextSiblingElement();
	}

	return true;
}

RemoveTargetData* RemoveData::getRemoveTargetData(string key)
{
	return (RemoveTargetData*)targetPlanes->objectForKey(key);
}