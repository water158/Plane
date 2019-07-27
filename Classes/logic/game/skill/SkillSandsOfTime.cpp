//
//  SkillSandsOfTime.cpp
//
//  Created by renyi on 14-9-4.
//
//

#include "SkillSandsOfTime.h"

SkillSandsOfTime::SkillSandsOfTime(GameCombatLayer *parent)
{
    m_pParent = parent;
    
    m_bIsRunning = false;
    
    m_fTotalLastTime = 0.0f;
    
    m_fCurrentLastTime = 0.0f;
    
    m_fTimeFlag = 0.0f;
}

SkillSandsOfTime::~SkillSandsOfTime()
{
    
}

void SkillSandsOfTime::begin()
{
    //播放动画
    if (m_pParent->m_bSkillEnable)
    {
        m_pParent->m_bSkillEnable = false;
        
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP, EFF_SKILL);
        
        m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SANDS_OF_TIME);
        
        m_pParent->m_pCombatScheduler->setTimeScale(TIME_SCALE_SLOW);
        
        m_pParent->m_battleUI->setSkillPercent(0);
        
        m_fTimeFlag = 0.0f;
        
        m_bIsRunning = true;
        
        //获取技能数据
        SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("1");
        SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
        
        //获取持续时间
        m_fTotalLastTime = pSkillLevelData->getLastTime();
    }

}

void SkillSandsOfTime::update(float delta)
{
    
    if (m_bIsRunning)
    {
        m_fCurrentLastTime += delta;
    }
    
    m_fTimeFlag += delta;
    
    //获取技能数据
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("1");
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    float percent = (m_fTimeFlag / pSkillLevelData->getCD()) * 100;
    
    m_pParent->m_battleUI->setSkillPercent(percent);

    
    if(m_bIsRunning)
    {
        m_fTimeFlag = 0;
    }
    
    if (percent >= 100)
    {
        m_pParent->callBackEnableSkill(NULL);
    }
    
    if (m_fCurrentLastTime > m_fTotalLastTime)
    {
        end();
    }
}

void SkillSandsOfTime::end()
{
    if (!m_pParent->m_bSkillEnable) {
        m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SANDS_OF_TIME,true);
    }
    
    m_pParent->m_pCombatScheduler->setTimeScale(TIME_SCALE_NORMAL);
    
    m_bIsRunning = false;
    
    m_fCurrentLastTime = 0.0f;
}

void SkillSandsOfTime::reset()
{
    m_fTotalLastTime = 0.0f;
    m_fCurrentLastTime = 0.0f;
    m_fTimeFlag = 0.0f;
    
    m_pParent->m_battleUI->setSkillPercent(0.0f);
    
    m_pParent->m_bSkillEnable = false;
    m_pParent->m_bFull = false;
}