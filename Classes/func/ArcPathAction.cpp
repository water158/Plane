//
//  ArcPathAction.cpp
//  ambition
//
//  Created by mini02 on 15/2/27.
//
//

#include "ArcPathAction.h"

ArcPathAction::ArcPathAction():radius(0), angleBegin(0), angleEnd(0), m_bIsRotate(true)
{
}

ArcPathAction::~ArcPathAction()
{
}

void ArcPathAction::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    
    m_startPosition = pTarget->getPosition();//这里得到刚开始的位置
    
    m_centerPosition.x = m_startPosition.x - cos(CC_DEGREES_TO_RADIANS(angleRotationArmBegin)) * radius;
    m_centerPosition.y = m_startPosition.y - sin(CC_DEGREES_TO_RADIANS(angleRotationArmBegin)) * radius;
    
}

ArcPathAction* ArcPathAction::create(float duration, float r, float angle_begin, float angle_delta, bool is_rotate,bool is_anticlockwise)
{
    ArcPathAction * pArcPathAction = new ArcPathAction();
    if(pArcPathAction && pArcPathAction->init(duration, r, angle_begin, angle_delta, is_rotate,is_anticlockwise))
    {
        pArcPathAction->autorelease();
        return pArcPathAction;
    }else
    {
        CC_SAFE_DELETE(pArcPathAction);
        return NULL;
    }
}

//这个函数得到初始化数据
bool ArcPathAction::init(float duration, float r, float angle_begin, float angle_delta, bool is_rotate,bool is_anticlockwise)
{
    if( !CCActionInterval::initWithDuration(duration))
    {
        return false;
    }
    
    this->angleBegin = angle_begin;
    this->angleDelta = angle_delta;
    this->m_bIsRotate = is_rotate;
    
    this->_anticlockwise = is_anticlockwise;
    
    angleRotationArmBegin = this->angleBegin - 90.0f;
    
    this->radius = r;
    
    return true;
}

void ArcPathAction::update(float time)
{
    
    if (!m_pTarget)
    {
        return;
    }
    
    //如果是逆时针旋转
    if (!_anticlockwise)
    {
        time *=-1;
    }
    
    //得到半径
    float r = radius;
    
    //计算需要旋转的弧度数
    double rad_arm_begin = CC_DEGREES_TO_RADIANS(angleRotationArmBegin);
    double rad_begin = CC_DEGREES_TO_RADIANS(angleBegin);
    double rad_delta = CC_DEGREES_TO_RADIANS(angleDelta) * time;
    
    //计算更新后的具体位置
    CCPoint temp = ccp(m_centerPosition.x + cos(rad_arm_begin + rad_delta) * r,
                      m_centerPosition.y + sin(rad_arm_begin + rad_delta) * r);
    
    m_pTarget->setPosition(temp);
    
    //设置是否旋转角度
    if(m_bIsRotate)
    {

        float rotation = CC_RADIANS_TO_DEGREES(rad_begin - rad_delta);
        
        if (_anticlockwise)
        {
            // rotation = rotation - 180;
        }
        else
        {
            rotation = rotation + 180;
        }

        m_pTarget->setRotation(rotation);
    }
}