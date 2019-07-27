//
//  LaserBeam.h]
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillNull__
#define __FERRISGAME__SkillNull__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"

using namespace cocos2d;


class SkillNull : public SkillDelegate
{
    
private:
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsRunning;
    
public:
    
    SkillNull();
    virtual ~SkillNull();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
};

#endif /* defined(__FERRISGAME__SkillNull__) */
