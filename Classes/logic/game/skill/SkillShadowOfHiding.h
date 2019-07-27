//
//  影遁
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillShadowOfHiding__
#define __FERRISGAME__SkillShadowOfHiding__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

using namespace cocos2d;

class GameCombatLayer;

class SkillShadowOfHiding : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsStartiming;
    
public:
    
    SkillShadowOfHiding(GameCombatLayer *parent);
    virtual ~SkillShadowOfHiding();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();

};

#endif /* defined(__FERRISGAME__SkillShadowOfHiding__) */
