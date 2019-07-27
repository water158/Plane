//
//  LaserBeam.h]
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillAddedBlood__
#define __FERRISGAME__SkillAddedBlood__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

using namespace cocos2d;


class SkillAddedBlood : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsRunning;
    
public:
    
    SkillAddedBlood(GameCombatLayer *parent);
    virtual ~SkillAddedBlood();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
};

#endif /* defined(__FERRISGAME__SkillAddedBlood__) */
