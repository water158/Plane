//
//  SoundEffectManager.cpp
//  plane
//
//  Created by mini01 on 14-3-28.
//
//

#include "SoundEffectManager.h"
#include "SimpleAudioEngine.h"
#include "UserData.h"
#include "GlobalEnum.h"

using namespace CocosDenshion;

SoundEffectManager* SoundEffectManager::m_SharedSoundEffectManager = NULL;

SoundEffectManager* SoundEffectManager::shared()
{
    if (!m_SharedSoundEffectManager)
    {
        m_SharedSoundEffectManager = new SoundEffectManager();
        m_SharedSoundEffectManager->init();
    }
    return m_SharedSoundEffectManager;
}

SoundEffectManager::SoundEffectManager()
{
    
}

SoundEffectManager::~SoundEffectManager()
{
    if (m_SharedSoundEffectManager)
    {
        delete m_SharedSoundEffectManager;
    }
}

bool SoundEffectManager::init()
{
    this->setSoundEffectPriority(0);
    this->setSoundEffectFileName("");
    this->m_curPlaySoundEffectID = 0;
    this->setLayserSoundEnable(true);
    this->setBulletSoundEnable(true);
    this->setInvincibleSoundEnable(true);
    this->setLightningSoundEnable(true);
    this->m_curWarningID = 0;
    this->m_curInvincibleEffectID = 0;
    this->m_curLayserEffectID = 0;
    this->m_curBulletEffectID = 0;
    this->m_curMissileEffectID = 0;
    this->m_curCoinEffectID = 0;
    this->m_curScoreEffectID = 0;
    //this->setSoundEffectEnabled(true);
    //this->setBackgroundEnabled(true);
    return true;
}

void SoundEffectManager::clear()
{
    this->m_curPlaySoundEffectID = 0;
}

void SoundEffectManager::preLoadGameMusic()
{
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MIS_MAIN_BG);
    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MIS_BATTLE_BG);
}

void SoundEffectManager::preLoadGameSound()
{
    //按钮
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_CLICK_HEAVY);
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_CLICK_LIGHT);
    //开门
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_OPENDOOR);
    
    //技能
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_SKILL);
    //使用道具
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_USE_TOOL_1);
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_USE_TOOL_2);
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_USE_TOOL_3);
    //获得道具
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_GET_TOOL);
    //获得金币
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_GET_COIN);
    //获得积分
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_GET_SCORE);
    //子弹 和 导弹
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_NORMAL_BULLET);
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_MISSLE);
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_LAYSER);
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_LIGHTNING);
    //爆炸
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_BOOM_SMALL);
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_BOOM_BIG);
    //受击
    SimpleAudioEngine::sharedEngine()->preloadEffect(EFF_HURT_HERO);
    
}

void SoundEffectManager::unloadGameSound(int state)
{
    
    //main
    if (state == GAME_SCENE_TYPE_MAIN)
    {
        //按钮
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_CLICK_HEAVY);
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_CLICK_LIGHT);
        
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_OPENDOOR);
    }
    //game
    else if (state == GAME_SCENE_TYPE_BATTLE)
    {
        if (!UserData::shared()->getSoundEnable())
        {
            return;
        }
        
        //技能
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_SKILL);
        //使用道具
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_USE_TOOL_1);
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_USE_TOOL_2);
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_USE_TOOL_3);
        //获得道具
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_GET_TOOL);
        //获得金币
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_GET_COIN);
        //获得积分
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_GET_SCORE);
        //子弹 和 导弹
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_NORMAL_BULLET);
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_MISSLE);
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_LAYSER);
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_LIGHTNING);
        //爆炸
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_BOOM_SMALL);
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_BOOM_BIG);
        //受击
        SimpleAudioEngine::sharedEngine()->unloadEffect(EFF_HURT_HERO);
    }
    
}

void SoundEffectManager::requestPlayBackgroundMisc(const char *fileName)
{
    if (!UserData::shared()->getMusicEnable())
    {
        return;
    }
    this->setBackgroundMiscFileName(fileName);
    this->playBackgroundSound();
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.4f);
}

void SoundEffectManager::playBackgroundSound()
{
    //如果有背景音乐正在运行，先停止正在播放的背景音乐，然后播放背景音乐
    if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(this->getBackgroundMiscFileName().c_str(), true);
}

void SoundEffectManager::requestPlaySoundEffect(int soundEffectPriority, std::string soundEffectFileName)
{
    if (!UserData::shared()->getSoundEnable())
    {
        return;
    }
    
    /************************
     *如果传入需要播放的音效优先级大于当前音效优先级重置成员变量
     ************************/
    if (soundEffectPriority > curSoundEffectPriority)
    {
        this->setSoundEffectPriority(soundEffectPriority);
        this->setSoundEffectFileName(soundEffectFileName);
    }
    
    this->playSoundEffect();
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.4f);
}

void SoundEffectManager::playSoundEffect()
{
    //todo::判断音乐是否可以播放
    if (curSoundEffectPriority == 0)
    {
        return;
    }
    //CCLog("PLAYING %s", pszPersSoundFileName.c_str());
    SimpleAudioEngine::sharedEngine()->playEffect(pszSoundEffectFileName.c_str());
    
    this->setSoundEffectFileName("");
    //将当前音效的优先级重置为持续播放音效的优先级®
    this->setSoundEffectPriority(0);
}

void SoundEffectManager::playPersistentSoundEffect()
{
    m_curPlaySoundEffectID = SimpleAudioEngine::sharedEngine()->playEffect(pszPersSoundFileName.c_str(), true);
}

void SoundEffectManager::stopPersistentSoundEffect()
{
    SimpleAudioEngine::sharedEngine()->stopEffect(m_curPlaySoundEffectID);
    m_curPlaySoundEffectID = 0;
}

void SoundEffectManager::requestPlayPerSoundEffect(const char *fileName)
{
    if (!UserData::shared()->getSoundEnable())
    {
        return;
    }
    this->setBulletSoundEnable(true);
    this->setPerSoundFileName(fileName);
    this->playPersistentSoundEffect();
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.8f);
}

void SoundEffectManager::requestStopPlayPerSoundEffect()
{
    this->stopPersistentSoundEffect();
    this->setBulletSoundEnable(false);
}

void SoundEffectManager::pauseAllSound()
{
    
    //SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    this->requestStopPlayPerSoundEffect();
}

void SoundEffectManager::resumeAllSound()
{
    if (UserData::shared()->getSoundEnable())
    {
        SimpleAudioEngine::sharedEngine()->resumeAllEffects();
        this->requestPlayPerSoundEffect(0);
    }
    else
    {
        this->setBulletSoundEnable(false);
    }
    if (UserData::shared()->getMusicEnable())
    {
        //CCLOG("%s",this->getBackgroundMiscFileName().c_str());
        //SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        //this->playBackgroundSound();
    }
}

void SoundEffectManager::stopAllSound()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
}

void SoundEffectManager::stopAllSoundEffect()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void SoundEffectManager::stopBackgroundSound()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundEffectManager::pauseBackgroundSound()
{
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    //this->setBackgroundMiscFileName("");
}

void SoundEffectManager::resumeBackgroundSound()
{
    if (UserData::shared()->getMusicEnable())
    {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
}

void SoundEffectManager::requestPlayLayserEffect(const char* fileName)
{
    if (!this->getLayserSoundEnable())
    {
        return;
    }
    else
    {
        CCLog("play warning sound");
        if (UserData::shared()->getSoundEnable())
        {
            m_curLayserEffectID = SimpleAudioEngine::sharedEngine()->playEffect(fileName, true);
            this->setLayserSoundEnable(false);
        }
    }
    
}

void SoundEffectManager::requestStopLayserEffect()
{
    //如果当前激光可播放状态为true 则说明音效未播放 直接返回
    if (this->getLayserSoundEnable())
    {
        return;
    }
    else if (m_curLayserEffectID != 0)
    {
        CCLog("stop warning sound");
        SimpleAudioEngine::sharedEngine()->stopEffect(m_curLayserEffectID);
        this->setLayserSoundEnable(true);
        m_curLayserEffectID = 0;
    }
}

void SoundEffectManager::requestPlayWarningSound(const char *fileName)
{
    if(UserData::shared()->getSoundEnable())
    {
        m_curWarningID = SimpleAudioEngine::sharedEngine()->playEffect(fileName);
    }
}

void SoundEffectManager::requestStopWarningSound()
{
    SimpleAudioEngine::sharedEngine()->stopEffect(m_curWarningID);
}

//请求播放无敌道具声音
void SoundEffectManager::requestPlayInvincibleSound(const char* fileName)
{
    if (!this->getInvincibleSoundEnable())
    {
        return;
    }
    else
    {
        CCLog("play invincible sound");
        if (UserData::shared()->getSoundEnable())
        {
            m_curInvincibleEffectID = SimpleAudioEngine::sharedEngine()->playEffect(fileName, true);
            this->setInvincibleSoundEnable(false);
        }
    }
    
}

//请求停止无敌道具音效
void SoundEffectManager::requestStopInvincibleSound()
{
    if (this->getInvincibleSoundEnable())
    {
        return;
    }
    else if (m_curInvincibleEffectID != 0)
    {
        CCLog("stop invincible sound");
        SimpleAudioEngine::sharedEngine()->stopEffect(m_curInvincibleEffectID);
        this->setInvincibleSoundEnable(true);
        m_curInvincibleEffectID = 0;
    }
    
}

//请求播放闪电声音
void SoundEffectManager::requestPlayLightningSound(const char* fileName)
{
    if (!this->getLightningSoundEnable())
    {
        return;
    }
    else
    {
        CCLog("play warning sound");
        if (UserData::shared()->getSoundEnable())
        {
            m_curLightningEffectID = SimpleAudioEngine::sharedEngine()->playEffect(fileName, true);
            this->setLightningSoundEnable(false);
        }
    }
    
}

//请求停止闪电音效
void SoundEffectManager::requestStopLightningSound()
{
    if (this->getLightningSoundEnable())
    {
        return;
    }
    else if (m_curLightningEffectID != 0)
    {
        CCLog("stop lightning sound");
        SimpleAudioEngine::sharedEngine()->stopEffect(m_curLightningEffectID);
        this->setLightningSoundEnable(true);
        m_curLightningEffectID= 0;
    }
    
}

//请求播放子弹音效
void SoundEffectManager::requestPlayBulletSouncEffect(const char* fileName)
{
    //    if (!this->getBulletSoundEnable())
    //    {
    //        return;
    //    }
    //    else
    //    {
    if (UserData::shared()->getSoundEnable())
    {
        m_curBulletEffectID = SimpleAudioEngine::sharedEngine()->playEffect(fileName);
        //            this->setBulletSoundEnable(false);
    }
    //    }
}

//停止子弹音效
void SoundEffectManager::stopBulletSoundEffect()
{
    //    if (this->getBulletSoundEnable())
    //    {
    //        return;
    //    }
    //    else
    if (m_curBulletEffectID != 0)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(m_curBulletEffectID);
        //        this->setBulletSoundEnable(true);
        m_curBulletEffectID= 0;
    }
    
}

//恢复子弹音效
void SoundEffectManager::resumeBulletSoundEffect()
{
    //    if (!this->getBulletSoundEnable())
    //    {
    //        return;
    //    }
    //    else
    //    {
    if (UserData::shared()->getSoundEnable())
    {
        SimpleAudioEngine::sharedEngine()->resumeEffect(m_curBulletEffectID);
        //            this->setBulletSoundEnable(false);
    }
    //    }
}

//暂停子弹音效
void SoundEffectManager::pauseBulletSoundEffect()
{
    //    if (this->getBulletSoundEnable())
    //    {
    //        return;
    //    }
    //    else
    if (m_curBulletEffectID != 0)
    {
        CCLog("m_curBulletEffectID   ========   %d",m_curBulletEffectID);
        SimpleAudioEngine::sharedEngine()->pauseEffect(m_curBulletEffectID);
        //        this->setBulletSoundEnable(false);
    }
    
}

