//
//  UserEquipBag.cpp
//  ambition
//
//  Created by mini01 on 14-12-10.
//
//

#include "UserEquipBag.h"


UserEquipBag::UserEquipBag()
{

}

UserEquipBag::~UserEquipBag()
{

}

UserEquipBag* UserEquipBag::s_sharedUserEquipBag = NULL;

UserEquipBag* UserEquipBag::shared()
{
    if (!s_sharedUserEquipBag)
    {
        s_sharedUserEquipBag = new UserEquipBag();
        s_sharedUserEquipBag->init();
    }
    
    return s_sharedUserEquipBag;
}

void UserEquipBag::init()
{
    m_dictEquipBag = CCDictionary::create();
    
    m_dictEquipBag->retain();
    
    m_arrEquipGetRecent = CCArray::create();
    
    m_arrEquipGetRecent->retain();
    
    this->checkVersion();
}

void UserEquipBag::check()
{
    m_sUserDataPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "UserEquipBag_out.xml";
    
    bool exsit = isFileExist(m_sUserDataPath.c_str());
    
    if(!exsit)
    {
        copyData("common/UserEquipBag.xml", "UserEquipBag_out.xml");
    }
}

void UserEquipBag::checkVersion()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLongth = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLongth);
    xmlDoc->LoadMemory(pBuff, nLongth);
    
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

void UserEquipBag::loadDataFromVer(int version)
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

void UserEquipBag::load()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
    {
        EquipItem* item = EquipItem::create(elmCursor);
        
        m_dictEquipBag->setObject(item, item->getID());
        
        elmCursor = elmCursor->NextSiblingElement();
    }
}

void UserEquipBag::deletEquipByID(string equip_id)
{
    
    EquipItem* item = (EquipItem*)m_dictEquipBag->objectForKey(equip_id);
    
    if (!item)
    {
        return;
    }
    
    switch (item->getEquipType())
    {
        case 1:    //主炮
            UserEquip::shared()->setMainGunID("0");
            break;
            
        case 2:    //边跑1
            UserEquip::shared()->setFirSubGunID("0");
            break;
            
        case 3:    //边跑2
            UserEquip::shared()->setSecSubGunID("0");
            break;
            
        case 4:    //僚机
            UserEquip::shared()->setWingID("0");
            break;
        default:
            break;
    }
    
    UserEquip::shared()->save();
    
    m_dictEquipBag->removeObjectForKey(equip_id);
    
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
    {
        if (compareString(equip_id.c_str(), elmCursor->Attribute("id")))
        {
            elmRoot->RemoveChild(elmCursor);
            
            break;
        }
        
        elmCursor = elmCursor->NextSiblingElement();
    }
}

//获取最小id
int UserEquipBag::getMinID()
{
    //关于id 重复 和 最小id查找问题
    
    int min_id = 0;
    
    for (int i = 1; ; i++)
    {
        string str = getStringFromInt(i);
        
        EquipItem* item = (EquipItem*)m_dictEquipBag->objectForKey(str);
        
        if (!item)
        {
            min_id = i;
            
            break;
        }
    }
    
    return min_id;
}

void UserEquipBag::addEquip(EquipItem *item)
{
    CCLog("item id : %s",item->getID().c_str());
    
    m_dictEquipBag->setObject(item, item->getID());
    
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    TiXmlElement *newElm = new TiXmlElement("EquipItem");
    newElm->SetAttribute("id", item->getID());
    newElm->SetAttribute("equip_type",getStringFromInt(item->getEquipType()));
    newElm->SetAttribute("img_id", item->getImgName());
    newElm->SetAttribute("name", item->getName());
    newElm->SetAttribute("attack", getStringFromInt(item->getAttack()));
    newElm->SetAttribute("blood", getStringFromInt(item->getHp()));
    newElm->SetAttribute("attack_add", getStringFromInt(item->getAttackAdd()));
    newElm->SetAttribute("blood_add", getStringFromInt(item->getHpAdd()));
    newElm->SetAttribute("tools_drop_rate", getStringFromInt(item->getToolsDrop()));
    newElm->SetAttribute("blood_drop_rate", getStringFromInt(item->getBloodDrop()));
    newElm->SetAttribute("level", getStringFromInt(item->getLevel()));
    newElm->SetAttribute("level_limit", getStringFromInt(item->getLevelLimit()));
    newElm->SetAttribute("quality", getStringFromInt(item->getGrade()));
    newElm->SetAttribute("skill_id", getStringFromInt(item->getSkillID()));
    newElm->SetAttribute("is_using", getStringFromInt(item->getIsEquiped() ? 1 : 0));
    newElm->SetAttribute("ext_att_1", item->getExtAtt1Name());
    newElm->SetAttribute("ext_att_2", item->getExtAtt2Name());
    
    elmRoot->LinkEndChild(newElm);
    
    xmlDoc->SaveFile(m_sUserDataPath);
}

EquipItem* UserEquipBag::getEquipById(string id)
{
    EquipItem* item = (EquipItem*)m_dictEquipBag->objectForKey(id);
    
    if (!item)
    {
        //CCMessageBox("UserEquipBag::getEquipById is null", "error");
    }
    
    return item;
}

void UserEquipBag::useEquip(EquipItem *item)
{
    if (!item)
    {
        //CCMessageBox("Equip is NULL", "error");
    }
    
    item->setIsEquiped(true);
}

void UserEquipBag::unUseEquip(EquipItem *item)
{
    if (!item)
    {
        //CCMessageBox("Equip is NULL", "error");
    }
    
    item->setIsEquiped(false);
}

void UserEquipBag::swapEquip(EquipItem *item1, EquipItem *item2)
{
    if (!item1 || !item2)
    {
        //CCMessageBox("Equip is NULL", "error");
    }
    
    bool isUsingItem1 = item1->getIsEquiped();
    bool isUsingItem2 = item2->getIsEquiped();
    
    item1->setIsEquiped(isUsingItem2);
    item2->setIsEquiped(isUsingItem1);
}

void UserEquipBag::dressEquipToPos(string equip_id, int pos)
{
    EquipItem* item = getEquipById(equip_id);
    
    if (!item)
    {
        return;
    }
    
    //如果该装备是原来身上的装备 则将原来的对应位置的装备id置为0
    switch (item->getEquipType())
    {
        case EQUIPMENT_TYPE_MAIN_GUN:
            UserEquip::shared()->setMainGunID("0");
            break;
            
        case EQUIPMENT_TYPE_LEFT_GUN:
            UserEquip::shared()->setFirSubGunID("0");
            break;
            
        case EQUIPMENT_TYPE_RIGHT_GUN:
            UserEquip::shared()->setSecSubGunID("0");
            break;
            
        case EQUIPMENT_TYPE_WING:
            UserEquip::shared()->setWingID("0");
            break;
            
        default:
            break;
    }
    
    item->setEquipType(pos);
    
    switch (pos)
    {
        case EQUIPMENT_TYPE_MAIN_GUN:
            UserEquip::shared()->setMainGunID(item->getID());
            break;
        
        case EQUIPMENT_TYPE_LEFT_GUN:
            UserEquip::shared()->setFirSubGunID(item->getID());
            break;
        
        case EQUIPMENT_TYPE_RIGHT_GUN:
            UserEquip::shared()->setSecSubGunID(item->getID());
            break;
            
        case EQUIPMENT_TYPE_WING:
            UserEquip::shared()->setWingID(item->getID());
            break;
            
        default:
            break;
    }
    
    UserEquip::shared()->save();
}

void UserEquipBag::swapEquip(string equip1, string equip2)
{
    if (compareString(equip1.c_str(), "0") || compareString(equip2.c_str(), "0"))
    {
        return;
    }

    EquipItem* item1 = getEquipById(equip1);
    EquipItem* item2 = getEquipById(equip2);
//    
//    bool isUsingItem1 = item1->getIsEquiped();
//    bool isUsingItem2 = item2->getIsEquiped();
    
    int posid1 = item1->getEquipType();
    int posid2 = item2->getEquipType();
    
//    item1->setIsEquiped(isUsingItem2);
//    item2->setIsEquiped(isUsingItem1);
    item1->setEquipType(posid2);
    item2->setEquipType(posid1);
    
    CCLog("item1 pos:%d,item2 pos: %d",item1->getEquipType(),item2->getEquipType());

    
    //if (item1->getIsEquiped())
    {
        switch (item1->getEquipType())
        {
            //主炮位置
            case 1:
                UserEquip::shared()->setMainGunID(item1->getID());
                break;
            //副炮1位置
            case 2:
                UserEquip::shared()->setFirSubGunID(item1->getID());
                break;
            //副炮2位子
            case 3:
                UserEquip::shared()->setSecSubGunID(item1->getID());
                break;
            case 4: //僚机位置
                UserEquip::shared()->setWingID(item1->getID());
                break;
            default:
                break;
        }
    }
    
    //if (item2->getIsEquiped())
    {
        switch (item2->getEquipType())
        {
                //主炮位置
            case 1:
                UserEquip::shared()->setMainGunID(item2->getID());
                break;
                //副炮1位置
            case 2:
                UserEquip::shared()->setFirSubGunID(item2->getID());
                break;
                //副炮2位子
            case 3:
                UserEquip::shared()->setSecSubGunID(item2->getID());
                break;
            case 4:    //僚机位置
                UserEquip::shared()->setWingID(item2->getID());
                break;
            default:
                break;
        }
    }
    
    //this->resetUserEquip();
    
    UserEquip::shared()->save();
    
    save();
}

void UserEquipBag::resetUserEquip()
{
    string equip_id_main = "0";
    string equip_id_side1 = "0";
    string equip_id_side2 = "0";
    string equip_id_wing = "0";
    
    CCDictElement* elm;
    
    CCDICT_FOREACH(m_dictEquipBag, elm)
    {
        EquipItem* item = (EquipItem*)elm->getObject();
        
        switch (item->getEquipType())
        {
            case 1:      //主炮
                equip_id_main = item->getID();
                break;
                
            case 2:      //边炮1
                equip_id_side1 = item->getID();
                break;
                
            case 3:      //边炮2
                equip_id_side2 = item->getID();
                break;
                
            case 4:      //僚机
                equip_id_wing = item->getID();
                break;
                
            default:
                break;
        }
    }
    
    UserEquip::shared()->setMainGunID(equip_id_main);
    UserEquip::shared()->setFirSubGunID(equip_id_side1);
    UserEquip::shared()->setSecSubGunID(equip_id_side2);
    UserEquip::shared()->setWingID(equip_id_wing);
    
    CCLog("MainGun equip id : %s",UserEquip::shared()->getMainGunID().c_str());
    CCLog("Wing equip id : %s",UserEquip::shared()->getWingID().c_str());
    
}


void UserEquipBag::save()
{
    check();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(m_sUserDataPath.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    elmRoot->SetAttribute("version", this->getVersion());//userdata版本
    
    //获取第一个元素
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    CCDictElement* elm = NULL;
    
    CCDICT_FOREACH(m_dictEquipBag, elm)
    {
        EquipItem* item = (EquipItem*)elm->getObject();
        
        if (item && elmCursor)
        {
            elmCursor->SetAttribute("id", item->getID());
            elmCursor->SetAttribute("equip_type",getStringFromInt(item->getEquipType()));
            elmCursor->SetAttribute("img_id", item->getImgName());
            elmCursor->SetAttribute("name", item->getName());
            elmCursor->SetAttribute("attack", getStringFromInt(item->getAttack()));
            elmCursor->SetAttribute("blood", getStringFromInt(item->getHp()));
            elmCursor->SetAttribute("attack_add", getStringFromInt(item->getAttackAdd()));
            elmCursor->SetAttribute("blood_add", getStringFromInt(item->getHpAdd()));
            elmCursor->SetAttribute("tools_drop_rate", getStringFromInt(item->getToolsDrop()));
            elmCursor->SetAttribute("blood_drop_rate", getStringFromInt(item->getBloodDrop()));
            elmCursor->SetAttribute("level", getStringFromInt(item->getLevel()));
            elmCursor->SetAttribute("level_limit", getStringFromInt(item->getLevelLimit()));
            elmCursor->SetAttribute("quality", getStringFromInt(item->getGrade()));
            elmCursor->SetAttribute("skill_id", getStringFromInt(item->getSkillID()));
            elmCursor->SetAttribute("ext_att_1", getStringFromInt(item->getExtAtt1()));
            elmCursor->SetAttribute("ext_att_2", getStringFromInt(item->getExtAtt2()));
            
            elmCursor = elmCursor->NextSiblingElement();
        }
    }
    
    //为加密做准备
    string strContent;
    xmlDoc->Serialize(strContent);
    
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, m_sUserDataPath);
    
    delete xmlDoc;
}


CCDictionary* UserEquipBag::getDicEquip()
{
    return m_dictEquipBag;
}

EquipItem* UserEquipBag::getWingEquip()
{
    return (EquipItem*)m_dictEquipBag->objectForKey(UserEquip::shared()->getWingID());
}

EquipItem* UserEquipBag::getMainEquip()
{
    return (EquipItem*)m_dictEquipBag->objectForKey(UserEquip::shared()->getMainGunID());
}

EquipItem* UserEquipBag::getFirSubEquip()
{
    return (EquipItem*)m_dictEquipBag->objectForKey(UserEquip::shared()->getFirSubGunID());
}

EquipItem* UserEquipBag::getSecSubEquip()
{
    return (EquipItem*)m_dictEquipBag->objectForKey(UserEquip::shared()->getSecSubGunID());
}

/****************获取玩家所有装备属性值*****************/
//获取生命值
int UserEquipBag::getTotalHp()
{
    int total_hp = 0;
    
    
    //获取主炮数值
    if (getMainEquip())
    {
        total_hp += getMainEquip()->getHp() * getMainEquip()->getLevel();
    }
    
    //获取副炮一数值
    if (getEquipById(UserEquip::shared()->getFirSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getFirSubGunID());
        
        total_hp += item->getHp() * item->getLevel();
    }
    
    //获取副炮二数值
    if (getEquipById(UserEquip::shared()->getSecSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getSecSubGunID());
        
        total_hp += item->getHp() * item->getLevel();
    }
    
    //获取僚机数值
    if (getWingEquip())
    {
        total_hp += getWingEquip()->getHp() * getWingEquip()->getLevel();
    }
    
    return total_hp;
}

//获取攻击值
int UserEquipBag::getTotalAttack()
{
    int total_attack = 0;
    
    
    //获取主炮数值
    if (getMainEquip())
    {
        total_attack += getMainEquip()->getAttack() * getMainEquip()->getLevel();
    }
    
    //获取副炮一数值
    if (getEquipById(UserEquip::shared()->getFirSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getFirSubGunID());
        
        total_attack += item->getAttack() * item->getLevel();
    }
    
    //获取副炮二数值
    if (getEquipById(UserEquip::shared()->getSecSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getSecSubGunID());
        
        total_attack += item->getAttack() * item->getLevel();
    }
    
    //获取僚机数值
    if (getWingEquip())
    {
        total_attack += getWingEquip()->getAttack() * getWingEquip()->getLevel();
    }
    
    return total_attack;
}

//获取血量掉率
int UserEquipBag::getTotalDropBlood()
{
    int total_dropBlood = 0;
    
    
    //获取主炮数值
    if (getMainEquip())
    {
        total_dropBlood += getMainEquip()->getBloodDrop();
    }
    
    //获取副炮一数值
    if (getEquipById(UserEquip::shared()->getFirSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getFirSubGunID());
        
        total_dropBlood += item->getBloodDrop() * 0.6f;
    }
    
    //获取副炮二数值
    if (getEquipById(UserEquip::shared()->getSecSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getSecSubGunID());
        
        total_dropBlood += item->getBloodDrop() * 0.5f;
    }
    
    //获取僚机数值
    if (getWingEquip())
    {
        total_dropBlood += getWingEquip()->getBloodDrop() * 0.33f;
    }
    
    return total_dropBlood;
}

//获取道具掉率
int UserEquipBag::getTotalDropTool()
{
    int total_dropTool = 0;
    
    
    //获取主炮数值
    if (getMainEquip())
    {
        total_dropTool += getMainEquip()->getToolsDrop();
    }
    
    //获取副炮一数值
    if (getEquipById(UserEquip::shared()->getFirSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getFirSubGunID());
        
        total_dropTool += item->getToolsDrop() * 0.6f;
    }
    
    //获取副炮二数值
    if (getEquipById(UserEquip::shared()->getSecSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getSecSubGunID());
        
        total_dropTool += item->getToolsDrop() * 0.5f;
    }
    
    //获取僚机数值
    if (getWingEquip())
    {
        total_dropTool += getWingEquip()->getToolsDrop() * 0.33f;
    }
    
    return total_dropTool;
}

//获取攻击加成
int UserEquipBag::getTotalAttackAdd()
{
    int total_attackAdd = 0;
    
    
    //获取主炮数值
    if (getMainEquip())
    {
        total_attackAdd += getMainEquip()->getAttackAdd();
    }
    
    //获取副炮一数值
    if (getEquipById(UserEquip::shared()->getFirSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getFirSubGunID());
        
        total_attackAdd += item->getAttackAdd();
    }
    
    //获取副炮二数值
    if (getEquipById(UserEquip::shared()->getSecSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getSecSubGunID());
        
        total_attackAdd += item->getAttackAdd();
    }
    
    //获取僚机数值
    if (getWingEquip())
    {
        total_attackAdd += getWingEquip()->getAttackAdd();
    }
    
    return total_attackAdd;

}

//获取血量加成
int UserEquipBag::getTotalHpAdd()
{
    int total_hpAdd = 0;
    
    
    //获取主炮数值
    if (getMainEquip())
    {
        total_hpAdd += getMainEquip()->getHpAdd();
    }
    
    //获取副炮一数值
    if (getEquipById(UserEquip::shared()->getFirSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getFirSubGunID());
        
        total_hpAdd += item->getHpAdd() * 0.6f;
    }
    
    //获取副炮二数值
    if (getEquipById(UserEquip::shared()->getSecSubGunID()))
    {
        EquipItem* item = getEquipById(UserEquip::shared()->getSecSubGunID());
        
        total_hpAdd += item->getHpAdd() * 0.5f;
    }
    
    //获取僚机数值
    if (getWingEquip())
    {
        total_hpAdd += getWingEquip()->getHpAdd() * 0.33f;
    }
    
    return total_hpAdd;
}

//获取装备的图标路径
string UserEquipBag::getIconPath(EquipItem *item)
{
    if (item)
    {
        string strPath = "equip_";
        
        strPath.append(item->getImgName().c_str());
        
        switch (item->getGrade())
        {
            case EQUIP_QUALITY_WHITE:
                strPath.append("_white.png");
                break;
                
            case EQUIP_QUALITY_GREEN:
                strPath.append("_green.png");
                break;
                
            case EQUIP_QUALITY_BLUE:
                strPath.append("_blue.png");
                break;
                
            case EQUIP_QUALITY_PURPLE:
                strPath.append("_purple.png");
                break;
                
            case EQUIP_QUALITY_ORRANGE:
                strPath.append("_orange.png");
                break;
                
            default:
                break;
        }
        
        CCLog("strPath : %s",strPath.c_str());
        
        return strPath;
    }
    
    return "0";
}

void UserEquipBag::quick_sortArr(cocos2d::CCArray *arr,int low,int high)
{
    if (low >= high)
    {
        return;
    }
    
    int i,j;
    EquipItem* itemFlag = NULL;
    
    if (low < high)
    {
        i = low;
        j = high;
        
        itemFlag = (EquipItem*)arr->objectAtIndex(low);
        
        CCLog("itemFlag --- level limit : %d",itemFlag->getLevelLimit());
    }
    
    while (i < j)
    {
        EquipItem* itemLast = (EquipItem*)arr->objectAtIndex(j);
        
        CCLog("itemLast --- level limit : %d",itemLast->getLevelLimit());
        while (i < j && itemLast->getLevelLimit() >= itemFlag->getLevelLimit())
        {
            --j;
        }
        
        if (i < j)
        {
            EquipItem* item_i = (EquipItem*)arr->objectAtIndex(i);
            
            CCLog("item_i --- level limit : %d",item_i->getLevelLimit());
            EquipItem* item_j = (EquipItem*)arr->objectAtIndex(j);
            
            CCLog("item_j --- level limit : %d",item_j->getLevelLimit());
            item_i = item_j;
            
            CCLog("item_i --- level limit : %d",item_i->getLevelLimit());
            CCLog("item_j --- level limit : %d",item_j->getLevelLimit());
            
            i++;
        }
        
        CCObject* obj = NULL;
        CCARRAY_FOREACH(arr, obj)
        {
            EquipItem* item = (EquipItem*)obj;
            
            CCLog("item---- level limit : %d",item->getLevelLimit());
        }
        
        EquipItem* itemPre = (EquipItem*)arr->objectAtIndex(i);
        CCLog("itemPre --- level limit : %d",itemPre->getLevelLimit());
        while (i < j && itemPre->getLevelLimit() <= itemFlag->getLevelLimit())
        {
            ++i;
        }
        
        if (i < j)
        {
            EquipItem* item_i = (EquipItem*)arr->objectAtIndex(i);
            CCLog("item_i --- level limit : %d",item_i->getLevelLimit());
            EquipItem* item_j = (EquipItem*)arr->objectAtIndex(j);
            CCLog("item_j --- level limit : %d",item_j->getLevelLimit());
            
            item_j = item_i;
            CCLog("item_i --- level limit : %d",item_i->getLevelLimit());
            CCLog("item_j --- level limit : %d",item_j->getLevelLimit());
            
            --j;
        }
        
        obj = NULL;
        CCARRAY_FOREACH(arr, obj)
        {
            EquipItem* item = (EquipItem*)obj;
            
            CCLog("item---- level limit : %d",item->getLevelLimit());
        }
    }
    
    EquipItem* item_i = (EquipItem*)arr->objectAtIndex(i);
    CCLog("item_i --- level limit : %d",item_i->getLevelLimit());
    item_i = itemFlag;
    CCLog("item_i --- level limit : %d",item_i->getLevelLimit());
    
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(arr, obj)
    {
        EquipItem* item = (EquipItem*)obj;
        
        CCLog("item---- level limit : %d",item->getLevelLimit());
    }
    
    quick_sortArr(arr, low, i - 1);
    
    quick_sortArr(arr, i + 1, high);
}

void UserEquipBag::pupple_sortArr(cocos2d::CCArray *arr)
{
    for (int j = 0; j < arr->count(); j ++)
    {
        for (int i = 0; i < arr->count(); i ++)
        {
            EquipItem* item_i = (EquipItem*)arr->objectAtIndex(i);
            
            EquipItem* item_j = (EquipItem*)arr->objectAtIndex(j);
            
            if (item_i->getLevelLimit() <= item_j->getLevelLimit())
            {
                //arr->exchangeObjectAtIndex(i, j);
                arr->exchangeObject(item_i, item_j);
            }
        }
    }
}

void UserEquipBag::test_quick(int a[],int low,int high)
{
    if (low >= high)
    {
        return;
    }
    
    int i,j,k;
    
    if (low < high)
    {
        i = low;
        j = high;
        
        k = a[i];
        
        CCLog("k : %d",k);
    }
    
    while (i < j)
    {
        
        while (i < j && a[j] >= k)
        {
            --j;
        }
        
        if (i < j)
        {

            a[i] = a[j];
            
            i++;
        }
        
        while (i < j && a[i] <= k)
        {
            ++i;
        }
        
        if (i < j)
        {
            a[j] = a[i];
            
            --j;
        }
        
    }
    

    a[i] = k;

    
    test_quick(a, low, i - 1);
    
    test_quick(a, i + 1, high);
}

CCArray* UserEquipBag::getEquipWhiteArr()
{
    CCArray* arr = CCArray::create();
    
    arr->retain();
    
    CCDictElement* elm = NULL;
    
    CCDICT_FOREACH(m_dictEquipBag, elm)
    {
        EquipItem* item = (EquipItem*)elm->getObject();
        if (item->getGrade() == EQUIP_QUALITY_WHITE)
        {
            arr->addObject(item);
        }
    }
    
    if (arr->count() <= 0)
    {
        return NULL;
    }
    
    //quick_sortArr(arr, 0, arr->count() - 1);
    pupple_sortArr(arr);
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr, obj)
    {
        EquipItem* item = (EquipItem*)obj;
        
        CCLog("item---- level limit : %d",item->getLevelLimit());
    }
    
    return arr;
}

CCArray* UserEquipBag::getEquipGreenArr()
{
    CCArray* arr = CCArray::create();
    
    arr->retain();
    
    CCDictElement* elm = NULL;
    
    CCDICT_FOREACH(m_dictEquipBag, elm)
    {
        EquipItem* item = (EquipItem*)elm->getObject();
        if (item->getGrade() == EQUIP_QUALITY_GREEN)
        {
            arr->addObject(item);
        }
    }
    
    if (arr->count() <= 0)
    {
        return NULL;
    }
    
    //quick_sortArr(arr, 0, arr->count() - 1);
    pupple_sortArr(arr);
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr, obj)
    {
        EquipItem* item = (EquipItem*)obj;
        
        CCLog("item---- level limit : %d",item->getLevelLimit());
    }
    
    return arr;

}

CCArray* UserEquipBag::getEquipBlueArr()
{
    CCArray* arr = CCArray::create();
    
    arr->retain();
    
    CCDictElement* elm = NULL;
    
    CCDICT_FOREACH(m_dictEquipBag, elm)
    {
        EquipItem* item = (EquipItem*)elm->getObject();
        if (item->getGrade() == EQUIP_QUALITY_BLUE)
        {
            arr->addObject(item);
        }
    }
    
    if (arr->count() <= 0)
    {
        return NULL;
    }
    
    //quick_sortArr(arr, 0, arr->count() - 1);
    pupple_sortArr(arr);
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr, obj)
    {
        EquipItem* item = (EquipItem*)obj;
        
        CCLog("item---- level limit : %d",item->getLevelLimit());
    }
    
    return arr;

}

CCArray* UserEquipBag::getEquipPupleArr()
{
    CCArray* arr = CCArray::create();
    
    arr->retain();
    
    CCDictElement* elm = NULL;
    
    CCDICT_FOREACH(m_dictEquipBag, elm)
    {
        EquipItem* item = (EquipItem*)elm->getObject();
        if (item->getGrade() == EQUIP_QUALITY_PURPLE)
        {
            arr->addObject(item);
        }
    }
    
    if (arr->count() <= 0)
    {
        return NULL;
    }
    
    //quick_sortArr(arr, 0, arr->count() - 1);
    pupple_sortArr(arr);
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr, obj)
    {
        EquipItem* item = (EquipItem*)obj;
        
        CCLog("item---- level limit : %d",item->getLevelLimit());
    }
    
    return arr;

}

CCArray* UserEquipBag::getEquipOrangeArr()
{
    CCArray* arr = CCArray::create();
    
    arr->retain();
    
    CCDictElement* elm = NULL;
    
    CCDICT_FOREACH(m_dictEquipBag, elm)
    {
        EquipItem* item = (EquipItem*)elm->getObject();
        if (item->getGrade() == EQUIP_QUALITY_ORRANGE)
        {
            arr->addObject(item);
        }
    }
    
    if (arr->count() <= 0)
    {
        return NULL;
    }
    
    //quick_sortArr(arr, 0, arr->count() - 1);
    pupple_sortArr(arr);
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(arr, obj)
    {
        EquipItem* item = (EquipItem*)obj;
        
        CCLog("item---- level limit : %d",item->getLevelLimit());
    }
    
    return arr;

}
