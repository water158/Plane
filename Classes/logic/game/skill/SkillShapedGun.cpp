//
//  SkillShapedGun.cpp
//
//  Created by renyi on 14-9-4.
//
//

#include "SkillShapedGun.h"
#include "FerrisParticle.h"


ShapeGunHitEffect::ShapeGunHitEffect()
{
    
    m_pHalo=NULL;
    m_pHightlight=NULL;
    m_pRadiation=NULL;
    
}

ShapeGunHitEffect::~ShapeGunHitEffect()
{
    
}

bool ShapeGunHitEffect::init()
{
    m_pHalo = CCSprite::create("game/animations/shaoe_gun_hit_halo.png");
    m_pHightlight = CCSprite::create("game/animations/shaoe_gun_hit_highlight.png");
    m_pRadiation = CCSprite::create("game/animations/shaoe_gun_hit_radiation.png");
    
    m_pRadiation->setOpacity(200);
    
    m_pHightlight->setScale(2.0);
    m_pRadiation->setScale(0.4f);
    m_pRadiation->setRotation(rand()*360);
    
    this->addChild(m_pHalo,0);
    
    this->addChild(m_pRadiation,1);
    this->addChild(m_pHightlight,2);
    ccBlendFunc func = {GL_SRC_ALPHA,GL_ONE};
    
    //m_pHalo->setBlendFunc(func);
    //m_pHightlight->setBlendFunc(func);
    m_pHightlight->setBlendFunc(func);
    
    FerrisParticle* p = FerrisParticle::create("game/particles/particle_effect_hit_shapegun.plist");
    p->setPosition(this->getPosition());
    p->setAutoRemoveOnFinish(true);
    this->addChild(p,3);
    
    //高光放大缩小
    CCActionInterval  * scaleto = CCScaleBy ::create(0.3f, 1.5f);
    //    CCActionInterval * actionbyback = CCScaleTo ::create(0.3f, 1.0f);
    
    CCSequence* seq = CCSequence::createWithTwoActions(scaleto,scaleto->reverse());
    
    m_pHightlight->runAction(CCRepeatForever::create(seq));
    
    
    //射线放大，渐隐消失
    CCActionInterval  * scaleto1 = CCScaleTo ::create(0.3f, 1.0f);
    CCActionInterval * fadeout = CCFadeOut::create(0.5);
    
    //CCSpawn* spaRadiation= CCSpawn::create(fadeout,scaleto1->reverse());
    CCSequence* seqRadiation = CCSequence::create(scaleto1,fadeout,
                                                  CCCallFuncN::create(this, callfuncN_selector(ShapeGunHitEffect::callBcakRemoveRadiation)),
                                                  NULL);
    m_pRadiation->runAction(seqRadiation);
    
    
    //每2秒出一个射线
    CCActionInterval * delaytime = CCDelayTime::create(0.2);
    CCSequence* time02 = CCSequence::create(delaytime,
                                            CCCallFuncN::create(this, callfuncN_selector(ShapeGunHitEffect::callBackAddRadiation)),
                                            NULL);
    this->runAction(CCRepeatForever::create(time02));
    
    
    return true;
}

void ShapeGunHitEffect::removeEffect()
{
    this->removeChild(m_pHightlight);
    m_pHalo->stopAllActions();
    m_pRadiation->stopAllActions();
    CCActionInterval * fadeout = CCFadeOut::create(0.5f);
    
    CCSequence* seqHalo = CCSequence::create(fadeout,
                                             CCCallFuncN::create(this, callfuncN_selector(ShapeGunHitEffect::callBackRemoveHalo)),
                                             NULL);
    CCSequence* seqRadiation = CCSequence::create(fadeout,
                                                  CCCallFuncN::create(this, callfuncN_selector(ShapeGunHitEffect::callBcakRemoveRadiation)),
                                                  NULL);
    m_pHalo->runAction(seqHalo);
    m_pRadiation->runAction(seqRadiation);
}

void ShapeGunHitEffect::callBackRemoveHalo(CCNode* pSender)
{
    this->removeChild(m_pHalo);
    
}

void ShapeGunHitEffect::callBcakRemoveRadiation(CCNode* pSender)
{
    this->removeChild(m_pRadiation);
}

void ShapeGunHitEffect::callBackAddRadiation(CCNode* pSender)
{
    CCSprite* m_pNEWRadiation = CCSprite::create("game/animations/shaoe_gun_hit_radiation.png");
    m_pNEWRadiation->setScale(0.4f);
    m_pNEWRadiation->setRotation(getRandom(0,36)*10.0f);
    pSender->addChild(m_pNEWRadiation,1);
    
    m_pNEWRadiation->setOpacity(200);
    
    CCActionInterval  * scaleto1 = CCScaleTo ::create(0.3f, 1.0f);
    CCActionInterval * fadeout = CCFadeOut::create(0.5);
    CCSequence* seqRadiation = CCSequence::create(scaleto1,fadeout,
                                                  CCCallFuncN::create(this, callfuncN_selector(ShapeGunHitEffect::callBcakRemoveRadiation)),
                                                  NULL);
    m_pNEWRadiation->runAction(seqRadiation);
}

SkillShapedGun::SkillShapedGun(GameCombatLayer *parent)
{
    pShapedGun = NULL;
    
    m_pParent = parent;
    
    m_fCurrentLastTime = 0.0f;
    
    m_fTotalLastTime = 0.0f;
    
    m_isFull = false;
    
    m_bIsRunning = false;
    
    m_isHit = false;
    
    pShaoeGunEmissionEffect = NULL;
    
    pShapeGunHitEffect = NULL;
}

SkillShapedGun::~SkillShapedGun()
{
    CC_SAFE_DELETE(pShapedGun);
}

void SkillShapedGun::begin()
{
    if (m_pParent->m_bSkillEnable)
    {
        addShapeGun();
        
        m_pParent->setHeroAttrackState(false);
        
        m_bIsRunning = true;
        
        if (!m_isFull) {
            SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP, EFF_SKILL);
            //播放动画
            m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SHAPED_GUN);
        }
        
        m_pParent->m_battleUI->setSkillPercent(0);
        
        m_isFull  = true;
    }
}

void SkillShapedGun::update(float delta)
{
    bool is_touch = m_pParent->m_bIsTouch;
    
    //获取技能数据
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("3");
    
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    //聚能炮达到最大时的持续时间
    float f_maxTime = pSkillLevelData->getMaxTime();
    
    m_fCurrentLastTime += delta;
    
    m_pParent->m_battleUI->setSkillPercent((m_fCurrentLastTime / pSkillLevelData->getCD()) * 100);
    
    if (m_fCurrentLastTime >= pSkillLevelData->getCD())
    {
        m_pParent->m_bSkillEnable = true;
        //更新位置
        if(pShapedGun && !is_touch)
        {
            //pShapedGun->setPosition(m_pParent->m_pHeroView->getPosition());
            
            //当前阶段累计时间
            float fStrengthenInterval = pSkillLevelData->getStrengthenInterval();
            
            if(!is_touch)
            {
                //累计时间
                m_pParent->m_fShapedGunCurrentPhraseTime += 1.0f/60.0f;
                
                //超过时间就进入下一个阶段
                if(m_pParent->m_fShapedGunCurrentPhraseTime >= fStrengthenInterval)
                {
                    m_pParent->m_fShapedGunCurrentPhraseTime = 0;
                    
                    m_pParent->m_nShapedGunCurrentPhrase ++;
                    
                    pShaoeGunEmissionEffect->setScale( 1+m_pParent->m_nShapedGunCurrentPhrase*0.25);
                }
                
                
                //阶段二
                if(m_pParent->m_nShapedGunCurrentPhrase > 2)
                {
                    //阶段2 时间累加
                    m_fTotalLastTime += delta;
                    
                    if (m_fTotalLastTime >= f_maxTime)
                    {
                        end();
                    }
                    
                }
                else
                {
                    //激光增宽
                    if (pShapedGun)
                    {
                        pShapedGun->setScaleX(0.6f+m_pParent->m_nShapedGunCurrentPhrase*0.4f);
                    }
                }
            }
        }
        
    }
    
}


void SkillShapedGun::end()
{
    m_pParent->removeChildByTag(kTagSkillShapedGunEmission);
    
    m_pParent->removeChildByTag(kTagSkillShapedGun);
    pShapedGun = NULL;
    m_pParent->m_nShapedGunCurrentPhrase = 0;
    m_pParent->m_fShapedGunCurrentPhraseTime = 0;
    
    this->removeHitEffecct();
    
    if (m_bIsRunning)
    {
        m_pParent->m_bSkillEnable = false;
        
        m_pParent->setHeroAttrackState(true);
        
        m_bIsRunning = false;
        
        //获取技能数据
        SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("3");
        
        SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
        
        if (m_fCurrentLastTime >= pSkillLevelData->getCD())
        {
            m_fCurrentLastTime = 0.0f;
        }
    }
    
    if (m_isFull) {
        m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SHAPED_GUN,true);
    }
    
    m_pParent->m_battleUI->setSkillPercent(0);
    
    m_isFull = false;
}

void SkillShapedGun::addShapeGun()
{
    EquipItem* pEquipData = m_pParent->m_pMainGun->getEquipItemData();
    if(pEquipData)
    {
        if(!pShapedGun)
        {
            pShapedGun = CCSprite::create("game/skill/shaped_gun.png");
            pShapedGun->setAnchorPoint(ccp(0.5f, 0.0f));
            pShapedGun->setScaleX(0.5f);
            pShapedGun->setScaleY(3.0f);
            pShapedGun->setPosition(m_pParent->m_pHeroView->getPosition());
            m_pParent->addChild(pShapedGun, GAME_ZORDER_HERO, kTagSkillShapedGun);
            
            ccBlendFunc func = {GL_SRC_ALPHA,GL_ONE};
            
            pShaoeGunEmissionEffect = CCSprite::create("game/animations/shaoe_gun_emission_effect.png");
            pShaoeGunEmissionEffect->setAnchorPoint(ccp(0.5,0.3));
            //pShaoeGunEmissionEffect->setOpacity(160);
            pShaoeGunEmissionEffect->setBlendFunc(func);
            pShaoeGunEmissionEffect->setPosition(m_pParent->m_pHeroView->getPosition());
            m_pParent->addChild(pShaoeGunEmissionEffect, GAME_ZORDER_HERO_BULLET, kTagSkillShapedGunEmission);
        }
    }
}

void SkillShapedGun::addHitEffecct(CCPoint pos)
{
    if(!m_isHit)
    {
        m_isHit=true;
        pShapeGunHitEffect = new ShapeGunHitEffect();
        pShapeGunHitEffect->autorelease();
        pShapeGunHitEffect->init();
        pShapeGunHitEffect->setPosition(pos);
        m_pParent->addChild(pShapeGunHitEffect,GAME_ZORDER_HERO_BULLET,kTagSkillShapedGunHitEmission);
        
    }else
    {
        m_isHit=true;
        //skill->pShaoeGunHitEffect->setPosition(pos);
        pShapeGunHitEffect->setPosition(pos);
    }
    
    
}
void SkillShapedGun::removeHitEffecct()
{
    //skill->pShaoeGunHitEffect->release();
    if(m_isHit)
    {
        m_pParent->removeChildByTag(kTagSkillShapedGunHitEmission);
        
        m_isHit=false;
    }
}

void SkillShapedGun::reset()
{
    m_fTotalLastTime = 0.0f;
    m_fCurrentLastTime = 0.0f;
    m_isFull = false;
//    m_bIsPrapare = false;
    
    m_pParent->m_battleUI->setSkillPercent(0.0f);
    m_pParent->m_bSkillEnable = false;
    m_pParent->m_bFull = false;
    
    m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SHAPED_GUN,true);
}
