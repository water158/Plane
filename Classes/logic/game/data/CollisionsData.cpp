#include "CollisionsData.h"



CollisionLineData::CollisionLineData()
{
    m_fLeftX = 0.0f;
    
    m_fLeftY = 0.0f;
    
    m_fRightX = 0.0f;
    
    m_fRightY = 0.0f;
}

CollisionLineData::~CollisionLineData()
{
    
}

CollisionLineData* CollisionLineData::create(TiXmlElement* elm)
{
    CollisionLineData* data = new CollisionLineData();
    
    if (data && data->loadData(elm))
    {
        return data;
    }
    
    return NULL;
}


bool CollisionLineData::loadData(TiXmlElement *elm)
{
    if (!elm)
    {
        return false;
    }
    
    if (elm->Attribute("left_x"))
    {
        this->setLeftX(atof(elm->Attribute("left_x")));
    }
    
    if (elm->Attribute("left_y"))
    {
        this->setLeftY(atof(elm->Attribute("left_y")));
    }
    
    if (elm->Attribute("right_x"))
    {
        this->setRightX(atof(elm->Attribute("right_x")));
    }
    
    if (elm->Attribute("right_y"))
    {
        this->setRightY(atof(elm->Attribute("right_y")));
    }
    
    return true;
}


CollisionsData::CollisionsData()
{
    m_pCollisionsLineData = NULL;
}

CollisionsData::~CollisionsData()
{

}

CollisionsData* CollisionsData::create(TiXmlElement *elm)
{
	CollisionsData* pData = new CollisionsData();
	if(pData && pData->loadData(elm))
	{
		return pData;
	}

	return NULL;
}

//加载CollisionGroupData
bool CollisionsData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("CollisionsData::loadData is null", "");
		return false;
	}
    
    //加载碰撞线的位置
    if (elm)
    {
        this->setCollisionLineData(CollisionLineData::create(elm));
    }

	m_pDictCollisionsData = CCDictionary::create();
	m_pDictCollisionsData->retain();

	TiXmlElement *elmGroup = elm->FirstChildElement();

	while (elmGroup)
	{
		CollisionGroupData* pGroupData = CollisionGroupData::create(elmGroup);

		m_pDictCollisionsData->setObject(pGroupData, pGroupData->getID());

		elmGroup = elmGroup->NextSiblingElement();
	}
    
	return true;
}

//通过ID获取数据CollisionGroupData
CollisionGroupData* CollisionsData::getCollisionGroupDataByKeys(string key)
{
	return (CollisionGroupData*)m_pDictCollisionsData->objectForKey(key);
}