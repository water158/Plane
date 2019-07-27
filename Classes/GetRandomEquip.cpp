//
//  GetRandomEquip.cpp
//  ambition
//
//  Created by mini01 on 14-12-17.
//
//

#include "GetRandomEquip.h"

GetRandomEquip::GetRandomEquip()
{

}

GetRandomEquip::~GetRandomEquip()
{

    
}

GetRandomEquip* GetRandomEquip::m_sharedInstance = NULL;

GetRandomEquip* GetRandomEquip::shared()
{
    if (!m_sharedInstance)
    {
        m_sharedInstance = new GetRandomEquip();
        
        m_sharedInstance->m_rate = 0;
        
        m_sharedInstance->m_wave = 0;
    }
    
    return m_sharedInstance;
}

EquipItem* GetRandomEquip::getEquipByLottery()
{
    EquipItem* data = EquipItem::createNewEquip();
    
    return data;
}

//给装备随机属性
void GetRandomEquip::randAttribute(EquipItem* item)
{
    
    //设置装备的默认属性值
    item->setAttackAdd(0);
    
    item->setBloodDrop(0);
    
    item->setHpAdd(0);
    
    item->setToolsDrop(0);
    
    item->setExtAtt1(0);
    
    item->setExtAtt2(0);
    
    int nAttriNum = getRandom(1, 100);
    
    int nFirst = 0;
    
    int nSec = 0;
    
    switch (item->getGrade())
    {
        case EQUIP_QUALITY_WHITE:
        {
            //50%的几率 出现1条属性
            if (nAttriNum < 50)
            {
                nFirst = getRandom(1, 4);
            }
            else if (nAttriNum == 51)
            {
                //1%的几率  两条属性
                nFirst = getRandom(1, 4);
                
                nSec = getSecAttri(nFirst);
                
            }
            break;
        }
            
        case EQUIP_QUALITY_GREEN:
        {
            //50%的几率 出现1条属性
            if (nAttriNum < 70)
            {
                nFirst = getRandom(1, 4);
                
            }
            else if (nAttriNum < 75)
            {
                //1%的几率  两条属性
                nFirst = getRandom(1, 4);
                
                nSec = getSecAttri(nFirst);
                
            }
            break;
        }
            
        case EQUIP_QUALITY_BLUE:
        {
            //50%的几率 出现1条属性
            if (nAttriNum < 40)
            {
                nFirst = getRandom(1, 4);
                
            }
            else if (nAttriNum < 100)
            {
                //1%的几率  两条属性
                nFirst = getRandom(1, 4);
                
                nSec = getSecAttri(nFirst);
                
            }
            break;
        }
            
        case EQUIP_QUALITY_PURPLE:
        {
            //50%的几率 出现1条属性
            if (nAttriNum < 20)
            {
                nFirst = getRandom(1, 4);
                
            }
            else if (nAttriNum < 100)
            {
                //1%的几率  两条属性
                nFirst = getRandom(1, 4);
                
                nSec = getSecAttri(nFirst);
                
            }
            break;
        }
            
        case EQUIP_QUALITY_ORRANGE:
        {
            //1%的几率  两条属性
            nFirst = getRandom(1, 4);
            
            nSec = getSecAttri(nFirst);
            
            break;
        }
            
        default:
            break;
    }
    
    if (nFirst != 0)
    {
        setAttribute(item, nFirst);
        
        item->setExtAtt1(nFirst);
        
        if (nSec != 0)
        {
            setAttribute(item, nSec);
            
            item->setExtAtt2(nSec);
        }
    }
    
}



void GetRandomEquip::randAttributeWithRecast(EquipItem* item)
{
    
    //设置装备的默认属性值
    item->setAttackAdd(0);
    
    item->setBloodDrop(0);
    
    item->setHpAdd(0);
    
    item->setToolsDrop(0);
    
    int nFirst = item->getExtAtt1();
    
    int nSec = item->getExtAtt2();
    
    
    if (nFirst != 0)
    {
        setAttribute(item, nFirst);
        
        if (nSec != 0)
        {
            setAttribute(item, nSec);
        }
    }
}

void GetRandomEquip::randHpAndAttack(EquipItem *item)
{
    switch (item->getGrade())
    {
        case EQUIP_QUALITY_WHITE:
        {
            item->setAttack(getRandom(20, 30));
            
            item->setHp(getRandom(40, 60));
            break;
        }
        
        case EQUIP_QUALITY_GREEN:
        {
            item->setAttack(getRandom(30, 40));
            
            item->setHp(getRandom(60, 80));
            break;
        }
            
        case EQUIP_QUALITY_BLUE:
        {
            item->setAttack(getRandom(40, 50));
            
            item->setHp(getRandom(80, 100));
            break;
        }
            
        case EQUIP_QUALITY_PURPLE:
        {
            item->setAttack(getRandom(50, 60));
            
            item->setHp(getRandom(100, 120));
            break;
        }
            
        case EQUIP_QUALITY_ORRANGE:
        {
            item->setAttack(getRandom(60, 100));
            
            item->setHp(getRandom(120, 150));
            break;
        }
            
        default:
            break;
    }
}

EquipItem* GetRandomEquip::getEquipNormal(int wave)
{
    if (m_wave != wave)
    {
        m_rate += 5;
        
        m_wave = wave;
    }
    
    //装备是否爆出
    int nExit = getRandom(1, 100);
    
    if (nExit <= m_rate)
    {
        //表示出现了  重置爆率为0
        m_rate = 7;
        
        EquipItem* data = EquipItem::createNewEquip();
        
        //装备品级
        int nQuilaty = getRandom(1, 10000);
        //nQuilaty = nQuilaty % 100;
        
        if (nQuilaty <= 5429)
        {
            //54.29%的几率 白色装备
            data->setGrade(EQUIP_QUALITY_WHITE);
            
            data->setAttack(getRandom(20, 30));
            
            data->setHp(getRandom(40, 60));
        }
        else if (nQuilaty <= 8429)
        {
            //30%的几率 绿色装备
            data->setGrade(EQUIP_QUALITY_GREEN);
            
            data->setAttack(getRandom(30, 40));
            
            data->setHp(getRandom(60, 80));
            
            GameCountData::sharedInstance()->addGreenEquip();
        }
        else if (nQuilaty <= 9929)
        {
            //15%的几率 蓝色装备
            data->setGrade(EQUIP_QUALITY_BLUE);
            
            data->setAttack(getRandom(40, 50));
            
            data->setHp(getRandom(80, 100));
            
            GameCountData::sharedInstance()->addBlueEquip();
            
        }
        else if (nQuilaty <= 9999)
        {
            //0.7%的几率 紫色装备
            data->setGrade(EQUIP_QUALITY_PURPLE);
            
            data->setAttack(getRandom(50, 60));
            
            data->setHp(getRandom(100, 120));
            
            GameCountData::sharedInstance()->addPupleEquip();
            
        }
        else if (nQuilaty <= 10000)
        {
            //0.01%的几率 橙色装备
            data->setGrade(EQUIP_QUALITY_ORRANGE);
            
            data->setAttack(getRandom(60, 100));
            
            data->setHp(getRandom(120, 150));
            
            GameCountData::sharedInstance()->addOrangeEquip();
        }
        
        //关于装备图标的随机问题
        data->setImgName(getStringFromInt(getRandom(0, 10)));
        
        randSkill(data);
        
        randAttribute(data);
        
        //等级上限属性
        int level_limit = 0;
        
        if (m_wave <= 30)
        {
            level_limit = ((int)(m_wave / 10.0) + 1) * 5;
        }
        else
        {
            level_limit = ((int)((m_wave - 30) / 20.0) + 1) * 5 + 15;
        }
        
        data->setLevelLimit(level_limit);
        
        return data;
    }
    
    return NULL;
}

void GetRandomEquip::setAttribute(EquipItem *item, int tag_att)
{
    int value = getAttributeValue(item->getGrade());
    
    switch (tag_att)
    {
        case eEquip_extAttribute_addAttack:
            item->setAttackAdd(value);
            break;
        case eEquip_extAttribute_addHp:
            item->setHpAdd(value);
            break;
        case eEquip_extAttribute_addHpDrop:
            item->setBloodDrop(value);
            break;
        case eEquip_extAttribute_addToolDrop:
            item->setToolsDrop(value);
            break;
            
        default:
            break;
    }
}

int GetRandomEquip::getSecAttri(int first)
{
    int sec = getRandom(1, 4);
    
    while (sec == first)
    {
        sec = getRandom(1, 4);
    }
    
    return sec;
}

int GetRandomEquip::getAttributeValue(int grade)
{
    int randnum = getRandom(1, 100);
    
    int value = 0;
    
    switch (grade)
    {
        case EQUIP_QUALITY_WHITE:
            if (randnum < 35)
            {
                value = 1;
            }
            else if (randnum < 75)
            {
                value = 2;
            }
            else if (randnum < 100)
            {
                value = 3;
            }
            break;
            
        case EQUIP_QUALITY_GREEN:
            if (randnum < 10)
            {
                value = 1;
            }
            else if (randnum < 25)
            {
                value = 2;
            }
            else if (randnum < 45)
            {
                value = 3;
            }
            else if (randnum < 85)
            {
                value = 4;
            }
            else if (randnum < 95)
            {
                value = 5;
            }
            else if (randnum < 100)
            {
                value = 6;
            }
            break;
        
        case EQUIP_QUALITY_BLUE:
            if (randnum < 2)
            {
                value = 1;
            }
            else if (randnum < 6)
            {
                value = 2;
            }
            else if (randnum < 13)
            {
                value = 3;
            }
            else if (randnum < 23)
            {
                value = 4;
            }
            else if (randnum < 43)
            {
                value = 5;
            }
            else if (randnum < 83)
            {
                value = 6;
            }
            else if (randnum < 91)
            {
                value = 7;
            }
            else if (randnum < 97)
            {
                value = 8;
            }
            else
            {
                value = 9;
            }
            break;
            
        case EQUIP_QUALITY_PURPLE:
            if (randnum < 2)
            {
                value = 1;
            }
            else if (randnum < 5)
            {
                value = 2;
            }
            else if (randnum < 9)
            {
                value = 3;
            }
            else if (randnum < 14)
            {
                value = 4;
            }
            else if (randnum < 20)
            {
                value = 5;
            }
            else if (randnum < 28)
            {
                value = 6;
            }
            else if (randnum < 38)
            {
                value = 7;
            }
            else if (randnum < 68)
            {
                value = 8;
            }
            else if (randnum < 85)
            {
                value = 9;
            }
            else if (randnum < 93)
            {
                value = 10;
            }
            else if (randnum < 98)
            {
                value = 11;
            }
            else
            {
                value = 12;
            }
            break;
            
        case EQUIP_QUALITY_ORRANGE:
            if (randnum < 2)
            {
                value = 1;
            }
            else if (randnum < 5)
            {
                value = 2;
            }
            else if (randnum < 9)
            {
                value = 3;
            }
            else if (randnum < 15)
            {
                value = 4;
            }
            else if (randnum < 21)
            {
                value = 5;
            }
            else if (randnum < 27)
            {
                value = 6;
            }
            else if (randnum < 33)
            {
                value = 7;
            }
            else if (randnum < 41)
            {
                value = 8;
            }
            else if (randnum < 51)
            {
                value = 9;
            }
            else if (randnum < 71)
            {
                value = 10;
            }
            else if (randnum < 83)
            {
                value = 11;
            }
            else if (randnum < 91)
            {
                value = 12;
            }
            else if (randnum < 95)
            {
                value = 13;
            }
            else if (randnum < 98)
            {
                value = 14;
            }
            else
            {
                value = 15;
            }
            
        default:
            break;
    }
    
    return value;
}

//设置装备的技能id
void GetRandomEquip::randSkill(EquipItem* item)
{
    switch (item->getGrade())
    {
        case EQUIP_QUALITY_WHITE:
        {
            int randnum = getRandom(1, 100);
            if (randnum < 5)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SANDS_OF_TIME);
            }
            else if (randnum < 9)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_WAVE_GUN);
            }
            else if (randnum < 13)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHAPED_GUN);
            }
            else if (randnum < 17)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING);
            }
            else if (randnum < 21)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_REPULSION_SHIELD);
            }
            else if (randnum < 25)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING);
            }
            else if (randnum < 27)
            {
                item->setSkillID(WING_SKILL_TYPE_DEFENSE_BOMBS);
            }
            else if (randnum < 28)
            {
                item->setSkillID(WING_SKILL_TYPE_PROPS_WAREHOUSE);
            }
            else if (randnum < 31)
            {
                item->setSkillID(WING_SKILL_TYPE_ADDED_BLOOD);
            }
            else
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_NULL);
            }
            
            break;
        }
        
        case EQUIP_QUALITY_GREEN:
        {
            int randnum = getRandom(1, 100);
            if (randnum < 7)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SANDS_OF_TIME);
            }
            else if (randnum < 13)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_WAVE_GUN);
            }
            else if (randnum < 19)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHAPED_GUN);
            }
            else if (randnum < 25)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING);
            }
            else if (randnum < 31)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_REPULSION_SHIELD);
            }
            else if (randnum < 37)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING);
            }
            else if (randnum < 42)
            {
                item->setSkillID(WING_SKILL_TYPE_DEFENSE_BOMBS);
            }
            else if (randnum < 45)
            {
                item->setSkillID(WING_SKILL_TYPE_PROPS_WAREHOUSE);
            }
            else if (randnum < 51)
            {
                item->setSkillID(WING_SKILL_TYPE_ADDED_BLOOD);
            }
            else
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_NULL);
            }
            
            break;
        }
            
        case EQUIP_QUALITY_BLUE:
        {
            int randnum = getRandom(1, 100);
            if (randnum < 9)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SANDS_OF_TIME);
            }
            else if (randnum < 17)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_WAVE_GUN);
            }
            else if (randnum < 25)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHAPED_GUN);
            }
            else if (randnum < 33)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING);
            }
            else if (randnum < 41)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_REPULSION_SHIELD);
            }
            else if (randnum < 49)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING);
            }
            else if (randnum < 54)
            {
                item->setSkillID(WING_SKILL_TYPE_DEFENSE_BOMBS);
            }
            else if (randnum < 56)
            {
                item->setSkillID(WING_SKILL_TYPE_PROPS_WAREHOUSE);
            }
            else if (randnum < 61)
            {
                item->setSkillID(WING_SKILL_TYPE_ADDED_BLOOD);
            }
            else
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_NULL);
            }
            
            break;
        }
            
        case EQUIP_QUALITY_PURPLE:
        {
            int randnum = getRandom(1, 100);
            if (randnum < 10)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SANDS_OF_TIME);
            }
            else if (randnum < 19)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_WAVE_GUN);
            }
            else if (randnum < 28)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHAPED_GUN);
            }
            else if (randnum < 37)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING);
            }
            else if (randnum < 46)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_REPULSION_SHIELD);
            }
            else if (randnum < 55)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING);
            }
            else if (randnum < 61)
            {
                item->setSkillID(WING_SKILL_TYPE_DEFENSE_BOMBS);
            }
            else if (randnum < 64)
            {
                item->setSkillID(WING_SKILL_TYPE_PROPS_WAREHOUSE);
            }
            else if (randnum < 71)
            {
                item->setSkillID(WING_SKILL_TYPE_ADDED_BLOOD);
            }
            else
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_NULL);
            }
            
            break;
        }
            
        case EQUIP_QUALITY_ORRANGE:
        {
            int randnum = getRandom(1, 100);
            if (randnum < 12)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SANDS_OF_TIME);
            }
            else if (randnum < 23)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_WAVE_GUN);
            }
            else if (randnum < 34)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHAPED_GUN);
            }
            else if (randnum < 45)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING);
            }
            else if (randnum < 56)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_REPULSION_SHIELD);
            }
            else if (randnum < 67)
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING);
            }
            else if (randnum < 72)
            {
                item->setSkillID(WING_SKILL_TYPE_DEFENSE_BOMBS);
            }
            else if (randnum < 75)
            {
                item->setSkillID(WING_SKILL_TYPE_PROPS_WAREHOUSE);
            }
            else if (randnum < 81)
            {
                item->setSkillID(WING_SKILL_TYPE_ADDED_BLOOD);
            }
            else
            {
                item->setSkillID(MAINGUN_SKILL_TYPE_NULL);
            }
            
            break;
        }
            
        default:
            break;
    }
}

EquipItem* GetRandomEquip::getEquipGoldLottery()
{
    EquipItem* item = EquipItem::createNewEquip();
    
    int randnum = getRandom(1, 100);
    
    int grade = 0;
    
    if (randnum <= 80)
    {
        //随机出白色装备
        grade = EQUIP_QUALITY_WHITE;
        
        item->setHp(getRandom(40, 60));
        
        item->setAttack(getRandom(20, 30));
    }
    else if(randnum <= 95)
    {
        //随机出绿色装备
        grade = EQUIP_QUALITY_GREEN;
        
        item->setHp(getRandom(60, 80));
        
        item->setAttack(getRandom(30, 40));
        
        GameCountData::sharedInstance()->addGreenEquip();
    }
    else if (randnum <= 100)
    {
        //随机出蓝色装备
        grade = EQUIP_QUALITY_BLUE;
        
        item->setHp(getRandom(80, 100));
        
        item->setAttack(getRandom(40, 50));
        
        GameCountData::sharedInstance()->addBlueEquip();
    }
    
    GameCountData::sharedInstance()->save();
    
    item->setGrade(grade);
    
    item->setImgName(getStringFromInt(getRandom(0, 10)));
    
    int level_limit = getRandom(1, 20);
    
    if (level_limit == 20)
    {
        level_limit = 99;
    }
    else
    {
        level_limit *= 5;
    }
    
    item->setLevelLimit(level_limit);
//    item->setLevelLimit(getRandLevel(LOTTERY_USE_COIN, 5));
    
    randSkill(item);
    
    randAttribute(item);
    
    CCLog("lottery equip: level : %d",item->getLevel());
    
    
    
    return item;
}

EquipItem* GetRandomEquip::getEquipFewGemLottery()
{
    EquipItem* item = EquipItem::createNewEquip();
    
    int randnum = getRandom(1, 100);
    
    int grade = 0;
    
    if (randnum <= 80)
    {
        //随机出绿色装备
        grade = EQUIP_QUALITY_GREEN;
        
        item->setHp(getRandom(80, 100));
        
        item->setAttack(getRandom(20, 30));
        
        GameCountData::sharedInstance()->addGreenEquip();
    }
    else if (randnum <= 95)
    {
        //随机出蓝色装备
        grade = EQUIP_QUALITY_BLUE;
        
        item->setHp(getRandom(80, 100));
        
        item->setAttack(getRandom(40, 50));
        
        GameCountData::sharedInstance()->addBlueEquip();
    }
    else if (randnum <= 100)
    {
        //随机出紫色装备
        grade = EQUIP_QUALITY_PURPLE;
        
        item->setHp(getRandom(100, 120));
        
        item->setAttack(getRandom(50, 60));
        
        GameCountData::sharedInstance()->addPupleEquip();
    }

    GameCountData::sharedInstance()->save();
    
    item->setGrade(grade);
    
    item->setImgName(getStringFromInt(getRandom(0, 10)));
    
    int level_limit = getRandom(1, 20);
    
    if (level_limit == 20)
    {
        level_limit = 99;
    }
    else
    {
        level_limit *= 5;
    }
    
    item->setLevelLimit(level_limit);
//    item->setLevelLimit(getRandLevel(LOTTERY_USE_TEN_CRYSTAL, 35));
    
    randSkill(item);
    
    randAttribute(item);
    
    return item;
}

EquipItem* GetRandomEquip::getEquipManyGemLottery()
{
    EquipItem* item = EquipItem::createNewEquip();
    
    int randnum = getRandom(1, 100);
    
    int grade = 0;
    
    if (randnum <= 75)
    {
        //随机出蓝色装备
        grade = EQUIP_QUALITY_BLUE;
        
        item->setHp(getRandom(80, 100));
        
        item->setAttack(getRandom(40, 50));
        
        GameCountData::sharedInstance()->addBlueEquip();
    }
    else if (randnum <= 95)
    {
        //随机出紫色装备
        grade = EQUIP_QUALITY_PURPLE;
        
        item->setHp(getRandom(100, 120));
        
        item->setAttack(getRandom(50, 60));
        
        GameCountData::sharedInstance()->addPupleEquip();
    }
    else
    {
        //随机出橙色装备
        grade = EQUIP_QUALITY_ORRANGE;
        
        item->setHp(getRandom(120, 150));
        
        item->setAttack(getRandom(60, 100));
        
        GameCountData::sharedInstance()->addOrangeEquip();
    }
    
    GameCountData::sharedInstance()->save();
    
    item->setGrade(grade);
    
    int level_limit = getRandom(1, 20);
    
    if (level_limit == 20)
    {
        level_limit = 99;
    }
    else
    {
        level_limit *= 5;
    }
    
    item->setLevelLimit(level_limit);
    
//    item->setLevelLimit(getRandLevel(LOTTERY_USE_FOURTY_CRYSTAL, 55));
    
    item->setImgName(getStringFromInt(getRandom(0, 10)));
    
    randSkill(item);
    
    randAttribute(item);
    
    return item;
}


int GetRandomEquip::getRandLevel(LotteryType lotteryType, int curLevel)
{
    //获取当前装备等级的四舍五入之后的结果
    int roundNum = 0;
    
    //随机后的装备等级
    int finalLevel = 0;
    
    if (curLevel % 10 >= 5)
    {
        roundNum = (curLevel / 10 + 1) * 10;
    }
    else
    {
        roundNum = (curLevel / 10) * 10;
    }
    
    //根据抽奖消耗类型随机出等级
    if (lotteryType == LOTTERY_USE_COIN)
    {
        //使用金币抽奖

            //随机出高等级
            roundNum = 85;
            
            //随机一个数判断是随机出的装备是高于85还是低于85
            int rand = getRandom(1, 100);
            if (rand <= 20)
            {
                finalLevel = roundNum + 5;
            }
            else
            {
                finalLevel = roundNum - 5;
            }

    }
    else if (lotteryType == LOTTERY_USE_TEN_CRYSTAL)
    {
        //使用10钻抽奖

            //随机出高等级
            roundNum = 85;
            
            //随机一个数判断是随机出的装备是高于85还是低于85
            int rand = getRandom(1, 20);
            if (rand <= 4)
            {
                finalLevel = roundNum + 5;
            }
            else
            {
                finalLevel = roundNum - 5;
            }
    }
    else if (lotteryType == LOTTERY_USE_FOURTY_CRYSTAL)
    {
        //使用40钻抽奖

            //随机出高等级
            roundNum = 85;
            
            //随机一个数判断是随机出的装备是高于85还是低于85
            int rand = getRandom(1, 2);
            if (rand == 1)
            {
                finalLevel = roundNum + 5;
            }
            else
            {
                finalLevel = roundNum - 5;
            }
    }
    
    return finalLevel;
}

//获取重铸之后的装备品级
int GetRandomEquip::getQualityRecast()
{
    int randnum = getRandom(1, 100);
    
    if (randnum <= 25)
    {
        //重铸后出现白色装备 25%
        return EQUIP_QUALITY_WHITE;
    }
    else if (randnum <= 52)
    {
        //重铸后出现绿色装备 27%
        return EQUIP_QUALITY_GREEN;
    }
    else if (randnum <= 85)
    {
        //重铸后出现蓝色装备 33%
        return EQUIP_QUALITY_BLUE;
    }
    else if (randnum <= 98)
    {
        //重铸后出现紫色装备 13%
        return EQUIP_QUALITY_PURPLE;
    }
    else
    {
        //重铸后出现橙色装备 2%
        return EQUIP_QUALITY_ORRANGE;
    }
    
    return 0;
}



