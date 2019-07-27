/*********
	created at 2014 07 14
	加速、减速运动，分上下
	
*********/

#ifndef __FERRIS_GAME__JumpByAction__
#define __FERRIS_GAME__JumpByAction__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "actions/CCActionInterval.h"
#include "FerrisTools.h"

USING_NS_CC;
USING_NS_STD;

//重力加速度 10
#define G 3;

//米和像素的换算 1米等于200像素
#define MeterTransPixel 100


class JumpByAction : public CCActionInterval
{
public:
	/** initializes the action */
	bool initWithDuration(float duration, const CCPoint& position, bool speedup);

	virtual CCObject* copyWithZone(CCZone* pZone, const CCPoint& position, bool speedup);
	virtual void startWithTarget(CCNode *pTarget);
	virtual void update(float time);
	virtual CCActionInterval* reverse(void);

public:
	/** creates the action */
	static JumpByAction* create(const CCPoint& vec, bool speedup = false);
protected:
	CCPoint         m_startPosition;       //初始位置
	CCPoint         m_delta;			   //位移向量
	CCPoint         m_previousPos;		   //上一个位置
	float           m_fDuring;			   //周期
	bool            m_bUp;				   //是否向上运动
	bool            m_bSpeedUp;			   //是否加速
	float           m_fPreTime;			   //上一次的时间

};

#endif