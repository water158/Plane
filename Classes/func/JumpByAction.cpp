#include "JumpByAction.h"


JumpByAction* JumpByAction::create(const CCPoint& vec, bool speedup)
{
	//计算向量的长度
	float fLen = calcTwoPointLength(CCPointZero, vec);
	//获取时间
	float g = G;
	float fTime = sqrt(2.0f*fLen/g/MeterTransPixel);

	JumpByAction *pJumpBy = new JumpByAction();
	pJumpBy->initWithDuration(fTime, vec, speedup);
	pJumpBy->autorelease();

	return pJumpBy;
}

bool JumpByAction::initWithDuration(float duration, const CCPoint& position, bool speedup)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_delta = position;
		m_fDuring = duration;
		m_bUp = position.y >= 0 ? true : false;
		m_fPreTime = 0.0f;
		m_bSpeedUp = speedup;
		return true;
	}

	return false;
}

CCObject* JumpByAction::copyWithZone(CCZone *pZone, const CCPoint& position, bool speedup)
{
	CCZone* pNewZone = NULL;
	JumpByAction* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (JumpByAction*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new JumpByAction();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_delta, speedup);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void JumpByAction::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_previousPos = m_startPosition = pTarget->getPosition();
}

void JumpByAction::update(float t)
{
	if (m_pTarget)
	{
		//当前时间下的位移
		float time = m_fDuring * t;
		if (!m_bSpeedUp)
		{
			time = m_fDuring * (1.0f - t);
		}
		//米和像素的换算 按200计算
		float g = G;
		float h = (g / 2.0f) * time * time * MeterTransPixel;

		//上一次时间下的位移
		float time1 = m_fDuring * m_fPreTime;
		if (!m_bSpeedUp)
		{
			time1 = m_fDuring * (1.0f - m_fPreTime);
		}
		//米和像素的换算 按100计算
		float h1 = (g / 2.0f) * time1 * time1 * MeterTransPixel;

		float dela_h = h - h1;

		dela_h = (m_bSpeedUp == true) ? dela_h : -dela_h;

		float d_x = m_delta.x * dela_h / calcTwoPointLength(CCPointZero, m_delta);
		float d_y = m_delta.y * dela_h / calcTwoPointLength(CCPointZero, m_delta);

		m_pTarget->setPosition(ccpAdd( m_pTarget->getPosition(), ccp(d_x, d_y)));

		//CCLog("---y-==-(%f)-----y1-==-(%f)-----time-==-(%f)-", h, h1, t);
	}
	m_fPreTime = t;
}

CCActionInterval* JumpByAction::reverse(void)
{
	//return JumpByAction::create(m_fDuration);
	return NULL;
}
