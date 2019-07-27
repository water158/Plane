//
//  GameDelegate.h
//  plane
//
//  Created by renyi on 14-01-12.
//
//

#ifndef __FERRIS_GAME__GameDelegate__
#define __FERRIS_GAME__GameDelegate__

#include "cocos2d.h"
#include "GlobalTagZOrder.h"

#include "FilterNode.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

//ad
#include "AdManager.h"

#endif

//layers
#include "SplashLayer.h"
#include "GameMainLayer.h"
//#include "GameHelpLayer.h"
#include "GameConfigLayer.h"
#include "GameAboutLayer.h"
#include "SoundEffectLayer.h"
#include "NotificationLayer.h"
#include "LoadHelper.h"
#include "GameCombatLayer.h"
#include "GameAchieveLayer.h"
#include "GameSigninLayer.h"
#include "PauseLayer.h"
#include "GameLotteryLayer.h"
#include "EnergyStoreLayer.h"
#include "GoldStoreLayer.h"
#include "GemStoreLayer.h"
#include "ReviveLayer.h"
#include "GameAccountLayer.h"
#include "LoadingLayer.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

//#include "MobClickCpp.h"

#endif

USING_NS_CC;

class GameDelegate
{
private:
    
    static GameDelegate* s_SharedGameDelegate;
    
    void init();
    
    void initFunctionalLayer();
    
public:
    
    GameDelegate();
    ~GameDelegate();
    static GameDelegate* shared();
    
    /*****************************
     * 主要界面切换
     *****************************/
    
    //LOGO界面
    void openSceneSplash();
    
    //道具界面  是否有动画
    void openSceneTools(bool hasAnimation);
    
    //游戏主逻辑界面
    void openSceneCombat();
    
    //关于界面 About
    void openSceneAbout();
    
    //关于界面 Help
    void openSceneHelp();
    
    //加载界面 loading
    void openSceneLoading();
    
    /*****************************
     * Running界面管理
     *****************************/
    
    //界面切换的主函数
    void openSceneCommon(int tag,bool hasAnimation = false);
    
    //清理必要的Layer
    void closeRunningLayer();
    
    //暂停Running Layer
    void pauseRunningLayer();
    
    //恢复Running Layer
    void resumeRunningLayer();
    
    /*****************************
     * 弹出式界面切换
     *****************************/
    
    //打开提示
    void openNotification(const char* str);
    
    //弹出成就
    void openNotification(AchieveItem* item,achieve_type ach_type);
    
    //配置界面 Config
    void openSceneConfig();
    
    //关闭配置界面
    void closeSceneConfig();
    
    //打开签到界面 signin
    void openSceneSignin();
    
    //关闭签到界面
    void closeSceneSignin();
    
    //打开暂停界面 pause
    void openScenePause();
    
    //关闭暂停界面
    void closeScenePause();
    
    //打开成就界面 Achieve
    void openSceneAchieve();
    
    //关闭成就界面
    void closeSceneAchieve();
    
    //打开抽奖界面 Lottery
    void openSceneLottery();
    
    //关闭抽奖界面
    void closeSceneLottery();
    
    //打开商店界面 store
    void openSceneStoreLayer(StoreType type);
    
    //关闭商店界面
    void closeSceneStoreLayer();
    
    //打开复活界面 revive
    void openSceneRevive(ResultCountData* data);
    
    //关闭复活界面
    void closeSceneRevive();
    
    //打开结算界面 account
    void openSceneAccount(ResultCountData* data);
    
    //关闭结算界面
    void closeSceneAccount();
    
    /*****************************
     * 显示广告
     *****************************/
    void showInterstitial();

    
    /*****************************
     * 获取模糊节点
     *****************************/
    
    GaussianBlur* getBlureNode();
    
    //退出
    void exit();
    

public:
    
    //当前Layer的Tag
    int m_nRunningLayerTag;
    
private:
    
    //
    int m_nRunningLayerPauseTimes;
    
    //
    CCScene *pScene;
};

#endif