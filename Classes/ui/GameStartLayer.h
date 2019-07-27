//
//  GameStartLayer.h
//  ambition
//
//  Created by mini01 on 14-12-29.
//
//

#ifndef __ambition__GameStartLayer__
#define __ambition__GameStartLayer__

#include "cocos2d.h"

#include "cocos-ext.h"

#include "SoundEffectManager.h"
#include "GlobalEnum.h"
#include "UserData.h"
#include "GameDelegate.h"
#include "SmartRes.h"

USING_NS_CC;
USING_NS_CC_EXT;

USING_NS_TIMELINE;

#define TAG_CLIPPINGNODE 100

using namespace cocos2d::ui;

class GameStartLayer : public CCLayer
{
public:
    GameStartLayer();
    ~GameStartLayer();
    
    virtual bool init();
    
    CREATE_FUNC(GameStartLayer);
    
    void kickCallBack(CCObject* pSender,TouchEventType type);
    
    void openActionFinish();
    
    void moveActionFinish(CCObject* pSender);
    
private:
    
    UILayer* m_pUILayer;
    
    UIPanel* img_bg;
    
    CCClippingNode* clipper;
};

#endif /* defined(__ambition__GameStartLayer__) */
