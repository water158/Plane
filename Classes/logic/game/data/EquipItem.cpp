//
//  EquipItem.cpp
//  ambition
//
//  Created by mini01 on 14-12-10.
//
//

#include "EquipItem.h"


EquipItem::EquipItem()
{

}

EquipItem::~EquipItem()
{

}

EquipItem* EquipItem::create(TiXmlElement *elm)
{
    EquipItem* data = new EquipItem();
    
    if (data && data->loadData(elm))
    {
        data->autorelease();
        
        return data;
    }
    
    return NULL;
}

EquipItem* EquipItem::createNewEquip()
{
    EquipItem* data = new EquipItem();
    
    if (data && data->initNewEquip())
    {
        data->autorelease();
        
        return data;
    }
    
    return NULL;
}

bool EquipItem::initNewEquip()
{
    //id
    this->setID("0");
    
    //类型
    this->setEquipType(0);
    
    //装备图片名称
    this->setImgName("-1");
    
    //装备名字
    //this->setName("0");
    
    //攻击
    this->setAttack(0);
    
    //血量
    this->setHp(0);
    
    //攻击加成
    this->setAttackAdd(0);
    
    //血量加成
    this->setHpAdd(0);
    
    //道具掉率
    this->setToolsDrop(0);
    
    //血球掉率
    this->setBloodDrop(0);
    
    //等级
    this->setLevel(1);
    
    //等级上限
    this->setLevelLimit(0);
    
    //品质
    this->setGrade(0);
    
    //技能id
    this->setSkillID(0);
    
    //是否装备
    this->setIsEquiped(false);
    
    //附加属性1
    this->setExtAtt1(0);
    
    //附加属性2
    this->setExtAtt2(0);

    return true;
}

//加载属性数据
bool EquipItem::loadData(TiXmlElement *elm)
{
    if (!elm)
    {
        return false;
    }
    
    //id
    this->setID(elm->Attribute("id"));
    
    //类型
    this->setEquipType(atoi(elm->Attribute("equip_type")));
    
    //装备图片名称
    this->setImgName(elm->Attribute("img_id"));
    
    //装备名字
    //this->setName(elm->Attribute("name"));
    
    //攻击
    this->setAttack(atoi(elm->Attribute("attack")));
    
    //血量
    this->setHp(atoi(elm->Attribute("blood")));
    
    //攻击加成
    this->setAttackAdd(atoi(elm->Attribute("attack_add")));
    
    //血量加成
    this->setHpAdd(atoi(elm->Attribute("blood_add")));
    
    //道具掉率
    this->setToolsDrop(atoi(elm->Attribute("tools_drop_rate")));
    
    //血球掉率
    this->setBloodDrop(atoi(elm->Attribute("blood_drop_rate")));
    
    //等级
    this->setLevel(atoi(elm->Attribute("level")));
    
    //等级上限
    this->setLevelLimit(atoi(elm->Attribute("level_limit")));
    
    //品质
    this->setGrade(atoi(elm->Attribute("quality")));
    
    //技能id
    this->setSkillID(atoi(elm->Attribute("skill_id")));
    
    //是否装备
    this->setIsEquiped(atoi(elm->Attribute("is_using"))==1 ? true : false);
    
    //附加属性1
    this->setExtAtt1(atoi(elm->Attribute("ext_att_1")));
    
    //附加属性2
    this->setExtAtt2(atoi(elm->Attribute("ext_att_2")));
    
    return true;
}

string EquipItem::getSkillText()
{
    string skillname = "0";
    
    switch (m_skillID)
    {
        case MAINGUN_SKILL_TYPE_NULL:
            skillname = "";
            break;
            
        case MAINGUN_SKILL_TYPE_SANDS_OF_TIME:
            skillname = "时间之沙";
            break;
            
        case MAINGUN_SKILL_TYPE_WAVE_GUN:
            skillname = "光波炮";
            break;
            
        case MAINGUN_SKILL_TYPE_SHAPED_GUN:
            skillname = "聚能炮";
            break;
            
        case MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING:
            skillname = "影遁";
            break;
            
        case MAINGUN_SKILL_TYPE_REPULSION_SHIELD:
            skillname = "斥力盾";
            break;
            
        case MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING:
            skillname = "定位狙击";
            break;
            
        case WING_SKILL_TYPE_DEFENSE_BOMBS:
            skillname = "防御敌弹";
            break;
            
        case WING_SKILL_TYPE_PROPS_WAREHOUSE:
            skillname = "道具仓";
            break;
            
        case WING_SKILL_TYPE_ADDED_BLOOD:
            skillname = "血球";
            break;
            
        default:
            break;
    }
    
    return skillname;
}

string EquipItem::getExtAtt1Name()
{
    string attName = "0";
    
    switch (m_extAtt1)
    {
        case 0:
            break;
        
        case 1:
            attName = "攻击加成";
            break;
            
        case 2:
            attName = "血量加成";
            break;
            
        case 3:
            attName = "道具掉率";
            break;
            
        case 4:
            attName = "血量掉率";
            break;
            
        default:
            break;
    }
    
    return attName;
}

string EquipItem::getExtAtt2Name()
{
    string attName = "0";
    
    switch (m_extAtt2)
    {
        case 0:
            break;
            
        case 1:
            attName = "攻击加成";
            break;
            
        case 2:
            attName = "血量加成";
            break;
            
        case 3:
            attName = "道具掉率";
            break;
            
        case 4:
            attName = "血量掉率";
            break;
            
        default:
            break;
    }
    
    return attName;
}

int EquipItem::getExtAtt1Value()
{
    int value = 0;
    
    switch (m_extAtt1)
    {
        case 0:
            break;
            
        case 1:
            value = m_attackAdd;
            break;
            
        case 2:
            value = m_hpAdd;
            break;
            
        case 3:
            value = m_toolsDrop;
            break;
            
        case 4:
            value = m_bloodDrop;
            break;
            
        default:
            break;
    }
    
    return value;
}

void EquipItem::setExtAtt1Value(int value)
{
    switch (m_extAtt1)
    {
        case 0:
            break;
            
        case 1:
            m_attackAdd = value;
            break;
            
        case 2:
            m_hpAdd = value;
            break;
            
        case 3:
            m_toolsDrop = value;
            break;
            
        case 4:
            m_bloodDrop = value;
            break;
            
        default:
            break;
    }
}



int EquipItem::getExtAtt2Value()
{
    int value = 0;
    
    switch (m_extAtt2)
    {
        case 0:
            break;
            
        case 1:
            value = m_attackAdd;
            break;
            
        case 2:
            value = m_hpAdd;
            break;
            
        case 3:
            value = m_toolsDrop;
            break;
            
        case 4:
            value = m_bloodDrop;
            break;
            
        default:
            break;
    }
    
    return value;
}

void EquipItem::setExtAtt2Value(int value)
{
    switch (m_extAtt2)
    {
        case 0:
            break;
            
        case 1:
            m_attackAdd = value;
            break;
            
        case 2:
            m_hpAdd = value;
            break;
            
        case 3:
            m_toolsDrop = value;
            break;
            
        case 4:
            m_bloodDrop = value;
            break;
            
        default:
            break;
    }
}

string EquipItem::getName()
{
    if (!compareString(m_imgName.c_str(), "-1"))
    {
        if (m_grade != 0)
        {
            CCString* str = CCString::createWithFormat("%s_%d",m_imgName.c_str(),m_grade);
            
            string name = EquipNameLibrary::shared()->getNameByKey(str->getCString())->getCString();
            
            return name;
        }
    }
    
    return "";
}

string EquipItem::getTypeText()
{
    int type_id = atoi(m_imgName.c_str());
    
    if (type_id == -1)
    {
        return "";
    }
    else if (type_id <= 3)
    {
        return "机枪";
    }
    else if (type_id <= 6)
    {
        return "激光";
    }
    else
    {
        return "导弹";
    }
}

