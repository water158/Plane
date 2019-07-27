//
//  SoundEffectLayer.h
//  plane
//
//  Created by mini01 on 14-3-28.
//
//

#ifndef __FERRIS_GAME__SoundEffectLayer__
#define __FERRIS_GAME__SoundEffectLayer__

#include "cocos2d.h"
#include "UserData.h"

USING_NS_CC;

class SoundEffectLayer : public CCLayer
{
public:
    CREATE_FUNC(SoundEffectLayer);
    
    bool init();
    
    //游戏音效循环
    void soundEffectLoop(float dt);
    
    //普通子弹音效循环
    void playNormalBulletSoundEffect(float dt);
    
    //背景音乐音效循环
    void playBackgroundSound(float dt);
    
    //普通子弹声音播放时间积累值
    float m_fCurTime;
};

#endif
