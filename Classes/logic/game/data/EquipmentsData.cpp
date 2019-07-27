#include "EquipmentsData.h"


EquipmentsData::EquipmentsData()
{

}

EquipmentsData::~EquipmentsData()
{

}

EquipmentsData* EquipmentsData::create(TiXmlElement *elm)
{
	EquipmentsData* data = new EquipmentsData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载ShapePartData
bool EquipmentsData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{

	}

	m_pDictEquipmentsData = CCDictionary::create();
	m_pDictEquipmentsData->retain();

	TiXmlElement *elmData = elm->FirstChildElement();
	while(elmData)
	{
		EquipItemData* data = EquipItemData::create(elmData);

		m_pDictEquipmentsData->setObject(data, data->getID());

		elmData = elmData->NextSiblingElement();
	}
	
	return true;
}

//通过ID获取数据EquipItemData
EquipItemData* EquipmentsData::getEquipItemDataByKeys(string key)
{
	return (EquipItemData*)m_pDictEquipmentsData->objectForKey(key);
}