#include "BulletData.h"


BulletData::BulletData()
{
	m_pShapesData = NULL;
	m_pCollisionsData = NULL;
    m_pMiscData = NULL;
}

BulletData::~BulletData()
{
}

BulletData* BulletData::create(TiXmlElement *elm)
{
	BulletData *data = new BulletData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}
	return NULL;
}

bool BulletData::loadData(TiXmlElement *elm)
{
	if (!elm)
    {
        return false;
    }
    
    if (!ObjectData::loadData(elm))
    {
        return false;
    }
    
     elm = elm->FirstChildElement();
    //子元素 Parts
    if (elm)
    {
        this->loadShapesData(elm);

    }
    
    //子元素 Forms
    elm = elm->NextSiblingElement();
    if (elm)
    {
        this->loadCollisionsData(elm);
  
    }
    
    //子元素 misc
    elm = elm->NextSiblingElement();
    if (elm)
    {
        this->loadMiscData(elm);
    }
 
	return true;
}

//加载子弹形态数据
void BulletData::loadShapesData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("BulletData::loadShapesData  elm is null", "");
	}
	if(!m_pShapesData)
	{
		m_pShapesData = ShapesData::create(elm);
	}
}

//加载子弹碰撞数据
void BulletData::loadCollisionsData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("BulletData::loadCollisionsData  elm is null", "");
	}

	if(!m_pCollisionsData)
	{
		m_pCollisionsData = CollisionsData::create(elm);
	}
}

//加载杂项数据
void BulletData::loadMiscData(TiXmlElement *elm)
{
    if(!elm)
    {
        CCMessageBox("BulletData::loadMIscData  elm is null", "");
    }

    if(!m_pMiscData)
    {
        m_pMiscData = MiscData::create(elm);
    }
    
}