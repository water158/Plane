//
//  HelloCppAppDelegate.h
//  HelloCpp
//
//  Created by mini01 on 14-8-12.
//  Copyright __MyCompanyName__ 2014年. All rights reserved.
//

#ifndef __FERRIS_GAME__AppDelegate__
#define __FERRIS_GAME__AppDelegate__

#include "CCApplication.h"

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)

//#include "MobClickCpp.h"

#endif

/**
@brief    The cocos2d Application.

The reason to implement with private inheritance is to hide some interface details of CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function is called when the application enters the background
    @param  the pointer of the application instance
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function is called when the application enters the foreground
    @param  the pointer of the application instance
    */
    virtual void applicationWillEnterForeground();
};

#endif

