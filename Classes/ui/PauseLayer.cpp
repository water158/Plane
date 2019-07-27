//
//  PauseLayer.cpp
//  ambition
//
//  Created by mini01 on 14-11-26.
//
//

#include "PauseLayer.h"
#include "GameDelegate.h"

PauseLayer::PauseLayer()
{

}

PauseLayer::~PauseLayer()
{

}

bool PauseLayer::init()
{
    CCLayer::init();
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_pauselayer = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/pause_layer.ExportJson"));
    m_pUILayer->addWidget(root_pauselayer);
    
    Button* btn_continue = static_cast<Button* >(m_pUILayer->getWidgetByName("btn_continue"));
    btn_continue->addTouchEventListener(this, toucheventselector(PauseLayer::btnContinueCallBack));
    
    Button* btn_exit = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_exit"));
    btn_exit->addTouchEventListener(this, toucheventselector(PauseLayer::btnExitCallBacl));
    
    return true;
}

void PauseLayer::btnContinueCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        GameDelegate::shared()->closeScenePause();
    }
}

void PauseLayer::btnExitCallBacl(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlayBackgroundMisc(MIS_MAIN_BG);

        GameDelegate::shared()->closeSceneRevive();
        
        GameDelegate::shared()->closeScenePause();

        GameDelegate::shared()->openSceneTools(false);
        
        GameCountData::sharedInstance()->save();
        
        DailyCountData::sharedInstance()->save();
        
        Achieve::getInstance()->save();
        
        DailyTask::sharedInstance()->save();
        
        UserEquipBag::shared()->save();
        
        UserData::shared()->save();
    }
}


