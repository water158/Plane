//
//  HelloCppAppDelegate.cpp
//  HelloCpp
//
//  Created by mini01 on 14-8-12.
//  Copyright __MyCompanyName__ 2014年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SmartRes.h"
#include "GlobalEnum.h"
#include "SplashLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
    //释放音乐资源
    SimpleAudioEngine::sharedEngine()->end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

    //用于友盟
    //MobClickCpp::startWithAppkey(ID_UMENG);

#endif
    
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);
    SmartRes::sharedRes()->setVirtualScreenHeight(960);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // 调用Start界面
    GameDelegate::shared()->openSceneSplash();
    
    //预加载音乐和音效
    SoundEffectManager::shared()->preLoadGameMusic();
    SoundEffectManager::shared()->preLoadGameSound();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    if (GameDelegate::shared()->m_nRunningLayerTag == kTagGameCombatLayer)
    {
        GameDelegate::shared()->openScenePause();
    }

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

    //用于友盟
    //MobClickCpp::applicationDidEnterBackground();

#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    //调用恢复背景音乐和音效的代码
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

    //友盟统计
    //MobClickCpp::applicationWillEnterForeground();

#endif
}
