//
//  SkillWave.cpp
//
//  Created by renyi on 14-9-4.
//
//

#include "SkillWave.h"

SkillWave::SkillWave(GameCombatLayer *parent)
{
    m_pParent = parent;
    
    m_fCurrentLastTime = 0.0f;
    
    m_fTotalLastTime = 0.0f;
    
    m_bIsPrapare = false;
    
    m_fTimeFlag = 0.0f;
}

SkillWave::~SkillWave()
{
    
}

void SkillWave::begin()
{
  
    
}

void SkillWave::update(float delta)
{
    //获取技能数据
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("2");
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    //获取蓄力时间
    float fPrepareTime = pSkillLevelData->getPrepareTime();
    
    float f_cdTime = pSkillLevelData->getCD();
    
    m_fCurrentLastTime += delta;
    
    if(m_bIsPrapare)
    {
        m_pParent->m_battleUI->setSkillPercent(0);
    }
    else
    {
        m_pParent->m_battleUI->setSkillPercent((m_fCurrentLastTime / f_cdTime) * 100);
    }
    
    if (m_fCurrentLastTime >= f_cdTime)
    {
        //如果用户没有点击屏幕 而且目前不是满能量状态
        if(!m_pParent->m_bIsTouch && !m_pParent->m_bFull)
        {
            m_pParent->setHeroAttrackState(false);
            
            if (!m_bIsPrapare)
            {
                SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP, EFF_SKILL);
                m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_WAVE_GUN);
            }
            
            m_bIsPrapare = true;
            
            m_fTimeFlag += delta;
            
            m_pParent->m_pProgressForPrapare->setPercentage((m_fTimeFlag / fPrepareTime)* 100);
            
            if(m_fTimeFlag >= fPrepareTime)
            {
                m_pParent->m_bFull = true;
                
                end();
            }
        }
    }
}

void SkillWave::end()
{
    if (m_pParent->m_bFull)
    {
        //播放动画
        m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_WAVE_GUN ,true);
        
        m_pParent->m_battleUI->setSkillPercent(0);
        
        m_pParent->m_pProgressForPrapare->setPercentage(0);
        
        m_bIsPrapare = false;
        
        m_fCurrentLastTime = 0.0f;
        
        m_fTimeFlag = 0.0f;
        
        m_pParent->setHeroAttrackState(true);
        
        CCSprite* pWave = CCSprite::create("game/skill/wave.png");
        pWave->setScale(0.1f);   
        pWave->setPosition(m_pParent->m_pHeroView->getPosition());
        m_pParent->addChild(pWave, GAME_ZORDER_HERO, kTagSkillWaveGun);
        
        CCSequence* seq = CCSequence::create(CCScaleTo::create(0.5f, 0.8f, 0.8f),
                                             CCMoveBy::create(0.5f, ccp(0.0f, _s_height)),
                                             CCCallFuncN::create(m_pParent,
                                                                 callfuncN_selector(GameCombatLayer::removeNode)),
                                             NULL);
        pWave->runAction(seq);
        
        m_pParent->m_bFull = false;
    }
}

void SkillWave::reset()
{
    m_fTotalLastTime = 0.0f;
    m_fCurrentLastTime = 0.0f;
    m_fTimeFlag = 0.0f;
    m_bIsPrapare = false;
    
    m_pParent->m_battleUI->setSkillPercent(0.0f);
    
    m_pParent->m_pProgressForPrapare->setPercentage(0.0f);
    m_pParent->m_bSkillEnable = false;
    m_pParent->m_bFull = false;
    
    m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_WAVE_GUN ,true);
}