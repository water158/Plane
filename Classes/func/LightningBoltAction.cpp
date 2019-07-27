//
//  LightningBoltAction.cpp
//  HelloCpp
//
//  Created by renyi on 14/12/12.
//
//

#include "LightningBoltAction.h"

// creates the action
LightningBoltAction *LightningBoltAction::create(float flash_time)
{
    LightningBoltAction *p = new LightningBoltAction();
    p->initWithFlashTime(flash_time);
    p->autorelease();
    
    return p;
}

// initializes the action
bool LightningBoltAction::initWithFlashTime(float flash_time)
{
    if (CCActionInterval::initWithDuration(flash_time))
    {
        m_timeFoe = 0;
        m_timeCur = 0;
        m_fFlashTime = 0;
        m_jitterTimeOfOneFlash = 0;
        m_jitterInterval = 0;
        m_isDoJitter = false;
        m_startOpacity = 200;
        m_endOpacity = 80;

        m_fFlashTime = flash_time;
        return true;
    }
    
    return false;
}

void LightningBoltAction::update(float dt)
{
    //
}

bool LightningBoltAction::isDone()
{
    return false;
}

void LightningBoltAction::startWithTarget(CCNode *pTarget)
{
    // set some members of pTarget
    startAnimation(pTarget);
    
    // call father startWithTarget
    CCActionInterval::startWithTarget(pTarget);
}


void LightningBoltAction::stop()
{
    // clean up
    stopAnimation(m_pTarget);

    // call father stop
    CCActionInterval::stop();
}


void LightningBoltAction::startAnimation(CCNode *pTarget)
{
    LightningBoltNode *lbNode = (LightningBoltNode *)pTarget;
    
    LightningBoltSprite *lbSprite = lbNode->m_lbSprite;
    LightningBoltSprite *lbSprite2 = lbNode->m_lbSprite2;
    //
    stopAnimation(pTarget);
    //
    lbSprite->setOpacity(m_startOpacity);
    lbSprite2->setOpacity(m_endOpacity);
    // run action
    CCDelayTime *delay = CCDelayTime::create(m_fFlashTime / 2);
    CCCallFuncN *callFunc_flashForever = CCCallFuncN::create(this, callfuncN_selector(LightningBoltAction::callbackFlashForever));
    CCAction *action = CCSequence::create(callFunc_flashForever, NULL);
    CCAction *action2 = CCSequence::create(delay, callFunc_flashForever, NULL);
    lbSprite->runAction(action);
    lbSprite2->runAction(action2);
}

void LightningBoltAction::stopAnimation(CCNode *pTarget)
{
    LightningBoltNode *lbNode = (LightningBoltNode *)pTarget;
    LightningBoltSprite *lbSprite = lbNode->m_lbSprite;
    LightningBoltSprite *lbSprite2 = lbNode->m_lbSprite2;
    lbSprite->stopAllActions();
    lbSprite2->stopAllActions();
}

void LightningBoltAction::callbackFlashForever(CCNode *sender)
{
    m_jitterTimeOfOneFlash = m_fFlashTime / 4;
    m_jitterInterval = m_fFlashTime / 6;
    
    //----fadeTo action
    CCFadeTo *fadeTo = CCFadeTo::create(m_fFlashTime, m_endOpacity);
    CCCallFuncN *callFunc_genLightning = CCCallFuncN::create(this, callfuncN_selector(LightningBoltAction::callBack_genLightning));
    CCCallFuncN *callFunc_jitter = CCCallFuncN::create(this, callfuncN_selector(LightningBoltAction::callBack_jitter));
    CCCallFuncN *callFunc_resetOpacity = CCCallFuncN::create(this, callfuncN_selector(LightningBoltAction::callBack_resetOpacity));
    
    //----jitter action sequence
    CCArray *array = CCArray::create();
    float totalTime = 0;
    
    while (totalTime < m_jitterTimeOfOneFlash)
    {
        array->addObject(CCDelayTime::create(m_jitterInterval));
        
        if (m_isDoJitter)
        {
            array->addObject(callFunc_jitter);
        }
        
        totalTime += m_jitterInterval;
    }
    
    CCSequence *action_seq_jitter = CCSequence::create(array);
    
    CCSequence *action_seq_opacity = CCSequence::create(callFunc_resetOpacity,
                                                fadeTo,
                                                callFunc_genLightning,
                                                NULL);
    
    //----make whole action
    CCAction *action = CCRepeatForever::create(CCSpawn::create(action_seq_jitter,
                                                               action_seq_opacity,
                                                               NULL));
    //----runAction
    ((LightningBoltSprite *)sender)->runAction(action);
}

void LightningBoltAction::callBack_genLightning(CCNode *sender)
{
    ((LightningBoltSprite *)sender)->genLighting();
}

void LightningBoltAction::callBack_jitter(CCNode *sender)
{
    ((LightningBoltSprite *)sender)->doJitter();
}

void LightningBoltAction::callBack_resetOpacity(CCNode *sender)
{
    ((LightningBoltSprite *)sender)->setOpacity(m_startOpacity);
}
