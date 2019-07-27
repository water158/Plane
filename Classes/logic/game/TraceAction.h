#ifndef _TRACEACTION_H_
#define _TRACEACTION_H_

#include "cocos2d.h"
#include "BulletView.h"
#include "GlobalDelegate.h"
#include "CraftView.h"

USING_NS_CC;
using namespace std;

/***
 这个类实现了NODE的追踪运动
 */
class  TraceAction : public CCActionInterval
{
    
public:
	//析构
    virtual ~TraceAction();
	
    //参数：时间，目标位置
	static TraceAction * create(int aimed_craft_id, float speed, float direction_init);
    
public:
    //代理
    CrashedDelegate* delegateCrashed;
    
	//目标飞机
	//CraftView* m_pTargetPlane;
protected:
	bool init(int aimed_craft_id, float speed, float direction_init);
    
	virtual void startWithTarget(CCNode *pTarget);
	
    /**
     called once per frame. time a value between 0 and 1
     
     For example:
     - 0 means that the action just started
     - 0.5 means that the action is in the middle
     - 1 means that the action is over
	 */
	virtual void update(float dt);

protected:
	TraceAction();
    
    //速度
	float m_fSpeed;
    
    //初始角度
    float m_fAngleInit;
    
    //当前飞往的角度
    float m_fAngleCurrent;
    
    //偏移量
    CCPoint m_obLastDelta;
    
    //我的目标
    int m_nAimedCraftID;
    
    //我的目标的位置
	CCPoint m_obAimedCraftPosition;
};

#endif