//
//  GameStartLayer.cpp
//  ambition
//
//  Created by mini01 on 14-12-29.
//
//

#include "GameStartLayer.h"

GameStartLayer::GameStartLayer()
{

}

GameStartLayer::~GameStartLayer()
{
    m_pUILayer->removeFromParent();
    
    SceneReader::sharedSceneReader()->purge();
    GUIReader::shareReader()->purge();
    cocos2d::extension::ActionManager::shareManager()->purge();
}

bool GameStartLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_pUILayer = UILayer::create();
    
    addChild(m_pUILayer);
    
    Layout* root_startlayer = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/title_layer.ExportJson"));
    
    m_pUILayer->addWidget(root_startlayer);
    
     ActionManager::shareManager()->playActionByName("title_layer.ExportJson" , "Animation0" );
    
    img_bg = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("Panel_76"));
    
    img_bg->setTouchEnabled(true);
    
    img_bg->addTouchEventListener(this, toucheventselector(GameStartLayer::kickCallBack));
    
    ImageView* img_title = static_cast<ImageView*>(m_pUILayer->getWidgetByName("img_titile"));
    
    
    CCSprite* spr_test = CCSprite::create("ui/label/label_title.png");
    
     CCSprite* spr_effect = CCSprite::create("game/animations/baitiao.png");
    
    spr_effect->setPosition(ccp(-300, 0));
    
    CCMoveBy* move = CCMoveBy::create(0.5, ccp(600, 0));
    
    CCSequence* seq = CCSequence::create(CCDelayTime::create(2),move,CCCallFuncN::create(this, callfuncN_selector(GameStartLayer::moveActionFinish)),NULL);
    
    CCRepeatForever* repeat = CCRepeatForever::create(seq);
    
    spr_effect->runAction(repeat);
    
    //创建裁剪节点类
    clipper = CCClippingNode::create();
    clipper->setStencil(spr_test);
    clipper->setInverted(false);
    clipper->setAlphaThreshold(0);
    addChild(clipper,10);
    
    clipper->addChild(spr_effect);
    
    clipper->setPosition(ccp(_s_center.x, _s_center.y + 250));
    
    return true;
}

void GameStartLayer::kickCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    
    if (type == TOUCH_EVENT_ENDED)
    {
        
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_OPENDOOR);

        this->removeChild(clipper);
        
        UIPanel* panel = (UIPanel*)pSender;
        
        panel->setTouchEnabled(false);
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName("Image_22"));
        
        img->setVisible(false);
        
        ActionManager::shareManager()->playActionByName("title_layer.ExportJson" , "Animation1" ,CCCallFunc::create(this, callfunc_selector(GameStartLayer::openActionFinish)));
    }
}

void GameStartLayer::openActionFinish()
{
    
    this->removeFromParentAndCleanup(true);
    
    //如果是当日第一次打开游戏 弹出签到界面
    if (getCurrentTime(kGetTimeDay) != UserData::shared()->getLastLoginDay()
        || getCurrentTime(kGetTimeMonth) != UserData::shared()->getLastLoginMon())
    {
        GameDelegate::shared()->openSceneSignin();
    }
    
}

void GameStartLayer::moveActionFinish(cocos2d::CCObject *pSender)
{
    CCSprite* spr = (CCSprite*)pSender;
    
    spr->setPosition(ccp(-300, 0));
}