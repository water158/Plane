#include "PositionData.h"


PositionData::PositionData()
{

}

PositionData::~PositionData()
{

}

PositionData* PositionData::create(TiXmlElement *elm)
{
	PositionData* pData = new PositionData();
	if(pData && pData->loadData(elm))
	{
		//pData->autorelease();
		return pData;
	}

	return NULL;
}

//加载自身数据
bool PositionData::loadData(TiXmlElement *elm)
{
	this->setID("1");
	this->setX(0.0f);
	this->setY(0.0f);

	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
        CCLog("elm id is %s",elm->Attribute("id"));
	}

	if(elm->Attribute("x"))
	{
		this->setX(atoi(elm->Attribute("x")));
        CCLog("battry %s PosX is %f",m_strID.c_str(),m_fX);
	}

	if(elm->Attribute("y"))
	{
		this->setY(atoi(elm->Attribute("y")));
        CCLog("battry %s PosY is %f",m_strID.c_str(),m_fY);
	}

	return true;
}