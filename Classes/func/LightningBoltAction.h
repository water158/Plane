//
//  LightningBoltAction.h
//  HelloCpp
//
//  Created by renyi on 14/12/12.
//
//

#ifndef __HelloCpp__LightningBoltAction__
#define __HelloCpp__LightningBoltAction__

#include <stdio.h>
#include "cocos2d.h"
#include "LightningBoltSprite.h"
#include "LightningBoltNode.h"

USING_NS_CC;
using namespace std;

class LightningBoltAction : public CCActionInterval
{

public:
    //creates the action
    //if fDuration pass -1, will flash forever
    static LightningBoltAction* create(float flash_time);
    
    //initializes the action
    //if fDuration pass -1, will flash forever
    bool initWithFlashTime(float flash_time);
    
    virtual void stop();
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float dt);
    virtual bool isDone();
    LightningBoltAction(){initMembers();}
    virtual~LightningBoltAction(){
    }
protected:
    void startAnimation(CCNode*pTarget);
    void stopAnimation(CCNode*pTarget);
    void callbackFlashForever(CCNode*sender);
    void callBack_genLightning(CCNode* sender);
    void callBack_jitter(CCNode*sender);
    void callBack_resetOpacity(CCNode*sender);
protected:
    void initMembers(){

    }
protected:
    float m_timeFoe;
    float m_timeCur;
protected:
    float m_fFlashTime;
    float m_jitterTimeOfOneFlash;
    float m_jitterInterval;
    bool m_isDoJitter;
    GLubyte m_startOpacity;//start opacity of one flash
    GLubyte m_endOpacity;//end opacity of one flash
    

};

#endif /* defined(__HelloCpp__LightningBoltAction__) */
