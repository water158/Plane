//
//  SoundEffectLayer.cpp
//  plane
//
//  Created by mini01 on 14-3-28.
//
//

#include "SoundEffectLayer.h"
#include "SoundEffectLayer.h"
#include "SimpleAudioEngine.h"
#include "GlobalEnum.h"

using namespace CocosDenshion;

bool SoundEffectLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    //初始化普通子弹播放时间积累值
    m_fCurTime = 0.0f;
    
    this->schedule(schedule_selector(SoundEffectLayer::playBackgroundSound));
    this->schedule(schedule_selector(SoundEffectLayer::soundEffectLoop), 0.1f);
    this->schedule(schedule_selector(SoundEffectLayer::playNormalBulletSoundEffect));
    
    return true;
}


void SoundEffectLayer::soundEffectLoop(float dt)
{
    //SoundEffectLayer::sharedSoundEffectManager()->playSoundEffect();
}

void SoundEffectLayer::playNormalBulletSoundEffect(float dt)
{
    //if (!//SoundEffectLayer::sharedSoundEffectManager()->getBulletSoundEnable())
    //{
    //    return;
    //}
    m_fCurTime += dt;
    if (m_fCurTime > 5.0f)
    {
        //SoundEffectLayer::sharedSoundEffectManager()->stopPersistentSoundEffect();
        
        m_fCurTime = 0.0f;
        //SoundEffectLayer::sharedSoundEffectManager()->playPersistentSoundEffect();
    }
}

void SoundEffectLayer::playBackgroundSound(float dt)
{
    //todo::判断背景音乐是否能够播放
    if (false)
    {
        //SoundEffectLayer::sharedSoundEffectManager()->stopBackgroundSound();
    }
}