//
//  GetRandomEquip.h
//  ambition
//
//  Created by mini01 on 14-12-17.
//
//

#ifndef __ambition__GetRandomEquip__
#define __ambition__GetRandomEquip__

#include "cocos2d.h"
#include "FerrisTools.h"
#include "GlobalEnum.h"
#include "EquipItem.h"
#include "GameCountData.h"

USING_NS_CC;

class GetRandomEquip
{
public:
    
    static GetRandomEquip* shared();
    
    EquipItem* getEquipByLottery();
    
    EquipItem* getEquipNormal(int wave);
    
    EquipItem* getEquipGoldLottery();
    
    EquipItem* getEquipFewGemLottery();
    
    EquipItem* getEquipManyGemLottery();
    
    //获取重铸之后的装备品级
    int getQualityRecast();
    
    //给装备随机属性
    void randAttribute(EquipItem* item);
    
    //为重铸的装备随机属性
    void randAttributeWithRecast(EquipItem* item);
    
    //设置装备的技能id
    void randSkill(EquipItem* item);
    
    //随机装备的血量和攻击
    void randHpAndAttack(EquipItem* item);
    
private:
    GetRandomEquip();
    ~GetRandomEquip();
    
    static GetRandomEquip* m_sharedInstance;
    
    //随机得到等级 参数为使用的抽奖类型 和 装备的当前等级
    
    int getRandLevel(LotteryType lotteryType,int curLevel);
    
    
    
    //根据装备品级获取随机的属性属性值
    int getAttributeValue(int grade);
    
    //获取第二条属性 根据第一条属性
    int getSecAttri(int first);
    
    //设置装备的属性值
    void setAttribute(EquipItem* item,int tag_att);
    
    //当前装备的爆率
    int m_rate;
    
    //当前保存的波数
    int m_wave;
};

#endif /* defined(__ambition__GetRandomEquip__) */
