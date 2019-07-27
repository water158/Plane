//
//  GameDelegate.cpp
//  plane
//
//  Created by renyi on 14-01-12.
//
//

#include "GameDelegate.h"


GameDelegate* GameDelegate::s_SharedGameDelegate = NULL;

GameDelegate::GameDelegate()
{
    
}

GameDelegate::~GameDelegate()
{
    if (s_SharedGameDelegate)
    {
        delete s_SharedGameDelegate;
    }
}

GameDelegate* GameDelegate::shared()
{
    if (!s_SharedGameDelegate)
    {
        s_SharedGameDelegate = new GameDelegate();
        s_SharedGameDelegate->init();
    }
    
    return s_SharedGameDelegate;
}

void GameDelegate::init()
{
    CCLog("===================== GameDelegate Info =====================");
    
#if ENCRYPTION_ENABLE
	CCLog("ENCRYPTION_ENABLE = 1");
#else
	CCLog("ENCRYPTION_ENABLE = 0");
#endif
    
	CCLog("===================== GameDelegate  End =====================");
    
    m_nRunningLayerTag = 0;
    
    m_nRunningLayerPauseTimes = 0;
    
    pScene = CCScene::create();
    
    CCDirector::sharedDirector()->runWithScene(pScene);
}

void GameDelegate::initFunctionalLayer()
{
    //Scene
    //CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    
    //Notification layer
    if (!pScene->getChildByTag(kTagNotificationLayer))
    {
        NotificationLayer* layerNotification = NotificationLayer::create();
        layerNotification->setTag(kTagNotificationLayer);
        layerNotification->setZOrder(kZOrderNotificationLayer);
        pScene->addChild(layerNotification);
    }
    
    //Sound Layer
    if (!pScene->getChildByTag(kTagSoundLayer))
    {
        SoundEffectLayer* layerSound = SoundEffectLayer::create();
        layerSound->setTag(kTagSoundLayer);
        layerSound->setZOrder(kZOrderSoundLayer);
        pScene->addChild(layerSound);
    }
}

/*****************************
 * 主要界面切换
 *****************************/

//LOGO界面
void GameDelegate::openSceneSplash()
{
    openSceneCommon(kTagSplashLayer);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    AdManager::shared()->preloadAdmobInterstitial();

#endif
}

//道具界面
void GameDelegate::openSceneTools(bool hasAnimation)
{
    CCLog("********************** 1 ***********************");
    
    closeScenePause();
    
    CCLog("********************** 2 ***********************");
    
    openSceneCommon(kTagToolsLayer,hasAnimation);
    
    CCLog("********************** 3 ***********************");
    
    initFunctionalLayer();
    
    CCLog("********************** 4 ***********************");
}

//游戏主逻辑界面
void GameDelegate::openSceneCombat()
{
    
    LoadHelper::clearAllRes();
    
    //加载主机数据
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_HERO, 0);
    
    //加载僚机数据
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_WING, 0);
    
    //加载掉落物品（金币，道具，装备）
    DataManager::sharedDataManager()->loadDropData(101);//金币
    DataManager::sharedDataManager()->loadDropData(201);//积分
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 301);//道具
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 302);
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 303);
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 304);
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 305);
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 306);
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 307);
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 308);
    DataManager::sharedDataManager()->loadCraftData(CRAFT_DATA_TYPE_DROP, 309);
    
    //加载所有道具数据
    DataManager::sharedDataManager()->loadAllToolsData();
    
    //加载子弹数据
    DataManager::sharedDataManager()->loadAllBulletData();
    
    //加载炮台数据
    DataManager::sharedDataManager()->loadAllConnonData();
    
    //加载所有敌机数据
    DataManager::sharedDataManager()->loadAllEnemyData();
    
    //临时用
    DataManager::sharedDataManager()->loadAllTargetConnonData();
    
    //加载敌机数据
    DataManager::sharedDataManager()->loadAllTargetPlaneData();
    
    //加载关卡数据 和 敌机数据
    MissionDataManager::shared()->loadAllMissionData();
    
    //技能数据的加载
    SkillLibrary::sharedSkillLibrary()->preload();
    
    //其他动画加载（爆炸）
    LoadHelper::preloadAllRes(0, 0, 0, 0, 0, false);
    
    openSceneCommon(kTagGameCombatLayer);
}

//关于界面 Help
void GameDelegate::openSceneHelp()
{
    openSceneCommon(kTagGameHelpLayer);
}

//关于界面 About
void GameDelegate::openSceneAbout()
{
    openSceneCommon(kTagGameAboutLayer);
}

//成就界面 achieve
void GameDelegate::openSceneAchieve()
{
    
    openSceneCommon(kTagGameAchieveLayer);
}

//加载界面 loading
void GameDelegate::openSceneLoading()
{

    if (UserData::shared()->getEnergyNum() > 0)
    {
        if (UserData::shared()->getEnergyNum() == 10)
        {
            UserData::shared()->setTimeRecord(time(0));
        }
        UserData::shared()->decreaseEnergy(1);
        UserData::shared()->save();
    }
    else
    {
        openSceneStoreLayer(kStore_energy);
        
        return;
    }
    
    CCLog("UserData::shared()->getTimeRecord() : %d",UserData::shared()->getTimeRecord());
    
    openSceneCommon(kTagLoadingLayer);
}

/*****************************
 * Running界面管理
 *****************************/

//界面切换的主函数
void GameDelegate::openSceneCommon(int tag,bool hasAnimation)
{
    //Clear Running Layer
    closeRunningLayer();
    
    CCLog("----------------------     1     ------------------");
    
    //Scene
    //CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    
    //Running layer
    CCLayer* layer = NULL;
    
    switch (tag)
    {
        case kTagSplashLayer:
            layer = SplashLayer::create();
            break;
            
        case kTagToolsLayer:
            CCLog("----------------------     2     ------------------");
            layer = GameMainLayer::create(hasAnimation);
            break;
            
        case kTagGameCombatLayer:
            layer = GameCombatLayer::create();
            break;
            
        case kTagGameHelpLayer:
            //layer = GameHelpLayer::create();
            break;
            
        case kTagGameConfigLayer:
            layer = GameConfigLayer::create();
            break;
            
        case kTagGameAboutLayer:
            layer = GameAboutLayer::create();
            break;
            
        case kTagGameAchieveLayer:
            layer = GameAchieveLayer::create();
            break;
            
        case kTagLoadingLayer:
            layer = LoadingLayer::create();
            break;
            
        default:
            break;
    }
    
    layer->setTag(tag);
    layer->setZOrder(kZOrderRunningLayer);
    
    CCLog("----------------------     2     ------------------");
    
    //Add Running Layer to Scene
    pScene->addChild(layer);
    
    //Reset the Tag
    m_nRunningLayerTag = tag;
    
    CCLog("m_nRunningLayerTag = %d",m_nRunningLayerTag);
}

//清理必要的Layer
void GameDelegate::closeRunningLayer()
{
    //Scene
    //CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    
    if(m_nRunningLayerTag > 0)
    {
        CCLog("m_nRunningLayerTag = %d",m_nRunningLayerTag);
        
        pScene->removeChildByTag(m_nRunningLayerTag);
    }
}

//暂停Running Layer
void GameDelegate::pauseRunningLayer()
{
    m_nRunningLayerPauseTimes ++;
    
    CCLog("m_nRunningLayerPauseTimes = %d", m_nRunningLayerPauseTimes);
    
    //Scene
    //CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    
    if(m_nRunningLayerTag > 0)
    {
        pScene->getChildByTag(m_nRunningLayerTag)->onExitTransitionDidStart();
        
        pScene->getChildByTag(m_nRunningLayerTag)->onExit();
    }
}

//恢复Running Layer
void GameDelegate::resumeRunningLayer()
{
    m_nRunningLayerPauseTimes --;
    
    CCLog("m_nRunningLayerPauseTimes = %d", m_nRunningLayerPauseTimes);
    
    //Scene
    //CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    
    if(m_nRunningLayerTag > 0 && m_nRunningLayerPauseTimes == 0)
    {
        pScene->getChildByTag(m_nRunningLayerTag)->onEnter();
        pScene->getChildByTag(m_nRunningLayerTag)->onEnterTransitionDidFinish();
    }
}

/*****************************
 * 获取模糊节点
 *****************************/
GaussianBlur* GameDelegate::getBlureNode()
{
    CCLayer* currentlayer = NULL;
    
    //获取当前运行的层
    if (m_nRunningLayerTag > 0)
    {
        currentlayer = (CCLayer*)pScene->getChildByTag(m_nRunningLayerTag);
    }
    
    if (!currentlayer)
    {
        return NULL;
    }
    
    GaussianBlur::do_ready();
    
    currentlayer->setAnchorPoint(CCPointZero);
    
    GaussianBlur* blurenode = GaussianBlur::screenBlurNodeInstance();
    
    blurenode->reset(currentlayer);
    
    blurenode->setAnchorPoint(ccp(_s_center.x,_s_center.y));
    
    return blurenode;
}

/*****************************
 * 弹出式界面切换
 *****************************/

//打开结算界面 account
void GameDelegate::openSceneAccount(ResultCountData *data)
{
    pauseRunningLayer();
    
    GameAccountLayer* layer = GameAccountLayer::create(data);
    
    layer->setTag(kTagAccountLayer);
    
    layer->setZOrder(kZOrderAccountLayer);
    
    pScene->addChild(layer);
    
}

//关闭结算界面
void GameDelegate::closeSceneAccount()
{
    pScene->removeChildByTag(kTagAccountLayer);
    
    resumeRunningLayer();
}

//打开复活界面 revive
void GameDelegate::openSceneRevive(ResultCountData* data)
{
    if (pScene->getChildByTag(kTagReviveLayer))
    {
        return;
    }
    
    pauseRunningLayer();
    
    ReviveLayer* layer = ReviveLayer::create(data);
    layer->setTag(kTagReviveLayer);
    layer->setZOrder(kZOrderReviveLayer);
    pScene->addChild(layer);
    
}

//关闭复活界面
void GameDelegate::closeSceneRevive()
{
    if (pScene->getChildByTag(kTagReviveLayer))
    {
        pScene->removeChildByTag(kTagReviveLayer);
        
        resumeRunningLayer();
    }
}

//打开抽奖界面 lottery
void GameDelegate::openSceneLottery()
{
    pauseRunningLayer();
    
    GameLotteryLayer* layer = GameLotteryLayer::create();

    layer->setPosition(ccp(0, -670));
    layer->setTag(kTagGameLotteryLayer);
    layer->setZOrder(kZOrderLotteryLayer);
    layer->runAction(CCMoveBy::create(0.5, ccp(0, 670)));
    
    pScene->addChild(layer);
    GaussianBlur* node = getBlureNode();
    node->setTag(kTagBlureNode);
    node->setZOrder(kZOrderBlureNode);
    pScene->addChild(node);
}

//关闭抽奖界面
void GameDelegate::closeSceneLottery()
{
    pScene->removeChildByTag(kTagGameLotteryLayer);
    
    pScene->removeChildByTag(kTagBlureNode);
    
    GaussianBlur::do_free();
    
    resumeRunningLayer();
}

//打开暂停界面 pause
void GameDelegate::openScenePause()
{
    pauseRunningLayer();
    
    PauseLayer* layer = PauseLayer::create();
    layer->addChild(getBlureNode(),-1);
    layer->setTag(kTagGamePauseLayer);
    layer->setZOrder(kZOrderPauseLayer);
    pScene->addChild(layer);
}

//关闭暂停界面
void GameDelegate::closeScenePause()
{
    if(!pScene->getChildByTag(kTagGamePauseLayer))
    {
        return;
    }
    
    pScene->removeChildByTag(kTagGamePauseLayer);
    
    GaussianBlur::do_free();
    
    resumeRunningLayer();
}


//打开配置界面 Config
void GameDelegate::openSceneConfig()
{
    //Pause Running Layer
    pauseRunningLayer();
    
    //Scene
    //CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    
    //configure layer
    GameConfigLayer* layer = GameConfigLayer::create();
    layer->addChild(getBlureNode(),-1);
    layer->setTag(kTagGameConfigLayer);
    layer->setZOrder(kZOrderConfigureLayer);
    pScene->addChild(layer);
    
}

//关闭配置界面
void GameDelegate::closeSceneConfig()
{
    pScene->removeChildByTag(kTagGameConfigLayer);
    
    GaussianBlur::do_free();
    
    resumeRunningLayer();
}

//打开签到界面 signin
void GameDelegate::openSceneSignin()
{
    pauseRunningLayer();
    
    GameSigninLayer* layer = GameSigninLayer::create();
    layer->addChild(getBlureNode(),-1);
    layer->setTag(kTagGameSigninLayer);
    layer->setZOrder(kZOrderSigninLayer);
    pScene->addChild(layer);
    
}

//关闭签到界面
void GameDelegate::closeSceneSignin()
{
    pScene->removeChildByTag(kTagGameSigninLayer);
    
    GaussianBlur::do_free();
    
    resumeRunningLayer();
}

//打开商店界面 store
void GameDelegate::openSceneStoreLayer(StoreType type)
{
    pauseRunningLayer();
    
    switch (type)
    {
        case kStore_energy:
        {
            EnergyStoreLayer* layer = EnergyStoreLayer::create();
            layer->setTag(kTagGameStoreLayer);
            layer->setZOrder(kZOrderGameStoreLayer);
            pScene->addChild(layer);
            break;
        }
            
        case kStore_gold:
        {
            GoldStoreLayer* layer = GoldStoreLayer::create();
            layer->setTag(kTagGameStoreLayer);
            layer->setZOrder(kZOrderGameStoreLayer);
            pScene->addChild(layer);
            
            break;
        }
            
        case kStore_gem:
        {
            GemStoreLayer* layer = GemStoreLayer::create();
            layer->setTag(kTagGameStoreLayer);
            layer->setZOrder(kZOrderGameStoreLayer);
            pScene->addChild(layer);
            
            break;
        }
            
        default:
            break;
    }
    
    
    
}

//关闭商店界面
void GameDelegate::closeSceneStoreLayer()
{
    pScene->removeChildByTag(kTagGameStoreLayer);
    
    GaussianBlur::do_free();
    
    resumeRunningLayer();
}


//打开提示
void GameDelegate::openNotification(const char* str)
{
    //Scene
    //CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    
    //Notification
    NotificationLayer *layerNotification = (NotificationLayer *)pScene->getChildByTag(kTagNotificationLayer);
    
    layerNotification->show(str);
}

//弹出成就
void GameDelegate::openNotification(AchieveItem* item,achieve_type ach_type)
{
    //Notification
    NotificationLayer *layerNotification = (NotificationLayer *)pScene->getChildByTag(kTagNotificationLayer);
    
    layerNotification->showAchieve(item, ach_type);
}

/*****************************
 * 显示广告
 *****************************/
void GameDelegate::showInterstitial()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    AdManager::shared()->showAdmobInterstitial();
    AdManager::shared()->preloadAdmobInterstitial();

#endif
}


//退出
void GameDelegate::exit()
{
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    
	//用于友盟统计
	//MobClickCpp::end();
    
#endif
    
    CCDirector::sharedDirector()->end();
}

