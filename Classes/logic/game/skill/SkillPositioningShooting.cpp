//
//  SkillPositioningShooting.cpp
//
//  Created by renyi on 14-9-4.
//
//

#include "SkillPositioningShooting.h"

SkillPositioningShooting::SkillPositioningShooting(GameCombatLayer *parent)
{
    m_pParent = parent;
    
    m_bIsRunning = false;
    
    m_fCurrentLastTime = 0.0f;
}

SkillPositioningShooting::~SkillPositioningShooting()
{
    
}

void SkillPositioningShooting::begin()
{
    if (m_pParent->m_bSkillEnable) {
        m_pParent->m_bSkillEnable = false;
        
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP, EFF_SKILL);
        
        m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING);
        
        m_fCurrentLastTime = 0;
        
        m_bIsRunning = true;
        
        m_pParent->m_bTraceBulletEnable = true;
    }
}

void SkillPositioningShooting::update(float delta)
{
    //获取技能数据
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("6");
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    float fCDtime = pSkillLevelData->getCD();
    
    m_fCurrentLastTime += delta;
    
    if (m_bIsRunning)
    {
        m_pParent->m_battleUI->setSkillPercent(0);
        
        if(m_pParent->m_bTraceBulletEnable)
        {
            m_pParent->m_fPositionShootingLastTime += 1.0f/60.0f;
            
            //当前阶段累计时间
            float fLastTime = pSkillLevelData->getLastTime();
            
            if(m_pParent->m_fPositionShootingLastTime > fLastTime)
            {
                m_pParent->m_fPositionShootingLastTime = 0.0f;
                m_pParent->m_bTraceBulletEnable = false;
                
                
                m_pParent->callBackEnableTraceBullet(NULL);
                
                m_fCurrentLastTime = 0.0f;
                
                
                m_bIsRunning = false;
            }
        }
        
    }
    else
    {
        m_pParent->m_battleUI->setSkillPercent((m_fCurrentLastTime / fCDtime) * 100);
    }
    
    if (m_fCurrentLastTime >= fCDtime) {
        m_pParent->m_bSkillEnable = true;
    }
    
    
}

void SkillPositioningShooting::end()
{
    
}

void SkillPositioningShooting::reset()
{
    
}