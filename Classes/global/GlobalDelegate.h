#ifndef __FERRIS_GAME__GlobalDelegate__
#define __FERRIS_GAME__GlobalDelegate__

#include "cocos2d.h"
#include "GlobalEnum.h"

USING_NS_CC;
USING_NS_STD;

class CraftView;
class BulletView;
class BossView;
class EnemyView;
class GunDetail;
class DropView;

class BulletDelegate
{
public:
    //创建子弹
    virtual void createCraftBullet(CraftView *craft, GunDetail* detail) = 0;
    
    //获得Hero的位置
    virtual CCPoint getHeroPosition() = 0;
};

class SkillDelegate
{
public:
    virtual void begin() = 0;
    virtual void update(float dt) = 0;
    virtual void end() = 0;
    
    virtual void reset()= 0;
    
    bool m_bIsRunning;
//    float m_fTotalLastTime;//蓝条总时间
//    float m_fCurrentLastTime;//蓝条当前时间
//    float m_fTimeFlag; // 技能持续时间
//    bool m_bIsPrapare;;//光波炮技能cd时间
};

class LaserDelegate
{
public:
    virtual void updateLaser(float dt) = 0;
    
    //设置僚机激光是否可见
    virtual void setWingLaserVisible(bool visible) = 0;
    
    //设置肩炮激光是否可见
    virtual void setShoulderLaserVisible(bool visible) = 0;
    
    //设置翼炮激光是否可见
    virtual void setPinionLaserVisible(bool visible) = 0;
    
    //设置主机激光是否可见
    virtual void setMainLaserVisible(bool visible) = 0;
};

class CrashedDelegate
{
public:
    //子弹撞了
    virtual void bulletCrashed(BulletView *&bullet, bool is_play_animation) = 0;
    //飞机撞了
    virtual void craftCrashed(CraftView *craft, int reason, bool is_play_animation) = 0;
    
    //enemy恢复之前的动作
    virtual void resumeEnemyPreAction(EnemyView *craft) = 0;
    
    virtual bool getAimedCraftPosition(int aimed_craft_id, CCPoint &position) = 0;
    
    virtual void setCraftHeadStreak(bool enable) = 0;
    
};

class MapDelegate
{
public:
    virtual void enemyFly() = 0;
};
class DropDelegate
{
public:
    //飞机掉落金币的加速运动
    virtual void dropSpeedUp(DropView* drop) = 0;
};
#endif