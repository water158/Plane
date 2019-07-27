//
//  EquipNameLibrary.cpp
//  ambition
//
//  Created by mini01 on 15-2-5.
//
//

#include "EquipNameLibrary.h"


EquipNameLibrary::EquipNameLibrary()
{

}

EquipNameLibrary::~EquipNameLibrary()
{

}

EquipNameLibrary* EquipNameLibrary::s_sharedInstance = NULL;

EquipNameLibrary* EquipNameLibrary::shared()
{
    if (!s_sharedInstance)
    {
        s_sharedInstance = new EquipNameLibrary();
        
        s_sharedInstance->init();
    }
    
    return s_sharedInstance;
}

void EquipNameLibrary::init()
{
    string path = "common/equip_name.xml";
    
    dict_names = CCDictionary::create();
    
    dict_names->retain();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
    {
        if (elmCursor->Attribute("id"))
        {
            string key = elmCursor->Attribute("id");
            string value = elmCursor->Attribute("name");
            
            dict_names->setObject(CCString::create(value), key);
        }
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    delete xmlDoc;
}

CCString* EquipNameLibrary::getNameByKey(string key)
{
    CCObject* obj = dict_names->objectForKey(key);
    
    if (obj)
    {
        return (CCString*)dict_names->objectForKey(key);
    }
    
    return CCString::create("");
}