#ifndef __WING_NODE_H__
#define __WING_NODE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

//#include "CraftView.h"
#include "HeroView.h"
#include "CraftData.h"
#include "EquipItem.h"
#include "GlobalEnum.h"
#include "JumpByAction.h"
#include "DataManager.h"
#include "UserEquipBag.h"

USING_NS_CC;

USING_NS_CC_EXT;
using namespace gui;

#define MOVE_INTERVAL_TIME 10
#define ATTACK_INTERVAL_TIME 5

class WingView : public HeroView
{
public:
	virtual ~WingView();

	static WingView* create(CraftData* data, EquipItem* item_data);
    
    /***********************
            定时器
     ***********************/
    
    //炮管定时器
    void update(float dt);
    
    //僚机自身位置定时器
    void updatePosition(float dt);
    
    //僚机左右移动定时器
    void updateState(float dt);
    
    //僚机攻击定时器
    void updateAttack(float dt);
    
    virtual void changePhrase(int phrase);
    
    void startWingUpdate();
    
private:
    /********普通函数************/
    
    //设置变形动画
    void setTransformAnimation();
    
private:
    /***********************
             回调
     ***********************/
    
    //僚机向左向右结束移动回调
    void callbackMoveEnd();
    //僚机向左向右开始移动回调
    void callbackMoveStart();
    
    //僚机开始随机移动
    void callbackRandomMoveStart();
    //僚机结束随机移动
    void callbackRandomMoveEnd();
    
    //设置僚机位置状态回调
    void callbackSetMoveState();
    
    //开始攻击计时回调
    void callbackAttackStart();
    //结束攻击计时回调
    void callbackAttackStop();
    
    //开始攻击回调
    void callbackStartFire();
    //结束攻击回调
    void callbackStopFire();
    
    //准备开火回调
    void callbackIsFire();
    //解除开火回调
    void callbackNotFire();
    
    
    
    void initWeapon();
    
    /***********************
     飞机变形
     ***********************/
    //开始攻击变形动画
    void callbackStartAttackTransform();
    //停止攻击变形动画
    void callbackStopAttackTransform();
    
private:
	WingView();
	bool init(CraftData* data, EquipItem* item_data);

	//在主机上的右位置
	CC_SYNTHESIZE(CCPoint , m_posOffsetR, PosOffsetR)
    
    //在主机上的左位置
    CC_SYNTHESIZE(CCPoint , m_posOffsetL, PosOffsetL)

	//装备数据
	CC_SYNTHESIZE(EquipItem*, m_pEquipItemData, EquipItemData)

	//更换装备刷新后的位置
	CC_SYNTHESIZE(CCPoint , m_posChange, PosChange)
    
    //僚机位置状态（分左右）
    CC_SYNTHESIZE(int, m_posState, PosState);
    
    //变形动画name
    CC_SYNTHESIZE(string, m_wTransformName, TransformName);
    
    //炮管的阶段
    CC_SYNTHESIZE(int, m_phrase, Phrase);
    
    //是否能否攻击
    CC_SYNTHESIZE(bool, m_canAttack, CanAttack);
    
    //更新位置的时间
    int pos_time;
    
    //更新攻击的时间
    int attack_time;
    
    //是否开火中
    bool isFire;
    
    //动画变量
    CCArmature* wing_anim;
    
    
    //炮
    TargetPlaneData* dataTargetConon;
};


#endif