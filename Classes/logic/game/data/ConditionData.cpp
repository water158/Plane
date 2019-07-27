#include "ConditionData.h"



ConditionData::ConditionData(void)
{

}


ConditionData::~ConditionData(void)
{
//	 CC_SAFE_RELEASE(targetPlanes);
    CC_SAFE_RELEASE(target_gungroups);
}

ConditionData* ConditionData::create(TiXmlElement *elm)
{
	ConditionData *data = new ConditionData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}

bool ConditionData::init(TiXmlElement *elm)
{
	this->setID("");

	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}

	loadData(elm);

	return true;
}


bool ConditionData::loadData(TiXmlElement *elm)
{
//	targetPlanes = CCDictionary::create();
//	targetPlanes->retain();
    
    target_gungroups = CCArray::create();
    target_gungroups->retain();

	TiXmlElement *elmPlane = elm->FirstChildElement();
	while (elmPlane)
	{
        TargetGunGroupData* data = TargetGunGroupData::create(elmPlane);
//        targetPlanes->setObject(data,data->getID());
        
        target_gungroups->addObject(data);

		elmPlane = elmPlane->NextSiblingElement();
	}

	return true;
}

//ConditionTargetData* ConditionData::getConditionTargetData(string key)
//{
//	return (ConditionTargetData*)targetPlanes->objectForKey(key);
//}

//TargetGunGroupData* ConditionData::getTargetGunGroupData(string key)
//{
//    return (TargetGunGroupData*)targetPlanes->objectForKey(key);
//}