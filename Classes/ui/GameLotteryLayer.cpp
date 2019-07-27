//
//  GameLotteryLayer.cpp
//  ambition
//
//  Created by mini01 on 14-11-29.
//
//

#include "GameLotteryLayer.h"

GameLotteryLayer::GameLotteryLayer()
{

}

GameLotteryLayer::~GameLotteryLayer()
{

}

bool GameLotteryLayer::init()
{
    CCLayer::init();
    
    CCLayerColor* colorlayer = CCLayerColor::create(ccc4(0, 0, 0, 200.0f),_s_width,_s_height * 2);
    
    addChild(colorlayer);
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    UserEquipBag::shared()->getArrEquipGetRecent()->removeAllObjects();
    
    Layout* root_lotteryLayer = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/lottery_layer.ExportJson"));
    m_pUILayer->addWidget(root_lotteryLayer);
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    
    btn_close->setPressedActionEnabled(false);
    
    btn_close->addTouchEventListener(this, toucheventselector(GameLotteryLayer::btnLotteryCallBack));
    
    
    Button* btn_lotteryCoin = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_lottery_1"));
    
    btn_lotteryCoin->addTouchEventListener(this, toucheventselector(GameLotteryLayer::btnLotteryUseGold));
    
    Button* btn_lotteryFewGem = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_lottery_2"));
    
    btn_lotteryFewGem->addTouchEventListener(this, toucheventselector(GameLotteryLayer::btnLotteryUseFewGem));
    
    Button* btn_lotteryManyGem = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_lottery_3"));
    
    btn_lotteryManyGem->addTouchEventListener(this, toucheventselector(GameLotteryLayer::btnLotteryUseManyGem));
    
    return true;
}

void GameLotteryLayer::btnLotteryCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        this->runAction(CCSequence::create(CCMoveBy::create(0.5, ccp(0, -600)),CCCallFunc::create(this, callfunc_selector(GameLotteryLayer::actionFinish)),NULL));
    }
}

void GameLotteryLayer::actionFinish()
{
    GameDelegate::shared()->closeSceneLottery();
}

void GameLotteryLayer::btnLotteryUseGold(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        if (!UserData::shared()->isGoldEnough(10000))
        {
            GameDelegate::shared()->openSceneStoreLayer(kStore_gold);
            
            return;
        }
        
        DailyCountData::sharedInstance()->addDailyLotteryTime();
        
        DailyCountData::sharedInstance()->save();
        
        DailyTask::sharedInstance()->save();
        EquipItem* item = GetRandomEquip::shared()->getEquipGoldLottery();
        
        item->setID(getStringFromInt(UserEquipBag::shared()->getMinID()));
        
        GetEquipLayer* layer = GetEquipLayer::create(item);
        
        addChild(layer);
        
        UserData::shared()->decreaseGoldNum(10000);
        
        UserData::shared()->save();
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESHGOLD);
    }
}

void GameLotteryLayer::btnLotteryUseFewGem(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        //钻石不足弹出购买钻石界面
        if (!UserData::shared()->isGemEnough(10))
        {
            GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
            
            return;
        }
        
        DailyCountData::sharedInstance()->addDailyLotteryTime();
        
        DailyCountData::sharedInstance()->save();
        
        DailyTask::sharedInstance()->save();
        
        GameCountData::sharedInstance()->addConsumeGem(10);
        
        GameCountData::sharedInstance()->save();
        
        Achieve::getInstance()->save();
        
        EquipItem* item = GetRandomEquip::shared()->getEquipFewGemLottery();
        
        item->setID(getStringFromInt(UserEquipBag::shared()->getMinID()));
        
        GetEquipLayer* layer = GetEquipLayer::create(item);
        
        addChild(layer);
        
        UserData::shared()->decreaseGemNum(10);
        
        UserData::shared()->save();
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESHGOLD);
    }
}

void GameLotteryLayer::btnLotteryUseManyGem(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        //钻石不足 弹出购买钻石界面
        if (!UserData::shared()->isGemEnough(50))
        {
            GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
            
            return;
        }
        
        DailyCountData::sharedInstance()->addDailyLotteryTime();
        
        DailyCountData::sharedInstance()->save();
        
        DailyTask::sharedInstance()->save();
        
        GameCountData::sharedInstance()->addConsumeGem(50);
        
        GameCountData::sharedInstance()->save();
        
        Achieve::getInstance()->save();
        
        EquipItem* item = GetRandomEquip::shared()->getEquipManyGemLottery();
        
        item->setID(getStringFromInt(UserEquipBag::shared()->getMinID()));
        
        GetEquipLayer* layer = GetEquipLayer::create(item);
        
        addChild(layer);
        
        UserData::shared()->decreaseGemNum(50);
        
        UserData::shared()->save();
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESHGOLD);
    }
}


