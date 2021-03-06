﻿#ifndef _BulletRoundPathAction_H_
#define _BulletRoundPathAction_H_

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

/***
 这个类实现了NODE的圆弧运动，使用时，直接用时间初始化，之后直接runAction
 */
class  BulletRoundPathAction : public CCActionInterval
{
public:
	virtual ~BulletRoundPathAction();
	//参数：时间，半径 ，初始法线角度，偏移角度
	static BulletRoundPathAction * create(float duration, float r, float b_angle, float e_angle, bool is_rotate=true);
    
protected:
	bool Init(float duration,float r, float b_angle,float e_angle, bool is_rotate);
    
	virtual void startWithTarget(CCNode *pTarget);
	
    /**
     called once per frame. time a value between 0 and 1
     
     For example:
     - 0 means that the action just started
     - 0.5 means that the action is in the middle
     - 1 means that the action is over
     */
    virtual void update(float time);

private:
	BulletRoundPathAction();
    
	//CCPoint m_endPosition;
	CCPoint m_startPosition;
	//CCPoint m_oldPosition;
    CCPoint m_centerPosition;
	
    //半径
    float radius;
	
    //起始角度
    float angleBegin;
    
    //结束角度
	float angleEnd;
    
    
    float angleDelta;
    
    float angleRotationArmBegin;
    
    float angleRotationArmEnd;
    
	bool m_bIsRotate;
    //float radPrevious;
};




#endif