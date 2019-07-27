#ifndef _ENEMY_VIEW_H_
#define _ENEMY_VIEW_H_

#include "cocos2d.h"
#include "FerrisTools.h"

//global
#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "GlobalFunc.h"

//data
#include "CraftData.h"
#include "BulletData.h"
#include "PathGroupData.h"

//object
#include "CraftView.h"
#include "BulletView.h"

//misc


//old
#include "GameData.h"
#include "FerrisParticle.h"


#include <math.h>

USING_NS_CC;
using namespace std;

class EnemyView : public CraftView
{
    
public:

    EnemyView();

    virtual ~EnemyView();
    
    static EnemyView* create(CraftData* dataCraft, TargetPlaneData* dataTargetPlane);
    
    //碰撞检测 与子弹之间的碰撞(所有)
    virtual void testAllBulletCollision(CCArray *&bullets);
    
    //处理子弹的碰撞结果，比如减血啊之类的
    virtual void processBulletCollision(BulletView *&bullet);
    
    //根据波数来确定 炮台的阶段
    virtual void changePhrase(int wave);
    
    //开始检测炮的数量
    void startCheckBatterySize();
    //检测炮的数量
    void updateCheckBatterySize(float dt);
    //加入到敌机炮数组
    void addBatteryPlane(CraftView* craft);
    //从敌机炮数组中移除
    void removeBatteryPlane(CraftView* craft);
    //设置敌机炮击毁
    void setEnemyBatteryCrashed();
    //获取敌机炮数量
    int getBatteryPlaneSize();
    
    /** 冒烟 **/
    //开始检测炮的状态
    void startCheckBatteryIsDied();
    //检测炮的是否死亡
    void updateCheckBatteryIsDied();
    
    //敌机自身减血(只有自身带有炮才会调用)
    void callbackDecreaseHP(CCNode* pSender);
    
    //设置伤害
    void theHurted(float hp);
    
    //设置飞机上的炮可以受击
    void setBatteryCanInjured();

protected:

    bool init(CraftData* dataCraft, TargetPlaneData* dataTargetPlane);
    
    virtual void draw();
    
    void callbackClean();
    
private:

    void update(float dt);
    

    void initWeapon();
    
    
    CC_SYNTHESIZE(string, m_planeId, PlaneID);

    CC_SYNTHESIZE (int,typeInXMLFile,TypeInXMLFile);
    
	CC_SYNTHESIZE(bool, isTaskPlane, IsTaskPlane);

	CC_SYNTHESIZE(CCMotionStreak*, m_pMotionStreak, MotionStreak);
    
    CC_SYNTHESIZE(bool, isDisplayHp, IsDisplayHp);

	CC_SYNTHESIZE(bool, m_bBeginAction, BeginAction);

	CC_SYNTHESIZE(FerrisParticle*, m_pParticle, Particle);
    
    //附属飞机
    CC_SYNTHESIZE(CCArray*, batteryPlanes, BatteryPlanes);
    
    //发射延迟
    CC_SYNTHESIZE(float, m_fDelayTime, DelayTime);
    
    //是否可以受击
    CC_SYNTHESIZE(bool, m_bIsCanInjured, IsCanInjured);
    
};

#endif