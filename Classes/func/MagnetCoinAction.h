//
//  MagnetCoinAction.h
//  plane
//
//  Created by mini01 on 14-4-23.
//
//

#ifndef __FERRIS_GAME__MagnetCoinAction__
#define __FERRIS_GAME__MagnetCoinAction__

#include <iostream>
#include "cocos2d.h"
#include "GlobalEnum.h"

#define ADD_SPEED 10

USING_NS_CC;
USING_NS_STD;

/***
 这个类实现了NODE的追踪运动
 */
class  MagnetCoinAction : public CCActionInterval
{
    
public:
	//析构
    virtual ~MagnetCoinAction();
	
    //参数：速度，目标位置
	static MagnetCoinAction * create(float speed, CCPoint pos,float delay = 0);
    //设置终点坐标
    void setEndpos(CCPoint& pos);
public:
    
	//目标飞机
	//CraftView* m_pTargetPlane;
protected:
	bool init(float speed, CCPoint pos,float delay);
    
	virtual void startWithTarget(CCNode *pTarget);
	
	virtual void update(float dt);
    
protected:
	MagnetCoinAction();
    
    //速度
	float m_fSpeed;
    
    //初始角度
    float m_fAngleInit;
    
    //当前飞往的角度
    float m_fAngleCurrent;
    
    //偏移量
    CCPoint m_obLastDelta;
    
    //我的目标的位置
	CCPoint m_obAimedCraftPosition;
    
    //计时器
    float time;
    
    //获取延时
    float targetDelay;
};

#endif
