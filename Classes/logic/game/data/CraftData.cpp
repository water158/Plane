#include "CraftData.h"


CraftData::CraftData()
{
    m_pShapesData = NULL;
	m_pCollisionsData = NULL;
	m_pArrayWeaponData = NULL;
	m_pBatteryPositionData = NULL;
    m_connonType = 0;
    m_pMisc = NULL;
}

CraftData::~CraftData()
{
   
}

CraftData* CraftData::create(TiXmlElement *elm)
{
    CraftData *data = new CraftData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}
	return NULL;
}

bool CraftData::loadData(TiXmlElement *elm)
{
    if (!elm)
    {
        return false;
    }

    TiXmlElement* elmData = elm->FirstChildElement();
    
	//加载飞机形态数据
	this->loadShapesData(elmData);
	elmData = elmData->NextSiblingElement();

	//加载飞机碰撞数据
	this->loadCollisionsData(elmData);
	elmData = elmData->NextSiblingElement();

	//加载武器数据
	if(elmData)
	{
		this->loadWeaponData(elmData);
		elmData = elmData->NextSiblingElement();
	}

	//加载炮管位置数据
	if(elmData)
	{
		this->loadBatteryPositionData(elmData);
        elmData = elmData->NextSiblingElement();
	}
    
    if (elmData) {
        this->loadMisc(elmData);
    }
    
    return true;
}

//加载飞机形态数据
void CraftData::loadShapesData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("CraftData::loadShapesData  elm is null", "");
	}
	if(!m_pShapesData)
	{
		m_pShapesData = ShapesData::create(elm);
	}

}

//加载飞机碰撞数据
void CraftData::loadCollisionsData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("CraftData::loadCollisionsData  elm is null", "");
	}

	if(!m_pCollisionsData)
	{
		m_pCollisionsData = CollisionsData::create(elm);
	}
}

//加载飞机武器数据
void CraftData::loadWeaponData(TiXmlElement *elm)
{
	m_pArrayWeaponData = CCArray::create();
	m_pArrayWeaponData->retain();

	TiXmlElement *elmTarget = elm->FirstChildElement();

	while (elmTarget)
	{
		TargetGunGroupData* data = TargetGunGroupData::create(elmTarget);

		//加入到map中
		m_pArrayWeaponData->addObject(data);

		//下一个
		elmTarget = elmTarget->NextSiblingElement();
	}
}

//加载炮管位置数据
void CraftData::loadBatteryPositionData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("CraftData::loadBatteryPositionData  elm is null", "");
	}

	if(!m_pBatteryPositionData)
	{
		m_pBatteryPositionData = BatteryPositionData::create(elm);
	}
}

void CraftData::loadMisc(TiXmlElement *elm)
{
    if(!elm)
    {
        CCMessageBox("CraftData::loadMisc  elm is null", "");
    }
    
    if(!m_pMisc)
    {
        m_pMisc = MiscData::create(elm);
    }
}