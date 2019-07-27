//
//  UserEquip.cpp
//  ambition
//
//  Created by mini01 on 14-12-10.
//
//

#include "UserEquip.h"

UserEquip::UserEquip()
{
    m_mainGunID = "0";
    m_firSubGunID = "0";
    m_secSubGunID = "0";
    m_wingID = "0";
}

UserEquip::~UserEquip()
{

}

UserEquip* UserEquip::m_sharedInstance = NULL;

UserEquip* UserEquip::shared()
{
    if (!m_sharedInstance)
    {
        m_sharedInstance = new UserEquip();
        
        m_sharedInstance->init();
    }
    
    return m_sharedInstance;
}

void UserEquip::init()
{
    
    this->checkVersion();
}

void UserEquip::check()
{
    m_sUserEquipPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "UserEquip_out.xml";
    
    bool exsit = isFileExist(m_sUserEquipPath.c_str());
    
    if (!exsit)
    {
        copyData("common/UserEquip.xml", "UserEquip_out.xml");
    }
}

void UserEquip::checkVersion()
{
    check();
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sUserEquipPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获取根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    if(elmRoot->Attribute("version"))
    {
        this->setVersion(atoi(elmRoot->Attribute("version")));
    }
    else
    {
        //如果不存在version属性，默认为1.0版本
        this->setVersion(1);
    }
    
    this->loadDataFromVer(m_nVersion);
    
    //CCLOG("version==%f", this->getVersion());
    
    delete xmlDoc;
    
}

void UserEquip::loadDataFromVer(int version)
{
    int ver = version - 1;
    
    switch(ver)
    {
        case USERDATA_VERSION_1:
            CCLOG("version 1");
            load();
            break;
            
        case USERDATA_VERSION_2:
            CCLOG("version 2");
            break;
            
        case USERDATA_VERSION_3:
            CCLOG("version 3");
            break;
            
        case USERDATA_VERSION_4:
            CCLOG("version 4");
            break;
            
        case USERDATA_VERSION_5:
            CCLOG("version 5");
            break;
            
        default:
            load();
            break;
    }
}

void UserEquip::load()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserEquipPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //主炮id
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    this->setMainGunID(elmCursor->Attribute("equip_id"));
    
    //副炮1id
    elmCursor = elmCursor->NextSiblingElement();
    this->setFirSubGunID(elmCursor->Attribute("equip_id"));
    
    //副炮2id
    elmCursor = elmCursor->NextSiblingElement();
    this->setSecSubGunID(elmCursor->Attribute("equip_id"));
    
    //僚机id
    elmCursor = elmCursor->NextSiblingElement();
    this->setWingID(elmCursor->Attribute("equip_id"));
    
    delete xmlDoc;
}

void UserEquip::save()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserEquipPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    elmRoot->SetAttribute("version", this->getVersion());//userdata版本
    
    //主炮id
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    elmCursor->SetAttribute("equip_id", this->getMainGunID());
    
    //副炮1id
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->SetAttribute("equip_id", this->getFirSubGunID());
    
    //副炮2id
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->SetAttribute("equip_id", this->getSecSubGunID());
    
    //僚机id
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->SetAttribute("equip_id", this->getWingID());
    
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sUserEquipPath);
    
    delete xmlDoc;
}

