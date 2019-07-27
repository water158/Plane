//
//  GameMenuLayer.cpp
//  ambition
//
//  Created by mini01 on 14-11-18.
//
//
#include "GameMainLayer.h"

GameMainLayer::GameMainLayer()
{
    node_equipDetail = NULL;
    node_equipGroup = NULL;
    
    nPage = 0;
    
    m_selectEquip = "0";
    
    m_bCanChange = false;
    
    m_bCanKickEquip = true;
}

GameMainLayer::~GameMainLayer()
{
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("game/animations/effect_upgrade.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("ui/icon/equip/equip_icon.plist");
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("ui/effect/change_effect.plist");
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    NDKHelper::RemoveSelectorsInGroup(EXITGAME_GROUP_NAME);
}


void GameMainLayer::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    // register 'parent' nodes first
    // since events are propagated in reverse order
    if (m_bTouchEnabled)
    {
        this->registerWithTouchDispatcher();
    }
    
    // then iterate over all the children
    CCNode::onEnter();
    
    // add this layer to concern the Accelerometer Sensor
    if (m_bAccelerometerEnabled)
    {
        pDirector->getAccelerometer()->setDelegate(this);
    }
    
    // add this layer to concern the keypad msg
    if (m_bKeypadEnabled)
    {
        pDirector->getKeypadDispatcher()->addDelegate(this);
    }

    refreshGoldInfo(NULL);
    
    refreshEquipBagNoSort();
    
    refreshBtnAchieveInfo();
    
    refreshSigninEffect();
}


GameMainLayer* GameMainLayer::create(bool hasAnimation)
{
    GameMainLayer* menulayer = new GameMainLayer();
    if (menulayer && menulayer->init(hasAnimation))
    {
        menulayer->autorelease();
        
        return menulayer;
    }
    
    return NULL;
}

bool GameMainLayer::init(bool hasAnimation)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->scheduleUpdate();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    if (hasAnimation)
    {
        GameStartLayer* layer = GameStartLayer::create();
        
        addChild(layer,2);
    }
    
    //添加观察者处理金币变化
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameMainLayer::refreshGoldInfo), MSG_REFRESHGOLD, NULL);

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/animations/effect_upgrade.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/equip/equip_icon.plist");
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/effect/change_effect.plist");
    

    Layout* root_commonMenu = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/tools_layer.ExportJson"));
    m_pUILayer->addWidget(root_commonMenu);
    

    
    int interval = time(0) - UserData::shared()->getTimeRecord();
    CCLog("UserData::shared()->getTimeRecord() : %d",UserData::shared()->getTimeRecord());
    CCLog("interval : %ld",time(0) - UserData::shared()->getTimeRecord());
    
    //如果时间大于记录时间 50分钟以上
    
    if (UserData::shared()->getEnergyNum() < 10)
    {
        if (interval > 10 * 5 * 60)
        {
            UserData::shared()->setEnergyNum(10);
        }
        else
        {
            if (interval / (5 * 60) > 0)
            {
                //如果此段时间恢复的体力大于0 更新时间记录
                UserData::shared()->setTimeRecord(time(0));
            }
            
            int energy = UserData::shared()->getEnergyNum();
            
            CCLog("energy is : %d",energy);
            
            energy += interval / (5 * 60);
            
            CCLog("energy is : %d",energy);
            
            if (energy > 10)
            {
                energy = 10;
            }
            else
            {
                //如果体力不满  开启定时器  恢复体力
                m_currertTime = 300 - interval % 300;
                
                this->schedule(schedule_selector(GameMainLayer::updateEnergy), 1.0f);
            }
            
            UserData::shared()->setEnergyNum(energy);
        }
        UserData::shared()->save();
    }
    
    
    //初始化金币、体力和钻石
    CCLog("UserData::shared()->getEnergyNum() : %d",UserData::shared()->getEnergyNum());
    LabelAtlas* label_energy = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_energy"));
    label_energy->setStringValue(getStringFromInt(UserData::shared()->getEnergyNum()));
    
    CCLog("UserData::shared()->getGoldNum() : %d",UserData::shared()->getGoldNum());
    LabelAtlas* label_gold = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gold"));
    label_gold->setStringValue(getStringFromInt(UserData::shared()->getGoldNum()));
    
    CCLog("UserData::shared()->getGemNum() : %d",UserData::shared()->getGemNum());
    LabelAtlas* label_gem = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gem"));
    label_gem->setStringValue(getStringFromInt(UserData::shared()->getGemNum()));
    
    //购买按钮
    Button* btn_buyPower = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_buypower"));
    btn_buyPower->addTouchEventListener(this, toucheventselector(GameMainLayer::btnBuyCallBack));
    btn_buyPower->setTag(kTagBuyPower);
    
    Button* btn_buyGold = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_buygold"));
    btn_buyGold->addTouchEventListener(this, toucheventselector(GameMainLayer::btnBuyCallBack));
    btn_buyGold->setTag(kTagBuyGold);
    
    Button* btn_buyGem = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_buygem"));
    btn_buyGem->addTouchEventListener(this, toucheventselector(GameMainLayer::btnBuyCallBack));
    btn_buyGem->setTag(kTagBuyGem);
    
    
    //左侧按钮
    btn_left = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_left"));
    btn_left->addTouchEventListener(this, toucheventselector(GameMainLayer::btnLeftCallBack));
    
    //右侧按钮
    btn_right = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_right"));
    btn_right->addTouchEventListener(this, toucheventselector(GameMainLayer::btnRightCallBack));
    
    
    UIPanel* btn_lottery = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("btn_lottery"));
    btn_lottery->setTouchEnabled(true);
    btn_lottery->addTouchEventListener(this, toucheventselector(GameMainLayer::btnLotteryCallBack));
    
    
    //签到按钮
    Button* btn_signIn = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_signin"));
    btn_signIn->addTouchEventListener(this, toucheventselector(GameMainLayer::btnSignInCallBack));
    
    //成就按钮
    Button* btn_achieve = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_achieve"));
    btn_achieve->addTouchEventListener(this, toucheventselector(GameMainLayer::btnAchieveCallBack));
    
    //设置按钮
    Button* btn_configure = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_setting"));
    btn_configure->addTouchEventListener(this, toucheventselector(GameMainLayer::btnConfigureCallBack));
    
    //滑动页
    m_pPageView = static_cast<PageView*>(m_pUILayer->getWidgetByName("page_tools"));
    m_pPageView->addEventListenerPageView(this, pagevieweventselector(GameMainLayer::pageViewEvent));
    
    m_pPageView->setTouchEnabled(false);
    
    m_pPageView->getPage(0)->setTouchEnabled(false);
    m_pPageView->getPage(1)->setTouchEnabled(false);
    
    ImageView* pageviewBg_1 = ImageView::create();
    pageviewBg_1->loadTexture("ui/panel/bg_mainmenu1.png");
    
    float scale = CCDirector::sharedDirector()->getVisibleSize().width / 540.0 + 0.5f;
    
    //pageviewBg_1->setScale(scale);
    
    m_pPageView->addWidgetToPage(pageviewBg_1, 0, false);
    pageviewBg_1->setZOrder(-10);
    
    ImageView* pageviewBg_2 = ImageView::create();
    pageviewBg_2->loadTexture("ui/panel/bg_mainmenu2.jpg");
    //m_pPageView->addWidgetToPage(pageviewBg_2, 1, false);
    pageviewBg_2->setZOrder(-10);
    
    Layout* layer_animbg = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/equipgroup_animation.ExportJson"));
    
    m_pPageView->addWidgetToPage(layer_animbg, 1, false);
    
    ActionManager::shareManager()->playActionByName("equipgroup_animation.ExportJson", "Animation0");
    
    
    ActionManager::shareManager()->playActionByName("tools_layer.ExportJson", "Animation3");
    
    layer_animbg->setTouchEnabled(false);
    
    //初始化添加装备组节点
    equipGroupInit(false);
    
    initEquipBag();
    
    //更新界面装备属性
    refreshAttributeShow();
    
    //初始化道具UI
    initToolsUI();
    
    //添加4个主装备panel的点击属性
    UIPanel* panel_maingun = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_main"));
    
    panel_maingun->setTouchEnabled(true);
    
    panel_maingun->setName("panel_main");
    
    panel_maingun->addTouchEventListener(this, toucheventselector(GameMainLayer::attributePanelCallBack));
    
    UIPanel* panel_sidegun1 = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_side_1"));
    
    panel_sidegun1->setTouchEnabled(true);
    
    panel_sidegun1->setName("panel_side_1");
    
    panel_sidegun1->addTouchEventListener(this, toucheventselector(GameMainLayer::attributePanelCallBack));
    
    UIPanel* panel_sidegun2 = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_side_2"));
    
    panel_sidegun2->setTouchEnabled(true);
    
    panel_sidegun2->setName("panel_side_2");
    
    panel_sidegun2->addTouchEventListener(this, toucheventselector(GameMainLayer::attributePanelCallBack));
    
    UIPanel* panel_wing = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_wing"));
    
    panel_wing->setTouchEnabled(true);
    
    panel_wing->setName("panel_wing");
    
    panel_wing->addTouchEventListener(this, toucheventselector(GameMainLayer::attributePanelCallBack));
    
    
    //抽奖按钮的效果动画
    ActionManager::shareManager()->playActionByName("tools_layer.ExportJson", "Animation0");
    
    
    //main背景音乐
    SoundEffectManager::shared()->setBackgroundMiscFileName(MIS_MAIN_BG);
    if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        SoundEffectManager::shared()->requestPlayBackgroundMisc(MIS_MAIN_BG);
    }
    
    this->setKeypadEnabled(true);
    
    return true;
}

/******************道具********************/
void GameMainLayer::initToolsUI()
{
    //更新道具数量
    refreshToolsNum();
    
    //通过按钮名字获取按钮并添加点击回调处理 和设置tag
    for (int i = 1; i <= 6; i++)
    {
        string btn_name = "btn_tool_";
        
        btn_name.append(getStringFromInt(i));
        
        UIPanel* btn = static_cast<UIPanel*>(m_pUILayer->getWidgetByName(btn_name.c_str()));
        
        btn->setTag(i);
        
        btn->setTouchEnabled(true);
        
        btn->addTouchEventListener(this, toucheventselector(GameMainLayer::buyToolCallBack));
    }
    
}

void GameMainLayer::buyToolCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        if (this->getChildByTag(kTagEffectSelectPanel))
        {
            this->removeChildByTag(kTagEffectSelectPanel);
        }
        
        UIPanel* btn = (UIPanel*)pSender;
        
        int btn_tag = btn->getTag();
        
        //创建升级帧动画
        
        CCArray* arrFrame = CCArray::createWithCapacity(6);
        
        char str[100] = {0};
        
        for (int i = 1; i < 7; i++)
        {
            sprintf(str, "upgrade_%02d.png",i);
            
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
            
            arrFrame->addObject(frame);
        }
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(arrFrame , 0.1f);
        
        animation->setLoops(1);
        
        CCAnimate* animate = CCAnimate::create(animation);
        
        
        
        CCSprite* pSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("upgrade_01.png"));
        
        CCString* s_panelName = CCString::createWithFormat("img_%d",btn_tag);
        
        ImageView* icon = static_cast<ImageView*>(m_pUILayer->getWidgetByName(s_panelName->getCString()));
        
        
        icon->addNode(pSprite);
        
        pSprite->runAction(CCSequence::create(animate,CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::upgradeActionFinish)),NULL));
        
        int price = 0;
        
        if (btn_tag == 6)
        {
            Label* label = static_cast<Label*>(m_pUILayer->getWidgetByName("label_tools_gem_1"));
            
            price = atoi(label->getStringValue());
            
            CCLog("price : %d",price);
            
            if (UserData::shared()->isGemEnough(price))
            {
                
                GameCountData::sharedInstance()->addConsumeGem(price);
                
                Achieve::getInstance()->save();
                
                UserData::shared()->decreaseGemNum(price);
                
                UserData::shared()->increaseTool(6,1);
                
                UserData::shared()->save();
                
                refreshToolsNum();
                
                GetEquipLayer* layer = GetEquipLayer::createWithGift(kSignReward_tool_6);
                
                this->addChild(layer, KZOrderDialog);
            }
            else //处理钻石不够时的处理
            {
                GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
                
                return;
            }
        }
        else
        {
            string label_name = "label_tools_gold_";
            
            label_name.append(getStringFromInt(btn_tag));
            
            Label* label = static_cast<Label*>(m_pUILayer->getWidgetByName(label_name.c_str()));
            
            price = atoi(label->getStringValue());
            
            CCLog("price : %d",price);
            
            if (UserData::shared()->isGoldEnough(price))
            {
                UserData::shared()->decreaseGoldNum(price);
                
                UserData::shared()->increaseTool(btn_tag,1);
                
                UserData::shared()->save();
                
                refreshToolsNum();
                
                GetEquipLayer* layer = GetEquipLayer::createWithGift((SignRewardType)(btn_tag+7));
                
                this->addChild(layer,KZOrderDialog);
            }
            else  //金币不够时弹出购买金币界面
            {
                GameDelegate::shared()->openSceneStoreLayer(kStore_gold);
            }
        }
        
        refreshGoldInfo(NULL);
    }
}


void GameMainLayer::refreshToolsNum()
{
    //更新道具的数量
    for (int i = 1; i <= 6; i++)
    {
        string tool_id = "label_tools_number_";
        
        tool_id.append(getStringFromInt(i));
        
        Label* label_num = static_cast<Label*>(m_pUILayer->getWidgetByName(tool_id.c_str()));
        
        if (label_num)
        {
            label_num->setText(getStringFromInt(UserData::shared()->getToolNum(i)));
        }
    }
}

void GameMainLayer::updateEnergy(float dt)
{
    m_currertTime--;
    if (m_currertTime == 0)
    {
        int energy = UserData::shared()->getEnergyNum();
        
        energy += 1;
        
        UserData::shared()->setTimeRecord(time(0));
        
        if (energy >= 10)
        {
            energy = 10;
            this->unschedule(schedule_selector(GameMainLayer::updateEnergy));
        }
        else
        {
            m_currertTime = 300;
        }
        
        UserData::shared()->setEnergyNum(energy);
        UserData::shared()->save();
        
        refreshGoldInfo(NULL);
    }
}

void GameMainLayer::attributePanelCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (!m_bCanKickEquip)
    {
        return;
    }
    
    if (type == TOUCH_EVENT_ENDED)
    {
        unSelectEquip();
        
        if (m_bCanChange)
        {
            //被替换的装备id
            string isChanged = "0";
            
            UIPanel* panel = (UIPanel*)pSender;
            
            int pos = 0;
            
            if (compareString(panel->getName(), "panel_main"))
            {
                isChanged = UserEquip::shared()->getMainGunID();
                
                pos = EQUIPMENT_TYPE_MAIN_GUN;
            }
            else if (compareString(panel->getName(), "panel_side_1"))
            {
                isChanged = UserEquip::shared()->getFirSubGunID();
                
                pos = EQUIPMENT_TYPE_LEFT_GUN;
            }
            else if (compareString(panel->getName(), "panel_side_2"))
            {
                isChanged = UserEquip::shared()->getSecSubGunID();
                
                pos = EQUIPMENT_TYPE_RIGHT_GUN;
            }
            else if (compareString(panel->getName(), "panel_wing"))
            {
                isChanged = UserEquip::shared()->getWingID();
                
                pos = EQUIPMENT_TYPE_WING;
            }
            
            if (compareString(isChanged.c_str(), "0"))
            {
                UserEquipBag::shared()->dressEquipToPos(m_selectEquip, pos);
            }
            else
            {
                CCLog("m_selectEquip: %s, isChanged : %s",m_selectEquip.c_str(), isChanged.c_str());
                
                UserEquipBag::shared()->swapEquip(m_selectEquip, isChanged);
            }
            
            //关闭装备详细界面
            closeEquipDetailCallBack(NULL, TOUCH_EVENT_ENDED);
            
            //更新主界面属性显示
            refreshAttributeShow();
            
            //更新背包
            refreshEquipBagAndSort();
            
            m_bCanChange = false;
        }
        else
        {
            
            
            //如果未选择装备
            UIPanel* panel = (UIPanel*)pSender;
            
            CCSprite* effect_select = CCSprite::create("ui/effect/effect_selectPanel.png");
            
            if (compareString(panel->getName(), "panel_main"))
            {
                m_selectEquip = UserEquip::shared()->getMainGunID();
                
                effect_select->setPosition(ccp(_s_width - 109, 657));
            }
            else if (compareString(panel->getName(), "panel_side_1"))
            {
                m_selectEquip = UserEquip::shared()->getFirSubGunID();
                
                effect_select->setPosition(ccp(_s_width - 109, 525));
            }
            else if (compareString(panel->getName(), "panel_side_2"))
            {
                m_selectEquip = UserEquip::shared()->getSecSubGunID();
                
                effect_select->setPosition(ccp(_s_width - 109, 393));
            }
            else if (compareString(panel->getName(), "panel_wing"))
            {
                m_selectEquip = UserEquip::shared()->getWingID();
                
                effect_select->setPosition(ccp(_s_width - 109, 266));
            }
            
            if (compareString(m_selectEquip.c_str(), "0"))
            {

                //如果选择的panel代表的位置没有装备
                return;

            }
            else
            {
                CCLog("m_selectEquip : %s",m_selectEquip.c_str());
                
                if (node_equipDetail)
                {
                    refreshEquipDetail();
                }
                
                if (node_equipGroup)
                {
                    m_bCanKickEquip = false;
                    
                    node_equipGroup->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-500, 0)),CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::actionfinish)),NULL));
                }
                
                effect_select->setScale(0.8f);
                
                this->addChild(effect_select, kZOrderEffectSelect, kTagEffectSelectPanel);
                
                CCScaleTo* scaleto = CCScaleTo::create(0.5f, 1.0f);
                
                CCFadeTo* fadeto = CCFadeTo::create(0.5f, 0);
                
                CCSequence* seq = CCSequence::create(scaleto,fadeto,CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::actionSelectFinish)),NULL);
                
                CCRepeatForever* repeat = CCRepeatForever::create(seq);
                
                effect_select->runAction(repeat);
            }
        }
        
        if (node_equipGroup)
        {
            m_bCanKickEquip = false;
            
            node_equipGroup->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-500, 0)),CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::actionfinish)),NULL));
        }
    }
}

//选中动作完成回调
void GameMainLayer::actionSelectFinish(CCObject* pSender)
{
    CCSprite* spr = (CCSprite*)pSender;
    
    spr->setScale(0.8f);
    
    spr->setOpacity(255);
}

void GameMainLayer::changeEquipCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        m_bCanChange = true;
        
        unSelectEquip();
        
        ActionManager::shareManager()->playActionByName("equipdetail_node.ExportJson", "Animation0");
        
        //ActionManager::shareManager()->playActionByName("tools_layer.ExportJson", "Animation4");
        
        //添加4个主装备panel的点击属性
        UIPanel* panel_maingun = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_main"));
        
        //panel_maingun->addNode(addEffectForChange(panel_maingun));
        addEffectForChange(panel_maingun);
        
        UIPanel* panel_sidegun1 = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_side_1"));
        
        //panel_sidegun1->addNode(addEffectForChange(panel_sidegun1));
        addEffectForChange(panel_sidegun1);
        
        UIPanel* panel_sidegun2 = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_side_2"));
        
        //panel_sidegun2->addNode(addEffectForChange(panel_sidegun2));
        addEffectForChange(panel_sidegun2);
        
        UIPanel* panel_wing = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_wing"));
        
        //panel_wing->addNode(addEffectForChange(panel_wing));
        addEffectForChange(panel_wing);

    }
}

CCSprite* GameMainLayer::addEffectForChange(CCObject* obj)
{
    
    if (this->getChildByTag(kTagEffectSelectPanel))
    {
        this->removeChildByTag(kTagEffectSelectPanel);
    }
    
    UIPanel* panel = (UIPanel*)obj;
    
    if (panel)
    {
        CCSprite* spr_in = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("change_effect_1.png"));
        
        CCSprite* spr_out = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("change_effect_2.png"));
        
        spr_out->runAction(CCRepeatForever::create(
                                                   CCSequence::create(CCFadeOut::create(1.0f),
                                                                      CCFadeIn::create(1.0f),
                                                                      NULL)));
        
        spr_in->runAction(CCRepeatForever::create(
                                                  CCSequence::create(CCFadeOut::create(1.0f),
                                                                     CCFadeIn::create(1.0f),
                                                                     NULL)));
        
        spr_in->setAnchorPoint(CCPointZero);
        spr_out->setAnchorPoint(CCPointZero);
        
        if (compareString(panel->getName(), "panel_wing"))
        {
            panel->addNode(spr_in,0,kTagEffectChange);
        }
        else
        {
            panel->addNode(spr_out,0,kTagEffectChange);
        }
        
    }
    
    return NULL;
    
}


void GameMainLayer::pageViewEvent(cocos2d::CCObject *pSender, PageViewEventType type)
{
    
    UIPageView* pageView = dynamic_cast<UIPageView*>(pSender);
    
    if (type == PAGEVIEW_EVENT_TURNING)
    {
        setNPage(pageView->getCurPageIndex());
        refreshButtonState(NULL);
    }
}

void GameMainLayer::btnAchieveCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_HEAVY);
        
        //成就界面
        GameDelegate::shared()->openSceneAchieve();
    }
}

void GameMainLayer::equipDetailInit(bool runAction)
{
    
    if (this->getChildByTag(kTagEffectOverEquip))
    {
        this->removeChildByTag(kTagEffectOverEquip);
    }
    
    //如果存在装备详细界面时将装备详细界面弹出后 再加载装备组界面
    
    node_equipDetail = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/equipdetail_node.ExportJson"));
    
    m_pPageView->addWidgetToPage(node_equipDetail, 1, false);
    node_equipDetail->setZOrder(10);
    
    node_equipDetail->setTouchEnabled(false);
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    
    btn_close->addTouchEventListener(this, toucheventselector(GameMainLayer::closeEquipDetailCallBack));
    
    Button* btn_change = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_change"));
    
    btn_change->addTouchEventListener(this, toucheventselector(GameMainLayer::changeEquipCallBack));
    
    //初始化装备详细界面的信息
    EquipItem* curr_equip = UserEquipBag::shared()->getEquipById(m_selectEquip);
    
    if (!curr_equip)
    {
        CCLog("Equip id : %s",m_selectEquip.c_str());
        
        return;
    }
    
    //装备图标
    ImageView* icon = static_cast<ImageView*>(m_pUILayer->getWidgetByName("icon_equip"));
    
    icon->loadTexture(UserEquipBag::shared()->getIconPath(curr_equip).c_str(),UI_TEX_TYPE_PLIST);
    
    //装备名称
    Label* label_name = static_cast<Label*>(m_pUILayer->getWidgetByName("label_name"));
    
    CCLog("curr_equip->getName() : %s",curr_equip->getName().c_str());

    label_name->setText(curr_equip->getName());
    
    //装备类型
    Label* label_type = static_cast<Label*>(m_pUILayer->getWidgetByName("label_style"));
    
    label_type->setText(curr_equip->getTypeText());
    
    //装备技能
    Label* label_skill = static_cast<Label*>(m_pUILayer->getWidgetByName("label_skill"));
    
    label_skill->setText(curr_equip->getSkillText());
   
    //装备描述 描述未处理
    Label* label_des = static_cast<Label*>(m_pUILayer->getWidgetByName("label_dic"));
    
    Label* label_skillstyle = static_cast<Label*>(m_pUILayer->getWidgetByName("label_skillstyle"));
    
    label_des->setVisible(true);
    
    label_skillstyle->setVisible(true);
    
    if (curr_equip->getSkillID() != MAINGUN_SKILL_TYPE_NULL)
    {
        SkillData* skill_data = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID(curr_equip->getSkillID());
        
        SkillLevelData* skill_level_data = skill_data->getSkillLevelDataByKeys("1");
        
        label_des->setText(skill_level_data->getDes());
        
        label_skillstyle->setText(skill_level_data->getSkillType());
        
        label_skillstyle->setColor(ccORANGE);
    }
    else
    {
        label_des->setVisible(false);
        
        label_skillstyle->setVisible(false);
    }
    
    //装备出售可获得钱数
    Label* label_sell_gold = static_cast<Label*>(m_pUILayer->getWidgetByName("label_gold_sale"));
    
    label_sell_gold->setText(getStringFromInt(getGoldBySellEquip(curr_equip->getGrade(), curr_equip->getLevel())));
    
    //装备出售按钮
    Button* btn_sale = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_sale"));
    
    btn_sale->addTouchEventListener(this, toucheventselector(GameMainLayer::callBackDialogSell));
    
    btn_sale->setTouchEnabled(true);
    
    
    //装备升级按钮
    Button* btn_upgrade = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_upgrade"));
    
    btn_upgrade->addTouchEventListener(this, toucheventselector(GameMainLayer::upgradeEquipCallBack));
    
    ImageView* img_upgrade = static_cast<ImageView*>(m_pUILayer->getWidgetByName("img_upgrade"));
    
    ImageView* img_gold = static_cast<ImageView*>(m_pUILayer->getWidgetByName("img_gold"));
    
    Label* label_full = static_cast<Label*>(m_pUILayer->getWidgetByName("label_full"));
    
    //装备升级需要钱数
    Label* label_up_gold = static_cast<Label*>(m_pUILayer->getWidgetByName("label_gold_upgrade"));
    
    if (curr_equip->getLevel() == curr_equip->getLevelLimit())
    {
        
        img_gold->setVisible(false);
        
        img_upgrade->setVisible(false);
        
        label_up_gold->setVisible(false);
        
        label_full->setVisible(true);
    }
    else
    {
        label_full->setVisible(false);
        
        img_gold->setVisible(true);
        
        img_upgrade->setVisible(true);
        
        label_up_gold->setVisible(true);
        
        label_up_gold->setText(getStringFromInt(upgradeRequireGold(curr_equip->getGrade(), curr_equip->getLevel())));
    }
    
    
    
    //重铸按钮
    Button* btn_recast = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_recast_gem"));
    
    btn_recast->addTouchEventListener(this, toucheventselector(GameMainLayer::callBackDialogRecast));
    
    //重铸需要钻石
    Label* label_recast_gem = static_cast<Label*>(m_pUILayer->getWidgetByName("label_gem_recast"));
    label_recast_gem->setText(getStringFromInt(10));
    
    //等级上限
    Label* label_levlimit = static_cast<Label*>(m_pUILayer->getWidgetByName("label_leveltop"));
    
    label_levlimit->setText(getStringFromInt(curr_equip->getLevelLimit()));
    
    //当前等级
    Label* label_curlevel = static_cast<Label*>(m_pUILayer->getWidgetByName("label_levelnow"));
    
    label_curlevel->setText(getStringFromInt(curr_equip->getLevel()));
    
    //附加属性1
    Label* label_extatt_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_1"));
    
    Label* label_extatt_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_1_num"));
    
    if (curr_equip->getExtAtt1() != 0)
    {
        label_extatt_1->setText(curr_equip->getExtAtt1Name());
        
        string num = getStringFromInt(curr_equip->getExtAtt1Value()).append("%");
        
        label_extatt_1_num->setText(num);
    }
    else
    {
        label_extatt_1->setVisible(false);
        label_extatt_1_num->setVisible(false);
    }
    
    
    
    //附加属性2
    Label* label_extatt_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_2"));
    
    Label* label_extatt_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_2_num"));
    
    if (curr_equip->getExtAtt1() != 0 && curr_equip->getExtAtt2() != 0)
    {
        label_extatt_2->setText(curr_equip->getExtAtt2Name());
        
        string num = getStringFromInt(curr_equip->getExtAtt2Value()).append("%");
        
        label_extatt_2_num->setText(num);
    }
    else
    {
        label_extatt_2->setVisible(false);
        label_extatt_2_num->setVisible(false);
    }
    
    //攻击
    Label* label_attack = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_act"));
    
    label_attack->setText(getStringFromInt(curr_equip->getAttack() * curr_equip->getLevel()));
    
    //血量
    Label* label_hp = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_hp"));
    
    label_hp->setText(getStringFromInt(curr_equip->getHp() * curr_equip->getLevel()));
}

/******************刷新金币*****************/

//刷新金币
void GameMainLayer::refreshGoldInfo(CCObject* pSender)
{
    
    LabelAtlas* label_energy = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_energy"));
    label_energy->setStringValue(getStringFromInt(UserData::shared()->getEnergyNum()));
    
    LabelAtlas* label_gold = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gold"));
    label_gold->setStringValue(getStringFromInt(UserData::shared()->getGoldNum()));
    
   
    LabelAtlas* label_gem = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gem"));
    label_gem->setStringValue(getStringFromInt(UserData::shared()->getGemNum()));
    
    refreshToolsNum();
}

void GameMainLayer::refreshBtnAchieveInfo()
{

    if (GameCountData::sharedInstance()->hadAchieveUnReceive() || DailyCountData::sharedInstance()->hadAchieveUnReceive())
    {
        ActionManager::shareManager()->playActionByName("tools_layer.ExportJson", "Animation2");
    }

}

/******************装备********************/
//重铸装备弹出确认框
void GameMainLayer::callBackDialogRecast(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        FerrisDialog* dialog = FerrisDialog::createWithContent(DIALOG_TYPE_CONFIRM_CANCEL, "0", this, toucheventselector(GameMainLayer::recastEquipCallBack), NULL);
        
        addChild(dialog, KZOrderDialog, kTagDialog);
    }
}


//弹出装备出售确认框
void GameMainLayer::callBackDialogSell(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        EquipItem* curr_equip = UserEquipBag::shared()->getEquipById(m_selectEquip);
        
        FerrisDialog* dialog = NULL;
        
        if (curr_equip->getEquipType() == 0)
        {
            dialog = FerrisDialog::createWithStr(curr_equip->getName().c_str(), this, toucheventselector(GameMainLayer::sellEquipCallBack), NULL);
        }
        else
        {
            dialog = FerrisDialog::createWithStr(curr_equip->getName().c_str(), this, toucheventselector(GameMainLayer::sellEquipCallBack), NULL,false);
        }
        
        addChild(dialog,KZOrderDialog,kTagDialog);
    }
}

//出售装备
void GameMainLayer::sellEquipCallBack(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        
        //如果层中有对话框删除对话框
        if (this->getChildByTag(kTagDialog))
        {
            this->removeChildByTag(kTagDialog);
        }
        
        EquipItem* curr_equip = UserEquipBag::shared()->getEquipById(m_selectEquip);
        
        //如果是背包中的装备
        if (curr_equip->getEquipType() == 0)
        {
            int count = 0;
            CCDictElement* elm = NULL;
            
            CCDictionary* equip_dic = UserEquipBag::shared()->getDicEquip();
            
            CCDICT_FOREACH(equip_dic,elm)
            {
                EquipItem* item = (EquipItem*)elm->getObject();
                
                if (item->getEquipType() == 0)
                {
                    count++;
                }
            }
            
            //背包中只剩下一个装备的时候  不能出售
            if (count <= 1)
            {
                //return;
            }
        }
        
        int gold = getGoldBySellEquip(curr_equip->getGrade(), curr_equip->getLevel());
        
        UserData::shared()->increaseGoldNum(gold);
        
        UserEquipBag::shared()->deletEquipByID(m_selectEquip);
        
        UserEquipBag::shared()->save();
        
        refreshGoldInfo(NULL);
        
        m_nSellEquip++;
        

        DailyCountData::sharedInstance()->addDailySellEquip();
        
        DailyCountData::sharedInstance()->save();
        
        DailyTask::sharedInstance()->save();

        
        CCLog("current equip :%s",m_selectEquip.c_str());
        
        refreshEquipBagAndSort();
        
        closeEquipDetailCallBack(NULL, TOUCH_EVENT_ENDED);
        
        refreshAttributeShow();
        
        
    }
}

//重铸装备
void GameMainLayer::recastEquipCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
//        if (this->getChildByTag(kTagDialog))
//        {
//            this->removeChildByTag(kTagDialog);
//        }
        
        //重铸价格处理
        int price = 10;
        if (!UserData::shared()->isGemEnough(price))
        {
            GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
        }
        else
        {
            UserData::shared()->decreaseGemNum(price);
            
            UserData::shared()->save();
            
            GameCountData::sharedInstance()->addConsumeGem(price);
            
            Achieve::getInstance()->save();
            
            EquipItem* item = UserEquipBag::shared()->getEquipById(m_selectEquip);
            
            //重铸后的品级
            int recast_qua = GetRandomEquip::shared()->getQualityRecast();
            
            //重铸后的属性变化
            item->setGrade(recast_qua);
            
            GetRandomEquip::shared()->randHpAndAttack(item);
            
//更改 2015.3.4 有可能出现负值
//            if (!compareString(item->getExtAtt1Name().c_str(), "0"))
//            {
//                //如果属性1 存在 更改属性值
//                item->setExtAtt1Value(item->getExtAtt1Value() + (recast_qua - ori_qua) * 3);
//            }
//            if (!compareString(item->getExtAtt1Name().c_str(), "0") && !compareString(item->getExtAtt2Name().c_str(), "0"))
//            {
//                //如果属性2 存在 更改属性值
//                item->setExtAtt2Value(item->getExtAtt2Value() + (recast_qua - ori_qua) * 3);
//            }
            
            GetRandomEquip::shared()->randAttributeWithRecast(item);
            
            UserEquipBag::shared()->save();
            
            //调用获得装备层 显示获得装备
            GetEquipLayer* layer = GetEquipLayer::create(item,true);
            
            this->addChild(layer,kZOrderGetEquipLayer);
            
            refreshAttributeShow();
            
            refreshEquipDetail();
            
            refreshGoldInfo(NULL);
            
            refreshEquipBagAndSort();
            
            if (item->getEquipType() == 0)
            {
                Button* btn = static_cast<Button*>(m_pUILayer->getWidgetByName(item->getID().c_str()));
                
                if (btn)
                {
                    ImageView* effect_select = ImageView::create();
                    effect_select->loadTexture("ui/effect/effect_selectEquip.png");
                    btn->addChild(effect_select, kZOrderEffectSelect, kTagEffectSelectEquip);
                }
            }
        }
        
    }
}

//升级装备
void GameMainLayer::upgradeEquipCallBack(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        EquipItem* curr_equip = UserEquipBag::shared()->getEquipById(m_selectEquip);
        
        //如果装备等级达到装备等级上限 直接返回
        if (curr_equip->getLevel() == curr_equip->getLevelLimit())
        {
            CCLog("curretn is max level!");
            
            return;
        }
        
        int gold = upgradeRequireGold(curr_equip->getGrade(), curr_equip->getLevel());
        
        //如果金币不足 弹出购买金币界面
        if (!UserData::shared()->isGoldEnough(gold))
        {
            GameDelegate::shared()->openSceneStoreLayer(kStore_gold);
            
            return;
        }
        
        curr_equip->setLevel(curr_equip->getLevel() + 1);

        
        //日常统计 强化任一装备到等级
        if (curr_equip->getLevel() > DailyCountData::sharedInstance()->getStrengthEquip())
        {
            DailyCountData::sharedInstance()->setStrengthEquip(curr_equip->getLevel());
            
            DailyCountData::sharedInstance()->hadAchieveComplete();
        }
        
        m_nStrengthEquip++;
        
        if (m_nStrengthEquip > DailyCountData::sharedInstance()->getDailyStrengthTimes())
        {
            DailyCountData::sharedInstance()->setDailyStrengthTimes(m_nStrengthEquip);
            
            DailyCountData::sharedInstance()->hadAchieveComplete();
        }
        
        //成就统计 强化任一装备到等级
        if (curr_equip->getLevel() > GameCountData::sharedInstance()->getEquipStrength())
        {
            GameCountData::sharedInstance()->setEquipStrength(curr_equip->getLevel());
            
            GameCountData::sharedInstance()->hadAchieveComplete();
            
            GameCountData::sharedInstance()->save();
        }
        
        UserData::shared()->decreaseGoldNum(gold);
        
        refreshEquipDetail();
        
        refreshGoldInfo(NULL);
        
        //创建升级帧动画
        
        CCArray* arrFrame = CCArray::createWithCapacity(6);
        
        char str[100] = {0};
        
        for (int i = 1; i < 7; i++)
        {
            sprintf(str, "upgrade_%02d.png",i);
            
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
            
            arrFrame->addObject(frame);
        }
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(arrFrame , 0.1f);
        
        animation->setLoops(1);
        
        CCAnimate* animate = CCAnimate::create(animation);
        
        
        
        CCSprite* pSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("upgrade_01.png"));
        
        ImageView* icon = static_cast<ImageView*>(m_pUILayer->getWidgetByName("icon_equip"));
        
        pSprite->setPosition(ccp(100, _s_center.y + 320));
        
        CCPoint icon_pos = convertToWorldSpace(icon->getPosition());
        

        
        
        CCLog("icon pos x: %f, y: %f",icon_pos.x,icon_pos.y);
        
        pSprite->runAction(CCSequence::create(animate,CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::upgradeActionFinish)),NULL));
        
        this->addChild(pSprite,100);
        
        refreshAttributeShow();
        
        refreshBtnAchieveInfo();
        
        UserData::shared()->save();
        
        UserEquipBag::shared()->save();
        
        //icon->setColor(ccRED);
        
        
        //closeEquipDetailCallBack(NULL, TOUCH_EVENT_ENDED);
    }
}

//升级动作完成回调
void GameMainLayer::upgradeActionFinish(CCObject* pSender)
{
    CCSprite* psprite = (CCSprite*)pSender;
    
    psprite->removeFromParentAndCleanup(true);
}

//升级后更新装备详细属性
void GameMainLayer::refreshEquipDetail()
{
    //初始化装备详细界面的信息
    EquipItem* curr_equip = UserEquipBag::shared()->getEquipById(m_selectEquip);
    
    if (!curr_equip)
    {
        return;
    }
    
    //装备出售可获得钱数
    Label* label_sell_gold = static_cast<Label*>(m_pUILayer->getWidgetByName("label_gold_sale"));
    
    CCLog("curr_equip label_gold_sale : %d",getGoldBySellEquip(curr_equip->getGrade(), curr_equip->getLevel()));
    
    label_sell_gold->setText(getStringFromInt(getGoldBySellEquip(curr_equip->getGrade(), curr_equip->getLevel())));
    
    //装备出售按钮
    Button* btn_sale = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_sale"));
    
    btn_sale->addTouchEventListener(this, toucheventselector(GameMainLayer::callBackDialogSell));
    
    //装备升级需要钱数
    Label* label_up_gold = static_cast<Label*>(m_pUILayer->getWidgetByName("label_gold_upgrade"));
    
    //装备升级按钮
    Button* btn_upgrade = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_upgrade"));
    
    ImageView* img_upgrade = static_cast<ImageView*>(m_pUILayer->getWidgetByName("img_upgrade"));
    
    ImageView* img_gold = static_cast<ImageView*>(m_pUILayer->getWidgetByName("img_gold"));
    
    Label* label_full = static_cast<Label*>(m_pUILayer->getWidgetByName("label_full"));
    
    if (curr_equip->getLevel() == curr_equip->getLevelLimit())
    {
        
        label_up_gold->setVisible(false);
        
        img_gold->setVisible(false);
        
        img_upgrade->setVisible(false);
        
        label_full->setVisible(true);
    }
    else
    {
        
        label_full->setVisible(false);
        
        label_up_gold->setVisible(true);
        
        img_gold->setVisible(true);
        
        img_upgrade->setVisible(true);
        
        label_up_gold->setText(getStringFromInt(upgradeRequireGold(curr_equip->getGrade(), curr_equip->getLevel())));
        
        btn_upgrade->setTouchEnabled(true);
    }
    
    CCLog("curr_equip label_gold_upgrade : %d",upgradeRequireGold(curr_equip->getGrade(), curr_equip->getLevel()));
    
    //重铸需要钻石 todo：需要处理
    Label* label_recast_gem = static_cast<Label*>(m_pUILayer->getWidgetByName("label_gem_recast"));
    
    //等级上限
    Label* label_levlimit = static_cast<Label*>(m_pUILayer->getWidgetByName("label_leveltop"));
    
    label_levlimit->setText(getStringFromInt(curr_equip->getLevelLimit()));
    
    CCLog("curr_equip label_levlimit : %d",curr_equip->getLevelLimit());
    
    //当前等级
    Label* label_curlevel = static_cast<Label*>(m_pUILayer->getWidgetByName("label_levelnow"));
    
    label_curlevel->setText(getStringFromInt(curr_equip->getLevel()));
    
    CCLog("curr_equip curlevel : %d",curr_equip->getLevel());
    
    //攻击
    Label* label_attack = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_act"));
    
    label_attack->setText(getStringFromInt(curr_equip->getAttack() * curr_equip->getLevel()));
    
    CCLog("curr_equip attack : %d",curr_equip->getAttack());
    
    //血量
    Label* label_hp = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_hp"));
    
    label_hp->setText(getStringFromInt(curr_equip->getHp() * curr_equip->getLevel()));
    
    CCLog("curr_equip label_hp : %d",curr_equip->getHp());
    
    //装备图标
    ImageView* icon = static_cast<ImageView*>(m_pUILayer->getWidgetByName("icon_equip"));
    
    icon->loadTexture(UserEquipBag::shared()->getIconPath(curr_equip).c_str(),UI_TEX_TYPE_PLIST);
    
    //装备名称
    Label* label_name = static_cast<Label*>(m_pUILayer->getWidgetByName("label_name"));
    label_name->setText(curr_equip->getName());
    
    CCLog("curr_equip name : %s",curr_equip->getName().c_str());
    
    //装备类型
    Label* label_type = static_cast<Label*>(m_pUILayer->getWidgetByName("label_style"));
    
    label_type->setText(curr_equip->getTypeText());
    
    CCLog("curr_equip name : %d",curr_equip->getEquipType());
    
    //装备技能
    Label* label_skill = static_cast<Label*>(m_pUILayer->getWidgetByName("label_skill"));
    
    label_skill->setText(curr_equip->getSkillText());
    
    CCLog("curr_equip name : %s",curr_equip->getSkillText().c_str());
    
    //装备描述 描述未处理
    Label* label_des = static_cast<Label*>(m_pUILayer->getWidgetByName("label_dic"));
    
    Label* label_skillstyle = static_cast<Label*>(m_pUILayer->getWidgetByName("label_skillstyle"));
    
    label_des->setVisible(true);
    
    label_skillstyle->setVisible(true);
    
    if (curr_equip->getSkillID() != MAINGUN_SKILL_TYPE_NULL)
    {
        SkillData* skill_data = (SkillData*)SkillLibrary::sharedSkillLibrary()->getSkillDataByID(curr_equip->getSkillID());
        
        SkillLevelData* skill_level_data = skill_data->getSkillLevelDataByKeys("1");
        
        label_des->setText(skill_level_data->getDes());
        
        label_skillstyle->setText(skill_level_data->getSkillType());
        
        label_skillstyle->setColor(ccORANGE);
    }
    else
    {
        label_des->setVisible(false);
        
        label_skillstyle->setVisible(false);
    }
    
    
    //附加属性1
    Label* label_extatt_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_1"));
    
    Label* label_extatt_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_1_num"));
    
    label_extatt_1->setVisible(true);
    
    label_extatt_1_num->setVisible(true);
    
    if (curr_equip->getExtAtt1() != 0)
    {
        label_extatt_1->setText(curr_equip->getExtAtt1Name());
        
        string num = getStringFromInt(curr_equip->getExtAtt1Value()).append("%");
        
        CCLog("string value : %s",num.c_str());
        
        label_extatt_1_num->setText(num);
    }
    else
    {
        label_extatt_1->setVisible(false);
        label_extatt_1_num->setVisible(false);
    }
    
    
    
    //附加属性2
    Label* label_extatt_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_2"));
    
    Label* label_extatt_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("attribute_ext_2_num"));
    
    label_extatt_2->setVisible(true);
    
    label_extatt_2_num->setVisible(true);
    
    if (curr_equip->getExtAtt1() != 0 && curr_equip->getExtAtt2() != 0)
    {
        label_extatt_2->setText(curr_equip->getExtAtt2Name());
        
        string num = getStringFromInt(curr_equip->getExtAtt2Value()).append("%");
        
        CCLog("string value : %s",num.c_str());
        
        label_extatt_2_num->setText(num);
    }
    else
    {
        label_extatt_2->setVisible(false);
        label_extatt_2_num->setVisible(false);
    }

}

//升级所需金币
int GameMainLayer::upgradeRequireGold(int equip_grade,int equip_level)
{
    equip_level += 1;
    
    switch (equip_grade)
    {
        case EQUIP_QUALITY_WHITE:
            return (equip_level - 1) * 100;
            break;
            
        case EQUIP_QUALITY_GREEN:
            return (equip_level - 1) * 150;
            break;
            
        case EQUIP_QUALITY_BLUE:
            return (equip_level - 1) * 200;
            break;
            
        case EQUIP_QUALITY_PURPLE:
            return (equip_level - 1) * 250;
            break;
            
        case EQUIP_QUALITY_ORRANGE:
            return (equip_level - 1) * 300;
            break;
            
        default:
            break;
    }
    
    return 0;
}

//出售可获得金币
int GameMainLayer::getGoldBySellEquip(int equip_grade,int equip_level)
{
    switch (equip_grade)
    {
        case EQUIP_QUALITY_WHITE:
            return (equip_level - 1) * 30 + 750;
            break;
            
        case EQUIP_QUALITY_GREEN:
            return (equip_level - 1) * 60 + 2500;
            break;
            
        case EQUIP_QUALITY_BLUE:
            return (equip_level - 1) * 100 + 7500;
            break;
            
        case EQUIP_QUALITY_PURPLE:
            return (equip_level - 1) * 150 + 25000;
            break;
            
        case EQUIP_QUALITY_ORRANGE:
            return (equip_level - 1) * 200 + 50000;
            break;
            
        default:
            break;
    }
    
    return 0;
}

//取消之前选中的装备
void GameMainLayer::unSelectEquip()
{
    ListView* pList_equip = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_equip"));
    
    if (pList_equip)
    {
        CCArray* arr = pList_equip->getItems();
        
        CCObject* obj = NULL;
        
        CCARRAY_FOREACH(arr, obj)
        {
            Button* btn = (Button*)obj;
            
            if (btn->getChildByTag(kTagEffectSelectEquip))
            {
                btn->removeChildByTag(kTagEffectSelectEquip, true);
            }
        }
    }
    
    if (this->getChildByTag(kTagEffectSelectPanel))
    {
        this->removeChildByTag(kTagEffectSelectPanel);
    }
    
    UIPanel* panel_maingun = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_main"));
    
    if (panel_maingun->getNodeByTag(kTagEffectChange))
    {
        panel_maingun->removeNodeByTag(kTagEffectChange);
    }
    
    UIPanel* panel_sidegun1 = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_side_1"));
    
    if (panel_sidegun1->getNodeByTag(kTagEffectChange))
    {
        panel_sidegun1->removeNodeByTag(kTagEffectChange);
    }
    
    UIPanel* panel_sidegun2 = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_side_2"));
    
    if (panel_sidegun2->getNodeByTag(kTagEffectChange))
    {
        panel_sidegun2->removeNodeByTag(kTagEffectChange);
    }
    
    UIPanel* panel_wing = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_wing"));
    
    if (panel_wing->getNodeByTag(kTagEffectChange))
    {
        panel_wing->removeNodeByTag(kTagEffectChange);
    }
}

void GameMainLayer::equipGroupInit(bool runAction)
{
    
    node_equipGroup = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/equipgroup_node.ExportJson"));
    
    m_pPageView->addWidgetToPage(node_equipGroup, 1, false);
    node_equipGroup->setZOrder(10);
    
    node_equipGroup->setTouchEnabled(false);
    
    //主炮
    string gun_id = UserEquip::shared()->getMainGunID();
    
    if (!compareString(gun_id.c_str(), "0"))
    {
        EquipItem* equip_maingun = UserEquipBag::shared()->getEquipById(gun_id);
        
        string img_id = UserEquipBag::shared()->getIconPath(equip_maingun);
        
        Button* btn_maingun = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_maingun"));
        
        btn_maingun->setName(gun_id.c_str());
        
        CCLog("equip_maingun img_id : %s",img_id.c_str());
        
        btn_maingun->loadTextures(img_id.c_str(), img_id.c_str(), img_id.c_str(),UI_TEX_TYPE_PLIST);
        
        btn_maingun->addTouchEventListener(this, toucheventselector(GameMainLayer::kickEquipCallBack));
        btn_maingun->setEnabled(true);
    }
    
    //边炮1
    gun_id = UserEquip::shared()->getFirSubGunID();
    
    if (!compareString(gun_id.c_str(), "0"))
    {
        EquipItem* equip_subGun1 = UserEquipBag::shared()->getEquipById(gun_id);
        
        
        string img_id = UserEquipBag::shared()->getIconPath(equip_subGun1);
        
        Button* btn_sidegun_1 = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_sidegun_1"));
        
        CCLog("equip_subGun1 img_id : %s",img_id.c_str());
        
        btn_sidegun_1->loadTextures(img_id.c_str(), img_id.c_str(), img_id.c_str(),UI_TEX_TYPE_PLIST);
        
        btn_sidegun_1->setName(gun_id.c_str());
        
        btn_sidegun_1->addTouchEventListener(this, toucheventselector(GameMainLayer::kickEquipCallBack));
    }
    
    //边炮2
    gun_id = UserEquip::shared()->getSecSubGunID();
    
    CCLog("UserEquip::shared()->getSecSubGunID : %s",UserEquip::shared()->getSecSubGunID().c_str());
    
    if (!compareString(gun_id.c_str(), "0"))
    {
        CCLog("gun_id : %s",gun_id.c_str());
        
        EquipItem* equip_subGun2 = UserEquipBag::shared()->getEquipById(gun_id);
        
        string img_id = UserEquipBag::shared()->getIconPath(equip_subGun2);
        
        Button* btn_sidegun_2 = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_sidegun_2"));
        
        CCLog("equip_subGun2 img_id : %s",img_id.c_str());
        
        btn_sidegun_2->loadTextures(img_id.c_str(), img_id.c_str(), img_id.c_str(),UI_TEX_TYPE_PLIST);
        
        btn_sidegun_2->setName(gun_id.c_str());
        
        btn_sidegun_2->addTouchEventListener(this, toucheventselector(GameMainLayer::kickEquipCallBack));
    }
    
    //僚机
    gun_id = UserEquip::shared()->getWingID();
    
    if (!compareString(gun_id.c_str(), "0"))
    {
        EquipItem* equip_wing = UserEquipBag::shared()->getEquipById(gun_id);
        
        string img_id = UserEquipBag::shared()->getIconPath(equip_wing);
        
        Button* btn_wing = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_wing"));
        btn_wing->addTouchEventListener(this, toucheventselector(GameMainLayer::kickEquipCallBack));
        
        CCLog("equip_wing img_id : %s",img_id.c_str());
        
        btn_wing->loadTextures(img_id.c_str(), img_id.c_str(), img_id.c_str(),UI_TEX_TYPE_PLIST);
        
        btn_wing->setName(gun_id.c_str());
    }
}

void GameMainLayer::refreshTotalAtt(int att_id, int value,float percent)
{
    switch (att_id)
    {
        case 1:
            n_total_attack_add += value ;
            break;
            
        case 2:
            n_total_hp_add += value ;
            break;
            
        case 3:
            n_total_toolDrop += value ;
            break;
            
        case 4:
            n_total_bloodDrop += value ;
            break;
            
        default:
            break;
    }
}

//更新界面装备属性
void GameMainLayer::refreshSigninEffect()
{
    //如果当前记录时间与记录相等
    ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName("Image_7"));
    if (getCurrentTime(kGetTimeYear) == UserData::shared()->getLastLoginYear()
        && getCurrentTime(kGetTimeMonth) == UserData::shared()->getLastLoginMon()
        && getCurrentTime(kGetTimeDay) == UserData::shared()->getLastLoginDay())
    {
        img->setVisible(false);
    }
    else
    {
        ActionManager::shareManager()->playActionByName("tools_layer.ExportJson", "Animation1");
    }
}


//更新界面装备属性
void GameMainLayer::refreshAttributeShow()
{
    
    //总属性数字
    //总血量
    int n_total_hp = 0;
    
    //总攻击
    int n_total_attack = 0;
    
    //总攻击加成
    n_total_attack_add = 0;
    
    //总血量加成
    n_total_hp_add = 0;
    
    //总道具掉率
    n_total_toolDrop = 0;
    
    //总血量掉率
    n_total_bloodDrop = 0;
    
    /********************
     
        总属性规则：主炮属性为100% 肩炮属性为60% 翼炮属性为50% 僚机炮为33%
     
     ********************/
    
    /********************
            主炮
     ********************/
    string gun_id = UserEquip::shared()->getMainGunID();
    
    EquipItem* equip_maingun = UserEquipBag::shared()->getEquipById(gun_id);
    
    if (equip_maingun)
    {
        n_total_hp += equip_maingun->getHp() * equip_maingun->getLevel();
        n_total_attack += equip_maingun->getAttack() * equip_maingun->getLevel();
        
        //攻击
        Label* attack_miangun = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_act"));
        
        attack_miangun->setText(getStringFromInt(equip_maingun->getAttack() * equip_maingun->getLevel()));
        
        //血量
        Label* hp_maingun = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_hp"));
        hp_maingun->setText(getStringFromInt(equip_maingun->getHp() * equip_maingun->getLevel()));
        
        //技能
        Label* skill_maingun = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_skill"));
        if (equip_maingun->getSkillID() < WING_SKILL_TYPE_DEFENSE_BOMBS)
        {
            skill_maingun->setText(equip_maingun->getSkillText());
        }
        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_1"));
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_1_num"));
        
        attr_1->setVisible(true);
        
        attr_1_num->setVisible(true);
        if (equip_maingun->getExtAtt1() != 0)
        {
            attr_1->setText(equip_maingun->getExtAtt1Name());
            
            string num = getStringFromInt(equip_maingun->getExtAtt1Value()).append("%");
            
            attr_1_num->setText(num);
            
            refreshTotalAtt(equip_maingun->getExtAtt1(), equip_maingun->getExtAtt1Value(),1.0f);
            
        }
        else
        {
            attr_1->setVisible(false);
            
            attr_1_num->setVisible(false);
        }
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_2"));
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_2_num"));
        
        attr_2->setVisible(true);
        
        attr_2_num->setVisible(true);
        if (equip_maingun->getExtAtt1() != 0 && equip_maingun->getExtAtt2() != 0)
        {
            attr_2->setText(equip_maingun->getExtAtt2Name());
            
            string num = getStringFromInt(equip_maingun->getExtAtt2Value()).append("%");
            
            attr_2_num->setText(num);
            
            refreshTotalAtt(equip_maingun->getExtAtt2(), equip_maingun->getExtAtt2Value(),1.0f);
        }
        else
        {
            attr_2->setVisible(false);
            
            attr_2_num->setVisible(false);
        }
    }
    else
    {
        //攻击
        Label* attack_miangun = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_act"));
        
        attack_miangun->setText("0");
        
        //血量
        Label* hp_maingun = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_hp"));
        hp_maingun->setText("0");
        
        //技能
        Label* skill_maingun = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_skill"));

        skill_maingun->setText("");

        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_1"));
        attr_1->setVisible(false);
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_1_num"));
        attr_1_num->setVisible(false);
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_2"));
        attr_2->setVisible(false);
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("main_attribute_ext_2_num"));
        attr_2_num->setVisible(false);
    }
    
    /********************
            副炮1
     ********************/
    gun_id = UserEquip::shared()->getFirSubGunID();
    
    EquipItem* equip_subGun1 = UserEquipBag::shared()->getEquipById(gun_id);
    
    if (equip_subGun1)
    {
        n_total_hp += equip_subGun1->getHp() * equip_subGun1->getLevel();
        n_total_attack += equip_subGun1->getAttack() * equip_subGun1->getLevel();
        
        //血量
        Label* hp_sidegun1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_hp"));
        hp_sidegun1->setText(getStringFromInt(equip_subGun1->getHp() * equip_subGun1->getLevel()));
        
        //攻击
        Label* attack_sidegun1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_act"));
        attack_sidegun1->setText(getStringFromInt(equip_subGun1->getAttack() * equip_subGun1->getLevel()));
        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_1"));
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_1_num"));
        
        attr_1->setVisible(true);
        
        attr_1_num->setVisible(true);
        
        if (equip_subGun1->getExtAtt1() != 0)
        {
            attr_1->setText(equip_subGun1->getExtAtt1Name());
            
            string num = getStringFromInt(equip_subGun1->getExtAtt1Value()).append("%");
            
            attr_1_num->setText(num);
            
            refreshTotalAtt(equip_subGun1->getExtAtt1(), equip_subGun1->getExtAtt1Value(),0.6f);
        }
        else
        {
            attr_1->setVisible(false);
            
            attr_1_num->setVisible(false);
        }
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_2"));
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_2_num"));
        
        attr_2->setVisible(true);
        
        attr_2_num->setVisible(true);
        
        if (equip_subGun1->getExtAtt1() != 0 && equip_subGun1->getExtAtt2() != 0)
        {
            attr_2->setText(equip_subGun1->getExtAtt2Name());
            
            string num = getStringFromInt(equip_subGun1->getExtAtt2Value()).append("%");
            
            attr_2_num->setText(num);
            
            refreshTotalAtt(equip_subGun1->getExtAtt2(), equip_subGun1->getExtAtt2Value(),0.6f);
        }
        else
        {
            attr_2->setVisible(false);
            
            attr_2_num->setVisible(false);
        }
    }
    else
    {
        //血量
        Label* hp_sidegun1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_hp"));
        hp_sidegun1->setText("0");
        
        //攻击
        Label* attack_sidegun1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_act"));
        attack_sidegun1->setText("0");
        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_1"));
        attr_1->setVisible(false);
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_1_num"));
        attr_1_num->setVisible(false);
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_2"));
        attr_2->setVisible(false);
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side1_attribute_ext_2_num"));
        attr_2_num->setVisible(false);
        
        
    }
    
    /********************
            副炮2
     ********************/
    gun_id = UserEquip::shared()->getSecSubGunID();
    
    EquipItem* equip_subGun2 = UserEquipBag::shared()->getEquipById(gun_id);
    
    if (equip_subGun2)
    {
        n_total_hp += equip_subGun2->getHp() * equip_subGun2->getLevel();
        n_total_attack += equip_subGun2->getAttack() * equip_subGun2->getLevel();
        
        Label* hp_sidegun_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_hp"));
        
        hp_sidegun_2->setText(getStringFromInt(equip_subGun2->getHp() * equip_subGun2->getLevel()));
        
        Label* attack_sidegun_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_act"));
        
        attack_sidegun_2->setText(getStringFromInt(equip_subGun2->getAttack() * equip_subGun2->getLevel()));
        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_1"));
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_1_num"));
        
        attr_1->setVisible(true);
        
        attr_1_num->setVisible(true);
        
        if (equip_subGun2->getExtAtt1() != 0)
        {
            attr_1->setText(equip_subGun2->getExtAtt1Name());
            
            string num = getStringFromInt(equip_subGun2->getExtAtt1Value()).append("%");
            
            attr_1_num->setText(num);
            
            refreshTotalAtt(equip_subGun2->getExtAtt1(), equip_subGun2->getExtAtt1Value(),0.5f);
        }
        else
        {
            attr_1->setVisible(false);
            
            attr_1_num->setVisible(false);
        }
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_2"));
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_2_num"));
        
        attr_2->setVisible(true);
        
        attr_2_num->setVisible(true);
        
        if (equip_subGun2->getExtAtt1() != 0 && equip_subGun2->getExtAtt2() != 0)
        {
            attr_2->setText(equip_subGun2->getExtAtt2Name());
            
            string num = getStringFromInt(equip_subGun2->getExtAtt2Value()).append("%");
            
            attr_2_num->setText(num);
            
            refreshTotalAtt(equip_subGun2->getExtAtt2(), equip_subGun2->getExtAtt2Value(),0.5f);
        }
        else
        {
            attr_2->setVisible(false);
            
            attr_2_num->setVisible(false);
        }
    }
    else
    {
        Label* hp_sidegun_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_hp"));
        
        hp_sidegun_2->setText("0");
        
        Label* attack_sidegun_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_act"));
        
        attack_sidegun_2->setText("0");
        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_1"));
        attr_1->setVisible(false);
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_1_num"));
        attr_1_num->setVisible(false);
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_2"));
        attr_2->setVisible(false);
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("side2_attribute_ext_2_num"));
        attr_2_num->setVisible(false);
    }
    
    //
    /********************
            僚机
     ********************/
    gun_id = UserEquip::shared()->getWingID();
    
    EquipItem* equip_wing = UserEquipBag::shared()->getEquipById(gun_id);
    
    if (equip_wing)
    {
        n_total_hp += equip_wing->getHp() * equip_wing->getLevel();
        n_total_attack += equip_wing->getAttack() * equip_wing->getLevel();
        
        //血量
        Label* hp_wing = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_hp"));
        
        //攻击
        Label* attack_wing = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_act"));
        
        attack_wing->setText(getStringFromInt(equip_wing->getAttack() * equip_wing->getLevel()));
        
        hp_wing->setText(getStringFromInt(equip_wing->getHp() * equip_wing->getLevel()));
        
        Label* skill_wing = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_skill"));
        if (equip_wing->getSkillID() >= WING_SKILL_TYPE_DEFENSE_BOMBS || equip_wing->getSkillID() == MAINGUN_SKILL_TYPE_NULL)
        {
            skill_wing->setText(equip_wing->getSkillText());
        }
        else
        {
            skill_wing->setText("");
        }
        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_1"));
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_1_num"));
        
        attr_1->setVisible(true);
        
        attr_1_num->setVisible(true);
        
        if (equip_wing->getExtAtt1() != 0)
        {
            attr_1->setText(equip_wing->getExtAtt1Name());
            
            string num = getStringFromInt(equip_wing->getExtAtt1Value()).append("%");
            
            attr_1_num->setText(num);
            
            refreshTotalAtt(equip_wing->getExtAtt1(), equip_wing->getExtAtt1Value() ,0.33f);
        }
        else
        {
            attr_1->setVisible(false);
            
            attr_1_num->setVisible(false);
        }
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_2"));
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_2_num"));
        
        attr_2->setVisible(true);
        
        attr_2_num->setVisible(true);
        
        if (equip_wing->getExtAtt1() != 0 && equip_wing->getExtAtt2() != 0)
        {
            attr_2->setText(equip_wing->getExtAtt2Name());
            
            string num = getStringFromInt(equip_wing->getExtAtt2Value()).append("%");
            
            attr_2_num->setText(num);
            
            refreshTotalAtt(equip_wing->getExtAtt2(), equip_wing->getExtAtt2Value() , 0.33f);
        }
        else
        {
            attr_2->setVisible(false);
            
            attr_2_num->setVisible(false);
        }
    }
    else
    {
        //血量
        Label* hp_wing = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_hp"));
        
        //攻击
        Label* attack_wing = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_act"));
        
        attack_wing->setText("0");
        
        hp_wing->setText("0");
        
        Label* skill_wing = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_skill"));

            skill_wing->setText("");
        
        //副属性1
        Label* attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_1"));
        attr_1->setVisible(false);
        
        Label* attr_1_num = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_1_num"));
        attr_1_num->setVisible(false);
        
        //副属性2
        Label* attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_2"));
        attr_2->setVisible(false);
        
        Label* attr_2_num = static_cast<Label*>(m_pUILayer->getWidgetByName("wing_attribute_ext_2_num"));
        attr_2_num->setVisible(false);
    }
    
    UserData::shared()->setHeroHp(n_total_hp);
    
    //
    /********************
            总属性
     ********************/
    Label* total_hp = static_cast<Label*>(m_pUILayer->getWidgetByName("total_attribute_hp"));
    total_hp->setText(getStringFromInt(n_total_hp));
    
    //总血量掉率
    Label* total_blood_drop = static_cast<Label*>(m_pUILayer->getWidgetByName("total_attribute_ext_4_num"));
    //if(n_total_toolDrop != 0)
    {
        string str = getStringFromInt(n_total_bloodDrop).append("%");
        
        total_blood_drop->setText(str);
    }
    
    Label* total_attack = static_cast<Label*>(m_pUILayer->getWidgetByName("total_attribute_act"));
    total_attack->setText(getStringFromInt(n_total_attack));
    
    
    //总道具掉率
    Label* total_tool_drop = static_cast<Label*>(m_pUILayer->getWidgetByName("total_attribute_ext_3_num"));
    //if(n_total_toolDrop != 0)
    {
        string str = getStringFromInt(n_total_toolDrop).append("%");
        
        total_tool_drop->setText(str);
    }
    
    //总攻击加成
    Label* value_total_attr_1 = static_cast<Label*>(m_pUILayer->getWidgetByName("total_attribute_ext_1_num"));
    
    //if(n_total_attack_add != 0)
    {
        string str = "+";
        str.append(getStringFromInt(n_total_attack_add)).append("%");
        
        value_total_attr_1->setText(str);
    }
    
    //总血量加成
    Label* value_total_attr_2 = static_cast<Label*>(m_pUILayer->getWidgetByName("total_attribute_ext_2_num"));
    
    //if(n_total_hp_add != 0)
    {
        string str = "+";
        str.append(getStringFromInt(n_total_hp_add)).append("%");
        
        value_total_attr_2->setText(str);
    }
    
}

//初始化装备背包
void GameMainLayer::initEquipBag()
{
    refreshEquipBagAndSort();
    
    if (UserEquipBag::shared()->getArrEquipGetRecent()->count() != 0)
    {
        UserEquipBag::shared()->getArrEquipGetRecent()->removeAllObjects();
    }
}

//更新装备
void GameMainLayer::refreshEquipBagAndSort()
{
    
    ListView* pList_equip = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_equip"));
    
    pList_equip->removeAllItems();

    //先排序橙色
    insertEquipToBag(UserEquipBag::shared()->getEquipOrangeArr());

    //然后紫色
    insertEquipToBag(UserEquipBag::shared()->getEquipPupleArr());
    
    //然后蓝色
    insertEquipToBag(UserEquipBag::shared()->getEquipBlueArr());
    
    //然后绿色
    insertEquipToBag(UserEquipBag::shared()->getEquipGreenArr());
    
    //然后白色
    insertEquipToBag(UserEquipBag::shared()->getEquipWhiteArr());
    
}

//更新装备背包   不排序
void GameMainLayer::refreshEquipBagNoSort()
{
    insertEquipToBag(UserEquipBag::shared()->getArrEquipGetRecent());
}

//添加装备进入背包
void GameMainLayer::insertEquipToBag(CCArray* arr)
{
    ListView* pList_equip = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_equip"));
    if (pList_equip)
    {
        CCObject* obj = NULL;
        
        CCARRAY_FOREACH(arr, obj)
        {
            EquipItem* item = (EquipItem*)obj;
            
            if (item->getEquipType() == 0)
            {
                Button* btn = Button::create();
                
                btn->setName(item->getID().c_str());
                
                btn->addTouchEventListener(this, toucheventselector(GameMainLayer::kickEquipCallBack));
                
                string img_id = UserEquipBag::shared()->getIconPath(item);
                
                btn->loadTextures(img_id.c_str(), img_id.c_str(), img_id.c_str(),UI_TEX_TYPE_PLIST);
                
                pList_equip->pushBackCustomItem(btn);
            }
        }
    }
}

void GameMainLayer::btnLotteryCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        GameDelegate::shared()->openSceneLottery();
    }
}


void GameMainLayer::closeEquipDetailCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        if (node_equipDetail)
        {
            //将已选择的装备置空
            m_selectEquip = "0";
            
            m_bCanChange = false;
            
            unSelectEquip();
            
            m_bCanKickEquip = false;
            
            node_equipDetail->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-500, 0)),CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::actionfinish)),NULL));

        }
    }
}

void GameMainLayer::btnConfigureCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_HEAVY);
        
        //设置界面
        GameDelegate::shared()->openSceneConfig();
    }
}

void GameMainLayer::scrollToPage(int page)
{
    m_pPageView->scrollToPage(page);
}

void GameMainLayer::btnSignInCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_HEAVY);
        
        //签到界面
        GameDelegate::shared()->openSceneSignin();
        
    }
}

void GameMainLayer::actionfinish(cocos2d::CCObject *pSender)
{
    Layout* node = (Layout*)pSender;
    
    int tag = node->getTag();
    
    if (tag == kTagEquipGroup)
    {
        CCLog("add node detail");
        //m_pUILayer->removeChild(node_equipGroup);
        node_equipGroup->removeFromParentAndCleanup(true);
        node_equipGroup = NULL;
        equipDetailInit(true);
    }
    else if (tag == kTagEquipDetail)
    {
        CCLog("add node group");
        //m_pUILayer->removeChild(node_equipDetail);
        node_equipDetail->removeFromParentAndCleanup(true);
        node_equipDetail = NULL;
        equipGroupInit(true);
    }
    
    m_bCanKickEquip = true;
}

void GameMainLayer::kickEquipCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    
    if (!m_bCanKickEquip)
    {
        return;
    }
    
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        if (m_bCanChange)
        {
            //被替换的装备id
            string isChanged = "0";
            
            UIPanel* panel = (UIPanel*)pSender;
            
            if (panel)
            {
                if (compareString(panel->getName(), "panel_main"))
                {
                    isChanged = UserEquip::shared()->getMainGunID();
                }
                else if (compareString(panel->getName(), "panel_side_1"))
                {
                    isChanged = UserEquip::shared()->getFirSubGunID();
                }
                else if (compareString(panel->getName(), "panel_side_2"))
                {
                    isChanged = UserEquip::shared()->getSecSubGunID();
                }
                else if (compareString(panel->getName(), "panel_wing"))
                {
                    isChanged = UserEquip::shared()->getWingID();
                }
            }
            
            Button* btn = (Button*)pSender;
            if (btn)
            {
                isChanged = btn->getName();
            }
            
            CCLog("select equip : %s,change equip : %s",m_selectEquip.c_str(),isChanged.c_str());
            
            UserEquipBag::shared()->swapEquip(m_selectEquip, isChanged);
            
            //关闭装备详细界面
            closeEquipDetailCallBack(NULL, TOUCH_EVENT_ENDED);
            
            //更新主界面属性显示
            refreshAttributeShow();
            
            //更新背包
            refreshEquipBagAndSort();
            
            m_bCanChange = false;
            
            return;
        }
        
        Button* btn = (Button*)pSender;
        
        CCLog("Equip id : %s",btn->getName());
        
        //如果点击的位置没有装备  返回
        if (compareString(btn->getName(), "0"))
        {
            return;
        }
        else
        {
            //将选中效果加到按钮上
            unSelectEquip();
            
            ImageView* effect_select = ImageView::create();
            effect_select->loadTexture("ui/effect/effect_selectEquip.png");
            btn->addChild(effect_select, kZOrderEffectSelect, kTagEffectSelectEquip);
            
            //将点击的装备id,保存
            if(!compareString(m_selectEquip.c_str(), "0"))
            {
                //说明已经有装备被选中
                m_selectEquip = btn->getName();
                
                refreshEquipDetail();
                
                return;
            }
            else
            {
                m_selectEquip = btn->getName();
            }
            
            EquipItem* current_equip = UserEquipBag::shared()->getEquipById(m_selectEquip);
            
            if (current_equip->getEquipType() != 0)
            {
                CCSprite* effect_panel = CCSprite::create("ui/effect/effect_selectPanel.png");
                
                if (compareString(m_selectEquip.c_str(), UserEquip::shared()->getMainGunID().c_str()))
                {
                    effect_panel->setPosition(ccp(_s_width - 109, 657));
                }
                
                if (compareString(m_selectEquip.c_str(), UserEquip::shared()->getFirSubGunID().c_str()))
                {
                    effect_panel->setPosition(ccp(_s_width - 109, 525));
                }
                
                if (compareString(m_selectEquip.c_str(), UserEquip::shared()->getSecSubGunID().c_str()))
                {
                    effect_panel->setPosition(ccp(_s_width - 109, 393));
                }
                
                if (compareString(m_selectEquip.c_str(), UserEquip::shared()->getWingID().c_str()))
                {
                    effect_panel->setPosition(ccp(_s_width - 109, 266));
                }
                
                
                effect_panel->setScale(0.8f);
                
                this->addChild(effect_panel, kZOrderEffectSelect, kTagEffectSelectPanel);
                
                CCScaleTo* scaleto = CCScaleTo::create(0.5f, 1.0f);
                
                CCFadeTo* fadeto = CCFadeTo::create(0.5f, 0);
                
                CCSequence* seq = CCSequence::create(scaleto,fadeto,CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::actionSelectFinish)),NULL);
                
                CCRepeatForever* repeat = CCRepeatForever::create(seq);
                
                effect_panel->runAction(repeat);
            }
            
        }
        

        if (node_equipGroup)
        {
            m_bCanKickEquip = false;
            
            node_equipGroup->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-500, 0)),CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::actionfinish)),NULL));
        }
    }
}

void GameMainLayer::btnLeftCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_HEAVY);
        
        //如果当前是道具页 左侧是开始游戏右侧是装备界面
        if (nPage == 0)
        {
            GameDelegate::shared()->openSceneLoading();
        }
        else if (nPage == 1)
        {
            //如果当前是装备页 左侧是道具界面
            
            
            scrollToPage(0);
            setNPage(0);
            
            if (this->getChildByTag(kTagEffectSelectPanel))
            {
                this->removeChildByTag(kTagEffectSelectPanel);
            }
            
            if (this->getChildByTag(kTagEffectOverEquip))
            {
                this->removeChildByTag(kTagEffectOverEquip);
            }

        }
    }
}

void GameMainLayer::refreshButtonState(cocos2d::CCObject *pSender)
{

    if (getNPage() == 0)
    {
        btn_left->setTitleText("出击");
        btn_right->setTitleText("兵装库");
    
    }
    else
    {
        btn_left->setTitleText("道具");
        btn_right->setTitleText("出击");
    }

}

void GameMainLayer::btnRightCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_HEAVY);
        
        //如果当前是道具页 左侧是开始游戏右侧是装备界面
        if (nPage == 0)
        {
            m_pPageView->scrollToPage(1);
            setNPage(1);
            
            if (this->getChildByTag(kTagEffectSelectPanel))
            {
                this->removeChildByTag(kTagEffectSelectPanel);
            }
            
        }
        else if (nPage == 1)
        {
            //如果当前是装备页  右侧是开始游戏
            GameDelegate::shared()->openSceneLoading();
        }
    }
}

void GameMainLayer::btnBuyCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    CCLog("button clicked!");
    
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        
        if (tag == kTagBuyGold)
        {
            //打开金币购买界面
            GameDelegate::shared()->openSceneStoreLayer(kStore_gold);
        }
        else if (tag == kTagBuyGem)
        {
            //打开钻石购买界面
            GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
        }
        else if (tag == kTagBuyPower)
        {
            //打开购买体力的界面
            GameDelegate::shared()->openSceneStoreLayer(kStore_energy);
        }
    }
}

void GameMainLayer::keyBackClicked()
{
    //设置消息
    NDKHelper::AddSelector(EXITGAME_GROUP_NAME, EXITGAME_SELECTOR_NAME, NULL, this);
    
    CCDictionary* prms = CCDictionary::create();
    prms->setObject(CCString::create(EXITGAME_SELECTOR_NAME), "to_call");
    
    //调用java
    SendMessageWithParams(EXITGAME_SELECTOR_NAME, prms);
}