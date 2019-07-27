//
//  MagnetCoinAction.cpp
//  plane
//
//  Created by mini01 on 14-4-23.
//
//

#include "MagnetCoinAction.h"
#include <math.h>
#include "FerrisTools.h"


MagnetCoinAction::MagnetCoinAction()
{
}

MagnetCoinAction::~MagnetCoinAction()
{
}

void MagnetCoinAction::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
}

MagnetCoinAction* MagnetCoinAction::create(float speed, CCPoint pos,float delay)
{
	MagnetCoinAction * pRoundPathAction = new MagnetCoinAction();
	if(pRoundPathAction && pRoundPathAction->init(speed,pos,delay))
	{
		pRoundPathAction->autorelease();
		return pRoundPathAction;
	}
    
	return NULL;
}

//这个函数得到初始化数据
bool MagnetCoinAction::init(float speed, CCPoint pos,float delay)
{
	if(!CCActionInterval::initWithDuration(10.0f))
	{
		return false;
	}
    
    //速度
    m_fSpeed = speed;
    
    targetDelay = delay;
    
    //目标位置
    m_obAimedCraftPosition = pos;
    
    //角度
    m_fAngleInit = 90;
    m_fAngleCurrent = 90;
    
    //每帧能移动的距离
    float len_per_frame = m_fSpeed / CCDirector::sharedDirector()->getFrameRate();
    
    //delta位移
    float delta_x = len_per_frame * cosf(CC_DEGREES_TO_RADIANS(m_fAngleInit));
    float delta_y = len_per_frame * sinf(CC_DEGREES_TO_RADIANS(m_fAngleInit));
    m_obLastDelta = ccp(delta_x, delta_y);
    
	return true;
}

void MagnetCoinAction::setEndpos(cocos2d::CCPoint &pos)
{
    m_obAimedCraftPosition = pos;
}

void MagnetCoinAction::update(float dt)
{
	if (!m_pTarget)
	{
		return;
	}
    
    //m_fSpeed += ADD_SPEED;
    
    time+=0.1f;
    
    if (time < targetDelay)
    {
        return;
    }
    
    
    //每帧能移动的距离
    float len_per_frame = m_fSpeed / CCDirector::sharedDirector()->getFrameRate();
    
    //delta位移
    float delta_x = len_per_frame * cosf(CC_DEGREES_TO_RADIANS(m_fAngleCurrent));
    float delta_y = len_per_frame * sinf(CC_DEGREES_TO_RADIANS(m_fAngleCurrent));
    m_obLastDelta = ccp(delta_x, delta_y);
    
    //position
    CCPoint newPosition = m_pTarget->getPosition() + m_obLastDelta;
    m_pTarget->setPosition(newPosition);
    
    //转角
    m_fAngleCurrent = calcAngleFromOriginToTarget(m_pTarget->getPosition(), m_obAimedCraftPosition);
    
    //rotation
    m_pTarget->setRotation(m_fAngleCurrent);

    m_pTarget->setPosition(newPosition);

}