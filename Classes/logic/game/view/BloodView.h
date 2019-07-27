//
//  BloodView.h
//  ambition
//
//  Created by mini02 on 15/1/9.
//
//

#ifndef __ambition__BloodView__
#define __ambition__BloodView__

#include <stdio.h>

#include "cocos2d.h"
#include "SmartRes.h"
#include "GlobalTagZOrder.h"
#include "MagnetCoinAction.h"

USING_NS_CC;

class HeroView;
class BloodView : public CCNode
{
public:
    BloodView();
    ~BloodView();
    
    static BloodView* create();
    
    //道具在屏幕中碰撞移动
    void startMoveWithScreen();
    
    //道具位置定时器
    void updatePosition(float dt);
    
    //创建血球粒子
    void createBloodParticle();
    
    //是否出界
    bool outOfScreen();
    
    void updateMagnetAction(float dt);
    void startUpdateMagnet();
    
private:
    virtual bool init();
    
    //是否与与主机检测框碰撞
    CC_SYNTHESIZE(bool, m_bIsCollisionWithHero, IsCollisionWithHero);
    
    //y轴速度
    CC_SYNTHESIZE(int, m_nVy, SpeedVY);
    
    //血球是否存活
    CC_SYNTHESIZE(bool, m_fIsDied, IsDied);
    
    //血球携带血量
    CC_SYNTHESIZE(float, m_fCarryBlood, CarryBlood);
    
    //主机对象
    CC_SYNTHESIZE(HeroView*, heroTmp, HeroTmp);
};

#endif /* defined(__ambition__BloodView__) */
