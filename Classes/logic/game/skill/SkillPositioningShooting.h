//
//  定位狙击
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillPositioningShooting__
#define __FERRISGAME__SkillPositioningShooting__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

using namespace cocos2d;

class GameCombatLayer;

class SkillPositioningShooting : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsRunning;
    
public:
    
    SkillPositioningShooting(GameCombatLayer *parent);
    virtual ~SkillPositioningShooting();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
};

#endif /* defined(__FERRISGAME__SkillPositioningShooting__) */
