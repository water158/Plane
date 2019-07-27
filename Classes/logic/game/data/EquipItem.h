//
//  EquipItem.h
//  ambition
//
//  Created by mini01 on 14-12-10.
//
//

#ifndef __ambition__EquipItem__
#define __ambition__EquipItem__

#include "cocos2d.h"
#include "tinyxml.h"
#include "GlobalEnum.h"
#include "FerrisTools.h"
#include "EquipNameLibrary.h"

USING_NS_CC;
using namespace std;

class EquipItem : public CCObject
{
public:
    virtual ~EquipItem();
    
    static EquipItem* create(TiXmlElement* elm);
    
    //创建一个空属性的装备
    static EquipItem* createNewEquip();
    
    //初始化一个空的装备
    bool initNewEquip();
    
    //加载属性数据
    bool loadData(TiXmlElement *elm);
    
    
    
    
    /**************************
            装备属性
     **************************/
    
    //id
    CC_SYNTHESIZE (string , m_strID, ID);
    
    //类型
    CC_SYNTHESIZE(int, m_equipType, EquipType);
    
    //装备图片名称
    CC_SYNTHESIZE(string, m_imgName, ImgName);
    
    //装备名字
    string getName();
    
    //攻击
    CC_SYNTHESIZE(int, m_attack, Attack);
    
    //血量
    CC_SYNTHESIZE(int, m_nHp, Hp);
    
    //攻击加成
    CC_SYNTHESIZE(int, m_attackAdd, AttackAdd);
    
    //血量加成
    CC_SYNTHESIZE(int, m_hpAdd, HpAdd);
    
    //道具掉率
    CC_SYNTHESIZE(int, m_toolsDrop, ToolsDrop);
    
    //血球掉率
    CC_SYNTHESIZE(int, m_bloodDrop, BloodDrop);
    
    //等级
    CC_SYNTHESIZE(int, m_level, Level);
    
    //等级上限
    CC_SYNTHESIZE(int, m_levelLimit, LevelLimit);
    
    //品质
    CC_SYNTHESIZE(int, m_grade, Grade);
    
    //技能id
    CC_SYNTHESIZE(int, m_skillID, SkillID);
    
    //是否装备
    CC_SYNTHESIZE(bool, m_isEquiped, IsEquiped);
    
    //附加属性1
    CC_SYNTHESIZE(int, m_extAtt1, ExtAtt1);
    
    //附加属性2
    CC_SYNTHESIZE(int, m_extAtt2, ExtAtt2);
    
    
    string getSkillText();
    
    string getExtAtt1Name();
    
    int getExtAtt1Value();
    
    void setExtAtt1Value(int value);
    
    string getExtAtt2Name();
    
    int getExtAtt2Value();
    
    void setExtAtt2Value(int value);
    
    string getTypeText();
    
private:
    EquipItem();
};


#endif /* defined(__ambition__EquipItem__) */
