//
//  聚能炮
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __FERRISGAME__SkillShapedGun__
#define __FERRISGAME__SkillShapedGun__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"

#define kTagSkillShapedGunEmission 1011

#define kTagSkillShapedGunHitEmission 1012

using namespace cocos2d;

class GameCombatLayer;

class ShapeGunHitEffect : public CCNode
{
public:
    ShapeGunHitEffect();
    virtual ~ShapeGunHitEffect();
    
    bool init();
    void removeEffect();
    
    void callBackRemoveHalo(CCNode* pSender);
    void callBcakRemoveRadiation(CCNode* pSender);
    void callBackAddRadiation(CCNode* pSender);
    
private:
    CCSprite *m_pHalo;
    CCSprite *m_pHightlight;
    CCSprite *m_pRadiation;
    
};

class SkillShapedGun : public SkillDelegate
{
    
private:
    GameCombatLayer *m_pParent;
    
    CCSprite* pShapedGun;
    
    float m_fTotalLastTime;
    
    float m_fCurrentLastTime;
    
    bool m_bIsRunning;
    
    bool m_isFull;
    
    bool m_isHit;
    
    CCSprite* pShaoeGunEmissionEffect;
    ShapeGunHitEffect* pShapeGunHitEffect;
    
public:
    
    SkillShapedGun(GameCombatLayer *parent);
    virtual ~SkillShapedGun();
    
    
    virtual void begin();
    
    virtual void update(float delta);
    
    virtual void end();
    
    virtual void reset();
    
    void addShapeGun();
    
    void addHitEffecct(CCPoint pos);
    void removeHitEffecct();
};

#endif /* defined(__FERRISGAME__SkillShapedGun__) */
