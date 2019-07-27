//
//  StorePriceData.cpp
//  ambition
//
//  Created by mini01 on 15-1-7.
//
//

#include "StorePriceData.h"

PriceData::PriceData()
{
    m_id = "0";
    m_value = 0;
    m_consume = 0;
    m_desLabel = "0";
}

PriceData::~PriceData()
{
    
}

PriceData* PriceData::create(TiXmlElement *elm)
{
    PriceData* data = new PriceData();
    
    if (data && data->init(elm))
    {
        data->autorelease();
        return data;
    }
    
    return NULL;
}

bool PriceData::init(TiXmlElement *elm)
{
    if (!CCNode::init() || !elm)
    {
        return false;
    }
    
    if (elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }
    
    if (elm->Attribute("value"))
    {
        this->setValue(atoi(elm->Attribute("value")));
    }
    
    if (elm->Attribute("consume"))
    {
        this->setConsume(atoi(elm->Attribute("consume")));
    }
    
    if (elm->Attribute("label"))
    {
        this->setDesLabel(elm->Attribute("label"));
    }
    
    return true;
}


PriceDataManager::PriceDataManager()
{

}

PriceDataManager::~PriceDataManager()
{
    CC_SAFE_DELETE(m_dcit);
}

PriceDataManager* PriceDataManager::m_shared = NULL;

PriceDataManager* PriceDataManager::shared()
{
    if (!m_shared)
    {
        m_shared = new PriceDataManager();
        
        m_shared->init();
    }
    
    return m_shared;
}

void PriceDataManager::init()
{
    m_dcit = CCDictionary::create();
    
    m_dcit->retain();
    
    string strPath = "ui/store/price.xml";
    
    unsigned long nLength = 0;
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "rt", &nLength);
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
    {
        PriceData* item = PriceData::create(elmCursor);
        
        m_dcit->setObject(item, item->getID());
        
        elmCursor = elmCursor->NextSiblingElement();
    }
}

PriceData* PriceDataManager::getPriceData(string data_id)
{
    return (PriceData*)m_dcit->objectForKey(data_id);
}


