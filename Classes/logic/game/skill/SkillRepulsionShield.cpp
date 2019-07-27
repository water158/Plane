//
//  SkillRepulsionShield.cpp
//
//  Created by renyi on 14-9-4.
//
//

#include "SkillRepulsionShield.h"

SkillRepulsionShield::SkillRepulsionShield(GameCombatLayer *parent)
{
    m_pParent = parent;
    
    m_bIsRunning = false;
    
     m_fCurrentLastTime = 0.0f;
}

SkillRepulsionShield::~SkillRepulsionShield()
{
    
}

void SkillRepulsionShield::begin()
{
    if(m_pParent->m_pMainGun && m_pParent->m_bSkillEnable)
    {
        m_bIsRunning = true;
        
        m_pParent->m_bSkillEnable = false;
        
        EquipItem* pEquipData = m_pParent->m_pMainGun->getEquipItemData();
        
        if(pEquipData)
        {
            if (!m_pParent->getChildByTag(kTagSkillRepulsionShiled))
            {
                
                m_pParent->m_battleUI->setSkillPercent(0);
                
                SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP, EFF_SKILL);
                
                m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_REPULSION_SHIELD);
                
                CCSprite* pShapedGun = CCSprite::create("game/skill/repulsion_shiled.png");
                pShapedGun->setAnchorPoint(ccp(0.5f, 0.5f));
                pShapedGun->setScale(4.5f);
                pShapedGun->setPosition(m_pParent->m_pHeroView->getPosition());
                m_pParent->addChild(pShapedGun, GAME_ZORDER_HERO, kTagSkillRepulsionShiled);
            }
        }
        
    }
}

void SkillRepulsionShield::update(float delta)
{
    //this->setHeroAttrackState(false);
    CCSprite* pShiled = (CCSprite*)m_pParent->getChildByTag(kTagSkillRepulsionShiled);
    
    //获取技能数据
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("5");
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    m_fCurrentLastTime += delta;

    if (m_bIsRunning)
    {
        m_pParent->m_battleUI->setSkillPercent(0);
        
        if(pShiled)
        {
            //更新持续时间
            m_pParent->m_fRepulsionShiledLastTime += 1.0f/60.0f;
            
            //当前阶段累计时间
            float fLastTime = pSkillLevelData->getLastTime();

            if(m_pParent->m_fRepulsionShiledLastTime > fLastTime)
            {
                
                m_pParent->m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_REPULSION_SHIELD,true);
                
                m_pParent->removeChildByTag(kTagSkillRepulsionShiled);
                m_pParent->m_fRepulsionShiledLastTime = 0.0f;
                
                m_fCurrentLastTime = 0.0f;
                
                m_bIsRunning = false;
                
                m_pParent->m_bSkillEnable = false;
                
            }
        }

    }
    else
    {
        m_pParent->m_battleUI->setSkillPercent((m_fCurrentLastTime / pSkillLevelData->getCD()) * 100);
    }
    
    if (m_fCurrentLastTime >= pSkillLevelData->getCD())
    {
        m_pParent->m_bSkillEnable = true;
    }
    
}

void SkillRepulsionShield::end()
{
    
}

void SkillRepulsionShield::reset()
{
    m_fTotalLastTime = 0.0f;
    m_fCurrentLastTime = 0.0f;
    
    m_pParent->m_battleUI->setSkillPercent(0.0f);
    
    m_pParent->m_bSkillEnable = false;
    m_pParent->m_bFull = false;
}