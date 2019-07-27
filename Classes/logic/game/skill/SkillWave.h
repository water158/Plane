//
//  光波炮
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillWave__
#define __FERRISGAME__SkillWave__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

using namespace cocos2d;


class GameCombatLayer;

class SkillWave : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsRunning;
    
    bool m_bIsPrapare;
    
    float m_fTimeFlag;
    
public:
    
    SkillWave(GameCombatLayer *parent);
    virtual ~SkillWave();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
};

#endif /* defined(__FERRISGAME__SkillWave__) */
