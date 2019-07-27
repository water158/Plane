//
//  SkillDefenseBombs.cpp
//
//  Created by renyi on 14-9-4.
//
//

#include "SkillDefenseBombs.h"

SkillDefenseBombs::SkillDefenseBombs(GameCombatLayer *parent)
{
    m_pParent = parent;
    
    m_bIsRunning = false;
    
    m_fCurrentLastTime = 0.0f;
    
    m_fTotalLastTime = 0.0f;
    
    m_fInterval = 0.0f;
}

SkillDefenseBombs::~SkillDefenseBombs()
{
    
}

void SkillDefenseBombs::begin()
{

}

void SkillDefenseBombs::update(float delta)
{
    m_fCurrentLastTime += delta;
    
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("7");
    
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    float f_cdTime = pSkillLevelData->getCD();
    
    if (m_fCurrentLastTime < f_cdTime)
    {
        return;
    }
    else
    {
    
        m_fTotalLastTime += delta;
        
        if (m_fTotalLastTime >= 3/*pSkillLevelData->getLastTime()*/)
        {
            end();
            
            return;
        }
        
        m_fInterval += delta;
        
        if (m_fInterval >= 0.33f)
        {
            //如果在持续时间内
            m_pParent->attackBullet();
            
            m_fInterval = 0.0f;
        }
        
    }
}

void SkillDefenseBombs::end()
{
    m_fCurrentLastTime = 0.0f;
    
    m_fTotalLastTime = 0.0f;
}

void SkillDefenseBombs::reset()
{
    
}