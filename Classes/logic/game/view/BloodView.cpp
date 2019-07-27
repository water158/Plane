//
//  BloodView.cpp
//  ambition
//
//  Created by mini02 on 15/1/9.
//
//

#include "BloodView.h"
#include "HeroView.h"
#include "FerrisParticle.h"

BloodView::BloodView()
{
    m_fCarryBlood = 0.0f;
    m_fIsDied = false;
    m_bIsCollisionWithHero = false;
}

BloodView::~BloodView()
{
   // CC_SAFE_RELEASE(heroTmp);
}

BloodView* BloodView::create()
{
    BloodView* blood = new BloodView();
    if (blood && blood->init())
    {
        blood->autorelease();
        return blood;
    }

    return NULL;
}

bool BloodView::init()
{
    if (!CCNode::init())
    {
        return false;
    }
    
    //设置大小
    this->setContentSize(CCSizeMake(10, 10));
    
    this->setSpeedVY(5);
    
    return true;
}

//血球在屏幕中碰撞移动
void BloodView::startMoveWithScreen()
{
    this->schedule(schedule_selector(BloodView::updatePosition),0.1f / 60.0f);
}

//血球位置定时器
void BloodView::updatePosition(float dt)
{
    
    this->setPositionY(this->getPositionY() - m_nVy);
    
}
//创建血球粒子
void BloodView::createBloodParticle()
{
    FerrisParticle* particle = FerrisParticle::create("game/particles/repair_drop.plist");
    particle->setPosition(ccp(0,0));
    //particle->setScale(1.5f);
    particle->setPositionType(kCCPositionTypeGrouped);
    particle->setAutoRemoveOnFinish(true);
    this->addChild(particle);
}
//是否出界
bool BloodView::outOfScreen()
{
    float xx = this->getPositionX();
    float yy = this->getPositionY();
    
    int n_offset_x = 30;
    int n_offset_y_bottom = 30;
    
    if(xx < - n_offset_x || xx > _s_width + n_offset_x || yy < - n_offset_y_bottom || yy > _s_height + n_offset_y_bottom)
    {
        return true;
    }
    
    return false;
}

void BloodView::updateMagnetAction(float dt)
{
    if (!heroTmp || heroTmp->getIsDied())
    {
        return;
    }
    
    //如果存在动作
    MagnetCoinAction* act = (MagnetCoinAction*)this->getActionByTag(TAG_SPEEDUP_ACTION);
    
    if(act)
    {
        CCPoint pos = heroTmp->getPosition();
        act->setEndpos(pos);
    }
}

void BloodView::startUpdateMagnet()
{
    this->schedule(schedule_selector(BloodView::updateMagnetAction), 1.0f / 60);
}
