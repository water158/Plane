//
//  SoundEffectManager.h
//  plane
//
//  Created by mini01 on 14-3-28.
//
//

#ifndef __FERRIS_GAME__SoundEffectManager__
#define __FERRIS_GAME__SoundEffectManager__

#include "cocos2d.h"

USING_NS_CC;

class SoundEffectManager : public CCNode
{
public:
    static SoundEffectManager* shared();
    
    void requestPlaySoundEffect(int soundEffectPriority,std::string soundEffectFileName);
    
    void playSoundEffect();
    
    //请求播放普通子弹音效
    void requestPlayPerSoundEffect(const char* fileName);
    
    //播放普通子弹音效
    void playPersistentSoundEffect();
    
    //请求停止普通音效子弹
    void requestStopPlayPerSoundEffect();
    
    //停止普通子弹音效
    void stopPersistentSoundEffect();
    
    //请求播放背景音乐
    void requestPlayBackgroundMisc(const char* fileName);
    
    //请求播放激光音效
    void requestPlayLayserEffect(const char* fileName);
    
    //请求停止激光音效
    void requestStopLayserEffect();
    
    //请求播放警告声音
    void requestPlayWarningSound(const char* fileName);
    
    //请求停止警告音效
    void requestStopWarningSound();
    
    //请求播放无敌道具声音
    void requestPlayInvincibleSound(const char* fileName);
    
    //请求停止警告音效
    void requestStopInvincibleSound();
    
    //请求播放闪电声音
    void requestPlayLightningSound(const char* fileName);
    
    //请求停止闪电音效
    void requestStopLightningSound();
    
    //播放背景音乐
    void playBackgroundSound();
    
    //停止背景音乐
    void stopBackgroundSound();
    
    //暂停背景音乐
    void pauseBackgroundSound();
    
    //恢复背景音乐
    void resumeBackgroundSound();
    
    //暂停所有声音
    void pauseAllSound();
    
    //恢复所有声音
    void resumeAllSound();
    
    //停止所有声音
    void stopAllSound();
    
    //停止播放所有音效  用于回合结束
    void stopAllSoundEffect();
    
    //当前播放持续音效的ID
    int m_curPlaySoundEffectID;
    
    //当前激光音效的ID
    int m_curLayserEffectID;
    
    //当前警告声音ID
    int m_curWarningID;
    
    //当前无敌道具音效的ID
    int m_curInvincibleEffectID;
    
    //当前闪电声音ID
    int m_curLightningEffectID;
    
    //当前普通子弹音效ID
    int m_curBulletEffectID;
    
    //当前导弹音效ID
    int m_curMissileEffectID;
    
    //当前金币音效ID
    int m_curCoinEffectID;
    
    //当前积分音效ID
    int m_curScoreEffectID;
    
    //请求播放子弹音效
    void requestPlayBulletSouncEffect(const char* fileName);
    //停止子弹音效
    void stopBulletSoundEffect();
    //暂停子弹音效
    void pauseBulletSoundEffect();
    //恢复子弹音效
    void resumeBulletSoundEffect();
    
    void clear();
    
    //预加载音乐和音效
    void preLoadGameMusic();
    void preLoadGameSound();
    
    //卸载音效
    void unloadGameSound(int state);

private:
    static SoundEffectManager* m_SharedSoundEffectManager;
    
    bool init();
    
    SoundEffectManager();
    ~SoundEffectManager();
    
    //当前音效的优先级，并设置获取设置方法
    CC_SYNTHESIZE(int, curSoundEffectPriority, SoundEffectPriority);
    
    //当前音效的文件名，并设置获取设置方法
    CC_SYNTHESIZE(std::string, pszSoundEffectFileName, SoundEffectFileName);
    
    //当前持续音效的文件名，并设置获取设置方法
    CC_SYNTHESIZE(std::string, pszPersSoundFileName, PerSoundFileName);
    
    //当前背景音乐文件名
    CC_SYNTHESIZE(std::string, pszBackgroundMiscName, BackgroundMiscFileName);
    
    //是否播放子弹音效 用于回合结束停止普通子弹音效
    CC_SYNTHESIZE(bool, isPlayBulletSound, BulletSoundEnable);
    
    //激光音效是否可播放
    CC_SYNTHESIZE(bool, isPlayLayser, LayserSoundEnable);
    
    //无敌道具音效是否可播放
    CC_SYNTHESIZE(bool, isPlayInvincible, InvincibleSoundEnable);
    
    //闪电音效是否可播放
    CC_SYNTHESIZE(bool, isPlayLightning, LightningSoundEnable);
    
    //音效是否可播放
    //CC_SYNTHESIZE(bool, soundEffectEnabled, SoundEffectEnabled);
    
    //背景音乐是否可播放
    //CC_SYNTHESIZE(bool, backgroundEnabled, BackgroundEnabled);
};

#endif
