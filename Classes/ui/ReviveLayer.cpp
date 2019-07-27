//
//  ReviveLayer.cpp
//  ambition
//
//  Created by mini01 on 14-12-22.
//
//

#include "ReviveLayer.h"
#include "GameDelegate.h"

ReviveLayer::ReviveLayer()
{

}

ReviveLayer::~ReviveLayer()
{

}

ReviveLayer* ReviveLayer::create(ResultCountData* data)
{
    ReviveLayer* layer = new ReviveLayer();
    if (layer && layer->init(data))
    {
        layer->autorelease();
        
        return layer;
    }
    
    return NULL;
}

bool ReviveLayer::init(ResultCountData* data)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    this->getScheduler()->setTimeScale(1.0f);
    
    m_currentPercentage = 100;
    
    m_pUILayer = TouchGroup::create();
    
    addChild(m_pUILayer);
    
    m_accoutData = data;
    
    Layout* root_revive = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/reborn_layer.ExportJson"));
    
    m_pUILayer->addWidget(root_revive);
    
    Button* btn_revive = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_reborn"));
    
    btn_revive->addTouchEventListener(this, toucheventselector(ReviveLayer::btnReviveCallBack));
    
    LoadingBar* bar_time = static_cast<LoadingBar*>(m_pUILayer->getWidgetByName("ProgressBar_time"));
    
    bar_time->setPercent(m_currentPercentage);
    
    this->schedule(schedule_selector(ReviveLayer::update), 0.1f);
    
    int price = (UserData::shared()->getReviveTimes() + 1) * PRICE_REVIVE;
    
    Label* label_price = static_cast<Label*>(m_pUILayer->getWidgetByName("gem_reborn"));
    
    label_price->setText(getStringFromInt(price));
    
    return true;
}

void ReviveLayer::update(float dt)
{
    m_currentPercentage--;
    
    if (m_currentPercentage <= 0)
    {
        this->unscheduleAllSelectors();
        
        GameDelegate::shared()->closeSceneRevive();
        
        GameDelegate::shared()->openSceneAccount(m_accoutData);
    }
    
    LoadingBar* bar_time = static_cast<LoadingBar*>(m_pUILayer->getWidgetByName("ProgressBar_time"));
    
    bar_time->setPercent(m_currentPercentage);
}

void ReviveLayer::btnReviveCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        int price = (UserData::shared()->getReviveTimes() + 1) * PRICE_REVIVE;
        
        UserData::shared()->decreaseGemNum(price);
        
        GameCountData::sharedInstance()->addConsumeGem(price);
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REVIVE);
        
        GameDelegate::shared()->closeSceneRevive();
    }
}