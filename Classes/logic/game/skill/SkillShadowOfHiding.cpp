//
//  SkillShadowOfHiding.cpp
//
//  Created by renyi on 14-9-4.
//
//

#include "SkillShadowOfHiding.h"

SkillShadowOfHiding::SkillShadowOfHiding(GameCombatLayer *parent)
{
    m_pParent = parent;
    
    m_fCurrentLastTime = 0.0f;
    
    m_fTotalLastTime = 0.0f;
    
    m_bIsStartiming = false;
}

SkillShadowOfHiding::~SkillShadowOfHiding()
{
    
}

void SkillShadowOfHiding::begin()
{
    if (!m_pParent->m_bSkillEnable)
    {
        return;
    }
    
    m_pParent->m_bIsSkillRunning = true;
    m_pParent->m_bSkillEnable = false;
    
    m_bIsStartiming = true;
    
    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP, EFF_SKILL);
    
    //播放动画
    m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING);
    
    //计算CD时间
    m_fCurrentLastTime = 0.0f;

    //计算持续时间
    m_pParent->calculateDurationOfSkill();
    
    m_pParent->setHeroAttrackState(false);

}

void SkillShadowOfHiding::update(float delta)
{
   
    
    m_pParent->m_battleUI->setSkillPercent(100);
    
    //技能CD时间
    m_fCurrentLastTime += delta;
    
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("4");
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    float f_cdTime = pSkillLevelData->getCD();
    
    m_pParent->m_battleUI->setSkillPercent((m_fCurrentLastTime / f_cdTime) * 100);
    
    if (m_fCurrentLastTime >= f_cdTime)
    {
        m_pParent->m_battleUI->setSkillPercent(100);
        
        m_pParent->m_bSkillEnable = true;
        
        m_bIsStartiming = false;
        
    }
}

void SkillShadowOfHiding::end()
{
    if (!m_pParent->m_bIsSkillRunning)
    {
        return;
    }
    
    m_pParent->m_bIsSkillRunning = false;
    
    m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING,true);
    
    m_pParent->releaseShadowOfHiding();
    
    m_pParent->setHeroAttrackState(true);

}

void SkillShadowOfHiding::reset()
{
    m_fTotalLastTime = 0.0f;
    m_fCurrentLastTime = 0.0f;
    
    m_pParent->m_battleUI->setSkillPercent(0.0f);
    
    m_pParent->m_bSkillEnable = false;
    m_pParent->m_bFull = false;
}