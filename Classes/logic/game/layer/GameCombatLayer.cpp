#include "GameCombatLayer.h"
#include "FerrisParticle.h"

GameCombatLayer::GameCombatLayer(void)
{
    mission = NULL;
    
    /******* plist载入 *******/
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/equip/equip_icon.plist");//装备icon
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/achieve/achieve.plist");//读取 成就图标的文件
}

GameCombatLayer::~GameCombatLayer(void)
{
    
    CC_SAFE_RELEASE_NULL(m_pDictEnemy);
    CC_SAFE_RELEASE_NULL(m_pArrayTaskEnemy);
    CC_SAFE_RELEASE_NULL(m_pArrayHeroBullet);
    CC_SAFE_RELEASE_NULL(m_pArrayWingBullet);
    CC_SAFE_RELEASE_NULL(m_pArrayEnemyBullet);
    CC_SAFE_RELEASE_NULL(m_pArrayToolBullets);
    CC_SAFE_RELEASE_NULL(m_pArrayRemove);
    CC_SAFE_RELEASE_NULL(m_pArrayCoin);
    CC_SAFE_RELEASE_NULL(m_pArrayTool);
    CC_SAFE_RELEASE_NULL(m_pArrInvincibleTool);
    CC_SAFE_RELEASE_NULL(m_pArrayFlyTool);
    CC_SAFE_RELEASE_NULL(m_pArrayToolPlane);
    CC_SAFE_RELEASE_NULL(m_pArrayBloodParticles);
    CC_SAFE_RELEASE_NULL(m_pArrayBlood);
    CC_SAFE_RELEASE_NULL(m_pArrayScore);
    CC_SAFE_RELEASE_NULL(m_pArrayBolts);
    
    /* 释放 */
    
    //僚机动画
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("game/wing/plane_wing.ExportJson");
    //主机动画
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("game/hero/fuckingplane_final.ExportJson");
    //爆炸1\2\3
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/craft_boom_1.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/craft_boom_2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/craft_boom_3.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/craft_boom_4.plist");
    //子弹
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/ammo/bullet.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/drops/drops.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/effect_expand.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/effect_hit.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/trace_missile_hit.plist");
    //装备icon
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("ui/icon/equip/equip_icon.plist");
    //成就图标的文件
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("ui/icon/achieve/achieve.plist");
    
    this->getScheduler()->setTimeScale(1.0f);
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    UserData::shared()->setReviveTimes(0);
    
}

GameCombatLayer* GameCombatLayer::create()
{
    GameCombatLayer* layer = new GameCombatLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    
    return NULL;
    
}

bool GameCombatLayer::init()
{
    CCLayer::init();
    
    //设置随机种子
    srand((unsigned)time(NULL));
    
    /***** 统计初始化 *****/
    
    //日常统计出击次数
    DailyCountData::sharedInstance()->addAttackTime();
    //游戏统计出击次数
    GameCountData::sharedInstance()->addAttackTime();
    m_resultCountData = new ResultCountData();
    
    //添加观察者处理复活事件
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameCombatLayer::revive), MSG_REVIVE, NULL);
    
    /******属性数值初始化********/
    f_toolDrop = BASE_TOOL_DROP;
    f_toolDrop = f_toolDrop *((UserEquipBag::shared()->getTotalDropTool() + 100.0f) / 100.0f);
    if (UserData::shared()->getToolNum(1) > 0)
    {
        f_toolDrop = f_toolDrop * 1.5f;
    }
    
    f_bloodDrop = BASE_BLOOD_DROP;
    f_bloodDrop = f_bloodDrop *((UserEquipBag::shared()->getTotalDropBlood() + 100.0f) / 100.0f);
    if (UserData::shared()->getToolNum(2) > 0)
    {
        f_bloodDrop = f_bloodDrop * 1.5f;
    }
    
    f_hpAdd = UserEquipBag::shared()->getTotalHpAdd();
    if (UserData::shared()->getToolNum(3) > 0)
    {
        f_hpAdd = f_hpAdd * 1.3f;
    }
    
    b_skipTwenty = false;
    if (UserData::shared()->getToolNum(4) > 0)
    {
        b_skipTwenty = true;
    }
    
    b_canRevive = false;
    if (UserData::shared()->getToolNum(5) > 0)
    {
        b_canRevive = true;
    }
    
    b_tripleCoin = false;
    if (UserData::shared()->getToolNum(6) > 0)
    {
        b_tripleCoin = true;
    }
    
    UserData::shared()->decreaseTool();
    
    /***** 初始化自定义sheduler和actionmanager****/
    //时间管理器
    CCScheduler *defaultScheduler = CCDirector::sharedDirector()->getScheduler();
    m_pCombatScheduler = new CCScheduler();
    defaultScheduler->scheduleUpdateForTarget(m_pCombatScheduler, 0, false);
    m_pCombatActionManager = new CCActionManager();
    m_pCombatScheduler->scheduleUpdateForTarget(m_pCombatActionManager, 0, false);
    
    /***** 变量初始化 *****/
    m_pWingSkill = NULL;
    
    gameOver = false;
    m_nGameTime = 0;
    m_nGameWave = 0;
    m_nPlaneGroupId = 1;
    m_nStrengtimes = 0;
    m_nBloodAddedValue = 3;
    m_enemyAttackBase = 11;
    m_bSkillEnable = false;
    m_bFull = false;
    m_bTraceBulletEnable = false;
    m_bWingSkillEnable = true;
    m_posPrePoint = CCPointZero;
    m_fCurrentPrepareTime = 0.0f;
    m_nShapedGunCurrentPhrase = 0;
    m_fShapedGunCurrentPhraseTime = 0.0f;
    m_fRepulsionShiledLastTime = 0.0f;
    m_fPositionShootingLastTime = 0.0f;
    m_nToolGridNum = 1;
    m_nInvincibleTimes = 0;
    batteryNumCount = 0;
    m_nScore = 0;
    curMainGunPhrase = 1;//主炮攻击阶段
    curDeputy1GunPhrase = 3;//副炮1攻击阶段
    curDeputy2GunPhrase = 3;//副炮2攻击阶段
    bulletAppearTime = 0.0f;
    toolPlaneNumCount = 0;
    m_bIsSkillRunning = false;
    preNormalToolRate = 0;
    m_fDelayTime = 0.0f;
    
    flyPlaneCount = 0;
    
    m_nShatter = 0;
    m_nCoin = 0;
    m_nScore = 0;
    
    /* 清理音效ID */
    SoundEffectManager::shared()->clear();
    
    /*****  判断各种炮是否激光  *****/
    EquipItem* equip_mainGun = UserEquipBag::shared()->getMainEquip();
    
    int nType = atoi(equip_mainGun->getImgName().c_str());
    
    if (nType > 3 && nType <= 6)
    {
        m_bIsMainLaser = true;
    }
    else
    {
        m_bIsMainLaser = false;
    }
    
    EquipItem* equip_shoulder = UserEquipBag::shared()->getFirSubEquip();
    
    nType = atoi(equip_shoulder->getImgName().c_str());
    
    if (nType > 3 && nType <= 6)
    {
        m_bIsShoulderLaser = true;
    }
    else
    {
        m_bIsShoulderLaser = false;
    }
    
    EquipItem* equip_pinion = UserEquipBag::shared()->getSecSubEquip();
    
    nType = atoi(equip_pinion->getImgName().c_str());
    
    if (nType > 3 && nType <= 6)
    {
        m_bIsPinionLaser = true;
    }
    else
    {
        m_bIsPinionLaser = false;
    }
    
    EquipItem* equip_wing = UserEquipBag::shared()->getWingEquip();
    
    nType = atoi(equip_wing->getImgName().c_str());
    
    if (nType > 3 && nType <= 6)
    {
        m_bIsWingLaser = true;
    }
    else
    {
        m_bIsWingLaser = false;
    }
    
    
    /***** 容器初始化 *****/
    
    //敌机子弹数组
    m_pArrayEnemyBullet = CCArray::create();
    m_pArrayEnemyBullet->retain();
    //主机子弹数组
    m_pArrayHeroBullet = CCArray::create();
    m_pArrayHeroBullet->retain();
    //僚机子弹数组
    m_pArrayWingBullet = CCArray::create();
    m_pArrayWingBullet->retain();
    //道具飞机子弹数组
    m_pArrayToolBullets = CCArray::create();
    m_pArrayToolBullets->retain();
    
    //主机最大血量  是加上原来装备所具有的血量
    m_heroMaxHp = 0;
    m_heroMaxHp += UserData::shared()->getHeroHp();
    
    //要移除的数组
    m_pArrayRemove = CCArray::create();
    m_pArrayRemove->retain();
    
    //任务敌机
    m_pArrayTaskEnemy = CCArray::create();
    m_pArrayTaskEnemy->retain();
    
    //金币
    m_pArrayCoin = CCArray::create();
    m_pArrayCoin->retain();
    
    //道具
    m_pArrayTool = CCArray::create();
    m_pArrayTool->retain();
    
    //强化道具
    m_pArrInvincibleTool = CCArray::create();
    m_pArrInvincibleTool->retain();
    
    m_pArrayFlyTool= CCArray::createWithCapacity(m_nToolGridNum);
    m_pArrayFlyTool->retain();
    
    //道具飞机
    m_pArrayToolPlane = CCArray::create();
    m_pArrayToolPlane->retain();
    
    //血球粒子
    m_pArrayBloodParticles = CCArray::create();
    m_pArrayBloodParticles->retain();
    
    //血球
    m_pArrayBlood = CCArray::create();
    m_pArrayBlood->retain();
    
    //积分
    m_pArrayScore = CCArray::create();
    m_pArrayScore->retain();
    
    //敌机
    m_pDictEnemy = CCDictionary::create();
    m_pDictEnemy->retain();
    
    //闪电
    m_pArrayBolts = CCArray::create();
    m_pArrayBolts->retain();
    
    /******* 关卡初始化 ******/
    mission = MissionDataManager::shared()->getMissionByWave(1);
    
    /******* SpriteBatchNode ******/
    //主机子弹
    m_pHeroBullets = CCSpriteBatchNode::create("game/ammo/bullet.png", 1000);
    m_pHeroBullets->retain();
    this->addChild(m_pHeroBullets, GAME_ZORDER_HERO_BULLET);
    
    ccBlendFunc func = {GL_SRC_ALPHA,GL_ONE};
    m_pHeroBullets->setBlendFunc(func);
    //敌机子弹
    m_pEnemyBullets = CCSpriteBatchNode::create("game/ammo/bullet.png", 1000);
    m_pEnemyBullets->retain();
    this->addChild(m_pEnemyBullets, GAME_ZORDER_ENEMY_BULLET);
    //积分
    m_pDropBatchNode = CCSpriteBatchNode::create("game/drops/drops.png", 1000);
    m_pDropBatchNode->retain();
    this->addChild(m_pDropBatchNode,GAME_ZORDER_TOOLS);
    //子弹发射动画节点
    m_pSpriteExpands = CCSpriteBatchNode::create("game/animations/effect_expand.png", 1000);
    m_pSpriteExpands->retain();
    this->addChild(m_pSpriteExpands, GAME_ZORDER_SHOOT_EFFECT);
    
    m_pParticleNode = CCNode::create();
    m_pParticleNode->retain();
    m_pParticleNode->setPosition(CCPointZero);
    this->addChild(m_pParticleNode, GAME_ZORDER_HERO_BULLET);
    
    /******* 添加地图层 *******/
    BackgroundView* backgound = BackgroundView::create();
    
    backgound->setScheduler(m_pCombatScheduler);
    backgound->moveStart();
    
    backgound->mapdalegate = this;
    this->addChild(backgound,GAME_ZORDER_BACKGROUND,GAME_TAG_BACKGROUND);
    /******* 添加UI层 *******/
    m_battleUI = GameBattleUILayer::create();
    addChild(m_battleUI,10);
    m_battleUI->setHpPercent(100);
    /******* 创建主机 *******/
    this->createHero();
    /******* 触摸 *******/
    m_bIsTouch = false;
    
    /* 开始游戏 */
    if (b_skipTwenty)
    {
        this->gameFlyOverTwentyWave();
    }
    else
    {
        this->gameFlyNormalWave();
    }
    
    //背景音乐
    SoundEffectManager::shared()->requestPlayBackgroundMisc(MIS_BATTLE_BG);
    
    return true;
}

void GameCombatLayer::gameFlyNormalWave()
{
    //飞机飞出
    float pos_y_start = 0;
    float pos_y_by = _s_height / 5;
    
    updateHeroPosition(ccp(_s_center.x, pos_y_start));
    
    //飞机出屏,显示胜利节点
    CCSequence *seq_run_into_screen = CCSequence::create(CCMoveBy::create(1.0f, ccp(0, pos_y_by)),
                                                         CCCallFunc::create(this,
                                                                            callfunc_selector(GameCombatLayer::gameStart)),
                                                         NULL);
    
    m_pHeroView->runAction(seq_run_into_screen);
}

void GameCombatLayer::gameFlyOverTwentyWave()
{
    //地图快速移动
    BackgroundView* backgound = (BackgroundView*)this->getChildByTag(GAME_TAG_BACKGROUND);
    backgound->setAllMapSpeed(MAP_COVER_CHANGE_SPEED);
    
    //气流动画
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("game/hero/rapid-1.ExportJson");
    CCArmature* shiled = CCArmature::create("rapid-1");
    shiled->setPositionY(-100);
    m_pHeroView->addChild(shiled,1,kTagSkipWaveAnimation);
    CCArmatureAnimation* animation = shiled->getAnimation();
    animation->playWithIndex(0);
    
    //飞机飞行
    float pos_y_start = 0;
    float pos_y_by = 200;
    
    updateHeroPosition(ccp(_s_center.x, pos_y_start));
    
    CCSequence *seq_run_into_screen = CCSequence::create(CCMoveBy::create(3.5f, ccp(0, pos_y_by)),
                                                         CCDelayTime::create(3.5f),
                                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackRemoveSkipAni)),
                                                         //CCMoveTo::create(1.5f, ccp(_s_center.x,_s_center.y / 5)),
                                                         CCCallFunc::create(this,callfunc_selector(GameCombatLayer::gameStart)),
                                                         CCCallFunc::create(this, callfunc_selector(GameCombatLayer::callbackDropSevenToolsForInvincible)),
                                                         NULL);
    
    m_pHeroView->runAction(seq_run_into_screen);
    
    //增加波数 和 积分
    this->schedule(schedule_selector(GameCombatLayer::callbackAddGameWaveAndScore), 0.35f);
}

void GameCombatLayer::callbackAddGameWaveAndScore()
{
    
    if (m_nGameWave >= 20)
    {
        this->unschedule(schedule_selector(GameCombatLayer::callbackAddGameWaveAndScore));
        return;
    }
    
    m_nGameWave++;
    m_battleUI->setWave(m_nGameWave);
    
    m_battleUI->setBoxNum(m_nGameWave / 10);
    
    m_resultCountData->m_wave++;
    
    m_resultCountData->m_nBox = m_nGameWave / 10;
    
    //日常统计 当日波数累积
    DailyCountData::sharedInstance()->setOneMissionTotalWaves(m_nGameWave);
    
    DailyCountData::sharedInstance()->addDailayTotalWaves();
    
    //成就统计 里程数
    int mileage = GameCountData::sharedInstance()->getMileage();
    if (m_nGameWave > mileage)
    {
        GameCountData::sharedInstance()->setMileage(m_nGameWave);
        
        GameCountData::sharedInstance()->hadAchieveComplete();
    }
    
    m_nScore += getRandom(500, 1000);
    m_battleUI->setScore(m_nScore);
}

void GameCombatLayer::callbackRemoveSkipAni(CCNode *pSender)
{
    if (pSender->getChildByTag(kTagSkipWaveAnimation))
    {
        pSender->removeChildByTag(kTagSkipWaveAnimation);
        CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("game/hero/rapid-1.ExportJson");
        
        //地图快速移动
        BackgroundView* backgound = (BackgroundView*)this->getChildByTag(GAME_TAG_BACKGROUND);
        backgound->setAllMapSpeed(0.0f);
    }
}

void GameCombatLayer::gameStart()
{
    //开火
    startFireBatteryAndWing();
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    //开启update 敌机出击
    float fps = CCDirector::sharedDirector()->getAnimationInterval();
    
    this->schedule(schedule_selector(GameCombatLayer::update), fps);
    this->schedule(schedule_selector(GameCombatLayer::updateCollisions), fps);
    this->schedule(schedule_selector(GameCombatLayer::updateTime), fps);
    this->schedule(schedule_selector(GameCombatLayer::updateSkills), fps);
    this->schedule(schedule_selector(GameCombatLayer::updateLaserTest), fps);
}

void GameCombatLayer::startFireBatteryAndWing()
{
    //创建僚机
    int nSelectedWingGun = atoi(UserEquip::shared()->getWingID().c_str());
    //创建僚机
    if(nSelectedWingGun > 0)
    {
        
        EquipItem* pEquipItem = UserEquipBag::shared()->getWingEquip();
        
        TargetPlaneData* tarconnon = DataManager::sharedDataManager()->getTargetConnon(pEquipItem->getImgName());
        
        CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_CONNON, getIntFromS(tarconnon->getPlaneID().c_str()));
        dataCraft->setType(CRAFT_DATA_TYPE_WING);
        dataCraft->setConnonType(EQUIPMENT_TYPE_WING);
        
        //僚机的创建
        m_pWingView = WingView::create(dataCraft, pEquipItem);
        m_pWingView->setScheduler(m_pCombatScheduler);
        //代理
        m_pWingView->delegateBullet = this;
        m_pWingView->delegateCrashed = this;
        //开火
        if (m_bIsWingLaser)
        {
            m_pWingView->setIsBeginFly(false);
        }
        else
        {
            m_pWingView->setIsBeginFly(true);
        }
        
        m_pWingView->changePhrase(1);
        //类型
        m_pWingView->setType(CRAFT_TYPE_WING);
        //转角
        m_pWingView->setRotation(90.0f);
        //设置僚机本体角度
        CCNode* node = m_pWingView->nodeContainer->getChildByTag(kTagShape);
        node->setRotation(0);
        //位置
        m_pWingView->setPosition(_s_center);
        //设置僚机左右移动初始点
        m_pWingView->setPosOffsetR(ccp(_s_center.x - 170 * _s_width / 720,_s_height / 2));
        m_pWingView->setPosOffsetL(ccp(_s_center.x + 170 * _s_width / 720,_s_height / 2));
        
        m_pWingView->startWingUpdate();
        
        this->addChild(m_pWingView,GAME_ZORDER_HERO);
        m_pWingView->setBatteryDirection(1);
        
        //如果僚机携带了技能
        switch (pEquipItem->getSkillID())
        {
            case MAINGUN_SKILL_TYPE_NULL:
                
                m_pWingSkill = new SkillNull();
                break;
                
            case WING_SKILL_TYPE_DEFENSE_BOMBS:
                m_pWingSkill = new SkillDefenseBombs(this);
                break;
                
            case WING_SKILL_TYPE_ADDED_BLOOD:
                m_pWingSkill = new SkillAddedBlood(this);
                break;
                
            case WING_SKILL_TYPE_PROPS_WAREHOUSE:
                //m_pWingSkill = new SkillPropsWareHouse(this);
                m_nToolGridNum = 2;
                m_pArrayFlyTool->initWithCapacity(m_nToolGridNum);
                break;
                
            default:
                break;
        }
        
    }
    
    //副炮开火
    if (m_pMainGun)
    {
        if (m_bIsMainLaser)
        {
            m_pMainGun->setIsBeginFly(false);
        }
        else
        {
            m_pMainGun->setIsBeginFly(true);
            
            /* 如果是子弹播放子弹持续音效 */
            //            SoundEffectManager::shared()->requestPlayPerSoundEffect(SOUND_NORMAL_BULLET);
        }
    }
    if (m_pBearLeftGun)
    {
        m_pBearLeftGun->setIsBeginFly(false);
    }
    if (m_pBearRightGun)
    {
        m_pBearRightGun->setIsBeginFly(false);
    }
    if (m_pWingLeftGun)
    {
        m_pWingLeftGun->setIsBeginFly(false);
    }
    if (m_pWingRightGun)
    {
        m_pWingRightGun->setIsBeginFly(false);
    }
    
}

void GameCombatLayer::callbackDropSevenToolsForInvincible()
{
    for (int i=0;i<INIT_DROPTOOL_NUM;i++)
    {
        dropToolsForInvincible();
    }
    
}

void GameCombatLayer::dropToolsForInvincible()
{
    
    //强化道具出现次数加1
    m_nInvincibleTimes ++;
    
    //读取数据
    CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_DROP, getToolShapeID(3));
    
    //创建道具
    ToolsView* tool = ToolsView::create(dataCraft,DataManager::sharedDataManager()->getTargetTool(getStringFromInt(3)),false);
    tool->setType(CRAFT_TYPE_TOOL);
    
    //设置道具功能类型
    tool->setToolType(3);
    
    //代理
    tool->delegateCrashed = this;
    
    
    tool->setPosition(ccp(_s_width/2,_s_height/2+m_nInvincibleTimes*50));
    
    tool->createToolParticle();
    
    tool->setHeroTmp(m_pHeroView);
    tool->startUpdateMagnet();
    
    //加到飞机层 用plane得m_uID作为tag
    this->addChild(tool, GAME_ZORDER_TOOLS);
    
    m_pArrInvincibleTool->addObject(tool);
    
}

void GameCombatLayer::revive(cocos2d::CCObject *pSender)
{
    
    if (m_pHeroView)
    {
        //满血
        m_pHeroView->setCurrentHP(m_heroMaxHp);
        m_pHeroView->setIsDied(false);
        m_battleUI->setHpPercent(m_pHeroView->getCurrentHP() / m_heroMaxHp * 100);
        
        //复活道具使用计数
        if (!b_canRevive)
        {
            UserData::shared()->setReviveTimes(UserData::shared()->getReviveTimes() + 1);
        }
        
        //技能重置
        if (m_pMainSkill) {
            
            m_pMainSkill->reset();
            
        }
        
        if (m_pMainSkill && UserEquipBag::shared()->getMainEquip()->getSkillID() != MAINGUN_SKILL_TYPE_SANDS_OF_TIME)
        {
            m_pMainSkill->end();
        }
        
        //add护盾
        CCSprite* pShiled = CCSprite::create("game/tools/tools_coin.png");
        pShiled->setScale(6.0f);
        m_pHeroView->addChild(pShiled,1,kTagReviveInvincible);
        
        pShiled->runAction(CCSequence::create(CCDelayTime::create(2.0f),CCBlink::create(2.0f, 5),CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),NULL));
        
        //清除敌机
        CCDictElement *_ele = NULL;
        CCDICT_FOREACH(m_pDictEnemy, _ele)
        {
            EnemyView *plane = (EnemyView *)_ele->getObject();
            
            if(plane && !plane->getIsDied())
            {
                this->craftCrashed(plane, 0, true);
                
                int type = getCraftSizeType(plane);
                
                if (plane->getIsBattery() || type < CRAFT_SIZE_BIG)
                {
                    //播放爆炸声音
                    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_SMALL);
                }
                else
                {
                    //播放爆炸声音
                    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_BIG);
                }
            }
        }
        
        //清空道具仓
        CCObject* tool_obj = NULL;
        CCARRAY_FOREACH(m_pArrayFlyTool, tool_obj)
        {
            ToolsView* tool =(ToolsView*)tool_obj;
            tool->setIsDied(true);
            
            tool->removeFromParent();
        }
        //设置道具仓容量
        m_pArrayFlyTool->initWithCapacity(m_nToolGridNum);
        
    }
}

void GameCombatLayer::updateLaserTest(float delta)
{
    
    if (m_pLaser)
    {
        m_pLaser->updateLaser(delta);
    }
    
}

//循环
void GameCombatLayer::update(float delta)
{
    //检测主角血量
    checkHeroHP();
    
    //创建清屏道具闪电
    createLightningBolt();
}

//触摸
void GameCombatLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    if (gameOver)
    {
        return;
    }
    
    //如果当前已经有触摸 则触发道具使用
    if(m_bIsTouch)
    {
        this->useTool(true);
    }
    else
    {
        CCSetIterator it = pTouches->begin();
        
        for(; it != pTouches->end(); it++)
        {
            CCTouch *touch = (CCTouch *)(*it);
            
            if(touch->getID() == 0)
            {
                CCPoint location = touch->getLocation();
                m_posPrePoint = location;
                
                m_bIsTouch = true;
                
                //                //如果当前主炮技能是光波炮
                //                if (UserEquipBag::shared()->getMainEquip() && UserEquipBag::shared()->getMainEquip()->getSkillID() == MAINGUN_SKILL_TYPE_WAVE_GUN)
                //                {
                //                    if (m_pMainSkill->m_bIsRunning)
                //                    {
                //                        m_pMainSkill->end();
                //                    }
                //                }
                
                if (m_pMainSkill && UserEquipBag::shared()->getMainEquip()->getSkillID() != MAINGUN_SKILL_TYPE_SANDS_OF_TIME)
                {
                    m_pMainSkill->end();
                }
                
            }
            
        }
    }
    
}
void GameCombatLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (gameOver)
    {
        return;
    }
    
    CCSetIterator it = pTouches->begin();
    
    CCPoint location;
    
    for(; it != pTouches->end(); it++)
    {
        CCTouch *touch = (CCTouch *)(*it);
        
        if(touch->getID() == 0)
        {
            location = touch->getLocation();
        }
    }
    
    //做一下处理  如果两个手指在屏幕上  第二个手指划一下  会导致传入的location为空
    if (location.x == 0 || location.y == 0)
    {
        return;
    }
    
    if(m_bIsTouch)
    {
        //手指滑动距离
        float offset_x = location.x - m_posPrePoint.x;
        float offset_y = location.y - m_posPrePoint.y;
        
        //地图移动
        BackgroundView* back = (BackgroundView*)getChildByTag(GAME_TAG_BACKGROUND);
        if (back)
        {
            if (offset_x > 0 && back->getPositionX() > -30)
            {
                back->setPositionX(back->getPositionX() - offset_x / 7);
            }
            
            if (offset_x < 0 && back->getPositionX() < 30)
            {
                back->setPositionX(back->getPositionX() - offset_x / 7);
            }
            
        }
        
        //主机移动
        CCPoint pointNew = ccp(m_pHeroView->getPositionX() + offset_x, m_pHeroView->getPositionY() + offset_y);
        this->updateHeroPosition(pointNew);
        
        if (this->getChildByTag(kTagSkillRepulsionShiled))
        {
            CCSprite* spr = (CCSprite*)this->getChildByTag(kTagSkillRepulsionShiled);
            
            spr->setPosition(pointNew);
        }
        
        m_posPrePoint = location;
    }
}
void GameCombatLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (gameOver)
    {
        return;
    }
    
    CCSetIterator it = pTouches->begin();
    
    for(; it != pTouches->end(); it++)
    {
        CCTouch * pTouch = (CCTouch *)(* it);
        if(pTouch->getID() == 0)
        {
            
            m_bIsTouch = false;
            
            m_pPosOri = m_pHeroView->getPosition();
            
            if (UserEquipBag::shared()->getMainEquip())
            {
                //抬手 释放技能
                if (m_pMainSkill)
                {
                    m_pMainSkill->begin();
                }
            }
        }
        
    }
    
}

//按键处理
void GameCombatLayer::keyBackClicked()
{
    GameDelegate::shared()->openScenePause();
}

//菜单
void GameCombatLayer::keyMenuClicked()
{
    GameDelegate::shared()->openScenePause();
}

/************************
 * 时间轴
 ************************/
//时间轴滚动
void GameCombatLayer::updateTime(float dt)
{
    //更新闪电
    updateLightningBolt();
    
    //    updateLinkLaser();
    
    //随机飞机组
    bool isTaskEmpty = (m_pArrayTaskEnemy->count() == 0) ? true : false;
    
    isEnemyFly = false;
    
    if (isTaskEmpty) {
        //等待1s出敌机
        flyPlaneCount++;
        if (flyPlaneCount >= ENEMYFLY_INTERVAL) {
            flyPlaneCount = 0;
            
            isEnemyFly = true;
        }
    }
    
    
    if (isEnemyFly && isTaskEmpty && m_nPlaneGroupId > 0)
    {
        //随机敌机组
        
        m_nPlaneGroupId += 1;
        
        m_nGameWave++;
        
        if (m_nGameWave >= 20)
        {
            m_nBloodAddedValue = 0.75 * 0.66 * (1501 * (m_nGameWave / 20 + 1) - 151);
        }
        
        m_resultCountData->m_wave++;
        
        m_resultCountData->m_nBox = m_nGameWave / 10;
        
        m_battleUI->setWave(m_nGameWave);
        
        if (m_nGameWave % 10 == 0)
        {
            m_battleUI->setBoxNum(m_nGameWave / 10);
        }
        
        //日常统计 当日波数累积
        DailyCountData::sharedInstance()->setOneMissionTotalWaves(m_nGameWave);
        
        DailyCountData::sharedInstance()->addDailayTotalWaves();
        
        //成就统计 里程数
        int mileage = GameCountData::sharedInstance()->getMileage();
        if (m_nGameWave > mileage)
        {
            GameCountData::sharedInstance()->setMileage(m_nGameWave);
            
            GameCountData::sharedInstance()->hadAchieveComplete();
        }
        
        //切换地图
        if (isEnemyFly && m_nGameWave > 0 && m_nGameWave % 20 == 0)
        {
            
            if (m_pArrayTaskEnemy->count() == 0)
            {
                BackgroundView* ground = (BackgroundView*)this->getChildByTag(GAME_TAG_BACKGROUND);
                ground->switchingMap();
                isEnemyFly = false;
                ground->setIsSwitching(false);
            }
            else
            {
                isEnemyFly = false;
            }
            
        }
        
    }
    
    if (!isEnemyFly)
    {
        return;
    }
    
    if (!isTaskEmpty)
    {
        return;
    }
    
    if (m_nPlaneGroupId > 10)
    {
        mission = MissionDataManager::shared()->getMissionByWave(m_nGameWave);
        
        m_nPlaneGroupId = 1;
    }
    
    if (!mission)
    {
        return;
    }
    
    //获得TIME数据
    std::string time_id = getStringFromInt(m_nPlaneGroupId);
    TimeData* dataTime = mission->getTimeData(time_id);
    
    if(!dataTime)
    {
        return;
    }
    
    if(!dataTime->getIsEnd())
    {
        
        CCArray* dataTimePlaneAll = dataTime->getTimePlanes();
        
        CCLog("dataTimePlaneAll count : %d",dataTimePlaneAll->count());
        CCObject* pObj = NULL;
        
        int planeNum = 0;
        
        m_fDelayTime = 0.0f;
        
        CCARRAY_FOREACH(dataTimePlaneAll, pObj)
        {
            if(!pObj)
            {
                break;
            }
            
            TimePlaneData *dataTimePlane = (TimePlaneData *)pObj;
            string key = dataTimePlane->getType();
            
            CCLog("datatimeplane type:%s",key.c_str());
            
            //创建敌机的时候 用的是Enemy中的Targetplane
            TargetPlaneData *dataTargetPlane = DataManager::sharedDataManager()->getTargetPlane(key);
            
            planeNum++;
            
            CCLog("current planeNum is %d",planeNum);
            
            createEnemy(dataTargetPlane, dataTimePlane, dataTime->getIsPause());
        }
        
    }
    else
    {
        CCLog("GAME TIME ====> %d IS END", m_nGameTime);
    }
    
    
}

/******************子弹*******************/
//子弹代理
//创建子弹
void GameCombatLayer::createCraftBullet(CraftView *craft, GunDetail* detail)
{
    
    if (gameOver)
    {
        return;
    }
    
    //武器开关未开启就不发射子弹
    if(!craft->getGunSwitch())
    {
        return;
    }
    
    bulletAppearTime+=0.1f;
    
    //准备数据
    TargetGunGroupData *dataTargetGG = detail->getTargetGunGroupData();
    
    GunData *dataGun = detail->getGunData();
    
    BulletData *bullet_data = DataManager::sharedDataManager()->getBulletData(getIntFromS(dataGun->getBulletID().c_str()));
    BulletView *bullet = BulletView::create(bullet_data);
    bullet->setActionManager(m_pCombatActionManager);
    
    //设置伤害分两部分处理
    //敌机
    if (craft->getType() == CRAFT_TYPE_ENEMY)
    {
        int mul = m_nGameWave / 20 + 1;
        
        bullet->setHarm(mul * 197 - 182);
    }
    else if (craft->getType() == CRAFT_TYPE_BATTERY_TOOL)
    {
        GunGroupData* groupdata = (GunGroupData*)WeaponLibrary::sharedWeaponLibrary()->getGunGroupDataByID(dataTargetGG->getGunGroupID());
        
        EquipItem* item = UserEquipBag::shared()->getMainEquip();
        
        float base_harm = item->getAttack() * item->getLevel();
        
        base_harm = base_harm * (100 + item->getAttackAdd()) / 100.0;
        
        bullet->setHarm((base_harm / groupdata->getGuns()->count() * detail->getHarmRatio())*2.5f);
        
    }
    else
    {
        GunGroupData* groupdata = (GunGroupData*)WeaponLibrary::sharedWeaponLibrary()->getGunGroupDataByID(dataTargetGG->getGunGroupID());
        
        EquipItem* item = NULL;
        
        switch (craft->getConnonType())
        {
            case EQUIPMENT_TYPE_MAIN_GUN:
            {
                item = UserEquipBag::shared()->getMainEquip();
                
                float base_harm = item->getAttack() * item->getLevel();
                
                base_harm = base_harm * (100 + item->getAttackAdd()) / 100.0;
                
                bullet->setHarm(base_harm / groupdata->getGuns()->count() * detail->getHarmRatio());
                
                break;
            }
                
            case EQUIPMENT_TYPE_LEFT_GUN:
            {
                item = UserEquipBag::shared()->getFirSubEquip();
                
                float base_harm = item->getAttack() * item->getLevel();
                
                base_harm = base_harm * (100 + item->getAttackAdd() * 0.6f) / 100.0;
                
                bullet->setHarm(base_harm / groupdata->getGuns()->count() * detail->getHarmRatio());
                break;
            }
                
            case EQUIPMENT_TYPE_RIGHT_GUN:
            {
                item = UserEquipBag::shared()->getSecSubEquip();
                
                float base_harm = item->getAttack() * item->getLevel();
                
                base_harm = base_harm * (100 + item->getAttackAdd() * 0.5f) / 100.0;
                
                bullet->setHarm(base_harm / groupdata->getGuns()->count() * detail->getHarmRatio());
                break;
            }
                
            case EQUIPMENT_TYPE_WING:
            {
                item = UserEquipBag::shared()->getWingEquip();
                
                float base_harm = item->getAttack() * item->getLevel();
                
                base_harm = base_harm * (100 + item->getAttackAdd() * 0.33f) / 100.0;
                
                bullet->setHarm(base_harm / groupdata->getGuns()->count() * detail->getHarmRatio());
                break;
            }
                
            default:
                break;
        }
    }
    
    //是否是时时跟踪子弹
    bool b_is_trace = dataGun->getIsTrace();
    
    bool b_isPositionShooting = false;
    
    if((craft->getConnonType() == EQUIPMENT_TYPE_MAIN_GUN) && m_bTraceBulletEnable)//
    {
        if (UserEquipBag::shared()->getMainEquip()->getSkillID() == MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING)
        {
            
            CraftView* temp = getAimedCraft();
            
            if (temp)
            {
                b_is_trace = true;
                
                b_isPositionShooting = true;
            }
            else
            {
                b_is_trace = false;
                
                b_isPositionShooting = false;
            }
            
        }
    }
    
    //设置子弹归属哪个炮
    bullet->setBelongCount(craft->getBelongCount());
    
    //设置子弹是否是导弹
    if (dataGun->getIsMissile())
    {
        bullet->setIsMissile(true);
    }
    
    //设置子弹ID
    bullet->setID(dataGun->getBulletID());
    
    //获取子弹ID int
    int bullet_id = getIntFromS(bullet->getID().c_str());
    
    //创建子弹
    CCSequence *seq = NULL;
    
    if(!b_is_trace)
    {
        //直线子弹
        CCAffineTransform transform = CCAffineTransformConcat(detail->getGunGroupTransform(), craft->getCraftTransform());
        
        CCPoint pos_bullet = CCPointZero;
        
        //如果开启了固定枪口
        if (detail->getIsFixedGunDetail())
        {
            if (detail->needRefreshPosition())
            {
                pos_bullet = CCPointApplyAffineTransform(detail->getGunPosition(), transform);
                
                //敌机炮管
                if(craft->getIsAffiliatedPlane() && craft->getParent() != NULL)
                {
                    pos_bullet = pos_bullet + craft->getParent()->getPosition();
                }
                
                //主机炮管
                if((craft->getType() == CRAFT_TYPE_BATTERY || craft->getType() == CRAFT_TYPE_BATTERY_TOOL) && craft->getParent() != NULL)
                {
                    pos_bullet = pos_bullet + craft->getParent()->getPosition();
                }
                
                //僚机炮管
                if (craft->getType() == CRAFT_TYPE_WING && craft->getParent() != NULL)
                {
                    pos_bullet = pos_bullet + craft->getParent()->getPosition();
                }
                
                detail->setBulletTempPosition(pos_bullet);
            }
            else
            {
                pos_bullet = detail->getBulletTempPosition();
            }
        }
        else
        {
            pos_bullet = CCPointApplyAffineTransform(detail->getGunPosition(), transform);
            
            //敌机炮管
            if(craft->getIsAffiliatedPlane() && craft->getParent() != NULL)
            {
                pos_bullet = pos_bullet + craft->getParent()->getPosition();
            }
            
            //主机炮管
            if((craft->getType() == CRAFT_TYPE_BATTERY || craft->getType() == CRAFT_TYPE_BATTERY_TOOL) && craft->getParent() != NULL)
            {
                pos_bullet = pos_bullet + craft->getParent()->getPosition();
            }
            
            //僚机炮管
            if (craft->getType() == CRAFT_TYPE_WING && craft->getParent() != NULL)
            {
                pos_bullet = pos_bullet + craft->getParent()->getPosition();
            }
        }
        
        bullet->setPosition(pos_bullet);
        
        //Bullet Fire Direction(the Angle)
        float rot_laser = craft->getRotation() + detail->getGunGroupRotation() + detail->getGunRotation();
        bullet->setRotation(rot_laser);
        
        float speed = detail->getGunData()->getSpeed();
        bullet->setSpeed(speed);
        bullet->setOffsetAngle(dataGun->getOffsetAngle());
        bullet->setChildBulletID(dataGun->getChildBulletID());
        float time = MAX_DISTANCE / speed;
        
        CCPoint point_by = ccp(MAX_DISTANCE * cos(CC_DEGREES_TO_RADIANS(rot_laser)),
                               MAX_DISTANCE * sin(CC_DEGREES_TO_RADIANS(rot_laser)));
        
        int nTrailGroupID = dataGun->getTrailGroupID();
        float fOffsetAngle = dataGun->getOffsetAngle();
        //在这里区分是否是放射型子弹
        bool bRadiateBullet = (fOffsetAngle >= 0.0f) ? true : false;
        
        //非放射型子弹
        if(!bRadiateBullet)
        {
            if(nTrailGroupID < 0)
            {
                //直接向前飞行
                seq = CCSequence::create(CCMoveBy::create(time, point_by), NULL);
            }
            else
            {
                //bullet->setRotation(bullet->getRotation() + 180);
                //按照子弹轨迹路径飞行
                TrailGroupData* dataTrailGroup = (TrailGroupData*)TrailLibrary::sharedTrailLibrary()->getTrailGroupDataByID(dataGun->getTrailGroupID());
                //设置子弹路径组的整体角度
                //dataTrailGroup->setAngle(dataGun->getTrailGroupAngle());
                bullet->setTrailGroupAngle(dataGun->getTrailGroupAngle());
                //给子弹路径组赋值
                bullet->setDataTrailGroup(dataTrailGroup);
                CCFiniteTimeAction* action1 = bullet->getDataTrailGroup()->getTrailActionByID("1", bullet, bullet->getTrailGroupAngle(), dataGun->getSpeed());
                bullet->setSpeed(dataGun->getSpeed());
                seq = CCSequence::create(action1,
                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackNextTrailAction)),
                                         NULL);
                
                //子弹转角
                if(bullet->getDataTrailGroup()->getPathAutoRotation(1))
                {
                    float rot = 0;
                    
                    if(bullet->getDataTrailGroup()->getFirstPathRotation(1, rot, dataGun->getTrailGroupAngle()))
                    {
                        bullet->setRotation(rot);
                    }
                }
                
            }
            
            bullet->runAction(seq);
        }
        //放射型子弹
        else
        {
            //区分是哪种放射型子弹
            int type = dataGun->getRadiateType();
            
            if (type == 1)
            {
                //1
                //子弹爆炸的高度位置
                float fPercent = dataGun->getPercentageHeight();
                float fAimY = fPercent * _s_height / 100.0f;
                //计算相对Y变量
                float fOffsetY = fAimY - pos_bullet.y;
                
                //计算时间
                time = fOffsetY / speed;
                
                if(time <= 0.0f)
                {
                    time = -time;
                }
                
                seq = CCSequence::create(CCMoveBy::create(time, ccp(0.0f, fOffsetY)),
                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackRadiateBulletBoom)),
                                         NULL);
                
                bullet->runAction(seq);
            }
            else if(type == 2)
            {
                //2
                //获取间隔时间
                float times = dataGun->getDelayTime();
                //0.5f;
                
                //父类子弹直线向下移动
                bullet->runAction(CCMoveBy::create(time, point_by));
                
                //以间隔时间 发射子子弹
                CCSequence* s = CCSequence::create(CCDelayTime::create(times),
                                                   CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackRadiaeBulletType2Boom)),
                                                   NULL);
                
                bullet->runAction(CCRepeatForever::create(s));
            }
            
        }
        
    }
    else
    {
        //获得目标
        CraftView *craftAimed = getAimedCraft();
        
        CCPoint pos_aimConnon = CCPointZero;
        
        //目标位置
        CCPoint p_aim = CCPointZero;
        
        int aimed_id = 0;
        
        //1、位置（副炮上分左右、主炮僚机上不分）
        bool is_left_plane = false;
        
        //如果是主炮打出来的
        if (craft->getBatteryDirection() == 1)
        {
            is_left_plane = dataGun->getDirectionAngle() < 0 ? true : false;
        }
        else
        {//副炮left
            if (craft->getBatteryDirection() == 2)
            {
                is_left_plane = true;
            }
            else //副炮right
            {
                is_left_plane = false;
            }
        }
        
        //如果是技能效果
        if (b_isPositionShooting)
        {
            //目标为 NULL 直接返回
            if(craftAimed)
            {
                p_aim = getAimedConnonPosition();
                
                if (!getAimedConnon())
                {
                    return;
                }
                aimed_id = getAimedConnon()->m_uID;
            }
            
            float pre_radius = 100.0f;
            //起始点坐标
            CCPoint p_origin = craft->getPosition();
            
            //分左右
            bool isleft = (p_origin.x > p_aim.x) ? false : true;
            bool isup = (p_origin.y > p_aim.y) ? true : false;
            
            //半径
            float temp_radius = p_origin.getDistance(p_aim) / 2;
            
            if (this->getChildByTag(100050))
            {
                this->removeChildByTag(100050);
            }
            
            //确定圆心位置
            CCPoint circle_center = CCPointZero;
            
            circle_center = ccp(p_origin.x + temp_radius, p_origin.y);
            
            //三个角度之和是180
            float temp_angele1 = 0.0f;
            float temp_angele2 = 0.0f;
            float temp_angele3 = 0.0f;
            
            //圆心到目标点的距离
            float temp_len = circle_center.getDistance(p_aim);
            //calcTwoPointLength(circle_center, p_aim);
            temp_angele2 = CC_RADIANS_TO_DEGREES(acos(temp_radius / temp_len));
            temp_angele3 = calcAngleFromOriginToTarget(circle_center, p_aim);
            temp_angele1 = 180.0f - temp_angele2 - temp_angele3;
            
            if(!isleft)
            {
                if(!isup)
                {
                    temp_angele1 = temp_angele3 - temp_angele2;
                }
                else
                {
                    temp_angele3 = -temp_angele3;
                    temp_angele1 = temp_angele3 - temp_angele2;
                    temp_angele1 += 180.0f;
                }
            }
            
            //求出弧长和速度确定时间
            float speed = dataGun->getSpeed();
            
            if(isleft)
            {
                temp_angele1 = -temp_angele1;
            }
            
            TraceAction* temp_ac2 = TraceAction::create(aimed_id, speed, 90 + temp_angele1);
            temp_ac2->delegateCrashed = this;
            bullet->setPosition(ccpAdd(m_pHeroView->getPosition(), ccp(dataGun->getY(),dataGun->getX())));
            
            //如果是炮台或僚机发射的
            if(craft->getType() == CRAFT_TYPE_BATTERY ||craft->getType() == CRAFT_TYPE_BATTERY_TOOL || craft->getType() == CRAFT_TYPE_WING)
            {
                bullet->setPosition(ccpAdd(m_pHeroView->getPosition(), craft->getPosition()));
            }
            
            bullet->runAction(temp_ac2);
            
        }
        else
        {
            //2、种类 （螺旋11 20，半弧18）
            //种类根据子弹id判断
            if (bullet_id == 11 || bullet_id == 20)
            {
                if (craftAimed)
                {
                    bullet->setRooundIsAnticlockwise(is_left_plane);
                    bullet->setMissileTarget(craftAimed);
                    bullet->callbackCircleTrace();
                }else
                {
                    //目标坐标
                    CCPoint p_aim = CCPointZero;
                    if(is_left_plane)
                    {
                        p_aim = ccp(_s_left, _s_top);
                    }
                    else
                    {
                        p_aim = ccp(_s_right, _s_top);
                    }
                    
                    if(craft->getType() == CRAFT_TYPE_BATTERY|| craft->getType() == CRAFT_TYPE_WING||craft->getType() == CRAFT_TYPE_BATTERY_TOOL)
                    {
                        bullet->setPosition(ccpAdd(m_pHeroView->getPosition(), craft->getPosition()));
                    }
                    
                    //起始坐标
                    CCPoint p_origin = bullet->getPosition();
                    
                    //角度
                    float degress = calcTwoPointAngle2(p_origin, p_aim);
                    
                    //设置子弹角度
                    bullet->setRotation(degress);
                    
                    CCMoveTo *temp_ac2 = CCMoveTo::create(1.0f, p_aim);
                    bullet->runAction(CCSequence::create(temp_ac2,CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackRemoveBullet)),NULL));
                }
            }
            else if(bullet_id == 18)
            {
                //目标为 NULL 直接返回
                if(craftAimed)
                {
                    p_aim = craftAimed->getPosition();
                    
                    bool anticlockwise = true;
                    
                    if (is_left_plane)
                    {
                        anticlockwise = false;
                    }
                    
                    //起始坐标
                    CCPoint p_origin = bullet->getPosition();
                    
                    //计算起始坐标 与 目标坐标角度
                    float angle = calcTwoPointAngle2(p_origin,p_aim);
                    
                    //起始目标所构成直线的切线角度
                    float tangent = angle - 90;
                    
                    //半径
                    float temp_radius = calcTwoPointLength(p_origin, p_aim) / 2.0f;
                    
                    ArcPathAction* temp_ac2 = ArcPathAction::create(0.7f,temp_radius, tangent, 180.0f,true,anticlockwise);
                    
                    bullet->runAction(CCSequence::create(temp_ac2,CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackRemoveBullet)),NULL));
                }
                //目标不存在 左边子弹向屏幕左上角飞去 右边子弹向屏幕右上方飞去
                else
                {
                    if(is_left_plane)
                    {
                        p_aim = ccp(- _s_width, 0);
                    }
                    else
                    {
                        p_aim = ccp(_s_right * 2, 0);
                    }
                    
                    CCMoveTo *temp_ac2 = CCMoveTo::create(1.0f, p_aim);
                    
                    bullet->runAction(CCSequence::create(temp_ac2,CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackRemoveBullet)),NULL));
                }
            }
        }
    }
    
    //子弹拖尾和粒子
    if (craft->getType() == CRAFT_TYPE_BATTERY || craft->getType() == CRAFT_TYPE_BATTERY_TOOL|| craft->getType() == CRAFT_TYPE_WING)
    {
        //添加拖尾
        int streakType = bullet->getStreakType();
        if (streakType > 0)
        {
            //创建拖尾  根据gungroup id判断要创建的拖尾类型 和 尾部粒子类型
            FerrisMotionStreak* pStreak = bullet->createMotionStreak();
            bullet->setMotionStreak(pStreak);
            this->addChild(pStreak, GAME_ZORDER_SHOOT_EFFECT);
        }
        
        //添加拖尾粒子
        int streakPType = bullet->getStreakParticleType();
        if (streakPType > 0)
        {
            //创建尾部粒子
            FerrisParticle* particle_temp = bullet->createStreakParticle();
            particle_temp->setPosition(bullet->getPosition());
            m_pParticleNode->addChild(particle_temp);
            
            bullet->setMissileParticle(particle_temp);
        }
        
        //添加子弹额外效果
        switch (bullet_id)
        {
            case 13:
                bullet->setOpacity(180);
                break;
            case 14:
            case 15:
                bullet->setAnchorPoint(ccp(0.0,0.5));
                break;
            case 16://慢慢渐隐
            case 19:
            {
                bullet->setOpacity(128);
                bullet->runAction(CCFadeOut::create(0.2f));
            }
                break;
            case 17:
            {
                CCSprite* sp = CCSprite::create("game/streak/streak_6.png");
                sp->setScale(1.2f);
                bullet->setSpriteExtra(sp);
                this->addChild(sp,GAME_ZORDER_SHOOT_EFFECT);
            }
                break;
            default:
                break;
        }
        
    }
    
    //添加发射效果
    if (craft->getType() == CRAFT_TYPE_BATTERY ||craft->getType() == CRAFT_TYPE_BATTERY_TOOL) {
        
        int shootType = bullet->getShootType();
        
        if (shootType > 0)
        {
            switch (shootType)
            {
                case BULLET_SHOOT_SHOOT1:
                {
                    if (bulletAppearTime >= detail->getTimeTarget()){
                        string name = "game/particles/particle_shoot";
                        
                        if (bullet_id == 14)
                        {
                            name.append("_1.plist");
                        }else if (bullet_id == 15)
                        {
                            name.append("_2.plist");
                        }
                        
                        //创建发射粒子
                        FerrisParticle* particle = FerrisParticle::create(name.c_str());
                        particle->setAutoRemoveOnFinish(true);
                        particle->setPosition(craft->getPosition());
                        m_pHeroView->addChild(particle,-1);
                    }
                    
                }
                    break;
                case BULLET_SHOOT_SHOOT2:
                {
                    //创建发射粒子
                    FerrisParticle* particle = FerrisParticle::create("game/particles/particle_effect_hit_5.plist");
                    particle->setPosition(ccp(0,0));
                    particle->setScale(3.0f);
                    particle->setAutoRemoveOnFinish(true);
                    m_pHeroView->addChild(particle,-1);
                }
                    break;
                default:
                    break;
            }
        }
    }
    
    //敌机添加子弹效果
    if (craft->getType() == CRAFT_TYPE_ENEMY)
    {
        //添加拖尾粒子
        int streakPType = bullet->getStreakParticleType();
        if (streakPType > 0)
        {
            //创建尾部粒子
            FerrisParticle* particle_temp = bullet->createStreakParticle();
            m_pParticleNode->addChild(particle_temp);
            
            bullet->setMissileParticle(particle_temp);
        }
        
        //发射动画
        CCSprite* spExpand = CCSprite::createWithTexture(m_pSpriteExpands->getTexture());
        spExpand->setPosition(bullet->getPosition());
        spExpand->setScale(0.5f);
        bullet->setSpriteExpand(spExpand);
        this->addChild(spExpand,GAME_ZORDER_ENEMY_BULLET);
        
        spExpand->runAction(CCScaleTo::create(0.1f, 1.3f));
        CCSequence* seqExpand = CCSequence::create(CCDelayTime::create(0.1f), CCFadeOut::create(0.0f), NULL);
        spExpand->runAction(seqExpand);
    }
    
    //设置子弹自转（敌机）
    if (craft->getType() == CRAFT_TYPE_ENEMY && bullet->getIsSelfRotate())
    {
        bullet->runSelfRotate();
    }
    
    //判断子弹是否是闪电子弹
    if (getIntFromS(bullet->getID().c_str()) == 22)
    {
        bullet->setIsLightningBullet(true);
    }
    
    //单独处理23子弹透明度
    if (bullet_id == 23) {
        bullet->setOpacity(160);
    }
    
    //播放子弹音效
    if (craft->getBatteryDirection() == 1 && craft->getType() == CRAFT_TYPE_BATTERY)
    {
        if (bulletAppearTime >= detail->getTimeTarget() + 0.1f)
        {
            if (bullet->getIsMissile())
            {
                SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_MISSLE, EFF_MISSLE);
            }
            else
            {
                SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_NORMAL_BULLET, EFF_NORMAL_BULLET);
            }
        }
    }
    
    //加入到子弹SpriteBatchNode
    if(craft->getType() == CRAFT_TYPE_BATTERY || craft->getType() == CRAFT_TYPE_WING || craft->getType() == CRAFT_TYPE_BATTERY_TOOL)
    {
        m_pHeroBullets->addChild(bullet);
    }
    else
    {
        m_pEnemyBullets->addChild(bullet);
    }
    
    //加入到子弹组
    if(craft->getType() == CRAFT_TYPE_ENEMY)
    {
        m_pArrayEnemyBullet->addObject(bullet);
    }
    else if(craft->getType() == CRAFT_TYPE_BATTERY)
    {
        m_pArrayHeroBullet->addObject(bullet);
    }
    else if(craft->getType() == CRAFT_TYPE_WING)
    {
        m_pArrayWingBullet->addObject(bullet);
    }else if (craft->getType() == CRAFT_TYPE_BATTERY_TOOL)
    {
        m_pArrayToolBullets->addObject(bullet);
    }
    
}

//获取主角位置
CCPoint GameCombatLayer::getHeroPosition()
{
    if (m_pHeroView) {
        return ccpAdd(m_pHeroView->getPosition(), ccp(0,POSY_HERO_HEAD));
    }
    
    return CCPointZero;
}

//获取僚机位置
CCPoint GameCombatLayer::getWingPosition()
{
    return m_pWingView->getPosition();
}

//移除NODE
void GameCombatLayer::removeNode(CCNode* pSender)
{
    pSender->removeFromParent();
}
/******************道具*******************/
void GameCombatLayer::useTool(bool isTouch)
{
    //道具数组为空 return
    if (m_pArrayFlyTool->count() < 1)
    {
        CCLog("m_pArrayFlyTool->count() == 0");
        return;
    }
    
    //获取道具
    ToolsView* tool = NULL;
    if (isTouch)
    {
        tool = (ToolsView*)m_pArrayFlyTool->lastObject();
    }
    else
    {
        tool = (ToolsView*)m_pArrayFlyTool->objectAtIndex(0);
    }
    
    if (!tool || tool->getIsDied())
    {
        return;
    }
    
    //释放粒子
    FerrisParticle* particle = FerrisParticle::create("game/particles/prop_kuosan.plist");
    particle->setPosition(tool->getPosition()+tool->getParent()->getPosition());
    particle->setAutoRemoveOnFinish(true);
    m_pParticleNode->addChild(particle);
    
    int type = tool->getToolType();
    tool->setIsDied(true);
    tool->removeFromParent();
    
    //道具释放音效
    if (type == TOOLS_TYPE_INVINCIBLE)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP,EFF_USE_TOOL_3);
    }
    else if (type == TOOLS_TYPE_UPDATEATTACK)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP,EFF_USE_TOOL_2);
    }
    
    GameCountData::sharedInstance()->addUseTools();
    
    switch (type)
    {
        case TOOLS_TYPE_INVINCIBLE://301  生成一个短时间无敌护罩
            
            useToolForInvincible();
            
            break;
        case TOOLS_TYPE_RECOVERHEALTH://302  回复满格血量
            
            useToolForRecoverHealth();
            
            break;
        case TOOLS_TYPE_MAKECOIN://303  把屏幕中的敌机子弹变成金币
            
            useToolForMakeCoin();
            
            break;
        case TOOLS_TYPE_VERTICALBULLET://305  竖向单道子弹
            
            useToolForVerticalBullet();
            
            break;
        case TOOLS_TYPE_DENSEBULLET: //306  密集子弹
            
            useToolForDenseBullet();
            
            break;
        case TOOLS_TYPE_TRACEMISSILE://307  追踪导弹
            
            //useToolForTraceMissile();
            
            break;
        case TOOLS_TYPE_LOCKTARGET://308  多道激光定目标攻击
            
            //useToolForLockTarget();
            
            break;
        case TOOLS_TYPE_CLEARMISSILE://309  清屏导弹
            
            useToolForClearMissile();
            break;
            
        default:
            break;
    }
    
}

//使用强化道具
void GameCombatLayer::useInvincibleTool()
{
    CCLog("usetool useToolForUpdateAttack");
    
    //随机要强化的哪个炮
    int index = getRandom(0, 2);
    
    switch (index)
    {
        case EQUIPMENT_KIND_MAIN_GUN:
        {
            if (curMainGunPhrase >= equipment_main_strengthen_upper) {
                
                return;
            }
            
            curMainGunPhrase++;
            
            m_pMainGun->changePhrase(curMainGunPhrase);
            
        }
            break;
        case EQUIPMENT_KIND_MAIN_BEAR:
        {
            if (curDeputy1GunPhrase >= equipment_deputy_strengthen_upper) {
                
                return;
            }
            
            //副炮1 phrase默认为3 上限为5
            if (!m_pBearLeftGun->getIsBeginFly())
            {
                if (!m_bIsShoulderLaser)
                {
                    m_pBearLeftGun->setIsBeginFly(true);
                    m_pBearRightGun->setIsBeginFly(true);
                }
                else
                {
                    if (m_pLaser)
                    {
                        m_pLaser->setShoulderLaserVisible(true);
                    }
                }
                
            }else{
                
                curDeputy1GunPhrase++;
                
                m_pBearLeftGun->changePhrase(curDeputy1GunPhrase);
                m_pBearRightGun->changePhrase(curDeputy1GunPhrase);
            }
        }
            break;
        case EQUIPMENT_KIND_MAIN_WING:
        {
            if (curDeputy2GunPhrase >= equipment_deputy_strengthen_upper) {
                
                return;
            }
            
            //副炮2 phrase默认为3 上限为5
            if (!m_pWingLeftGun->getIsBeginFly())
            {
                if (!m_bIsPinionLaser)
                {
                    m_pWingLeftGun->setIsBeginFly(true);
                    m_pWingRightGun->setIsBeginFly(true);
                }
                else
                {
                    if (m_pLaser)
                    {
                        m_pLaser->setPinionLaserVisible(true);
                    }
                }
                
            }else{
                curDeputy2GunPhrase++;
                
                m_pWingLeftGun->changePhrase(curDeputy2GunPhrase);
                m_pWingRightGun->changePhrase(curDeputy2GunPhrase);
                
            }
        }
            break;
    }
    
}

//301  生成一个短时间无敌护罩
void GameCombatLayer::useToolForInvincible()
{
    
    if (m_pHeroView->getChildByTag(kTagToolInvincible))
    {
        m_pHeroView->removeChildByTag(kTagToolInvincible);
    }
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("game/tools/Animationbra.ExportJson");
    
    CCArmature* shiled = CCArmature::create("Animationbra");
    shiled->setScale(1.5f);
    m_pHeroView->addChild(shiled,1,kTagToolInvincible);
    
    CCArmatureAnimation* animation = shiled->getAnimation();
    
    animation->setMovementEventCallFunc(this, movementEvent_selector(GameCombatLayer::removeShiled));
    
    animation->play("Animation1");
    
}
void GameCombatLayer::removeShiled(CCArmature *arm, MovementEventType type, const char *name)
{
    if (type == LOOP_COMPLETE)
    {
        arm->stopAllActions();
        arm->removeFromParentAndCleanup(true);
        
        CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("game/tools/Animationbra.ExportJson");
        arm = NULL;
    }
}
//302  回复满格血量
void GameCombatLayer::useToolForRecoverHealth()
{
    //播放粒子效果
    FerrisParticle* health = FerrisParticle::create("game/particles/repiar_tools.plist");
    health->cocos2d::CCParticleSystem::setTexture(CCTextureCache::sharedTextureCache()->addImage("game/particles/repiar_tools.png"));
    health->setScale(2.0);
    health->setPosition(m_pHeroView->getPosition());
    health->setAutoRemoveOnFinish(true);//播放完后删除
    m_pParticleNode->addChild(health);
    
    //英雄对象HP满血
    m_pHeroView->setCurrentHP(m_heroMaxHp);
    
    //UI血量满血
    m_battleUI->setHpPercent(100);
    
}
//303  把屏幕中的敌机子弹变成金币
void GameCombatLayer::useToolForMakeCoin()
{
    CCSprite* pShiled = CCSprite::create("game/tools/tools_coin.png");
    pShiled->setScale(10.0f);
    pShiled->setPosition(_s_center);
    this->addChild(pShiled,GAME_ZORDER_HERO_BULLET,kTagToolMakeCoin);
    
    //放大阶段1
    CCScaleTo* scaleBig = CCScaleTo::create(0.15f, 30.0f);
    //放大阶段2
    CCScaleTo* scaleBigagain = CCScaleTo::create(0.1f, 40.0f);
    //减淡
    CCFadeOut* fadeOut = CCFadeOut::create(0.1f);
    
    CCSpawn* spawn = CCSpawn::create(scaleBigagain,fadeOut,NULL);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
    
    CCSequence* seq = CCSequence::create(scaleBig,spawn,func,NULL);
    
    pShiled->runAction(seq);
    
    //遍历敌机子弹  子弹变金币
    for (int i =  0; i < m_pArrayEnemyBullet->count(); i ++)
    {
        BulletView* bullet = (BulletView*)m_pArrayEnemyBullet->objectAtIndex(i);
        if(bullet && !bullet->getIsDied())
        {
            this->addCoinFromBullet(bullet);
        }
    }
    
}
void GameCombatLayer::addCoinFromBullet(BulletView* bullet)
{
    //位置
    CCPoint pos = bullet->getPosition();
    bullet->setIsDied(true);
    bullet->removeFromParent();
    
    //速度
    float fSpeed = CCRANDOM_0_1() * 400;
    
    //读取数据
    DropData* data = DataManager::sharedDataManager()->getDropData(101);
    
    //创建金币
    CoinView* coin = CoinView::create(data);
    //代理
    coin->delegateDrop = this;
    
    //位置
    coin->setPosition(pos);
    
    CCSequence *seq = CCSequence::create(CCMoveBy::create(0.8f, ccp(0.0f, fSpeed)),
                                         NULL);
    CCSpeed *speed = CCSpeed::create(seq, 1.0f - CCRANDOM_0_1() * 0.3f);
    speed->setTag(kSPEEDUP_ACTION_TAG);
    coin->runAction(speed);
    
    coin->setHeroTmp(m_pHeroView);
    coin->startUpdateDrop();
    
    //加到金币数组中
    m_pArrayCoin->addObject(coin);
    
    m_pDropBatchNode->addChild(coin);
}
//305  竖向单道子弹
void GameCombatLayer::useToolForVerticalBullet()
{
    //创建飞机 101 然后发射子弹
    createToolPlane(DataManager::sharedDataManager()->getTargetPlane("1"));
}
//306  密集子弹
void GameCombatLayer::useToolForDenseBullet()
{
    //创建飞机 201然后发射子弹
    createToolPlane(DataManager::sharedDataManager()->getTargetPlane("2"));
}
//307  追踪导弹
void GameCombatLayer::useToolForTraceMissile()
{
    //创建飞机 202然后发射子弹
    createToolPlane(DataManager::sharedDataManager()->getTargetPlane("3"));
}
//308  多道激光定目标攻击
void GameCombatLayer::useToolForLockTarget()
{
    
}
//309  清屏导弹
void GameCombatLayer::useToolForClearMissile()
{
    //创建飞机 203然后发射子弹
    createToolPlane(DataManager::sharedDataManager()->getTargetPlane("4"));
}

void GameCombatLayer::createLightningBolt()
{
    
    if (m_pArrayToolBullets->count() < 1)
    {
        return;
    }
    
    
    for (int i =  0; i < m_pArrayToolBullets->count(); i ++)
    {
        
        BulletView* bullet = (BulletView*)m_pArrayToolBullets->objectAtIndex(i);
        
        if(!bullet->getIsDied() && bullet->getIsLightningBullet() && !bullet->getIsTakeLightning())
        {
            if(m_pDictEnemy->count()>0)
            {
                
                CCObject* obj = m_pDictEnemy->randomObject();
                
                EnemyView* enemy = NULL;
                if (obj == NULL) {
                    continue;
                }
                else
                {
                    //如果敌机存在
                    enemy = (EnemyView*)obj;
                }
                
                //敌机死了返回
                if (enemy->getIsDied()) {
                    continue;
                }
                
                //如果这个敌机是炮 加上它父类的pos
                CCPoint enemyPos = enemy->getPosition();
                if (enemy->getIsBattery())
                {
                    enemyPos = ccpAdd(enemyPos,enemy->getParent()->getPosition());
                }
                
                //检测敌机入屏
                if(enemyPos.y - _s_height < 0)
                {
                    bullet->setIsTakeLightning(true);
                    
                    LightningBoltNode* m_lbNode = new LightningBoltNode();
                    m_lbNode->autorelease();
                    m_lbNode->init(bullet->getPosition(), enemyPos);
                    this->addChild(m_lbNode,GAME_ZORDER_ENEMY_BULLET);
                    LightningBoltAction* flash= LightningBoltAction::create(1.0);//-1 for repeat forever
                    m_lbNode->runAction(flash);
                    bullet->setM_LBNode(m_lbNode);
                    
                    //敌机减血
                    enemy->theHurted(UserEquipBag::shared()->getTotalAttack() * 0.7 / 60.0f);
                    
                }
                
            }
            
        }
    }
    
}

void GameCombatLayer::updateLightningBolt()
{
    if(m_pArrayToolBullets->count()<1)
    {
        //        m_bIsUseToolFinish = false;
        
        return;
    }
    
    if (m_pDictEnemy->count() < 1)
    {
        CCObject* obj;
        
        CCARRAY_FOREACH(m_pArrayToolBullets, obj)
        {
            LightningBoltNode* bolt = ((BulletView*)obj)->getM_LBNode();
            if(((BulletView*)obj)->getIsTakeLightning())
            {
                bolt->removeFromParent();
                ((BulletView*)obj)->setIsTakeLightning(false);
            }
            
        }
        
        return;
    }
    
    for (int i =  0; i < m_pArrayToolBullets->count(); i ++)
    {
        EnemyView* enemy = (EnemyView*)m_pDictEnemy->randomObject();
        
        CCPoint enemyPos = enemy->getPosition();
        if (enemy->getIsBattery())
        {
            enemyPos = ccpAdd(enemyPos,enemy->getParent()->getPosition());
        }
        
        BulletView* bullet = (BulletView*)m_pArrayToolBullets->objectAtIndex(i);
        if(!bullet->getIsDied() && bullet->getIsTakeLightning())
        {
            
            LightningBoltNode* m_lbNode = bullet->getM_LBNode();
            CCLog("bullet %f   enemyPos  %f",bullet->getPositionX(),enemyPos.x);
            m_lbNode->updatePosition(bullet->getPosition(), enemyPos);
            bullet->setIsTakeLightning(true);
            enemy->theHurted(UserEquipBag::shared()->getTotalAttack() * 0.7 / 60.0f);
            
        }
        
    }
    
}
//检测血球是否出界
void GameCombatLayer::CheckBloodOutofScreen()
{
    //多有血球
    for (int i =  0; i < m_pArrayBlood->count(); i ++)
    {
        BloodView* blood = (BloodView*)m_pArrayBlood->objectAtIndex(i);
        if(!blood->getIsDied())
        {
            if(blood->outOfScreen())
            {
                blood->setIsDied(true);
                blood->removeFromParent();
            }
        }
    }
}

//检测道具飞机子弹是否全部打完
void GameCombatLayer::checkToolPlaneBulletofDied()
{
    //如果道具飞机数组为空 返回
    if (m_pArrayToolPlane->count() < 1)
    {
        return;
    }
    
    //从父节点删除道具飞机
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pArrayToolPlane, obj)
    {
        EnemyView* toolPlane = (EnemyView*)obj;
        
        if(toolPlane && !toolPlane->getIsDied())
        {
            this->removeToolPlaneFromHero(toolPlane);
        }
        
    }
    
}

//
void GameCombatLayer::removeToolPlaneFromHero(CraftView *craft)
{
    
    //如果道具飞机子弹数组为空
    if (m_pArrayToolBullets->count() < 1)
    {
        return;
    }
    
    //获取道具飞机的编号
    int planeBelong = craft->getBelongCount();
    if (planeBelong < 1)
    {
        return;
    }
    
    //统计存在的子弹个数
    int count = 0;
    
    int bulletBelong = 0;
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pArrayToolBullets, obj)
    {
        BulletView* bullet = (BulletView*)obj;
        
        bulletBelong = bullet->getBelongCount();
        
        if (bullet &&  bullet->getBelongCount() == planeBelong)
        {
            if (!bullet->getIsDied())
            {
                count++;
            }
        }
        
    }
    
    //
    if (count < 1)
    {
        //如果本次道具飞机和道具飞机子弹 不相同 则不处理道具飞机
        if (planeBelong != bulletBelong)
        {
            return;
        }
        
        //删除道具飞机
        craft->setIsDied(true);
        craft->removeFromParent();
        
        //道具飞机子弹移除
        for(int i = m_pArrayToolBullets->count() - 1; i >= 0; i--)
        {
            BulletView *bullet = (BulletView *)m_pArrayToolBullets->objectAtIndex(i);
            
            if(bullet && bullet->getIsDied())
            {
                if (bullet->getBelongCount() == planeBelong)
                {
                    m_pArrayToolBullets->fastRemoveObjectAtIndex(i);
                }
            }
        }
    }
    
}

/******************飞机*******************/
//创建主角
void GameCombatLayer::createHero()
{
    //主机数据
    CraftData* pData = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_HERO, 0);
    m_pHeroView = HeroView::create(pData);
    m_pHeroView->setPosition(ccp(_s_width/2, _s_height/8));
    m_pHeroView->setRotation(CRAFT_FLY_DIR);
    m_pHeroView->setType(CRAFT_TYPE_HERO);
    m_pHeroView->setScale(0.8f);//主角大小缩放
    m_pHeroView->setUUID(0);
    //设置血量
    m_heroMaxHp += m_pHeroView->getTotalHP();
    m_pHeroView->setTotalHP(m_heroMaxHp);
    m_pHeroView->setCurrentHP(m_heroMaxHp);
    
    //代理
    m_pHeroView->delegateBullet = this;
    m_pHeroView->delegateCrashed = this;
    
    this->addChild(m_pHeroView, GAME_ZORDER_HERO);
    
    //机头动画 和 拖尾
    m_pHeroView->createHeadAni();
    
    ccBlendFunc func = {GL_SRC_ALPHA,GL_ONE};
    //主机头部拖尾
    FerrisMotionStreak* headStreak = FerrisMotionStreak::create(0.2f, 3, 10, ccWHITE, "game/streak/streak_hero.png");
    headStreak->setPosition(ccpAdd(m_pHeroView->getPosition(), ccp(0,POSY_HERO_HEAD_STREAK)));
    this->addChild(headStreak,GAME_ZORDER_HERO,TAG_HERO_HEAD);
    headStreak->setBlendFunc(func);
    
    //创建技能准备的进度条
    CCSprite* spr_progress = CCSprite::create("game/skill/skill_prepare.png");
    m_pProgressForPrapare = CCProgressTimer::create(spr_progress);
    m_pProgressForPrapare->setScale(0.2);
    m_pProgressForPrapare->setType(kCCProgressTimerTypeRadial);
    m_pProgressForPrapare->setPercentage(0.0f);
    m_pHeroView->addChild(m_pProgressForPrapare);
    
    //创建 副炮 and 僚机
    createBatteryForHero();
}

void GameCombatLayer::setCraftHeadStreak(bool enable)
{
    if (m_pHeroView && !m_pHeroView->getIsDied())
    {
        if (this->getChildByTag(TAG_HERO_HEAD))
        {
            this->removeChildByTag(TAG_HERO_HEAD);
        }
        
        //创建显示拖尾
        if (enable == true)
        {
            ccBlendFunc func = {GL_SRC_ALPHA,GL_ONE};
            //主机头部拖尾
            FerrisMotionStreak* headStreak = FerrisMotionStreak::create(0.2f, 3, 10, ccWHITE, "game/streak/streak_hero.png");
            headStreak->setPosition(ccpAdd(m_pHeroView->getPosition(), ccp(0,POSY_HERO_HEAD_STREAK)));
            this->addChild(headStreak,GAME_ZORDER_HERO,TAG_HERO_HEAD);
            headStreak->setBlendFunc(func);
        }
    }
    
}

//为主机创建炮台
void GameCombatLayer::createBatteryForHero()
{
    //获取选择的主炮、副炮和僚机ID
    int nSelectedMainGun = atoi(UserEquip::shared()->getMainGunID().c_str());
    int nSelectedFirstGun = atoi(UserEquip::shared()->getFirSubGunID().c_str());
    int nSelectedSecondaryGun = atoi(UserEquip::shared()->getSecSubGunID().c_str());
    
    //获取主机的数据
    CraftData *heroCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_HERO, 0);
    
    
    //主炮
    if(nSelectedMainGun > 0)
    {
        CCLog("MainGun equip id : %s",UserEquip::shared()->getMainGunID().c_str());
        
        EquipItem* pEquipItem = UserEquipBag::shared()->getMainEquip();
        
        CCLog("equip ==   attack =   %d",pEquipItem->getAttack());
        
        TargetPlaneData* tarconnon = DataManager::sharedDataManager()->getTargetConnon(pEquipItem->getImgName());
        
        CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_CONNON, getIntFromS(tarconnon->getPlaneID().c_str()));
        
        dataCraft->setType(CRAFT_DATA_TYPE_CONNON);
        dataCraft->setConnonType(EQUIPMENT_TYPE_MAIN_GUN);
        
        //主炮的创建
        m_pMainGun = EquipView::create(dataCraft, pEquipItem);
        m_pMainGun->setScheduler(m_pCombatScheduler);
        //代理
        m_pMainGun->delegateBullet = this;
        m_pMainGun->delegateCrashed = this;
        
        m_pMainGun->setType(CRAFT_TYPE_BATTERY);
        
        //开火
        m_pMainGun->changePhrase(curMainGunPhrase);
        
        m_pMainGun->setRotation(90.0f);
        
        PositionData* posData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("1");
        
        m_pMainGun->setPosition(ccp(posData->getX(), posData->getY()));
        m_pHeroView->addChild(m_pMainGun);
        //设置为主炮
        m_pMainGun->setBatteryDirection(1);
        
        //初始化技能
        switch (pEquipItem->getSkillID())
        {
            case MAINGUN_SKILL_TYPE_NULL:
                
                m_pMainSkill = new SkillNull();
                break;
                
            case MAINGUN_SKILL_TYPE_SANDS_OF_TIME:
                
                m_pMainSkill = new SkillSandsOfTime(this);
                break;
                
            case MAINGUN_SKILL_TYPE_WAVE_GUN:
                
                m_pMainSkill = new SkillWave(this);
                break;
                
            case MAINGUN_SKILL_TYPE_SHAPED_GUN:
                
                m_pMainSkill = new SkillShapedGun(this);
                break;
                
            case MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING:
                
                m_pMainSkill = new SkillShadowOfHiding(this);
                break;
                
            case MAINGUN_SKILL_TYPE_REPULSION_SHIELD:
                
                m_pMainSkill = new SkillRepulsionShield(this);
                break;
                
            case MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING:
                
                m_pMainSkill = new SkillPositioningShooting(this);
                break;
                
            default:
                m_pMainSkill = new SkillNull();
                break;
        }
        
    }
    
    //副炮1
    if(nSelectedFirstGun > 0)
    {
        
        CCLog("FirSubGunID equip id : %s",UserEquip::shared()->getFirSubGunID().c_str());
        
        EquipItem* pEquipItem = UserEquipBag::shared()->getFirSubEquip();
        
        TargetPlaneData* tarconnon = DataManager::sharedDataManager()->getTargetConnon(pEquipItem->getImgName());
        
        CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_CONNON, getIntFromS(tarconnon->getPlaneID().c_str()));
        
        dataCraft->setType(CRAFT_DATA_TYPE_CONNON);
        dataCraft->setConnonType(EQUIPMENT_TYPE_LEFT_GUN);
        
        //肩炮 左
        m_pBearLeftGun = EquipView::create(dataCraft, pEquipItem);
        m_pBearLeftGun->setScheduler(m_pCombatScheduler);
        //代理
        m_pBearLeftGun->delegateBullet = this;
        m_pBearLeftGun->delegateCrashed = this;
        //类型
        m_pBearLeftGun->setType(CRAFT_TYPE_BATTERY);
        //主机起飞并开火
        m_pBearLeftGun->changePhrase(curDeputy1GunPhrase);
        //角度
        m_pBearLeftGun->setRotation(90.0f);
        //位置
        PositionData* posLData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("2");
        m_pBearLeftGun->setPosition(ccp(posLData->getX(), posLData->getY()));
        m_pHeroView->addChild(m_pBearLeftGun);
        //设置为左边炮
        m_pBearLeftGun->setBatteryDirection(2);
        
        //肩炮 右
        m_pBearRightGun = EquipView::create(dataCraft, pEquipItem);
        m_pBearRightGun->setScheduler(m_pCombatScheduler);
        //代理
        m_pBearRightGun->delegateBullet = this;
        m_pBearRightGun->delegateCrashed = this;
        //类型
        m_pBearRightGun->setType(CRAFT_TYPE_BATTERY);
        
        m_pBearRightGun->setEquipDirection(1);
        
        m_pBearRightGun->changePhrase(curDeputy1GunPhrase);
        //角度
        m_pBearRightGun->setRotation(90.0f);
        //位置
        PositionData* posRData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("3");
        m_pBearRightGun->setPosition(ccp(posRData->getX(), posRData->getY()));
        m_pHeroView->addChild(m_pBearRightGun);
        //设置为右边炮
        m_pBearRightGun->setBatteryDirection(3);
        
    }
    
    //副炮2
    if(nSelectedSecondaryGun > 0)
    {
        
        CCLog("SecSubGunID equip id : %s",UserEquip::shared()->getSecSubGunID().c_str());
        
        EquipItem* pEquipItem = UserEquipBag::shared()->getSecSubEquip();
        
        TargetPlaneData* tarconnon = DataManager::sharedDataManager()->getTargetConnon(pEquipItem->getImgName());
        
        CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_CONNON, getIntFromS(tarconnon->getPlaneID().c_str()));
        dataCraft->setType(CRAFT_DATA_TYPE_CONNON);
        dataCraft->setConnonType(EQUIPMENT_TYPE_RIGHT_GUN);
        
        //翼炮 左
        m_pWingLeftGun = EquipView::create(dataCraft, pEquipItem);
        m_pWingLeftGun->setScheduler(m_pCombatScheduler);
        //代理
        m_pWingLeftGun->delegateBullet = this;
        m_pWingLeftGun->delegateCrashed = this;
        //类型
        m_pWingLeftGun->setType(CRAFT_TYPE_BATTERY);
        
        m_pWingLeftGun->changePhrase(curDeputy2GunPhrase);
        //角度
        m_pWingLeftGun->setRotation(90.0f);
        //位置
        PositionData* posLData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("5");
        m_pWingLeftGun->setPosition(ccp(posLData->getX(), posLData->getY()));
        m_pHeroView->addChild(m_pWingLeftGun);
        //设置为左边炮
        m_pWingLeftGun->setBatteryDirection(2);
        
        //翼炮 左
        m_pWingRightGun = EquipView::create(dataCraft, pEquipItem);
        m_pWingRightGun->setScheduler(m_pCombatScheduler);
        //代理
        m_pWingRightGun->delegateBullet = this;
        m_pWingRightGun->delegateCrashed = this;
        //类型
        m_pWingRightGun->setType(CRAFT_TYPE_BATTERY);
        
        m_pWingRightGun->setEquipDirection(1);
        
        m_pWingRightGun->changePhrase(curDeputy2GunPhrase);
        //角度
        m_pWingRightGun->setRotation(90.0f);
        //位置
        PositionData* posRData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("4");
        m_pWingRightGun->setPosition(ccp(posRData->getX(), posRData->getY()));
        m_pHeroView->addChild(m_pWingRightGun);
        //设置为右边炮
        m_pWingRightGun->setBatteryDirection(3);
        
    }
    
    m_pLaser = NULL;
    
    
    //初始化激光
    m_pLaser = new LayserLayer(this);
    
}

void GameCombatLayer::afterRevive(cocos2d::CCObject *pSender)
{
    b_canRevive = false;
}

void GameCombatLayer::checkHeroHP()
{
    float hp = m_pHeroView->getCurrentHP();
    
    if (hp < m_pHeroView->getTotalHP() * 0.05f)
    {
        //如果道具格子1内的道具还存在 则释放该道具 然后主战机无敌
        if (m_pArrayFlyTool->count() > 0)
        {
            ToolsView* tool = (ToolsView*)m_pArrayFlyTool->objectAtIndex(0);
            
            if (tool && !tool->getIsDied())
            {
                this->useTool(false);
                
                //英雄无敌
                m_pHeroView->setIsInvincible(true);
                
                m_pHeroView->runAction(CCSequence::create(CCDelayTime::create(1.0f),CCCallFunc::create(this, callfunc_selector(GameCombatLayer::callbackCancelInvincible)),NULL));
            }
            
        }
        
        //如果主战机死亡
        if (m_pHeroView && m_pHeroView->getIsDied())
        {
            //如果购买了无敌
            if (b_canRevive)
            {
                //b_canRevive = false;
                CCCallFunc *func = CCCallFunc::create(this, callfunc_selector(GameCombatLayer::revive));
                CCCallFunc *func2 = CCCallFunc::create(this, callfunc_selector(GameCombatLayer::afterRevive));
                CCSequence* seq = CCSequence::create(func,func2,NULL);
                this->runAction(seq);
            }
            else
            {
                GameDelegate::shared()->openSceneRevive(m_resultCountData);
            }
            
            return;
        }
    }
    
}
//主机取消无敌
void GameCombatLayer::callbackCancelInvincible()
{
    m_pHeroView->setIsInvincible(false);
}
//飞机代理
//子弹撞了
void GameCombatLayer::bulletCrashed(BulletView *&bullet, bool is_play_animation)
{
    //获取子弹位置
    CCPoint bullet_pos = bullet->getPosition();
    
    bullet->setIsDied(true);
    bullet->removeFromParentAndCleanup(true);
    
    //击中样式
    int nType = bullet->getHitType();
    //CCSpriteFrameCache* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    switch (nType)
    {
        case BULLET_HIT_TYPE1:
        {
            //子弹1-10 11 12
            CCSpriteFrameCache* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            string path = "";
            
            int id = getIntFromS(bullet->getID().c_str());
            
            if (id == 11)
            {
                path = "effect_hit_7.png";
                
            }
            else if(id == 12)
            {
                path = "effect_hit_6.png";
                
            }else
            {
                path = "effect_hit_1.png";
                
            }
            
            CCSprite* hit_sp = CCSprite::createWithSpriteFrame(frame_cache->spriteFrameByName(path.c_str()));
            hit_sp->setPosition(bullet_pos);
            hit_sp->setScale(0.2f);
            this->addChild(hit_sp,GAME_ZORDER_HIT_EFFECT);
            
            hit_sp->setActionManager(m_pCombatActionManager);
            
            float offest = 1.0f - getRandom(1, 3) * 0.1;
            
            CCSpawn* spawn = NULL;
            
            if ((id == 11)||(id == 12))
            {
                spawn = CCSpawn::create(CCScaleTo::create(0.1f, offest,offest),
                                        CCFadeOut::create(0.1f),CCRotateBy::create(0.1f, getRandom(1, 360)),NULL);
            }
            else
            {
                spawn = CCSpawn::create(CCScaleTo::create(0.1f, offest,offest),
                                        CCFadeOut::create(0.1f),NULL);
            }
            
            CCSequence* seq =  CCSequence::create(spawn,
                                                  CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                                  NULL);
            hit_sp->runAction(seq);
            
        }
            break;
        case BULLET_HIT_TYPE2:
        {
            CCSpriteFrameCache* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            
            //子弹13
            //数量随机
            int num = getRandom(1, 3);
            
            for (int i = 0; i < num; i++)
            {
                CCSprite* hit_sp = CCSprite::createWithSpriteFrame(frame_cache->spriteFrameByName("effect_hit_2.png"));
                hit_sp->setPosition(getRandomPositionByOffset(bullet_pos, 2, 2));
                hit_sp->setScale(1.5f);
                this->addChild(hit_sp,GAME_ZORDER_HIT_EFFECT);
                
                hit_sp->setActionManager(m_pCombatActionManager);
                
                CCSequence* seq =  CCSequence::create(CCRotateBy::create(0.1f, getRandom(1, 360)),
                                                      CCFadeOut::create(0.1f),
                                                      CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                                      NULL);
                hit_sp->runAction(seq);
                
            }
            
        }
            break;
        case BULLET_HIT_TYPE3:
        {
            //子弹16 19 14 15
            //粒子效果
            FerrisParticle* hit = NULL;
            
            int id = getIntFromS(bullet->getID().c_str());
            
            //路径
            string path = "game/particles/";
            
            if ((id == 16)||( id == 19))
            {
                path.append("particle_effect_hit_1.plist");
            }
            else if (id == 14)
            {
                path.append("particle_effect_hit_2.plist");
            }
            else if (id == 15)
            {
                path.append("particle_effect_hit_3.plist");
            }
            
            hit = FerrisParticle::create(path.c_str());
            
            if (hit)
            {
                //角度
                if (id == 16)
                {
                    hit->setRotation(45);
                }
                else if( id == 19)
                {
                    hit->setRotation(-45);
                }
                
                //位置
                if ((id == 14)||(id == 15))
                {
                    hit->setPosition(ccp(bullet->getPositionX(),bullet->getPositionY() + bullet->getContentSize().height + 50));
                }else
                {
                    hit->setPosition(bullet_pos);
                }
                hit->setActionManager(m_pCombatActionManager);
                //自动移除
                hit->setAutoRemoveOnFinish(true);// 完成后自动移除
                this->addChild(hit,GAME_ZORDER_HIT_EFFECT);
                
            }
            
        }
            break;
        case BULLET_HIT_TYPE4://18
        {
            CCSpriteFrameCache* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            //子弹18
            CCSequence* seq = CCSequence::create(CCScaleTo::create(0.5f, 1.0f, 1.0f),
                                                 //CCFadeIn::create(0.2),
                                                 CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                                 NULL);
            CCSequence* seq1 = CCSequence::create(CCScaleTo::create(0.5f, 1.1f, 1.1f),
                                                  //CCFadeIn::create(0.2),
                                                  CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                                  NULL);
            CCSequence* seq2 = CCSequence::create(CCScaleTo::create(0.5f, 1.2f, 1.2f),
                                                  //CCFadeIn::create(0.2),
                                                  CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                                  NULL);
            CCSprite* sp1 = CCSprite::createWithSpriteFrame(frame_cache->spriteFrameByName("trace_missile_hit_1.png"));
            sp1->setPosition(bullet_pos);
            sp1->setScale(0.2);
            this->addChild(sp1,GAME_ZORDER_HIT_EFFECT);
            sp1->setActionManager(m_pCombatActionManager);
            sp1->setOpacity(128);
            sp1->runAction(CCRepeatForever::create(CCRotateBy::create(0.1f, 360.0f)));
            sp1->runAction(seq);
            
            CCSprite* sp2 = CCSprite::createWithSpriteFrame(frame_cache->spriteFrameByName("trace_missile_hit_2.png"));
            sp2->setPosition(bullet_pos);
            sp2->setScale(0.2);
            sp2->setRotation(120.0f);
            this->addChild(sp2,GAME_ZORDER_HIT_EFFECT);
            sp2->setActionManager(m_pCombatActionManager);
            sp2->setOpacity(128);
            sp2->runAction(CCRepeatForever::create(CCRotateBy::create(0.1f, 360.0f)));
            sp2->runAction(seq1);
            
            CCSprite* sp3 = CCSprite::createWithSpriteFrame(frame_cache->spriteFrameByName("trace_missile_hit_3.png"));
            sp3->setPosition(bullet_pos);
            sp3->setScale(0.2);
            sp3->setRotation(240.0f);
            this->addChild(sp3,GAME_ZORDER_HIT_EFFECT);
            sp3->setActionManager(m_pCombatActionManager);
            sp3->setOpacity(128);
            sp3->runAction(CCRepeatForever::create(CCRotateBy::create(0.1f, 360.0f)));
            sp3->runAction(seq2);
        }
            break;
        case BULLET_HIT_TYPE5:
        {
            CCSpriteFrameCache* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            //子弹17 21
            int id = getIntFromS(bullet->getID().c_str());
            
            //路径
            string path = "";
            float scale = 1.0f;
            float speed = 0.4f;
            float len = 220.0f;
            float interval= 1.2f;
            
            if (id == 17)
            {
                path.append("effect_hit_3.png");
                len = 150.0f;
                interval = 0.25f;
                speed = 0.25f;
            }
            else if (id == 21)
            {
                path.append("effect_hit_4.png");
                scale = 1.5f;
                interval = 0.8f;
                speed = 0.8f;
            }
            
            CCSprite* hit_sp = CCSprite::createWithSpriteFrame(frame_cache->spriteFrameByName(path.c_str()));
            hit_sp->setPosition(ccpAdd(bullet_pos, ccp(0,hit_sp->getContentSize().height / 2)));
            hit_sp->setOpacity(185);
            hit_sp->setScale(scale);
            this->addChild(hit_sp,GAME_ZORDER_HIT_EFFECT);
            
            hit_sp->setActionManager(m_pCombatActionManager);
            
            CCSpawn* spawn = CCSpawn::create(CCFadeOut::create(interval),
                                             CCMoveBy::create(speed, ccp(0,len))
                                             ,NULL);
            
            CCSequence* seq = CCSequence::create(spawn,CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),NULL);
            
            
            hit_sp->runAction(seq);
            
        }
            break;
        case BULLET_HIT_TYPE6:
        {
            CCSpriteFrameCache* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            //子弹23
            CCSprite* hit_sp = CCSprite::createWithSpriteFrame(frame_cache->spriteFrameByName("effect_hit_5.png"));
            hit_sp->setAnchorPoint(ccp(0.5f,1.0f));
            hit_sp->setPosition(ccpAdd(bullet_pos, ccp(0,hit_sp->getContentSize().height / 3)));
            hit_sp->setScaleX(0.8f);
            hit_sp->setScaleY(1.0f);
            this->addChild(hit_sp,GAME_ZORDER_HIT_EFFECT);
            
            hit_sp->setActionManager(m_pCombatActionManager);
            
            CCFadeOut* fadeOut = CCFadeOut::create(0.2f);
            
            CCScaleTo* scaleTo = CCScaleTo::create(0.5f, 2.5f,3.5f);
            
            CCSpawn* spawn = CCSpawn::create(fadeOut,scaleTo,NULL);
            
            CCSequence* seq = CCSequence::create(spawn,CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),NULL);
            
            hit_sp->runAction(seq);
        }
            break;
        case BULLET_HIT_TYPE7:
        {
            //子弹20
            //            SpriteAnimationNew* sp = SpriteAnimationNew::create("craft_boom_3", 1, 0.05f);
            //            sp->setPosition(bullet_pos);
            //            sp->setScale(2.5f);
            //
            //
            //            sp->setBlendFunc(func);
            //
            //            this->addChild(sp,GAME_ZORDER_HIT_EFFECT);
            
            ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE};
            CCSprite* sp = CCSprite::createWithSpriteFrameName("craft_boom_3_1.png");
            sp->setActionManager(m_pCombatActionManager);
            sp->setScale(2.5f);
            sp->setPosition(bullet_pos);
            this->addChild(sp,GAME_ZORDER_HIT_EFFECT);
            sp->setBlendFunc(func);
            
            CCAnimation* anim = CCAnimation::create();
            char ch[50] = {0};
            for (int k = 1 ; k < 7; k++)
            {
                sprintf(ch,"craft_boom_3_%d.png",k);
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ch);
                anim->addSpriteFrame(frame);
            }
            anim->setLoops(1);
            anim->setDelayPerUnit(0.05f);
            
            CCAnimate* animate = CCAnimate::create(anim);
            CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
            CCSequence* seq = CCSequence::create(animate,funcN,NULL);
            
            sp->runAction(seq);
            
        }
            break;
        default:
            break;
    }
    
}
//飞机撞了
void GameCombatLayer::craftCrashed(CraftView *craft, int reason, bool is_play_animation)
{
    //如果已经死亡 返回
    if(craft->getIsDied())
    {
        return;
    }
    
    craft->stopAllActions();
    
    //设置掉落什么东西
    if (craft->getIsBattery())
    {
        craft->setIsDropNormalTools(true);
        craft->setIsDropInvincibleTools(true);
        craft->setIsDropBloodCell(true);
    }
    else
    {
        craft->setIsDropScore(true);
        craft->setIsDropCoin(true);
        craft->setIsDropEquip(true);
    }
    
    //掉落
    //1、金币
    if(craft->getIsDropCoin() && craft->getType() == CRAFT_TYPE_ENEMY)
    {
        this->dropCoin(craft);
    }
    
    //2、装备
    if (craft->getIsDropEquip() && craft->getType() == CRAFT_TYPE_ENEMY)
    {
        this->dropEquip(craft);
    }
    
    //附属飞机（炮）
    //3、普通道具
    if(craft->getIsDropNormalTools() && (craft->getType() == CRAFT_TYPE_ENEMY || craft->getIsBattery()))
    {
        this->dropToolsForNormal(craft);
    }
    //强化道具
    if(craft->getIsDropInvincibleTools() && (craft->getType() == CRAFT_TYPE_ENEMY || craft->getIsBattery()))
    {
        this->dropToolsForInvincible(craft);
    }
    
    //4、积分
    if (craft->getIsDropScore() && (craft->getType() == CRAFT_TYPE_ENEMY))
    {
        this->dropScore(craft);
    }
    
    //5、血球
    if (craft->getIsDropBloodCell() && (craft->getType() == CRAFT_TYPE_ENEMY || craft->getIsBattery()))
    {
        this->dropBlood(craft);
    }
    
    //如果击毁的是炮
    if (craft->getIsBattery())
    {
        //遍历子弹变成积分
        this->enemyBulletToScore(craft);
    }
    
    craft->setIsDied(true);
    
    //爆炸
    if (craft->getType() == CRAFT_TYPE_ENEMY)
    {
        //飞机炮爆炸(没有光圈)
        if (craft->getIsBattery())
        {
            this->addBoomForBattery(craft);
            return;
        }
        
        //如果飞机有附属炮
        if (craft->getIsExistBattery())
        {
            EnemyView* enemy = (EnemyView *)craft;
            if(enemy)
            {
                enemy->setEnemyBatteryCrashed();
            }
        }
        
        //飞机爆炸
        int type = getCraftSizeType(craft);
        if (type > CRAFT_SIZE_SMALL)
        {
            //大飞机爆炸
            this->addBigBoomForCraft(craft);
            
            //日常统计 击毁
            DailyCountData::sharedInstance()->addDailyDestrotEnemy();
            
            //成就统计 击毁数
            GameCountData::sharedInstance()->addShatter();
            
            //增加击毁数
            m_nShatter++;
            
            if (m_nShatter > DailyCountData::sharedInstance()->getOneMissionDestoryEnemy())
            {
                DailyCountData::sharedInstance()->setOneMissionDestoryEnemy(m_nShatter);
                
                DailyCountData::sharedInstance()->hadAchieveComplete();
            }
        }
        else
        {
            //小飞机爆炸
            this->addSmallBoomForCraft(craft);
            
            //日常统计 击毁
            DailyCountData::sharedInstance()->addDailyDestrotEnemy();
            
            //成就统计 击毁数
            GameCountData::sharedInstance()->addShatter();
            
            //增加击毁数
            m_nShatter++;
            
            if (m_nShatter > DailyCountData::sharedInstance()->getOneMissionDestoryEnemy())
            {
                DailyCountData::sharedInstance()->setOneMissionDestoryEnemy(m_nShatter);
                
                DailyCountData::sharedInstance()->hadAchieveComplete();
            }
        }
    }
    else
    {
        //从父节点中删除
        craft->removeFromParentAndCleanup(true);
    }
    
}
//enemy恢复之前的动作
void GameCombatLayer::resumeEnemyPreAction(EnemyView *craft)
{
    
}

//创建道具飞机
void GameCombatLayer::createToolPlane(TargetPlaneData *dataTargetPlane)
{
    //读取数据
    CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(dataTargetPlane->getDataType(), getIntFromS(dataTargetPlane->getPlaneID().c_str()));
    
    //创建飞机
    EnemyView* plane = plane = EnemyView::create(dataCraft, dataTargetPlane);
    plane->setType(CRAFT_TYPE_BATTERY_TOOL);
    
    //代理
    plane->delegateBullet = this;
    plane->delegateCrashed = this;
    
    //血量
    plane->setCurrentHP(1000);
    plane->setTotalHP(1000);
    plane->setIsDisplayHp(false);
    
    //计数
    toolPlaneNumCount++;
    plane->setBelongCount(toolPlaneNumCount);
    
    //转角
    plane->setRotation(90.0f);
    
    //设置不显示
    plane->setVisible(false);
    
    //开火
    plane->setIsBeginFly(true);
    
    //进入第一阶段逻辑
    plane->changePhrase(1);
    
    //加入到飞机层
    m_pHeroView->addChild(plane,-1);
    
    //加到道具飞机数组中
    m_pArrayToolPlane->addObject(plane);
    
}

//创建普通敌人
void GameCombatLayer::createEnemy(TargetPlaneData *dataTargetPlane, TimePlaneData *dataTimePlane, bool is_pause)
{
    
    CCLog("targetplane id is %s",dataTargetPlane->getPlaneID().c_str());
    
    //路径编号
    int route = getIntFromS(dataTimePlane->getRoute().c_str());
    //是否随机轨迹
    bool bRandRoute = dataTimePlane->getIsRandRoute();
    
    //飞机类型
    int plane_type = dataTargetPlane->getPlaneType();
    
    //重复次数
    int repeat_times = dataTimePlane->getRepeatTimes();
    
    //创建ENEMY COIN STONE
    for(int i = 0; i < repeat_times; i++)
    {
        //读取数据
        CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(dataTargetPlane->getDataType(), getIntFromS(dataTargetPlane->getPlaneID().c_str()));
        
        //创建飞机 或者宝石 或者金币
        EnemyView* plane = NULL;
        
        if(plane_type == CRAFT_TYPE_ENEMY)
        {
            //敌人飞机
            plane = EnemyView::create(dataCraft, dataTargetPlane);
            plane->setType(CRAFT_TYPE_ENEMY);
        }
        
        //设置动作管理
        plane->setActionManager(m_pCombatActionManager);
        
        //代理
        plane->delegateBullet = this;
        plane->delegateCrashed = this;
        
        //血量
        plane->setCurrentHP(dataTargetPlane->getPlaneBlood());
        plane->setTotalHP(dataTargetPlane->getPlaneBlood());
        
        //速度
        plane->setSpeed(dataTimePlane->getSpeed());
        
        //路径
        plane->setDataPathGroup((PathGroupData *)PathLibrary::sharedPathLibrary()->getPathGroupDataByID(route));
        plane->setCurPath(1);
        
        //位置
        plane->setPosition(plane->getDataPathGroup()->getFirstPoint());
        
        //转角
        if(plane->getDataPathGroup()->getPathAutoRotation(1))
        {
            float rot = 0;
            
            if(plane->getDataPathGroup()->getCurPathRotation(1, plane->getPosition(), rot))
            {
                plane->setRotation(rot);
            }
        }
        
        //动作 首次延迟 入场处理 首次路径 获取下次路径
        CCSequence *seq = CCSequence::create(CCDelayTime::create(dataTimePlane->getFirstDelay() + dataTimePlane->getRepeatDelay() * i),
                                             CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackSetCraftBegin)),
                                             plane->getDataPathGroup()->getPathActionByID("1", plane, dataTimePlane->getSpeed()),
                                             CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackNextPathAction)),
                                             NULL);
        CCSequence *seq1 = CCSequence::create(CCDelayTime::create(dataTimePlane->getFirstDelay() + dataTimePlane->getRepeatDelay() * i),
                                              CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackSetCraftBegin)),
                                              plane->getDataPathGroup()->getPathActionByID("1", plane, dataTimePlane->getSpeed()),
                                              CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackRandAndMove)),
                                              CCDelayTime::create(ENEMY_INJURED_DELEAY),
                                              CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackSetEnemyInjured)),
                                              NULL);
        
        if(dataTimePlane->getIsMissile())
        {
            //设置血条不显示
            plane->setIsDisplayHp(false);
            //导弹动作
            CCSequence* seq_missile = CCSequence::create(CCDelayTime::create(dataTimePlane->getFirstDelay() - 0.8f),
                                                         //CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::showMissileWarning)),
                                                         NULL);
            
            plane->runAction(seq_missile);
            
            plane->runAction(seq);
            
            //是导弹就添加粒子特效
            CCMotionStreak* pStreak = CCMotionStreak::create(0.3f, 2, 40, ccRED, "game/ammo/streak.png");
            plane->setMotionStreak(pStreak);
            
            addChild(pStreak, 0);
        }
        else
        {
            int nSiseLevel = getCraftSizeType(plane);
            
            if((nSiseLevel == CRAFT_SIZE_SMALL||nSiseLevel == CRAFT_SIZE_MID || nSiseLevel == CRAFT_SIZE_BIG || nSiseLevel == CRAFT_SIZE_SMALLBOSS ||
                nSiseLevel == CRAFT_SIZE_BOSS) && bRandRoute)
            {
                //随机移动
                plane->runAction(seq1);
            }
            else
            {
                plane->runAction(seq);
            }
        }
        
        //如果is_pause 游戏主时间轴将不会增加 直到当前所有飞机被消灭
        if(is_pause)
        {
            plane->setIsTaskPlane(true);
            m_pArrayTaskEnemy->addObject(plane);
        }
        
        //加入到飞机层 用plane的m_uID作为tag
        this->addChild(plane, GAME_ZORDER_ENEMY, plane->m_uID);
        
        m_pDictEnemy->setObject(plane, plane->m_uID);
        
        //创建炮台
        BatteryPositionData* battery = BatteryPosManager::shared()->getBatteryPosData(dataTargetPlane->getPlaneID());
        
        CCLog("planeid %s",dataTargetPlane->getPlaneID().c_str());
        
        plane->setIsExistBattery(true);
        this->createBatteryForCraft(plane, battery->getConnonCount(),dataTimePlane);
    }
}


//为飞机创建炮台
void GameCombatLayer::createBatteryForCraft(EnemyView* craft, int connonCount, TimePlaneData *dataTimePlane)
{
    
    int totalBlood = 0;
    
    int flagBattery = 0;
    
    for (int i = 1; i <= connonCount; i++)
    {
        flagBattery++;
        
        //获取飞机大小
        int enemyType = getCraftSizeType(craft);
        
        //随机炮管id前缀
        int cannonPrefixID = getCannonPrefixID(enemyType);
        
        //根据游戏波数 获取炮管id后缀
        int cannonSuffixID = getCanninSuffixID(m_nGameWave);
        
        //cannonid
        string cannonID = getStringFromInt(cannonPrefixID).append("_").append(getStringFromInt(cannonSuffixID));
        
        CCLog("获取到的敌机炮 =====cannon_id==== %s",cannonID.c_str());
        
        TargetPlaneData *dataTargetPlane = DataManager::sharedDataManager()->getTargetConnon(cannonID);
        
        m_fDelayTime += 1.0f;
        
        dataTargetPlane->setDelayTime(m_fDelayTime);
        
        //读取数据
        std::string str = dataTargetPlane->getPlaneID();
        
        CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(dataTargetPlane->getDataType(), getIntFromS(str.c_str()));
        dataCraft->setType(CRAFT_DATA_TYPE_ENEMY);
        
        //创建炮台
        EnemyView* plane = NULL;
        
        int plane_type = dataTargetPlane->getDataType();
        if(plane_type == CRAFT_TYPE_ENEMY)
        {
            plane = EnemyView::create(dataCraft, dataTargetPlane);
            plane->setType(CRAFT_TYPE_ENEMY);
        }
        
        plane->setScheduler(m_pCombatScheduler);
        plane->setActionManager(m_pCombatActionManager);
        
        //代理
        plane->delegateBullet = this;
        plane->delegateCrashed = this;
        
        CCLog("plane->gettotalHP : %f",plane->getTotalHP());
        
        plane->setCurrentHP(/*dataTargetPlane->getPlaneBlood() + */ m_nBloodAddedValue);
        plane->setTotalHP(/*dataTargetPlane->getPlaneBlood() + */ m_nBloodAddedValue);
        plane->setIsDisplayHp(false);
        
        CCLog("plane->gettotalHP : %f,m_nBloodAddedValue :%d",plane->getTotalHP(),m_nBloodAddedValue);
        
        //速度
        plane->setSpeed(dataTimePlane->getSpeed());
        
        //路径
        plane->setDataPathGroup((PathGroupData *)PathLibrary::sharedPathLibrary()->getPathGroupDataByID(ENEMY_ROTE_ID));
        plane->setCurPath(1);
        
        //位置
        PositionData* posdata = BatteryPosManager::shared()->getBatteryPos(craft->getPlaneID(), getStringFromInt(flagBattery));
        plane->setPosition(ccp(posdata->getX(), posdata->getY()));
        
        //转角
        plane->setRotation(-90.0f);
        
        //开火
        plane->setIsBeginFly(true);
        
        //阶段逻辑  更改将波数传进去
        plane->changePhrase(1);
        
        //特殊的
        //在XMLFile中的类型
        plane->setTypeInXMLFile(getIntFromS(dataTimePlane->getType().c_str()));
        
        //添加到附属飞机组
        craft->addBatteryPlane(plane);
        //是否是炮台
        plane->setIsBattery(true);
        
        //是否是附属飞机
        plane->setIsAffiliatedPlane(true);
        
        batteryNumCount++;
        //设置炮的子弹type
        plane->setBelongCount(batteryNumCount);
        
        //加入到Enemy的Dict中
        m_pDictEnemy->setObject(plane, plane->m_uID);
        
        //把父辈的ID付给飞行器
        plane->setHolderID(craft->m_uID);
        
        //是否有后坐力
        plane->setRecoil(dataTargetPlane->getRecoil());
        
        //设置角速度
        plane->setBatteryRotateSpeed(30);
        
        int tag = kTagBattery;
        while (craft->getChildByTag(tag))
        {
            tag ++;
        }
        
        //添加到craft身上
        craft->addChild(plane, 100, tag);
        
        //累计敌机炮血量
        totalBlood += plane->getTotalHP();
        
    }
    
    //处理敌机血量 (总血量 = 敌机血量 + 炮管血量)
    craft->setCurrentHP(craft->getTotalHP() + totalBlood);
    craft->setTotalHP(craft->getTotalHP() + totalBlood);
    
    CCLog("craft totalHp : %f,m_ngameWave : %d",craft->getTotalHP(),m_nGameWave);
    
    //开始检测炮的数量
    craft->startCheckBatterySize();
    
    //开始检测炮的状态
    craft->startCheckBatteryIsDied();
}

//敌机可以受击回调
void GameCombatLayer::callbackSetEnemyInjured(CCNode* pSender)
{
    EnemyView* enemy = (EnemyView*)pSender;
    if (enemy) {
        enemy->setIsCanInjured(true);
        enemy->setBatteryCanInjured();
    }
}

//敌机炮子弹变积分
void GameCombatLayer::enemyBulletToScore(CraftView *craft)
{
    //获取炮的编号
    int batteryType = craft->getBelongCount();
    if (batteryType < 1)
    {
        return;
    }
    
    //遍历子弹
    CCObject *obj = NULL;
    CCARRAY_FOREACH(m_pArrayEnemyBullet, obj)
    {
        BulletView* bullet = (BulletView*)obj;
        if (bullet && !bullet->getIsDied())
        {
            if (bullet->getBelongCount() == batteryType)
            {
                //停止子弹所有action
                bullet->stopAllActions();
                
                //创建积分
                CCPoint pos = bullet->getPosition();
                this->createScore(pos);
                
                bullet->setIsDied(true);
                bullet->removeFromParent();
            }
            
        }
    }
    
}
//创建积分
void GameCombatLayer::createScore(CCPoint pos)
{
    CCPoint temp_pos = pos;
    
    DropData* data = DataManager::sharedDataManager()->getDropData(DROP_DATA_ID);
    ScoreView* score = ScoreView::create(data);
    //位置
    score->setPosition(temp_pos);
    //动作
    score->runAction(CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(GameCombatLayer::callbackAutoFindHero)),NULL));
    
    score->setHeroTmp(m_pHeroView);
    score->startUpdateDrop();
    
    m_pArrayScore->addObject(score);
    m_pDropBatchNode->addChild(score);
}

//延时调用自动吸附回调
void GameCombatLayer::callbackAutoFindHero(CCNode* pSender)
{
    ScoreView* score = (ScoreView*)pSender;
    
    //动作 增加了停顿时间
    MagnetCoinAction* act = MagnetCoinAction::create(1500, m_pHeroView->getPosition());
    act->setTag(TAG_SPEEDUP_ACTION);
    score->runAction(act);
}

void GameCombatLayer::dropEquip(CraftView *craft)
{
    
    int rate = m_nGameWave * 7;
    
    int randnum = getRandom(1, 100);
    
    if (randnum > rate)
    {
        return;
    }
    
    EquipItem* item = GetRandomEquip::shared()->getEquipNormal(m_nGameWave);
    
    if (item)
    {
        string strPath = UserEquipBag::shared()->getIconPath(item);
        
        switch (item->getGrade())
        {
            case EQUIP_QUALITY_WHITE:
                m_resultCountData->m_nEquip_white++;
                break;
                
            case EQUIP_QUALITY_GREEN:
                m_resultCountData->m_nEquip_green++;
                break;
                
            case EQUIP_QUALITY_BLUE:
                m_resultCountData->m_nEquip_blue++;
                break;
                
            case EQUIP_QUALITY_PURPLE:
                m_resultCountData->m_nEquip_puple++;
                break;
                
            case EQUIP_QUALITY_ORRANGE:
                m_resultCountData->m_nEquip_orange++;
                break;
                
            default:
                break;
        }
        
        DailyCountData::sharedInstance()->addDailyGetEquip();
        
        CCSprite* spr_equip = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strPath.c_str()));
        
        spr_equip->setPosition(craft->getPosition());
        
        this->addChild(spr_equip,GAME_ZORDER_TOOLS);
        
        CCFadeOut* fadeout = CCFadeOut::create(1.0f);
        
        spr_equip->runAction(fadeout);
        
        item->setID(getStringFromInt(UserEquipBag::shared()->getMinID()));
        
        //出现之后立刻消失  然后加到玩家背包里去
        UserEquipBag::shared()->addEquip(item);
    }
}
//掉落血球
void GameCombatLayer::dropBlood(CraftView* craft)
{
    //设置概率
    //随机血球出现的概率
    int fRate = getRandom(1, 100);
    
    if (fRate > f_bloodDrop)
    {
        return;
    }
    
    //设置位置
    //位置
    CCPoint posOffset = craft->getPosition();
    if(craft->getIsAffiliatedPlane() && craft->getParent())
    {
        //如果是附属物 则加上父节点的坐标
        CCPoint mposParent = craft->getParent()->getPosition();
        posOffset = ccpAdd(posOffset, mposParent);
    }
    
    this->createBloodCell(getRandomPositionByOffset(posOffset,craft->getContentSize().width/2,craft->getContentSize().height/4));
}
//创建血球
void GameCombatLayer::createBloodCell(CCPoint pos)
{
    
    CCPoint bloodPos = pos;
    
    //随机血球出现的个数
    int num = getRandom(1, 3);
    
    for (int i = 0; i < num; ++i)
    {
        //创建血球
        BloodView* bloodView = BloodView::create();
        bloodView->setPosition(bloodPos);
        
        //设置血球的携带血量
        bloodView->setCarryBlood(m_pHeroView->getTotalHP() * 0.08f);
        
        //创建血球粒子
        bloodView->createBloodParticle();
        
        //血球出现时向下移动
        bloodView->startMoveWithScreen();
        
        bloodView->setHeroTmp(m_pHeroView);
        bloodView->startUpdateMagnet();
        
        this->addChild(bloodView,GAME_ZORDER_HERO,bloodView->m_uID);
        
        m_pArrayBlood->addObject(bloodView);
    }
    
}
void GameCombatLayer::dropScore(CraftView *craft)
{
    int base_nScore = 0;   //掉落积分的基数
    
    int multiple = 0;      //倍数
    
    switch (getChildSizeType(craft))
    {
        case CRAFT_SIZE_SMALL:
            base_nScore = 50;
            multiple = 100;
            break;
            
        case CRAFT_SIZE_MID:
            base_nScore = 100;
            multiple = 300;
            break;
            
        case CRAFT_SIZE_BIG:
            base_nScore = 200;
            multiple = 500;
            break;
            
        case CRAFT_SIZE_SMALLBOSS:
            base_nScore = 500;
            multiple = 1100;
            break;
            
        case CRAFT_SIZE_BOSS:
            base_nScore = 1000;
            multiple = 2500;
            break;
            
        default:
            break;
    }
    
    int score = (m_nGameWave - 1) * multiple + base_nScore;
    
    m_resultCountData->m_score += score;
    
    if (m_resultCountData->m_score > GameCountData::sharedInstance()->getBestScore())
    {
        GameCountData::sharedInstance()->setBestScore(m_resultCountData->m_score);
    }
    
    if (m_resultCountData->m_score > DailyCountData::sharedInstance()->getOneMissionTotalScore())
    {
        DailyCountData::sharedInstance()->setOneMissionTotalScore(m_resultCountData->m_score);
    }
    
    DailyCountData::sharedInstance()->addDailyTotalScore(score);
    
    m_battleUI->setScore(m_resultCountData->m_score);
    
    CCLabelAtlas* label = CCLabelAtlas::create(getStringFromInt(score).c_str(), "font/atlas_score.png",37,41,'0');
    
    label->setPosition(craft->getPosition());
    
    this->addChild(label,GAME_ZORDER_TOOLS);
    
    label->runAction(CCFadeOut::create(1.0f));
    
}

void GameCombatLayer::dropCoin(CraftView* craft)
{
    
    //金币掉落概率 通过波数来定
    float fRate = (m_nGameWave - 1) * 0.01f + 0.4f;
    
    //掉落概率最高90%
    if (fRate >= 0.9f)
    {
        fRate = 0.9f;
    }
    
    bool bDrop = (fRate >= 0.5) ? true : false;
    
    if(fRate == 0.0f || !bDrop)
    {
        return;
    }
    
    //获取此类型的飞机的掉落金币的数量
    int nNum = getCoinNumByType(getCraftSizeType(craft));
    //craft->getCoinNumByType(getCraftSizeType(craft));
    
    for(int i = 0; i < nNum; i++)
    {
        
        int nRow = i / 3;
        float fSpeed = 200 + nRow * 200;
        
        //读取数据
        DropData* data = DataManager::sharedDataManager()->getDropData(101);
        
        //创建金币
        CoinView* coin = CoinView::create(data);
        
        //代理
        coin->delegateDrop = this;
        
        //位置
        CCPoint posOffset = craft->getPosition();
        
        if(craft->getIsAffiliatedPlane() && craft->getParent())
        {
            //如果是附属物 则加上父节点的坐标
            CCPoint mposParent = craft->getParent()->getPosition();
            posOffset = ccpAdd(posOffset, mposParent);
        }
        
        coin->setPosition(getRandomPositionByOffset(posOffset,
                                                    craft->getContentSize().width/2,
                                                    craft->getContentSize().height/4));
        
        
        CCLog("time scale : %f",this->getScheduler()->getTimeScale());
        
        float timeScale = this->getScheduler()->getTimeScale();
        
        //动作
        CCSequence *seq = CCSequence::create(CCMoveBy::create(1.0f * this->getScheduler()->getTimeScale(), ccp(0.0f, fSpeed)),
                                             NULL);
        CCSpeed *speed = CCSpeed::create(seq,(1.0f - (nRow * 0.2f))* timeScale);
        speed->setTag(kSPEEDUP_ACTION_TAG);
        coin->runAction(speed);
        
        coin->setHeroTmp(m_pHeroView);
        coin->startUpdateDrop();
        
        //加到金币数组中
        m_pArrayCoin->addObject(coin);
        
        m_pDropBatchNode->addChild(coin);
    }
    
}
//金币加速运动
void GameCombatLayer::dropSpeedUp(DropView* drop)
{
    drop->stopActionByTag(kSPEEDUP_ACTION_TAG);
    
    MagnetCoinAction* act = MagnetCoinAction::create(1500, getHeroPosition());
    act->setTag(TAG_SPEEDUP_ACTION);
    drop->runAction(act);
}

//掉落普通道具
void GameCombatLayer::dropToolsForNormal(CraftView* craft)
{
    //处理掉落概率 随机出装备种类
    int fRate = getRandom(1, 100);
    
    if (preNormalToolRate == fRate)
    {
        fRate = getRandom(1, 100);
    }
    
    preNormalToolRate = fRate;
    
    bool bDrop = fRate < (int)f_toolDrop ? true : false;//
    
    if (!bDrop)
    {
        return;
    }
    
    int k[8] = {301,302,303,305,306,307,308,309};
    
    int kind = getRandom(0, 7);
    int type = (k[kind] % 300) - 1;
    
    //读取数据
    CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_DROP, getToolShapeID(type));
    
    //创建道具
    ToolsView* tool = ToolsView::create(dataCraft,DataManager::sharedDataManager()->getTargetTool(getStringFromInt(type)),false);
    tool->setType(CRAFT_TYPE_TOOL);
    
    //设置道具功能类型
    tool->setToolType(type);
    
    //代理
    tool->delegateCrashed = this;
    
    //随机初始位置
    //如果是附属物 则加上父节点的坐标
    CCPoint posOffset = craft->getPosition();
    
    if(craft->getIsAffiliatedPlane() && craft->getParent())
    {
        //如果是附属物 则加上父节点的坐标
        CCPoint mposParent = craft->getParent()->getPosition();
        posOffset = ccpAdd(posOffset, mposParent);
    }
    
    tool->setPosition(getRandomPositionByOffset(posOffset,
                                                craft->getContentSize().width/2,
                                                craft->getContentSize().height/4));
    
    //粒子
    tool->createToolParticle();
    
    tool->setHeroTmp(m_pHeroView);
    tool->startUpdateMagnet();
    
    //加到飞机层 用plane得m_uID作为tag
    this->addChild(tool, GAME_ZORDER_TOOLS, tool->m_uID);
    
    m_pArrayTool->addObject(tool);
    
}

//掉落强化道具
void GameCombatLayer::dropToolsForInvincible(CraftView* craft)
{
    //如果强化道具出现次数小于15 概率为7%
    int fRate = getRandom(1, 100);
    bool bDrop = false;
    
    if (m_nInvincibleTimes < 12)
    {
        bDrop = fRate < 8 ? true : false;//8
    }
    else
        //否则概率为0.5%
    {
        if (fRate < 2)
        {
            bDrop = CCRANDOM_0_1() > 0.5 ? true :false;
        }
        
    }
    
    if (m_nGameWave <= 6)
    {
        bDrop = CCRANDOM_0_1() > 0.5 ? true :false;
    }
    
    if (!bDrop)
    {
        return;
    }
    
    //强化道具出现次数加1
    m_nInvincibleTimes ++;
    
    //读取数据
    CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_DROP, getToolShapeID(3));
    
    //创建道具
    ToolsView* tool = ToolsView::create(dataCraft,DataManager::sharedDataManager()->getTargetTool(getStringFromInt(3)),false);
    tool->setType(CRAFT_TYPE_TOOL);
    
    //设置道具功能类型
    tool->setToolType(3);
    
    //代理
    tool->delegateCrashed = this;
    
    //随机初始位置
    //如果是附属物 则加上父节点的坐标
    CCPoint posOffset = craft->getPosition();
    
    if(craft->getIsAffiliatedPlane() && craft->getParent())
    {
        //如果是附属物 则加上父节点的坐标
        CCPoint mposParent = craft->getParent()->getPosition();
        posOffset = ccpAdd(posOffset, mposParent);
    }
    
    tool->setPosition(getRandomPositionByOffset(posOffset,
                                                craft->getContentSize().width/2,
                                                craft->getContentSize().height/4));
    
    tool->createToolParticle();
    
    tool->setHeroTmp(m_pHeroView);
    tool->startUpdateMagnet();
    
    //加到飞机层 用plane得m_uID作为tag
    this->addChild(tool, GAME_ZORDER_TOOLS, tool->m_uID);
    
    m_pArrInvincibleTool->addObject(tool);
    
}
//创建在主机前面飞的道具
void GameCombatLayer::createToolWithHeroFront(int type)
{
    //读取数据
    CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_DROP, getToolShapeID(type));
    
    //创建道具
    ToolsView* tool = ToolsView::create(dataCraft,DataManager::sharedDataManager()->getTargetTool(getStringFromInt(type)),true);
    tool->setType(CRAFT_TYPE_TOOL);
    
    //设置道具功能类型
    tool->setToolType(type);
    
    //位置
    tool->setPosition(ccpAdd(m_pHeroView->getPosition(), ccp(0,200)));
    
    //缩放
    tool->setScale(0.5);
    
    tool->setHeroTmp(m_pHeroView);
    tool->startUpdateFlyToolPosition();
    
    //判断装上的道具个数
    if (m_pArrayFlyTool->count() > 0)
    {
        tool->setPosType(1);
        tool->setScale(0.3);
    }
    
    //加到主战机上
    this->addChild(tool, CRAFT_ZORDER_TOOL, tool->m_uID);
    
    //添加到随机移动的道具数组中
    m_pArrayFlyTool->addObject(tool);
    
}

//设置主角攻击状态
void GameCombatLayer::setHeroAttrackState(bool bEnable)
{
    m_pHeroView->setGunSwitch(bEnable);
    if(m_pMainGun)
    {
        m_pMainGun->setGunSwitch(bEnable);
    }
    if(m_pBearLeftGun)
    {
        m_pBearLeftGun->setGunSwitch(bEnable);
    }
    if(m_pBearRightGun)
    {
        m_pBearRightGun->setGunSwitch(bEnable);
    }
    if(m_pWingLeftGun)
    {
        m_pWingLeftGun->setGunSwitch(bEnable);
    }
    if(m_pWingRightGun)
    {
        m_pWingRightGun->setGunSwitch(bEnable);
    }
    if(m_pWingView)
    {
        m_pWingView->setGunSwitch(bEnable);
    }
}
//移除craft
void GameCombatLayer::callBackRemoveCraft(CCNode* pSender)
{
    CraftView* craft = (CraftView*)pSender;
    craft->setIsDied(true);
    
    pSender->removeFromParent();
}
//移除drop
void GameCombatLayer::callbackRemoveDrop(CCNode* pSender)
{
    DropView* craft = (DropView*)pSender;
    craft->setIsDied(true);
    
    pSender->removeFromParent();
}
//移除bullet
void GameCombatLayer::callbackRemoveBullet(CCNode* pSender)
{
    BulletView* bullet = (BulletView*)pSender;
    bullet->setIsDied(true);
    
    pSender->removeFromParent();
}
//给炮爆炸效果
void GameCombatLayer::addBoomForBattery(CraftView* craft)
{
    //获取位置
    CCPoint position = craft->getParent()->getPosition() + craft->getPosition();
    craft->runAction(CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackRemoveCraft)));
    
    //爆炸效果
    ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE};
    CCSprite* sp = CCSprite::createWithSpriteFrameName("craft_boom_3_1.png");
    sp->setScale(5.7f);
    sp->setPosition(position);
    sp->setBlendFunc(func);
    sp->setActionManager(m_pCombatActionManager);
    this->addChild(sp,GAME_ZORDER_BOOM_EFFECT);
    
    CCAnimation* anim = CCAnimation::create();
    char ch[50] = {0};
    for (int k = 1 ; k < 7; k++)
    {
        sprintf(ch,"craft_boom_3_%d.png",k);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ch);
        anim->addSpriteFrame(frame);
    }
    anim->setLoops(1);
    anim->setDelayPerUnit(BOOM_INTERVAL);
    
    CCAnimate* animate = CCAnimate::create(anim);
    CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
    CCSequence* seq = CCSequence::create(animate,funcN,NULL);
    sp->runAction(seq);
}
//给飞机添加小型爆炸效果
void GameCombatLayer::addSmallBoomForCraft(CraftView* craft)
{
    //获取位置
    CCPoint position = craft->getPosition();
    //渐隐
    craft->craftFadeIn(0.2f);
    CCSequence* seq = CCSequence::create(CCDelayTime::create(0.2f),
                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackRemoveCraft)),
                                         NULL);
    craft->runAction(seq);
    
    //爆炸效果
    ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE};
    
    //小爆炸
    for (int i = 0; i < 2; i++)
    {
        CCSprite* sp = CCSprite::createWithSpriteFrameName("craft_boom_1_1.png");
        sp->setScale(2.7f);
        sp->setPosition(position);
        sp->setBlendFunc(func);
        sp->setActionManager(m_pCombatActionManager);
        sp->setRotation(getRandom(30, 350));
        this->addChild(sp,GAME_ZORDER_BOOM_EFFECT);
        
        CCAnimation* anim = CCAnimation::create();
        char ch[50] = {0};
        for (int k = 1 ; k < 16; k++)
        {
            sprintf(ch,"craft_boom_1_%d.png",k);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ch);
            anim->addSpriteFrame(frame);
        }
        anim->setLoops(1);
        anim->setDelayPerUnit(BOOM_INTERVAL);
        
        CCAnimate* animate = CCAnimate::create(anim);
        CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
        CCSequence* seq = CCSequence::create(animate,funcN,NULL);
        sp->runAction(seq);
    }
    
    //小爆炸光圈
    this->addCircleForSmallPlane(position,127,3.5f, CIRCLE_INTERVAL);
    
}
//给飞机添加大型爆炸效果
void GameCombatLayer::addBigBoomForCraft(CraftView* craft)
{
    //获取位置
    CCPoint position = craft->getPosition();
    //渐隐
    craft->craftFadeIn(0.2f);
    CCSequence* seq = CCSequence::create(CCDelayTime::create(0.2f),
                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackRemoveCraft)),
                                         NULL);
    craft->runAction(seq);
    
    float scale_anim = getScaleAnimBoomByType(getCraftSizeType(craft));
    ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE};
    
    //爆炸1效果
    CCSprite* sp = CCSprite::createWithSpriteFrameName("craft_boom_2_1.png");
    sp->setPosition(position);
    sp->setScale(scale_anim);
    sp->setBlendFunc(func);
    sp->setActionManager(m_pCombatActionManager);
    this->addChild(sp,GAME_ZORDER_BOOM_EFFECT);
    
    CCAnimation* anim = CCAnimation::create();
    char ch[50] = {0};
    for (int k = 1 ; k < 16; k++)
    {
        sprintf(ch,"craft_boom_2_%d.png",k);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ch);
        anim->addSpriteFrame(frame);
    }
    anim->setLoops(1);
    anim->setDelayPerUnit(BOOM_INTERVAL);
    
    CCAnimate* animate = CCAnimate::create(anim);
    CCMoveBy* move = CCMoveBy::create(0.05f * 15, ccp(0,-45));
    CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
    CCSequence* seq_sp = CCSequence::create(CCSpawn::createWithTwoActions(animate, move),funcN,NULL);
    sp->runAction(seq_sp);
    
    //爆炸2效果
    CCSprite* sp_1 = CCSprite::createWithSpriteFrameName("craft_boom_4_1.png");
    sp_1->setPosition(position);
    sp_1->setScale(scale_anim + 3.3f);
    sp_1->setBlendFunc(func);
    sp_1->setActionManager(m_pCombatActionManager);
    sp_1->setRotation(getRandom(30, 350));
    this->addChild(sp_1,GAME_ZORDER_BOOM_EFFECT);
    
    CCAnimation* anim_sp1 = CCAnimation::create();
    char ch_sp1[50] = {0};
    for (int k = 1 ; k < 5; k++)
    {
        sprintf(ch_sp1,"craft_boom_4_%d.png",k);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ch_sp1);
        anim_sp1->addSpriteFrame(frame);
    }
    anim_sp1->setLoops(1);
    anim_sp1->setDelayPerUnit(BOOM_INTERVAL);
    
    CCAnimate* animate_sp1 = CCAnimate::create(anim_sp1);
    CCMoveBy* move_sp1 = CCMoveBy::create(0.2f, ccp(0,-45));
    CCCallFuncN* funcN_sp1 = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
    CCSequence* seq_sp1 = CCSequence::create(CCSpawn::createWithTwoActions(animate_sp1, move_sp1),funcN_sp1,NULL);
    sp_1->runAction(seq_sp1);
    
    //获取飞机型号
    int plane_size = getCraftSizeType(craft);
    CCPoint point = ccp(sp->getContentSize().width+50, sp->getContentSize().height+50);
    
    //大爆炸后的小爆炸
    if (plane_size > CRAFT_SIZE_SMALL)
    {
        addSmallBoomsForBigBoom(plane_size,position,point);
    }
    
    //粒子效果
    FerrisParticle* boomQuad = FerrisParticle::create("game/particles/boom_01.plist");
    boomQuad->setPosition(position);
    boomQuad->setAutoRemoveOnFinish(true);// 完成后自动移除
    this->addChild(boomQuad,GAME_ZORDER_BOOM_EFFECT);
    boomQuad->setActionManager(m_pCombatActionManager);
    
    //爆炸3效果
    //根据飞机类型获取光圈缩放值
    float scale = getScaleCircleByType(getCraftSizeType(craft));
    CCFloat* data = new CCFloat(scale);
    
    CCSequence* seq_extra = NULL;
    
    if (plane_size > CRAFT_SIZE_BIG)
    {
        seq_extra = CCSequence::create(CCSpawn::create(CCCallFunc::create(this, callfunc_selector(GameCombatLayer::addExtraShaking)),
                                                       CCDelayTime::create(0.5f)
                                                       ,NULL),
                                       CCCallFuncND::create(this, callfuncND_selector(GameCombatLayer::addExtraBoom),data),
                                       NULL);
    }else
    {
        seq_extra = CCSequence::create(CCDelayTime::create(0.5f),
                                       CCCallFuncND::create(this, callfuncND_selector(GameCombatLayer::addExtraBoom),data),
                                       NULL);
    }
    
    sp->runAction(seq_extra);
    
}
//给小飞机爆炸添加光圈
void GameCombatLayer::addCircleForSmallPlane(CCPoint pos,float opacity,float scale,float dt)
{
    CCSprite* circle = CCSprite::create("game/animations/circle.png");
    circle->setPosition(pos);
    circle->setScale(0.1f);
    circle->setOpacity(opacity);
    circle->setVisible(false);
    this->addChild(circle, GAME_ZORDER_BOOM_EFFECT);
    circle->setActionManager(m_pCombatActionManager);
    
    CCSpawn* sw = CCSpawn::create(CCScaleTo::create(0.25f,scale,scale),CCFadeOut::create(0.25f),NULL);
    
    CCSequence* seq = CCSequence::create(CCDelayTime::create(dt),CCShow::create(),sw,
                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                         NULL);
    circle->runAction(seq);
}
//给大飞机爆炸添加光圈
void GameCombatLayer::addCircleForBigPlane(CCPoint pos,float opacity,float scale,float dt)
{
    CCSprite* circle = CCSprite::create("game/animations/circle.png");
    circle->setPosition(pos);
    circle->setScale(0.2f);
    circle->setOpacity(opacity);
    this->addChild(circle, GAME_ZORDER_BOOM_EFFECT);
    circle->setActionManager(m_pCombatActionManager);
    
    CCSpawn* sw = CCSpawn::create(CCScaleTo::create(0.5f,scale,scale),CCFadeOut::create(0.5f),NULL);
    
    CCSequence* seq = CCSequence::create(CCDelayTime::create(dt), sw,
                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                         NULL);
    circle->runAction(seq);
}
//给BOSS添加屏幕的晃动
void GameCombatLayer::addExtraShaking()
{
    this->addShakingScreen(0.3f);
}
//给飞机额外的爆炸效果
void GameCombatLayer::addExtraBoom(CCNode* pSender,void* craftType)
{
    SpriteAnimationNew* spBig = (SpriteAnimationNew*)pSender;
    CCPoint position = spBig->getPosition();
    
    //爆炸3效果
    ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE};
    CCSprite* sp = CCSprite::createWithSpriteFrameName("craft_boom_3_1.png");
    sp->setScale(8.0f);
    sp->setPosition(position);
    sp->setBlendFunc(func);
    sp->setActionManager(m_pCombatActionManager);
    this->addChild(sp,GAME_ZORDER_BOOM_EFFECT);
    
    CCAnimation* anim = CCAnimation::create();
    char ch[50] = {0};
    for (int k = 1 ; k < 7; k++)
    {
        sprintf(ch,"craft_boom_3_%d.png",k);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ch);
        anim->addSpriteFrame(frame);
    }
    anim->setLoops(1);
    anim->setDelayPerUnit(BOOM_INTERVAL);
    
    CCAnimate* animate = CCAnimate::create(anim);
    CCMoveBy* moveby = CCMoveBy::create(0.65f, ccp(0,-400));
    CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
    CCSequence* seq = CCSequence::create(CCSpawn::createWithTwoActions(animate, moveby),funcN,NULL);
    sp->runAction(seq);
    
    CCFloat* scale = (CCFloat*)craftType;
    
    //大爆炸光圈
    this->addCircleForBigPlane(sp->getPosition(),255,scale->getValue(), CIRCLE_INTERVAL);
    
}
//大爆炸后期的小爆炸
void GameCombatLayer::addSmallBoomsForBigBoom(int craftType,CCPoint targetPos,CCPoint range)
{
    //随机小爆炸个数
    int small_bomm_num = getRandom(2, 4);
    
    if(craftType == CRAFT_SIZE_BIG)
    {
        small_bomm_num = getRandom(3, 5);
    }
    else if (craftType == CRAFT_SIZE_SMALLBOSS)
    {
        small_bomm_num = getRandom(5, 7);
    }else if(craftType == CRAFT_SIZE_BOSS)
    {
        small_bomm_num = getRandom(6, 9);
    }
    
    ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE};
    
    //小爆炸
    for (int i = 0; i < small_bomm_num; i++) {
        //范围随机位置
        CCPoint new_pos = getRandomPositionByOffset(targetPos, range.x, range.y);
        
        //随机小爆炸爆炸次数
        int times = getRandom(4, 6);
        
        if(craftType == CRAFT_SIZE_BIG)
        {
            times = getRandom(5, 7);
        }else if(craftType == CRAFT_SIZE_SMALLBOSS || craftType == CRAFT_SIZE_BOSS)
        {
            times = getRandom(6, 8);
        }
        
        for (int j = 0; j < times; j++) {
            
            int direction = getRandom(0, 1) == 0 ? 1 : -1;
            
            CCSprite* sp = CCSprite::createWithSpriteFrameName("craft_boom_1_1.png");
            sp->setScale(2.7f);
            sp->setPosition(new_pos + ccp(direction * getRandom(20, 30),-getRandom(30, 50) * j));
            sp->setVisible(false);
            this->addChild(sp,GAME_ZORDER_BOOM_EFFECT);
            sp->setBlendFunc(func);
            sp->setActionManager(m_pCombatActionManager);
            
            CCAnimation* anim = CCAnimation::create();
            char ch[50] = {0};
            for (int k = 1 ; k < 15; k++)
            {
                sprintf(ch,"craft_boom_1_%d.png",k);
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ch);
                anim->addSpriteFrame(frame);
            }
            
            anim->setLoops(1);
            anim->setDelayPerUnit(BOOM_INTERVAL);
            
            CCAnimate* animate = CCAnimate::create(anim);
            
            CCMoveBy* moveby = CCMoveBy::create(1.0f, ccp(0,-20));
            
            CCSpawn* spawn = CCSpawn::create(animate,moveby,NULL);
            
            CCDelayTime* delay = CCDelayTime::create(0.2f * ( j + 1));
            
            CCCallFuncN* funcN_circle = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackCircleForSmall));
            
            CCCallFuncN* funcN_remove = CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode));
            
            CCSequence* seq = CCSequence::create(delay,CCShow::create(),funcN_circle,spawn,funcN_remove,NULL);
            
            sp->runAction(seq);
            
        }
        
        int a = 0;
        while (a < 100) {
            a++;
        }
        
    }
    
}
void GameCombatLayer::callbackCircleForSmall(CCNode* pSender)
{
    CCSprite* sp = (CCSprite*)pSender;
    //小爆炸光圈
    this->addCircleForSmallPlane(sp->getPosition(),127,3.5f, CIRCLE_INTERVAL);
}
//Boss飞机爆炸后屏幕晃动
void GameCombatLayer::addShakingScreen(float dt)
{
    BackgroundView* ground = (BackgroundView*)this->getChildByTag(GAME_TAG_BACKGROUND);
    
    //样条轨迹
    CCPointArray* arr = CCPointArray::create(10);
    arr->addControlPoint(ccp(-30,0));
    arr->addControlPoint(ccp(0,30));
    arr->addControlPoint(ccp(30,0));
    arr->addControlPoint(ccp(0,-30));
    
    CCCardinalSplineTo* splineby = CCCardinalSplineTo::create(0.3f, arr, 4);
    
    CCSequence* seq = CCSequence::create(CCDelayTime::create(dt),splineby,splineby,
                                         NULL);
    
    ground->runAction(seq);
}

/************************
 * 轨迹
 ************************/
//飞机轨迹
void GameCombatLayer::callBackNextPathAction(CCNode* pSender)
{
    //return;
    CraftView *plane = (CraftView *)(pSender);
    int n_next_path = plane->getCurPath() + 1;
    
    //CCLog("plane->getCurPath  ==0000000000000000000000000000===   %d",n_next_path);
    int n_actiontype = ACTION_DIED;
    
    PathGroupData *dpg = plane->getDataPathGroup();
    //获取动作类型
    PathData* p_nextPathData = dpg->getPathData(getStringFromInt(n_next_path));
    
    if(p_nextPathData)
    {
        n_actiontype = plane->getDataPathGroup()->getPathData(getStringFromInt(n_next_path))->getType();
    }
    else
    {
        n_actiontype = ACTION_STAYAT;
    }
    
    //处理加血事件
    if(n_actiontype == ACTION_ADDHP)
    {
        bool b_play = (p_nextPathData && p_nextPathData->getIsPlayEffect()) ? true : false;
        plane->setCurrentHP(plane->getTotalHP());
        
        if(b_play)
        {
            //在这里播放加血的动画和特效
            
        }
    }
    //重复动作
    else if(n_actiontype == ACTION_REPEAT)
    {
        //循环次数走完了直接返回
        PathGroupData *p_pathgroupdata = plane->getDataPathGroup();
        
        if(!p_pathgroupdata)
        {
            return;
        }
        
        //第一次进入循环，初始化本次循环的数据
        if(plane->getCurRepeatTimes() == 0)
        {
            plane->setCurRepeatTimes(0);
            plane->setTotalRepeatTimes(p_nextPathData->getTotalRepeatTimes());
            plane->setCurRepeatBeginID(p_nextPathData->getBID());
            plane->setCurRepeatEndID(p_nextPathData->getEID());
        }
        
        //增加循环次数
        plane->setCurRepeatTimes(plane->getCurRepeatTimes() + 1);
        
        if(plane->isMoveOver())
        {
            //循环结束就继续往下走
            plane->setCurPath(p_nextPathData->getEID() + 2);
            plane->setTotalRepeatTimes(0);
            plane->setCurRepeatTimes(0);
        }
        else
        {
            plane->setCurPath(plane->getCurRepeatBeginID());
        }
        
        n_next_path = plane->getCurPath();
        n_actiontype = plane->getDataPathGroup()->getPathData(getStringFromInt(n_next_path))->getType();
    }
    
    if(plane->getDataPathGroup()->getPathActionByID(getStringFromInt(n_next_path), plane))
    {
        //CCLog("n_next_path == %d   n_actiontype = %d", n_next_path,n_actiontype);
        CCSequence *seq = CCSequence::create(plane->getDataPathGroup()->getPathActionByID(getStringFromInt(n_next_path),
                                                                                          plane,
                                                                                          plane->getSpeed()),
                                             CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackNextPathAction)),
                                             NULL);
        
        if(plane->getDataPathGroup()->getPathAutoRotation(n_next_path))
        {
            float rot = 0;
            
            if(plane->getDataPathGroup()->getCurPathRotation(n_next_path, plane->getPosition(), rot))
            {
                plane->setRotation(rot);
            }
        }
        
        seq->setTag(1);
        plane->runAction(seq);
        plane->setCurPath(n_next_path);
    }
    //处理销毁行为
    else if(n_actiontype == ACTION_DIED)
    {
        CCSequence *seq = CCSequence::create(CCCallFuncN::create(this,
                                                                 callfuncN_selector(GameCombatLayer::callbackSetCraftRemove)),
                                             NULL);
        plane->runAction(seq);
        plane->setCurPath(n_next_path);
    }
}
//子弹轨迹
void GameCombatLayer::callBackNextTrailAction(CCNode* pSender)
{
    BulletView *bullet = (BulletView *)(pSender);
    int n_next_path = bullet->getCurTrail() + 1;
    
    int n_actiontype = ACTION_DIED;
    
    TrailGroupData *dpg = bullet->getDataTrailGroup();
    //获取动作类型
    PathData* p_nextPathData = dpg->getPathData(getStringFromInt(n_next_path));
    
    if(p_nextPathData)
    {
        n_actiontype = bullet->getDataTrailGroup()->getPathData(getStringFromInt(n_next_path))->getType();
    }
    else
    {
        n_actiontype = ACTION_STAYAT;
    }
    
    //重复动作
    if(n_actiontype == ACTION_REPEAT)
    {
        //循环次数走完了直接返回
        TrailGroupData *p_pathgroupdata = bullet->getDataTrailGroup();
        
        if(!p_pathgroupdata)
        {
            return;
        }
        
        //第一次进入循环，初始化本次循环的数据
        if(bullet->getCurRepeatTimes() == 0)
        {
            bullet->setCurRepeatTimes(0);
            bullet->setTotalRepeatTimes(p_nextPathData->getTotalRepeatTimes());
            bullet->setCurRepeatBeginID(p_nextPathData->getBID());
            bullet->setCurRepeatEndID(p_nextPathData->getEID());
        }
        
        //增加循环次数
        bullet->setCurRepeatTimes(bullet->getCurRepeatTimes() + 1);
        
        if(bullet->isMoveOver())
        {
            //循环结束就继续往下走
            bullet->setCurTrail(p_nextPathData->getEID() + 2);
            bullet->setTotalRepeatTimes(0);
            bullet->setCurRepeatTimes(0);
        }
        else
        {
            bullet->setCurTrail(bullet->getCurRepeatBeginID());
        }
        
        n_next_path = bullet->getCurTrail();
        n_actiontype = bullet->getDataTrailGroup()->getPathData(getStringFromInt(n_next_path))->getType();
    }
    
    if(bullet->getDataTrailGroup()->getTrailActionByID(getStringFromInt(n_next_path), bullet, bullet->getTrailGroupAngle()))
    {
        //CCLog("n_next_path == %d   n_actiontype = %d", n_next_path,n_actiontype);
        CCSequence *seq = CCSequence::create(bullet->getDataTrailGroup()->getTrailActionByID(getStringFromInt(n_next_path),
                                                                                             bullet,
                                                                                             bullet->getTrailGroupAngle(),
                                                                                             bullet->getSpeed()),
                                             CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackNextTrailAction)),
                                             NULL);
        
        if(bullet->getDataTrailGroup()->getPathAutoRotation(n_next_path))
        {
            float rot = 0;
            
            if(bullet->getDataTrailGroup()->getCurPathRotation(n_next_path, bullet->getPosition(), rot))
            {
                bullet->setRotation(rot);
            }
        }
        
        seq->setTag(1);
        bullet->runAction(seq);
        bullet->setCurTrail(n_next_path);
    }
    //处理销毁行为
    else if(n_actiontype == ACTION_DIED)
    {
        CCSequence *seq = CCSequence::create(CCCallFuncN::create(this,
                                                                 callfuncN_selector(GameCombatLayer::callbackSetBulletRemove)),
                                             NULL);
        bullet->runAction(seq);
        bullet->setCurTrail(n_next_path);
    }
    
    //自旋处理
    //设置子弹自转（敌机）
    if (bullet->getIsSelfRotate())
    {
        bullet->runSelfRotate();
    }
    
}
//随机并移动到该点
void GameCombatLayer::callBackRandAndMove(CCNode* pSender)
{
    CraftView* craft = (CraftView *)(pSender);
    
    //加速度
    //float g = 2.0f;
    //飞机现在的位置
    CCPoint posNow = craft->getPosition();
    //下一个目标点
    CCPoint posNext = craft->getNextAimPos();
    //第二个目标点
    CCPoint posSecond = craft->getSecondAimPos();
    
    //获取范围
    CCPoint max = getEnemyRandomMoveForMax(craft);
    float dw = max.x;
    float dh = max.y;
    CCPoint min = getEnemyRandomMoveForMin(craft);
    float minw = min.x;
    float minh = min.y;
    //速度从0到最大运行的距离
    float fMaxLen = getEnemyRandomMoveLen(craft);
    //飞机最大速度(单位是像素)
    float maxSpeed = getEnemyRandomMoveSpeed(craft);
    //中心点
    CCPoint center = getEnemyRandomMoveCenter(craft);
    
    //    if(posNext.x == 0.0f && posNext.y == 0.0f)
    //    {
    //        //第一次随机一个
    //        posNext = getRandomPositionByBound(center, minw, minh, dw, dh);
    //        craft->setNextAimPos(posNext);
    //    }
    //    else
    //    {
    //        //将第二个随机点赋值给第一个
    //        posNext = posSecond;
    //    }
    
    posNext = getRandomPositionByMinLen(center,posNow,dw,dh,fMaxLen);
    
    //第二个随机点重新赋值
    posSecond = getRandomPositionByBound(center, minw, minh, dw, dh);
    craft->setSecondAimPos(posSecond);
    
    CCSequence* seq = NULL;
    
    //计算与目标点的距离
    float fLength = calcTwoPointLength(posNow, posNext);
    
    //位移向量
    CCPoint offset = ccpSub(posNext, posNow);
    //第一段加速
    CCPoint vec_first = CCPointZero;
    
    //匀速的距离
    float temp_len = fLength - fMaxLen * 2;
    //匀速的时间
    float fTime = temp_len/maxSpeed;
    
    vec_first = ccp(offset.x * ((fMaxLen) / fLength), offset.y * ((fMaxLen) / fLength));
    CCPoint vec_second = ccp(offset.x * ((temp_len) / fLength), offset.y * ((temp_len) / fLength));
    
    seq = CCSequence::create(
                             JumpByAction::create(vec_first, true),
                             CCMoveBy::create(fTime, vec_second),
                             JumpByAction::create(vec_first, false),
                             CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callBackRandAndMove)),
                             NULL);
    
    craft->runAction(seq);
}

/************************
 * 移除
 ************************/
//清理已消除项目
void GameCombatLayer::updateRemove(float dt)
{
    
    //从敌机dict移除敌机
    CCDictElement *_ele = NULL;
    CCDICT_FOREACH(m_pDictEnemy, _ele)
    {
        EnemyView *plane = (EnemyView *)_ele->getObject();
        
        if(plane && plane->getIsDied())
        {
            if (plane->getIsBattery())
            {
                
                EnemyView *parent = (EnemyView*)m_pDictEnemy->objectForKey(plane->getHolderID());
                
                if(parent)
                {
                    parent->removeBatteryPlane(plane);
                    //parent->getBatteryPlanes()->removeObject(plane);
                }
            }
            else
            {
                m_pArrayTaskEnemy->removeObject(plane);
            }
            
            m_pDictEnemy->removeObjectForElememt(_ele);
        }
    }
    
    //金币移除
    for(int i = m_pArrayCoin->count() - 1; i >= 0; i--)
    {
        CoinView *coin = (CoinView *)m_pArrayCoin->objectAtIndex(i);
        
        if(coin && coin->getIsDied())
        {
            m_pArrayCoin->removeObject(coin);
        }
    }
    
    //普通道具移除
    for(int i = m_pArrayTool->count() - 1; i >= 0; i--)
    {
        ToolsView *tool = (ToolsView *)m_pArrayTool->objectAtIndex(i);
        
        if(tool && tool->getIsDied())
        {
            m_pArrayTool->removeObject(tool);
        }
    }
    
    //强化道具移除
    for(int i = m_pArrInvincibleTool->count() - 1; i >= 0; i--)
    {
        ToolsView *tool = (ToolsView *)m_pArrInvincibleTool->objectAtIndex(i);
        
        if(tool && tool->getIsDied())
        {
            m_pArrInvincibleTool->removeObject(tool);
        }
    }
    
    //血球移除
    for(int i = m_pArrayBlood->count() - 1; i >= 0; i--)
    {
        BloodView *blood = (BloodView*)m_pArrayBlood->objectAtIndex(i);
        
        if(blood && blood->getIsDied())
        {
            m_pArrayBlood->removeObject(blood);
        }
    }
    
    //积分移除
    for(int i = m_pArrayScore->count() - 1; i >= 0; i--)
    {
        ScoreView *score = (ScoreView*)m_pArrayScore->objectAtIndex(i);
        
        if(score && score->getIsDied())
        {
            m_pArrayScore->removeObject(score);
        }
    }
    
    //在主战机前面飞的道具移除
    for(int i = m_pArrayFlyTool->count() - 1; i >= 0; i--)
    {
        ToolsView *tool = (ToolsView *)m_pArrayFlyTool->objectAtIndex(i);
        
        if(tool && tool->getIsDied())
        {
            m_pArrayFlyTool->removeObject(tool);
        }
    }
    
    //道具飞机移除
    for(int i = m_pArrayToolPlane->count() - 1; i >= 0; i--)
    {
        EnemyView *enemy = (EnemyView *)m_pArrayToolPlane->objectAtIndex(i);
        
        if(enemy && enemy->getIsDied())
        {
            m_pArrayToolPlane->removeObject(enemy);
        }
    }
    
    //敌机子弹移除
    for(int i = m_pArrayEnemyBullet->count() - 1; i >= 0; i--)
    {
        BulletView *enemy_b = (BulletView *)m_pArrayEnemyBullet->objectAtIndex(i);
        
        if(enemy_b && enemy_b->getIsDied())
        {
            m_pArrayEnemyBullet->fastRemoveObjectAtIndex(i);
        }
    }
    
    //主机子弹移除
    for(int i = m_pArrayHeroBullet->count() - 1; i >= 0; i--)
    {
        BulletView *bullet = (BulletView *)m_pArrayHeroBullet->objectAtIndex(i);
        
        if(bullet && bullet->getIsDied())
        {
            m_pArrayHeroBullet->fastRemoveObjectAtIndex(i);
        }
    }
    
    //僚机子弹移除
    for(int i = m_pArrayWingBullet->count() - 1; i >= 0; i--)
    {
        BulletView *bullet = (BulletView *)m_pArrayWingBullet->objectAtIndex(i);
        
        if(bullet && bullet->getIsDied())
        {
            m_pArrayWingBullet->fastRemoveObjectAtIndex(i);
        }
    }
    
    //    //道具飞机子弹移除
    //    for(int i = m_pArrayToolBullets->count() - 1; i >= 0; i--)
    //    {
    //        BulletView *bullet = (BulletView *)m_pArrayToolBullets->objectAtIndex(i);
    //
    //        if(bullet && bullet->getIsDied())
    //        {
    //            m_pArrayToolBullets->fastRemoveObjectAtIndex(i);
    //
    ////            if (m_pArrayBolts->count() > 0)
    ////            {
    ////                //从容器中删除某一个闪电
    ////                m_pArrayBolts->fastRemoveObjectAtIndex(i);
    ////            }
    //
    //        }
    //    }
    
}
//飞机起飞回调
void GameCombatLayer::callbackSetCraftBegin(CCNode* pSender)
{
    CraftView *craft = (CraftView *)pSender;
    
    if(craft)
    {
        craft->setIsBeginFly(true);
    }
}
//飞机坠毁回调
void GameCombatLayer::callbackSetCraftRemove(CCNode* pSender)
{
    CraftView *craft = (CraftView *)pSender;
    
    if(craft)
    {
        this->craftCrashed(craft, 0, false);
    }
}
//子弹销毁
void GameCombatLayer::callbackSetBulletRemove(CCNode* pSender)
{
    BulletView* bullet = (BulletView*)pSender;
    
    if(bullet)
    {
        this->bulletCrashed(bullet, false);
    }
}

//即将移除
void GameCombatLayer::bulletRemoveCallBack(CCNode* pSender)
{
    BulletView *temp_bullet = (BulletView *)(pSender);
    m_pArrayRemove->addObject(temp_bullet);
    temp_bullet->setIsDied(true);
}
//放射型子弹的爆炸回调（发射一圈子弹）
void GameCombatLayer::callbackRadiateBulletBoom(CCNode* pSender)
{
    BulletView* pBullet = (BulletView*&)pSender;
    
    //消除父子弹之前获取必须的数据
    CCPoint posInit = pBullet->getPosition();
    float fHarm = pBullet->getHarm();
    float fOffsetAngle = pBullet->getOffsetAngle();
    float fSpeed = pBullet->getSpeed();
    int nChildID = pBullet->getChildBulletID();
    int nNum = 360.0f / fOffsetAngle;
    //把父子弹干掉
    bulletCrashed(pBullet, false);
    
    for(int i = 0; i < nNum; i++)
    {
        BulletData *bullet_data = DataManager::sharedDataManager()->getBulletData(nChildID);
        BulletView *bullet = BulletView::create(bullet_data);
        
        bullet->setHarm(fHarm);
        bullet->setPosition(posInit);
        bullet->setRotation(i * fOffsetAngle);
        
        CCPoint posMoveOffset = ccp(MAX_DISTANCE * cos(CC_DEGREES_TO_RADIANS(bullet->getRotation())),
                                    MAX_DISTANCE * sin(CC_DEGREES_TO_RADIANS(bullet->getRotation())));
        
        float time = MAX_DISTANCE / fSpeed;
        
        
        CCSequence* seq = CCSequence::create(CCMoveBy::create(time, posMoveOffset), NULL);
        
        bullet->runAction(seq);
        
        m_pEnemyBullets->addChild(bullet);
        m_pArrayEnemyBullet->addObject(bullet);
    }
}

//放射型子弹2爆炸回调（发射两个相对的子弹）
void GameCombatLayer::callbackRadiaeBulletType2Boom(CCNode* pSender)
{
    BulletView* pBullet = (BulletView*&)pSender;
    
    //消除父子弹之前获取必须的数据
    CCPoint posInit = pBullet->getPosition();
    float fHarm = pBullet->getHarm();
    float fOffsetAngle = 180.0f;
    float fSpeed = pBullet->getSpeed();
    int nChildID = pBullet->getChildBulletID();
    int nNum = 360.0f / fOffsetAngle;
    
    for(int i = 0; i < nNum; i++)
    {
        BulletData *bullet_data = DataManager::sharedDataManager()->getBulletData(nChildID);
        BulletView *bullet = BulletView::create(bullet_data);
        
        bullet->setHarm(fHarm);
        bullet->setPosition(posInit);
        bullet->setRotation(i * fOffsetAngle);
        
        CCPoint posMoveOffset = ccp(MAX_DISTANCE * cos(CC_DEGREES_TO_RADIANS(bullet->getRotation())),
                                    MAX_DISTANCE * sin(CC_DEGREES_TO_RADIANS(bullet->getRotation())));
        
        float time = MAX_DISTANCE / fSpeed;
        
        
        CCSequence* seq = CCSequence::create(CCMoveBy::create(time, posMoveOffset), NULL);
        
        bullet->runAction(seq);
        
        m_pEnemyBullets->addChild(bullet);
        m_pArrayEnemyBullet->addObject(bullet);
    }
    
}

/************************
	* TRACE BULLET
 ************************/
bool GameCombatLayer::getAimedCraftPosition(int aimed_craft_id, CCPoint &position)
{
    CraftView *craft = (CraftView *)m_pDictEnemy->objectForKey(aimed_craft_id);
    
    if(craft)
    {
        if(craft->getIsDied() || craft->checkOutOfScreen())
        {
            return false;
        }
        
        position = craft->getPosition();
        if(craft->getIsAffiliatedPlane())
        {
            position = CCPointApplyAffineTransform(craft->getParent()->getPosition(), craft->getCraftTransform());
        }
        //炮台
        if(craft->getIsBattery())
        {
            position = ccpAdd(craft->getPosition(), craft->getParent()->getPosition());
            
            if(!craft->getChildByTag(kTagAimedCraftRemark))
            {
                
                CCSprite* pMark = CCSprite::create("game/skill/aim_mark.png");
                pMark->setPosition(position);
                pMark->runAction(CCRepeatForever::create(CCRotateBy::create(5, 360)));
                pMark->setScale(0.5f);
                
                craft->addChild(pMark, 1000,kTagAimedCraftRemark);
                
            }
        }
        return true;
    }
    
    return false;
}

BulletView* GameCombatLayer::getNearestBullet()
{
    /*
     * 寻找距离僚机最近的敌机子弹
     *
     */
    
    bool found_nearest = false;
    int nearest_bullet_tag = 0;
    float distance = MAX_DISTANCE;
    
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(m_pArrayEnemyBullet, obj)
    {
        BulletView* bullet = (BulletView*)obj;
        
        if (bullet->getIsDied())
        {
            continue;
        }
        
        CCPoint pos = bullet->getPosition();
        
        float distance_current = ccpDistance(pos, m_pWingView->getPosition());
        
        if (distance_current < distance)
        {
            distance = distance_current;
            
            found_nearest = true;
            
            nearest_bullet_tag = m_pArrayEnemyBullet->indexOfObject(bullet);
        }
    }
    
    if (found_nearest)
    {
        BulletView* bullet = (BulletView*)m_pArrayEnemyBullet->objectAtIndex(nearest_bullet_tag);
        
        return bullet;
    }
    
    return NULL;
}

void GameCombatLayer::attackBullet()
{
    if (m_pWingView)
    {
        BulletView* bullet = getNearestBullet();
        
        if (bullet)
        {
            FerrisDrawLine* line = FerrisDrawLine::create(bullet->getPosition(), m_pWingView->getPosition());
            
            line->runAction(CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)));
            
            this->addChild(line,GAME_ZORDER_ENEMY_BULLET);
            
            this->bulletCrashed(bullet,false);
        }
    }
}

CCPoint GameCombatLayer::getAimedConnonPosition()
{
    CraftView* craft = getAimedConnon();
    
    CCDictElement* el = NULL;
    
    CCDICT_FOREACH(m_pDictEnemy, el)
    {
        CraftView* tem = (CraftView*)el->getObject();
        
        if (tem != craft)
        {
            if (tem->getChildByTag(kTagAimedCraftRemark))
            {
                tem->removeChildByTag(kTagAimedCraftRemark);
            }
        }
    }
    
    if (craft)
    {
        return ccpAdd(craft->getParent()->getPosition(), craft->getPosition());
    }
    else
    {
        return CCPointZero;
    }
}

CraftView* GameCombatLayer::getAimedConnon()
{
    bool found_nearest = false;
    int nearest_enemy_tag = 0;
    float distance = MAX_DISTANCE;
    
    CCDictElement *_ele = NULL;
    CCDICT_FOREACH(m_pDictEnemy, _ele)
    {
        CraftView *craft = (CraftView *)_ele->getObject();
        
        if(craft->getIsDied() || craft->checkOutOfScreen() || !craft->getIsBattery())
        {
            continue;
        }
        
        CCPoint position = ccpAdd(craft->getParent()->getPosition(), craft->getPosition()) ;
        
        float distance_current = calcTwoPointLength(position, getHeroPosition());
        
        if(distance_current < distance)
        {
            distance = distance_current;
            found_nearest = true;
            nearest_enemy_tag = _ele->getIntKey();
        }
    }
    
    if(found_nearest)
    {
        CraftView *craft = (CraftView *)m_pDictEnemy->objectForKey(nearest_enemy_tag);
        craft->setIsBullletAim(true);
        
        if (!craft->getChildByTag(kTagAimedCraftRemark))
        {
            
            CCSprite* pMark = CCSprite::create("game/skill/aim_mark.png");
            pMark->setPosition(CCPointZero);
            pMark->runAction(CCRepeatForever::create(CCRotateBy::create(5, 360)));
            pMark->setScale(0.5f);
            
            craft->addChild(pMark, 1000,kTagAimedCraftRemark);
        }
        
        return craft;
    }
    
    return NULL;
}


CraftView* GameCombatLayer::getAimedCraft()
{
    /*
     * 找到距离最近的飞机作为目标
     * 在每次创建子弹的时候 寻找最近的目标 如果子弹发射不是非常频繁的话 性能是没有问题的
     */
    
    bool found_nearest = false;
    int nearest_enemy_tag = 0;
    float distance = MAX_DISTANCE;
    
    CCDictElement *_ele = NULL;
    CCDICT_FOREACH(m_pDictEnemy, _ele)
    {
        CraftView *craft = (CraftView *)_ele->getObject();
        
        if(craft->getIsDied() || craft->checkOutOfScreen() || craft->getIsBattery())
        {
            continue;
        }
        
        CCPoint position = craft->getPosition();
        
        if(craft->getIsAffiliatedPlane())
        {
            position = CCPointApplyAffineTransform(craft->getParent()->getPosition(), craft->getCraftTransform());
        }
        
        float distance_current = calcTwoPointLength(position, /*craftHero->getPosition()*/CCPointZero);
        
        if(distance_current < distance)
        {
            distance = distance_current;
            found_nearest = true;
            nearest_enemy_tag = _ele->getIntKey();
        }
    }
    
    if(found_nearest)
    {
        CraftView *craft = (CraftView *)m_pDictEnemy->objectForKey(nearest_enemy_tag);
        craft->setIsBullletAim(true);
        
        return craft;
    }
    
    return NULL;
}

/******************碰撞*******************/
//所有碰撞
void GameCombatLayer::updateCollisions(float dt)
{
    if (gameOver)
    {
        return;
    }
    
    /* 主机 */
    //主角子弹和敌机的碰撞
    this->collisionHeroBulletsWithEnemys();
    //主机和敌机的碰撞
    //this->collisionHeroWithEnemys();
    //主机和金币的碰撞
    this->collsiionHeroWithCoin(0.0f);
    //主机和道具的碰撞
    this->collsiionHeroWithCommonTool();
    //主机和强化道具碰撞
    this->collsiionHeroWithInvinibleTool();
    //主机和血球碰撞
    this->collisionHeroWithBlood();
    //主机和积分碰撞
    this->collisionHeroWithScore();
    
    //主机检测碰撞
    this->collisionHeroPreCheck();
    
    /* 僚机 */
    this->collisionWingBulletsWithEnemys();
    
    /* 敌机 */
    //敌机子弹和主机的碰撞
    this->collisionEnemyBulletsWithHero();
    //敌机级敌机子弹与光波炮的碰撞
    this->collisionEnemyAndBulletsWithWaveGun();
    //敌机级敌机子弹与聚能炮的碰撞
    this->collisionEnemyWithShapedGun();
    //敌机子弹和斥力盾之间的碰撞
    this->collisionEnemyBulletsWithRepulsionShiled();
    
    /* 道具飞机 */
    this->collisionToolPlaneBulletsWithEnemys();
    
    /* 其他 */
    //无敌护罩道具与子弹碰撞
    this->collisionToolsInvincibleWithBullet();
    //移除东西的清理
    this->updateRemove(0.0f);
    
    /* 检测 */
    //检测子弹是否出界
    this->checkBulletsOutofScreen();
    //检测血球是否出界
    this->CheckBloodOutofScreen();
    //检测道具飞机的子弹是否全部死掉
    this->checkToolPlaneBulletofDied();
}
//主角子弹和敌机的碰撞
void GameCombatLayer::collisionHeroBulletsWithEnemys()
{
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(m_pDictEnemy, _ele)
    {
        EnemyView* pEnemy = (EnemyView*)_ele->getObject();
        
        //如果敌机存在 并且 敌机可以受击
        if(!pEnemy->getIsDied() && pEnemy->getIsCanInjured())
        {
            pEnemy->testAllBulletCollision(m_pArrayHeroBullet);
            
            if (pEnemy->getIsBattery())
            {
                //炮台自动转向
                batteryAutomaticRotate(pEnemy);
                
            }
        }
    }
    
}

//僚机子弹与敌机碰撞
void GameCombatLayer::collisionWingBulletsWithEnemys()
{
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(m_pDictEnemy, _ele)
    {
        EnemyView* pEnemy = (EnemyView*)_ele->getObject();
        
        if(!pEnemy->getIsDied())
        {
            pEnemy->testAllBulletCollision(m_pArrayWingBullet);
        }
    }
    
}

//道具飞机子弹与敌机碰撞
void GameCombatLayer::collisionToolPlaneBulletsWithEnemys()
{
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(m_pDictEnemy, _ele)
    {
        EnemyView* pEnemy = (EnemyView*)_ele->getObject();
        
        if(!pEnemy->getIsDied())
        {
            pEnemy->testAllBulletCollision(m_pArrayToolBullets);
        }
    }
}

//主机和敌机的碰撞
void GameCombatLayer::collisionHeroWithEnemys()
{
    m_pHeroView->testAllCraftCollision(m_pDictEnemy);
    
    m_battleUI->setHpPercent(m_pHeroView->getCurrentHP() / m_heroMaxHp * 100);
}

//主角吃金币
void GameCombatLayer::collsiionHeroWithCoin(float dt)
{
    
    if(!m_pArrayCoin)
    {
        return;
    }
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pArrayCoin, obj)
    {
        
        CoinView* coin = (CoinView*)obj;
        
        if (coin && !coin->getIsDied()) {
            
            if (coin->getCollisionRect(COLLISIONRECT_TYPE_COMMON).intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_SPECIAL)))
            {
                
                coin->setIsDied(true);
                coin->removeFromParent();
                
                m_nCoin++;
                
                if (b_tripleCoin)
                {
                    m_resultCountData->m_gold += 3;
                }
                else
                {
                    m_resultCountData->m_gold++;
                }
                
                //日常统计
                DailyCountData::sharedInstance()->addDailyGetGold();
                
                //游戏统计 累积获得金币
                GameCountData::sharedInstance()->addTotalGold(1);
                
                if (m_nCoin > DailyCountData::sharedInstance()->getOneMissionGetGold())
                {
                    DailyCountData::sharedInstance()->setOneMissionGetGold(m_nCoin);
                    
                    DailyCountData::sharedInstance()->hadAchieveComplete();
                }
                
                if (m_nCoin % 3 == 0)
                {
                    //获得金币音效
                    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_GET_DROP, EFF_GET_COIN);
                }
            }
            
        }
        
    }
    
}

//主角与道具的碰撞
void GameCombatLayer::collsiionHeroWithCommonTool()
{
    
    if(!m_pArrayTool)
    {
        return;
    }
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pArrayTool, obj)
    {
        ToolsView* tool = (ToolsView*)obj;
        
        if (tool && !tool->getIsDied())
        {
            
            if (tool->getCollisionRect(COLLISIONRECT_TYPE_COMMON).intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_SPECIAL)))
            {
                //获得道具音效
                SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_GET_TOOL, EFF_GET_TOOL);
                
                //道具删除
                this->craftCrashed(tool, 0, false);
                
                //创建一个缩小的道具实体加到hero上
                this->createToolWithHeroFront(tool->getToolType());
                
                //如果道具库已经满了 在吃到道具则直接释放掉
                if (m_pArrayFlyTool->count() > m_nToolGridNum)
                {
                    this->useTool(true);
                }
                
            }
            
        }
        
    }
}

//主机和强化道具碰撞
void GameCombatLayer::collsiionHeroWithInvinibleTool()
{
    if(!m_pArrInvincibleTool)
    {
        return;
    }
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pArrInvincibleTool, obj)
    {
        ToolsView* tool = (ToolsView*)obj;
        
        if (tool && !tool->getIsDied())
        {
            if (tool->getCollisionRect(COLLISIONRECT_TYPE_COMMON).intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_SPECIAL)))
            {
                //释放强化道具音效
                SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_USE_PROP, EFF_USE_TOOL_2);
                
                //释放粒子
                FerrisParticle* particle = FerrisParticle::create("game/particles/prop_kuosan.plist");
                particle->setPosition(tool->getPosition()+tool->getParent()->getPosition());
                particle->setAutoRemoveOnFinish(true);
                m_pParticleNode->addChild(particle);
                
                //道具删除
                this->craftCrashed(tool, 0, false);
                
                //直接释放
                this->useInvincibleTool();
                
                //装备升级提示
                CCSprite* upgrades = CCSprite::create("game/animations/sprite_weapon_upgrades.png");
                upgrades->setPosition(ccp(_s_center.x,_s_center.y - 100));
                this->addChild(upgrades,GAME_ZORDER_UI_COVER);
                
                upgrades->runAction(CCSequence::create(CCSpawn::createWithTwoActions(CCMoveBy::create(1.6f, ccp(0,200)), CCFadeOut::create(1.6f)),
                                                       CCCallFuncN::create(this,callfuncN_selector(GameCombatLayer::removeNode)),
                                                       NULL));
            }
            
        }
        
    }
}

void GameCombatLayer::collisionHeroWithBlood()
{
    
    if(!m_pArrayBlood)
    {
        return;
    }
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pArrayBlood, obj)
    {
        BloodView* blood = (BloodView*)obj;
        if (blood)
        {
            if (blood->boundingBox().intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_SPECIAL)))
            {
                
                //获得血球音效
                SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_GET_TOOL, EFF_GET_TOOL);
                
                //主机加血
                if (m_pHeroView->getCurrentHP() + blood->getCarryBlood() >= m_heroMaxHp)
                {
                    m_pHeroView->setCurrentHP(m_heroMaxHp);
                }
                else
                {
                    m_pHeroView->setCurrentHP(m_pHeroView->getCurrentHP() + blood->getCarryBlood());
                }
                
                //ui 刷新血条
                m_battleUI->setHpPercent(m_pHeroView->getCurrentHP() / m_heroMaxHp * 100);
                
                blood->setIsDied(true);
                blood->removeFromParent();
                
            }
        }
    }
    
}

//主机和积分碰撞
void GameCombatLayer::collisionHeroWithScore()
{
    
    if(!m_pArrayScore)
    {
        return;
    }
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pArrayScore, obj)
    {
        ScoreView* score = (ScoreView*)obj;
        if (score && !score->getIsDied())
        {
            if (score->getCollisionRect(COLLISIONRECT_TYPE_COMMON).intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_SPECIAL)))
            {
                //UI刷新积分
                m_battleUI->setScore(++m_resultCountData->m_score);
                
                score->setIsDied(true);
                score->removeFromParent();
                
                m_nScore++;
                
                if (m_nScore % 5 == 0)
                {
                    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_GET_DROP, EFF_GET_SCORE);
                }
                
            }
        }
    }
}

//主机检测碰撞
void GameCombatLayer::collisionHeroPreCheck()
{
    //检测普通道具
    CCObject* obj_tool = NULL;
    CCARRAY_FOREACH(m_pArrayTool, obj_tool)
    {
        ToolsView* tool = (ToolsView*)obj_tool;
        
        if (tool && !tool->getIsDied())
        {
            if (tool->getIsCollisionWithHero())
            {
                return;
            }
            
            if (tool->getCollisionRect(COLLISIONRECT_TYPE_COMMON).intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_CHECKTOOL)))
            {
                MagnetCoinAction* act = MagnetCoinAction::create(100, m_pHeroView->getPosition());
                act->setTag(TAG_SPEEDUP_ACTION);
                tool->runAction(act);
                
                CCSequence* seq = CCSequence::create(CCDelayTime::create(1.0f),
                                                     CCCallFuncN::create(this,callfuncN_selector(GameCombatLayer::callbackToolMagnet)),
                                                     NULL);
                
                tool->runAction(seq);
                
                tool->setIsCollisionWithHero(true);
            }
            
        }
        
    }
    
    //检测强化道具
    CCObject* obj_invincibletool = NULL;
    CCARRAY_FOREACH(m_pArrInvincibleTool, obj_invincibletool)
    {
        ToolsView* tool = (ToolsView*)obj_invincibletool;
        
        if (tool && !tool->getIsDied())
        {
            if (tool->getIsCollisionWithHero())
            {
                return;
            }
            
            if (tool->getCollisionRect(COLLISIONRECT_TYPE_COMMON).intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_CHECKTOOL)))
            {
                MagnetCoinAction* act = MagnetCoinAction::create(100, m_pHeroView->getPosition());
                act->setTag(TAG_SPEEDUP_ACTION);
                tool->runAction(act);
                
                CCSequence* seq = CCSequence::create(CCDelayTime::create(1.0f),
                                                     CCCallFuncN::create(this,callfuncN_selector(GameCombatLayer::callbackToolMagnet)),
                                                     NULL);
                
                tool->runAction(seq);
                
                tool->setIsCollisionWithHero(true);
            }
            
        }
        
    }
    
    //检测血球
    CCObject* obj_blood;
    CCARRAY_FOREACH(m_pArrayBlood, obj_blood)
    {
        BloodView* blood = (BloodView*)obj_blood;
        if (blood && !blood->getIsDied())
        {
            if (blood->getIsCollisionWithHero())
            {
                return;
            }
            
            if (blood->boundingBox().intersectsRect(m_pHeroView->getCollisionRect(COLLISIONRECT_TYPE_CHECKBLOOD)))
            {
                
                MagnetCoinAction* act = MagnetCoinAction::create(500, m_pHeroView->getPosition());
                act->setTag(TAG_SPEEDUP_ACTION);
                blood->runAction(act);
                
                blood->setIsCollisionWithHero(true);
            }
        }
    }
    
}

void GameCombatLayer::callbackToolMagnet(CCNode* node)
{
    ToolsView* tool= (ToolsView*)node;
    
    tool->stopActionByTag(TAG_SPEEDUP_ACTION);
    tool->setIsCollisionWithHero(false);
}

//敌机子弹和主机的碰撞
void GameCombatLayer::collisionEnemyBulletsWithHero()
{
    if (m_pHeroView->getIsInvincible())
    {
        return;
    }
    
    m_pHeroView->testAllBulletCollision(m_pArrayEnemyBullet);
    
    m_battleUI->setHpPercent(m_pHeroView->getCurrentHP() / m_heroMaxHp * 100);
}
//检测子弹是否出界
void GameCombatLayer::checkBulletsOutofScreen()
{
    //主机子弹
    for (int i =  0; i < m_pArrayHeroBullet->count(); i ++)
    {
        BulletView* pBullet = (BulletView*)m_pArrayHeroBullet->objectAtIndex(i);
        if(pBullet && !pBullet->getIsDied())
        {
            //如果是18号子弹
            if (getIntFromS(pBullet->getID().c_str()) == 18)
            {
                continue;
            }
            
            if(pBullet->outOfScreen())
            {
                pBullet->setIsDied(true);
                pBullet->removeFromParent();
            }
        }
    }
    
    //敌机子弹
    for (int i =  0; i < m_pArrayEnemyBullet->count(); i ++)
    {
        BulletView* pBullet = (BulletView*)m_pArrayEnemyBullet->objectAtIndex(i);
        if(pBullet && !pBullet->getIsDied())
        {
            if(pBullet->outOfScreen())
            {
                pBullet->setIsDied(true);
                pBullet->removeFromParent();
            }
        }
    }
    
    //僚机子弹
    for (int i =  0; i < m_pArrayWingBullet->count(); i ++)
    {
        BulletView* pBullet = (BulletView*)m_pArrayWingBullet->objectAtIndex(i);
        if(pBullet && !pBullet->getIsDied())
        {
            if(pBullet->outOfScreen())
            {
                pBullet->setIsDied(true);
                pBullet->removeFromParent();
            }
        }
    }
    
    //道具飞机子弹
    for (int i =  0; i < m_pArrayToolBullets->count(); i ++)
    {
        BulletView* pBullet = (BulletView*)m_pArrayToolBullets->objectAtIndex(i);
        if(pBullet && !pBullet->getIsDied())
        {
            if(pBullet->outOfScreen())
            {
                if(pBullet->getIsTakeLightning())
                {
                    pBullet->getM_LBNode()->removeFromParent();
                    pBullet->setIsTakeLightning(false);
                }
                
                pBullet->setIsDied(true);
                pBullet->removeFromParent();
            }
        }
    }
    
}
//敌机子弹和飞机与光波炮的碰撞
void GameCombatLayer::collisionEnemyAndBulletsWithWaveGun()
{
    CCSprite* pWaveGun = (CCSprite*)this->getChildByTag(kTagSkillWaveGun);
    if(pWaveGun)
    {
        CCRect rect = pWaveGun->boundingBox();
        //与敌机子弹的碰撞
        for (int i = 0; i < m_pArrayEnemyBullet->count(); i++)
        {
            BulletView* bullet = (BulletView*)m_pArrayEnemyBullet->objectAtIndex(i);
            
            CCRect bullet_rect = bullet->boundingBox();
            
            if(rect.intersectsRect(bullet_rect))
            {
                bulletCrashed(bullet, false);
            }
        }
        
        //与敌机的碰撞
        CCDictElement *_ele = NULL;
        CCDICT_FOREACH(m_pDictEnemy, _ele)
        {
            CraftView *craft = (CraftView *)_ele->getObject();
            
            if(!craft->getCollisionWithWaveGun())
            {
                //所有碰撞数据
                CollisionsData* pCollisionData1 = craft->dataCraft->getCollisionsData();
                //碰撞组数据
                CollisionGroupData* pGroup1 = (CollisionGroupData*)pCollisionData1->getCollisionGroupDataByKeys("1");
                
                CCDictionary* pDict1 = pGroup1->getDictCollisionGroupRects();
                
                CCDictElement* _ele1 = NULL;
                CCDICT_FOREACH(pDict1, _ele1)
                {
                    CollisionRectData* pRectData1 = (CollisionRectData*)_ele1->getObject();
                    
                    float x1 = pRectData1->getX();
                    float y1 = pRectData1->getY();
                    float w1 = pRectData1->getWidth();
                    float h1 = pRectData1->getHeight();
                    
                    
                    CCRect rEnemy1 = CCRectMake(x1 + craft->getPositionX(), y1 + craft->getPositionY(), w1, h1);
                    
                    EquipItem* item = UserEquipBag::shared()->getMainEquip();
                    
                    float base_harm = item->getAttack() * item->getLevel();
                    
                    base_harm = base_harm * (100 + item->getAttackAdd()) / 100.0;
                    
                    
                    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("2");
                    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
                    
                    //获取光波炮的伤害倍数
                    float f_multiple = pSkillLevelData->getAttrackMultiple();
                    
                    if(rect.intersectsRect(rEnemy1))
                    {
                        //减去子弹的伤害
                        craft->setCurrentHP(craft->getCurrentHP() - base_harm * f_multiple);
                        craft->setCollisionWithWaveGun(true);
                        craft->resumeAction();
                        //判断最终是否没血了
                        if(craft->getCurrentHP() <= 0)
                        {
                            this->craftCrashed(craft,0, true);
                        }
                    }
                }
            }
            
        }
        
    }
}

//敌机子弹和飞机与聚能炮的碰撞
void GameCombatLayer::collisionEnemyWithShapedGun()
{
    CCSprite* pShapedGun = (CCSprite*)this->getChildByTag(kTagSkillShapedGun);
    
    if(pShapedGun)
    {
        //与敌机的碰撞
        CCDictElement *_ele = NULL;
        bool ishit = false;
        CCDICT_FOREACH(m_pDictEnemy, _ele)
        {
            CraftView *craft = (CraftView *)_ele->getObject();
            
            if(!craft->getIsBattery())
            {
                //所有碰撞数据
                CollisionsData* pCollisionData1 = craft->dataCraft->getCollisionsData();
                //碰撞组数据
                CollisionGroupData* pGroup1 = (CollisionGroupData*)pCollisionData1->getCollisionGroupDataByKeys("1");
                
                //碰撞线  确定激光的长短
                CollisionLineData* line = pCollisionData1->getCollisionLineData();
                
                CCPoint tar_a = ccp(craft->getPositionX() + line->getLeftX(), craft->getPositionY() + line->getLeftY());
                
                CCPoint tar_b = ccp(craft->getPositionX() + line->getRightX(), craft->getPositionY() + line->getRightY());
                
                CCPoint laser_a = m_pHeroView->getPosition();
                
                CCPoint laser_b = ccp(m_pHeroView->getPositionX(), m_pHeroView->getPositionY() + _s_height);
                
                
                if(ccpSegmentIntersect(tar_a, tar_b, laser_a, laser_b))
                {
                    CCPoint cross = CCPointZero;
                    bool isCross = ccpSegmentIntersectPoint(tar_a, tar_b, laser_a, laser_b,cross);
                    
                    if(isCross)
                    {
                        float dis = ccpDistance(m_pHeroView->getPosition(), cross);
                        
                        pShapedGun->setScaleY((dis) / pShapedGun->getContentSize().height);
                        
                        ((SkillShapedGun*)m_pMainSkill)->addHitEffecct(cross);
                        
                        ishit = true;
                        
                        if(m_nShapedGunCurrentPhrase == 0)
                        {
                            craft->setCurrentHP(craft->getCurrentHP() - UserEquipBag::shared()->getTotalAttack() * 0.5f / 60.0f);
                            
                        }
                        else if(m_nShapedGunCurrentPhrase == 1)
                        {
                            craft->setCurrentHP(craft->getCurrentHP() - UserEquipBag::shared()->getTotalAttack() * 0.75f / 60.0f);
                        }
                        else if(m_nShapedGunCurrentPhrase > 1)
                        {
                            craft->setCurrentHP(craft->getCurrentHP() - UserEquipBag::shared()->getTotalAttack() / 60.0f);
                        }
                        
                        
                        //判断最终是否没血了
                        if(craft->getCurrentHP() <= 0)
                        {
                            this->craftCrashed(craft, 0, true);
                        }
                        
                    }
                }
            }
            
        }
        
        if(!ishit)
        {
            float dis = _s_height - m_pHeroView->getPositionY();
            
            pShapedGun->setScaleY((dis) / pShapedGun->getContentSize().height);
            ((SkillShapedGun*)m_pMainSkill)->removeHitEffecct();
        }
        
    }
    
}
//敌机子弹与斥力盾的碰撞
void GameCombatLayer::collisionEnemyBulletsWithRepulsionShiled()
{
    CCSprite* pShiled = (CCSprite*)this->getChildByTag(kTagSkillRepulsionShiled);
    if(pShiled)
    {
        CCRect rect = pShiled->boundingBox();
        //与敌机子弹的碰撞
        for (int i = 0; i < m_pArrayEnemyBullet->count(); i++)
        {
            BulletView* bullet = (BulletView*)m_pArrayEnemyBullet->objectAtIndex(i);
            
            CCRect bullet_rect = bullet->boundingBox();
            
            if(rect.intersectsRect(bullet_rect))
            {
                this->bulletCrashed(bullet, false);
            }
        }
    }
}
//无敌护罩道具与子弹碰撞
void GameCombatLayer::collisionToolsInvincibleWithBullet()
{
    //道具护盾
    CCSprite* pShiled_tool = (CCSprite*)m_pHeroView->getChildByTag(kTagToolInvincible);
    
    if (pShiled_tool) {
        
        float scale = pShiled_tool->getScale();
        CCRect rect = pShiled_tool->boundingBox();
        CCPoint pos = pShiled_tool->getParent()->getPosition();
        
        //圆形与矩形（子弹）碰撞
        for (int i = 0; i < m_pArrayEnemyBullet->count(); i++)
        {
            BulletView* bullet = (BulletView*)m_pArrayEnemyBullet->objectAtIndex(i);
            
            CCRect bullet_rect = bullet->boundingBox();
            
            bool isCollision = collision_RectWithCircle(bullet_rect,pos,(rect.size.width - 27 * scale) / 2);
            if (isCollision)
            {
                this->bulletCrashed(bullet, false);
            }
            
        }
    }
    
    //复活护盾
    CCSprite* pShiled_revive = (CCSprite*)m_pHeroView->getChildByTag(kTagReviveInvincible);
    
    if (pShiled_revive) {
        
        float scale = pShiled_revive->getScale();
        CCRect rect = pShiled_revive->boundingBox();
        CCPoint pos = pShiled_revive->getParent()->getPosition();
        
        //圆形与矩形（子弹）碰撞
        for (int i = 0; i < m_pArrayEnemyBullet->count(); i++)
        {
            BulletView* bullet = (BulletView*)m_pArrayEnemyBullet->objectAtIndex(i);
            
            CCRect bullet_rect = bullet->boundingBox();
            
            bool isCollision = collision_RectWithCircle(bullet_rect,pos,(rect.size.width - 27 * scale) / 2);
            if (isCollision)
            {
                this->bulletCrashed(bullet, false);
            }
            
        }
    }
    
}

/******************装备技能释放*******************/
//影遁技能
void GameCombatLayer::releaseShadowOfHiding()
{
    //主机设置新位置
    m_pHeroView->setPosition(m_posPrePoint);
    
}
void GameCombatLayer::calculateDurationOfSkill()
{
    //在原有位置处添加残影效果
    CCSprite* spr = CCSprite::create("game/hero/hero_0_1.png");
    spr->setRotation(90);
    spr->setOpacity(128);
    spr->setPosition(m_pPosOri);
    this->addChild(spr,GAME_ZORDER_HERO,kTagHidingShadowsSpr);
    
    //动作
    spr->runAction(CCSequence::create(CCDelayTime::create(1.5f),
                                      CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::removeNode)),
                                      NULL));
    
    SkillData* pSkillData = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID("4");
    SkillLevelData* pSkillLevelData = pSkillData->getSkillLevelDataByKeys("1");
    
    CCSequence* seq = CCSequence::create(CCDelayTime::create(pSkillLevelData->getLastTime()),
                                         CCCallFuncN::create(this, callfuncN_selector(GameCombatLayer::callbackHeroRecover)),
                                         NULL);
    this->runAction(seq);
}
void GameCombatLayer::callbackHeroRecover(CCNode* pSender)
{
    if (!m_bIsSkillRunning)
    {
        return;
    }
    
    m_bIsSkillRunning = false;
    
    m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING,true);
    
    setHeroAttrackState(true);
}

//技能更新
void GameCombatLayer::updateSkills(float dt)
{
    //更新主炮技能
    if (m_pMainSkill)
    {
        m_pMainSkill->update(dt);
    }
    
    
    if (m_pWingSkill)
    {
        m_pWingSkill->update(dt);
    }
}
//技能CD回复回调
void GameCombatLayer::callBackEnableSkill(CCNode* pSender)
{
    m_bSkillEnable = true;
    this->getScheduler()->setTimeScale(1.0f);
}

//技能CD回复回调(定位狙击)
void GameCombatLayer::callBackEnableTraceBullet(CCNode* pSender)
{
    //    m_bTraceBulletEnable = true;
    
    CCDictElement* elm = NULL;
    
    CCDICT_FOREACH(m_pDictEnemy, elm)
    {
        CraftView* craft = (CraftView*)elm->getObject();
        
        if (craft->getChildByTag(kTagAimedCraftRemark))
        {
            craft->removeChildByTag(kTagAimedCraftRemark);
        }
    }
    
    m_pHeroView->playAnimationFromSkill(MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING,true);
}

/******************主机操作*******************/
void GameCombatLayer::updateHeroPosition(CCPoint new_position)
{
    float left_x = m_pHeroView->getContentSize().width / 2 - m_pHeroView->getContentSize().width / 4;
    float right_x = _s_width - m_pHeroView->getContentSize().width / 2 + m_pHeroView->getContentSize().width / 4;
    float top_y = _s_height - m_pHeroView->getContentSize().height / 2;
    float bottom_y = m_pHeroView->getContentSize().height / 2 - m_pHeroView->getContentSize().height / 4;
    
    if(new_position.x < left_x)
    {
        new_position.x = left_x;
    }
    else if(new_position.x > right_x)
    {
        new_position.x = right_x;
    }
    
    
    if(new_position.y < bottom_y)
    {
        new_position.y = bottom_y;
    }
    else if(new_position.y > top_y)
    {
        new_position.y = top_y;
    }
    
    m_pHeroView->setPosition(new_position);
    
    //更新拖尾位置
    if (this->getChildByTag(TAG_HERO_HEAD))
    {
        FerrisMotionStreak* streak = (FerrisMotionStreak*)this->getChildByTag(TAG_HERO_HEAD);
        streak->setPosition(ccpAdd(m_pHeroView->getPosition(), ccp(0,POSY_HERO_HEAD_STREAK)));
    }
    
}

void GameCombatLayer::enemyFly()
{
    isEnemyFly = true;
}

//炮台自动转向
void GameCombatLayer::batteryAutomaticRotate(CraftView* craft)
{
    //转向首先判断炮台是否死亡
    if(craft && craft->getParent() && !craft->getIsDied() && !craft->getIsCreatingBullet() && craft->getIsCanRotate())
    {
        //这里设置虚拟的角速度
        float fSpeedRot = craft->getBatteryRotateSpeed() / CCDirector::sharedDirector()->getFrameRate();
        //获取炮台和主角在世界坐标系的绝对位置
        CCPoint posHero = m_pHeroView->getPosition();
        CCPoint posBattery = craft->getPosition() + craft->getParent()->getPosition();
        
        //计算主角相对于炮台所需要旋转的角度（0---360 前闭后开区间）
        float fRot_aim = 180 - calcTwoPointAngle(posBattery, posHero);
        
        //不让角度离开计算范围
        if(craft->getRotation() < 0.0f)
        {
            craft->setRotation(craft->getRotation() + 360.0f);
        }
        else if(craft->getRotation() >= 360.0f)
        {
            craft->setRotation(craft->getRotation() - 360.0f);
        }
        
        float fRot_now = craft->getRotation();
        
        //消除角速度以内的像素误差
        if(fRot_now > fRot_aim && fRot_now - fRot_aim >= fSpeedRot)
        {
            //在一周的圆圈中，看顺逆时针哪个比较近
            if(fRot_now - fRot_aim <= 180)
            {
                craft->setRotation(fRot_now - fSpeedRot);
            }
            else
            {
                craft->setRotation(fRot_now + fSpeedRot);
            }
        }
        else if(fRot_now < fRot_aim && fRot_aim - fRot_now >= fSpeedRot)
        {
            //在一周的圆圈中，看顺逆时针哪个比较近
            if(fRot_aim - fRot_now <= 180)
            {
                craft->setRotation(fRot_now + fSpeedRot);
            }
            else
            {
                craft->setRotation(fRot_now - fSpeedRot);
            }
        }
        //在误差之内就不转角了
        else
        {
            
        }
    }
}
