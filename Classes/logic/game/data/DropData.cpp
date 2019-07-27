//
//  DropData.cpp
//  ambition
//
//  Created by mini02 on 15/1/14.
//
//

#include "DropData.h"

DropData::DropData()
{
    m_pShapesData = NULL;
    m_pCollisionsData = NULL;
}

DropData::~DropData()
{
}

DropData* DropData::create(TiXmlElement *elm)
{
    DropData *data = new DropData();
    if(data && data->loadData(elm))
    {
        //data->autorelease();
        return data;
    }
    return NULL;
}

bool DropData::loadData(TiXmlElement *elm)
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
    
    return true;
}

//加载子弹形态数据
void DropData::loadShapesData(TiXmlElement *elm)
{
    if(!elm)
    {
        CCMessageBox("DropData::loadShapesData  elm is null", "");
    }
    if(!m_pShapesData)
    {
        m_pShapesData = ShapesData::create(elm);
    }
}

//加载子弹碰撞数据
void DropData::loadCollisionsData(TiXmlElement *elm)
{
    if(!elm)
    {
        CCMessageBox("DropData::loadCollisionsData  elm is null", "");
    }
    
    if(!m_pCollisionsData)
    {
        m_pCollisionsData = CollisionsData::create(elm);
    }
}
