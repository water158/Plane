//
//  GameBattleUILayer.cpp
//  ambition
//
//  Created by mini01 on 14-11-26.
//
//

#include "GameBattleUILayer.h"
#include "GameDelegate.h"

GameBattleUILayer::GameBattleUILayer()
{

}

GameBattleUILayer::~GameBattleUILayer()
{

}

bool GameBattleUILayer::init()
{
    CCLayer::init();
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_pauselayer = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/battleui_layer.ExportJson"));
    m_pUILayer->addWidget(root_pauselayer);
    
    UILoadingBar* bar_hp = static_cast<UILoadingBar*>(m_pUILayer->getWidgetByName("progressbar_hp"));
    bar_hp->setPercent(100);
    
    UILoadingBar* bar_mp = static_cast<UILoadingBar*>(m_pUILayer->getWidgetByName("progressbar_mp"));
    
    bar_mp->setPercent(0);

    Button* btn_pause = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_pause"));
    btn_pause->addTouchEventListener(this, toucheventselector(GameBattleUILayer::btnPauseCallBack));
    
    return true;
}

void GameBattleUILayer::btnPauseCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        GameDelegate::shared()->openScenePause();
    }
}

void GameBattleUILayer::setHpPercent(int percent)
{
    UILoadingBar* bar_hp = static_cast<UILoadingBar*>(m_pUILayer->getWidgetByName("progressbar_hp"));
    
    bar_hp->setPercent(percent);
}

void GameBattleUILayer::setWave(int nWave)
{
    Label* label_wave = static_cast<Label*>(m_pUILayer->getWidgetByName("label_wave"));
    
    label_wave->setText(getStringFromInt(nWave));
    
}

void GameBattleUILayer::setScore(int nWave)
{
    Label* label_score = static_cast<Label*>(m_pUILayer->getWidgetByName("label_score"));
    
    label_score->setText(getStringFromInt(nWave));
}

void GameBattleUILayer::setSkillPercent(int percent)
{
    UILoadingBar* bar_mp = static_cast<UILoadingBar*>(m_pUILayer->getWidgetByName("progressbar_mp"));
    
    bar_mp->setPercent(percent);
}

int GameBattleUILayer::getSkillPercent()
{
    UILoadingBar* bar_mp = static_cast<UILoadingBar*>(m_pUILayer->getWidgetByName("progressbar_mp"));
    
    return bar_mp->getPercent();
}

void GameBattleUILayer::setBoxNum(int num)
{
    if (num > 9)
    {
        num = 9;
    }
    
    Label* label_box = static_cast<Label*>(m_pUILayer->getWidgetByName("label_box_get"));
    
    label_box->setText(getStringFromInt(num));
}