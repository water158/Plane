//
//  斥力盾
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillRepulsionShield__
#define __FERRISGAME__SkillRepulsionShield__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

using namespace cocos2d;


class SkillRepulsionShield : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsRunning;
    
public:
    
    SkillRepulsionShield(GameCombatLayer *parent);
    virtual ~SkillRepulsionShield();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
};

#endif /* defined(__FERRISGAME__SkillRepulsionShield__) */
