#include "BatteryPositionData.h"


BatteryPositionData::BatteryPositionData()
{

}

BatteryPositionData::~BatteryPositionData()
{

}

BatteryPositionData* BatteryPositionData::create(TiXmlElement *elm)
{
	BatteryPositionData* pData = new BatteryPositionData();
	if(pData && pData->loadData(elm))
	{
		//pData->autorelease();
		return pData;
	}

	return NULL;
}

//加载自身数据
bool BatteryPositionData::loadData(TiXmlElement *elm)
{
    this->setID(elm->Attribute("plane_id"));
    
    if (elm->Attribute("connon_num"))
    {
        this->setConnonCount(atoi(elm->Attribute("connon_num")));
    }
	
	m_pDictPositionsData = CCDictionary::create();
	m_pDictPositionsData->retain();

	TiXmlElement* elmPosition = elm->FirstChildElement();

	while (elmPosition)
	{
		PositionData* pData = PositionData::create(elmPosition);
		m_pDictPositionsData->setObject(pData, pData->getID());

		elmPosition = elmPosition->NextSiblingElement();
	}

	return true;
}