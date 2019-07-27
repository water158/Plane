#ifndef __EQUIP_NODE_H__
#define __EQUIP_NODE_H__

#include "cocos2d.h"

#include "CraftView.h"
#include "HeroView.h"
#include "CraftData.h"
#include "EquipItem.h"
#include "DataManager.h"
#include "UserEquipBag.h"
#include "ConditionData.h"

USING_NS_CC;

class EquipView : public HeroView
{
public:
	virtual ~EquipView();

	static EquipView* create(CraftData* data, EquipItem* item_data);
    
    /***********************
     定时器
     ***********************/
    
    //炮管定时器
    void update(float dt);
    
    virtual void changePhrase(int phrase);
    
    void initWeapon();

private:
	EquipView();
	bool init(CraftData* data, EquipItem* item_data);

	//在主机上的位置
	CC_SYNTHESIZE(CCPoint , m_posOffset, PosOffset)

	//装备数据
	CC_SYNTHESIZE(EquipItem*, m_pEquipItemData, EquipItemData)

	//更换装备刷新后的位置
	CC_SYNTHESIZE(CCPoint , m_posChange, PosChange)

    //炮管的阶段
    CC_SYNTHESIZE(int, m_phrase, Phrase);

    //炮
    TargetPlaneData* dataTargetConon;
    
    CC_SYNTHESIZE(int, m_equipDirection, EquipDirection);
    
};


#endif