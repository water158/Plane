//
//  LoadingLayer.cpp
//
//
//  Created on 14-4-16.
//
//

#include "LoadingLayer.h"

LoadingLayer::LoadingLayer()
{

}

LoadingLayer::~LoadingLayer()
{
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("game/hero/fuckingplane_final.ExportJson");
}

bool LoadingLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCSprite* spr_bg = CCSprite::create("ui/panel/bg_mainmenu1.png");
    
    float scale = CCDirector::sharedDirector()->getVisibleSize().width / 540.0;
    
    spr_bg->setScale(scale);
    
    spr_bg->setPosition(_s_center);
    
    addChild(spr_bg);
    
    
    m_pUILayer = TouchGroup::create();
    
    addChild(m_pUILayer);
    
    Layout* root_loading = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/loading_layer.ExportJson"));
    
    m_pUILayer->addWidget(root_loading);
    
    ActionManager::shareManager()->playActionByName("loading_layer.ExportJson", "Animation0");
    
    //加载动画
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("game/hero/fuckingplane_final.ExportJson");
    
    CCArmature* armation = CCArmature::create("fuckingplane_final");
    
    armation->setAnchorPoint(ccp(0.5, 0.5));
    armation->setPosition(ccp(_s_center.x, _s_center.y - 170));
    
    armation->setScale(0.55f);

    
    this->setContentSize(CCSizeMake(armation->getContentSize().width, armation->getContentSize().height));
    
    this->addChild(armation, 0, kTagShape);
    
    armation->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(LoadingLayer::transformFinish));
    
    armation->getAnimation()->play("type_7");
    
    return true;
}

void LoadingLayer::transformFinish(CCArmature *a, MovementEventType b, std::string c)
{
    if (b == COMPLETE)
    {
        //m_pUILayer->setVisible(false);
        //GameDelegate::shared()->openSceneTools(false);
        GameDelegate::shared()->openSceneCombat();
        
        a->removeFromParentAndCleanup(true);
        
    }
}

