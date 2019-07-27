#include "OvalPathAction.h"
#include <math.h>
#include "FerrisTools.h"

OvalPathAction::OvalPathAction():ra(0),rb(0),angleBegin(0), angleEnd(0), m_bIsRotate(true)
{
}

OvalPathAction::~OvalPathAction()
{
}

void OvalPathAction::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
    
	m_startPosition = pTarget->getPosition();//这里得到刚开始的位置
    
    m_centerPosition.x = m_startPosition.x - cos(CC_DEGREES_TO_RADIANS(angleRotationArmBegin)) * ra;
    m_centerPosition.y = m_startPosition.y - sin(CC_DEGREES_TO_RADIANS(angleRotationArmBegin)) * rb;
    
    ////CCLog("m_startPosition = (%f, %f)", m_startPosition.x, m_startPosition.y);
    ////CCLog("m_centerPosition = (%f, %f)", m_centerPosition.x, m_centerPosition.y);
}

OvalPathAction* OvalPathAction::create(float duration, float a,float b, float angle_begin, float angle_delta, bool is_rotate)
{
	OvalPathAction * pOvalPathAction = new OvalPathAction();
	if(pOvalPathAction && pOvalPathAction->Init(duration, a,b, angle_begin, angle_delta, is_rotate))
    {
		pOvalPathAction->autorelease();
		return pOvalPathAction;
	}
	return NULL;
}

//这个函数得到初始化数据
bool OvalPathAction::Init(float duration, float a,float b, float angle_begin, float angle_delta, bool is_rotate)
{
	if( !CCActionInterval::initWithDuration(duration))
	{
        return false;
    }
    
    this->angleBegin = angle_begin;
    this->angleDelta = angle_delta;
	this->m_bIsRotate = is_rotate;
	//this->angleEnd = angle_delta;
    
    //计算Arm角度
    if(angle_delta > 0)
    {
        //逆时针
        angleRotationArmBegin = this->angleBegin - 90.0f;
        //angle_end = angle_begin + angle_delta;
    }
    else
    {
        //顺时针
        angleRotationArmBegin = this->angleBegin + 90.0f;
        //angle_end = angle_begin + angle_delta;
    }
    
	//this->radius = r;
    
    this->ra = a;
    this->rb = b;

    //this->radPrevious = CC_DEGREES_TO_RADIANS(b_angle);
	return true;
}

void OvalPathAction::update(float time)
{
    ////CCLog("OvalPathAction::update(time = %f)", time);
    
	if (!m_pTarget)
	{
        return;
    }
    
	//根据起始角度算出顺时针还是逆时针,逆时针为-1
	//int isClockwise = (angleEnd - angleBegin > 0) ? -1 : 1;
    
	//记录未移动之前的位置，计算旋转角度
	//m_oldPosition = m_pTarget->getPosition();
    
	//得到半径
	//float r = radius;
    float a = ra;
    float b = rb;
	
    //计算需要旋转的弧度数
    double rad_arm_begin = CC_DEGREES_TO_RADIANS(angleRotationArmBegin);
	double rad_begin = CC_DEGREES_TO_RADIANS(angleBegin);
	double rad_delta = CC_DEGREES_TO_RADIANS(angleDelta) * time;
    
	//计算更新后的具体位置
	CCPoint temp = ccp(m_centerPosition.x + cos(rad_arm_begin + rad_delta) * a,
                       m_centerPosition.y + sin(rad_arm_begin + rad_delta) * b);
    
	m_pTarget->setPosition(temp);
	////设置旋转角度
    //radPrevious = radPrevious + rad_delta;
	//float rotation = calcAngleFromOriginToTarget(m_oldPosition, temp);
    //float rotation = CC_RADIANS_TO_DEGREES(rad_delta) - 90;
    float rotation = CC_RADIANS_TO_DEGREES(rad_begin + rad_delta);
    
   // //CCLog("OvalPathAction::update temp = (%f, %f) rot = %f", temp.x, temp.y, rotation);
	if(m_bIsRotate)
	{
		m_pTarget->setRotation(rotation);
	}
}