#include "TraceAction.h"
#include <math.h>
#include "FerrisTools.h"

TraceAction::TraceAction()
{
}

TraceAction::~TraceAction()
{
}

void TraceAction::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
}

TraceAction* TraceAction::create(int aimed_craft_id, float speed, float direction_init)
{
	TraceAction * pRoundPathAction = new TraceAction();
	if(pRoundPathAction && pRoundPathAction->init(aimed_craft_id, speed, direction_init))
	{
		pRoundPathAction->autorelease();
		return pRoundPathAction;
	}
    
	return NULL;
}

//这个函数得到初始化数据
bool TraceAction::init(int aimed_craft_id, float speed, float direction_init)
{
	if(!CCActionInterval::initWithDuration(10.0f))
	{
		return false;
	}
    
    //aimed id
    m_nAimedCraftID = aimed_craft_id;
    
    //速度
    m_fSpeed = speed;
    
    //角度
    m_fAngleInit = direction_init;
    m_fAngleCurrent = direction_init;
    
    //每帧能移动的距离
    float len_per_frame = m_fSpeed /CCDirector::sharedDirector()->getFrameRate();
    
    //delta位移
    float delta_x = len_per_frame * cosf(CC_DEGREES_TO_RADIANS(m_fAngleInit));
    float delta_y = len_per_frame * sinf(CC_DEGREES_TO_RADIANS(m_fAngleInit));
    m_obLastDelta = ccp(delta_x, delta_y);

	return true;
}

void TraceAction::update(float dt)
{
	if (!m_pTarget || !this->delegateCrashed)
	{
		return;
	}
    
	//获得目标位置 通过参数m_nAimedCraftID可以在Combat里面 同curAimNode的m_uID进行比对
    //如果比对失败(值不一样) 说明已经切换了目标 那么本子弹就可以一路向前了
    //getAimedCraftPosition的返回值是表明 目标是否存在
	bool is_aim_exsited = this->delegateCrashed->getAimedCraftPosition(m_nAimedCraftID, m_obAimedCraftPosition);
    
    if (is_aim_exsited)
    {
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
    }
    else
    {
        CCPoint newPosition = m_pTarget->getPosition() + m_obLastDelta;
        m_pTarget->setPosition(newPosition);
        
        //当目标丢失后 方向不变 一路向前 特此声明
        //m_pTarget->setRotation(m_fLastRotation);
    }
}