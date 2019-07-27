//
//  SkillAddedBlood.cpp
//  ambition
//
//  Created by mini02 on 15/2/5.
//
//

#include "SkillAddedBlood.h"

SkillAddedBlood::SkillAddedBlood(GameCombatLayer *parent)
{
    m_pParent = parent;
    
    m_bIsRunning = false;
    
    m_fCurrentLastTime = 0.0f;
    
    m_fTotalLastTime = 0.0f;
}

SkillAddedBlood::~SkillAddedBlood()
{
    
}

void SkillAddedBlood::begin()
{
    
}

void SkillAddedBlood::update(float delta)
{
    m_fCurrentLastTime += delta;
    
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("9");
    
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    float cdTime = pSkillLevelData->getCD();
    
    if (m_fCurrentLastTime >= cdTime)
    {
        m_pParent->createBloodCell(m_pParent->getWingPosition());
        
        end();
    }
}

void SkillAddedBlood::end()
{
    m_fCurrentLastTime = 0.0f;
}

void SkillAddedBlood::reset()
{

}