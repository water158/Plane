//
//  LaserBeam.h]
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillDefenseBombs__
#define __FERRISGAME__SkillDefenseBombs__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

using namespace cocos2d;


class SkillDefenseBombs : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    float m_fInterval;
    
    bool m_bIsRunning;
    
public:
    
    SkillDefenseBombs(GameCombatLayer *parent);
    virtual ~SkillDefenseBombs();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
};

#endif /* defined(__FERRISGAME__SkillDefenseBombs__) */
