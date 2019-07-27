//
//  时间之沙
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillSandsOfTime__
#define __FERRISGAME__SkillSandsOfTime__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

using namespace cocos2d;

#define TIME_SCALE_NORMAL 1.0
#define TIME_SCALE_SLOW 0.1

class GameCombatLayer;

class SkillSandsOfTime : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsRunning;
    
    float m_fTimeFlag; // 用于计算cd
    
public:
    
    SkillSandsOfTime(GameCombatLayer *parent);
    virtual ~SkillSandsOfTime();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
};

#endif /* defined(__FERRISGAME__SkillSandsOfTime__) */
