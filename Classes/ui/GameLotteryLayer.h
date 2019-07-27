//
//  GameLotteryLayer.h
//  ambition
//
//  Created by mini01 on 14-11-29.
//
//

#ifndef __ambition__GameLotteryLayer__
#define __ambition__GameLotteryLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameDelegate.h"
#include "GlobalEnum.h"
#include "SoundEffectManager.h"
#include "GetRandomEquip.h"
#include "GetEquipLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;

using namespace cocos2d::ui;

class GameLotteryLayer : public CCLayer
{
public:
    GameLotteryLayer();
    ~GameLotteryLayer();
    
    virtual bool init();
    
    CREATE_FUNC(GameLotteryLayer);
    
private:
    
    TouchGroup* m_pUILayer;
    
    void btnLotteryCallBack(CCObject* pSender,TouchEventType type);
    
    void btnLotteryUseGold(CCObject* pSender,TouchEventType type);
    
    void btnLotteryUseFewGem(CCObject* pSender,TouchEventType type);
    
    void btnLotteryUseManyGem(CCObject* pSender,TouchEventType type);
    
    void actionFinish();
    
};

#endif /* defined(__ambition__GameLotteryLayer__) */
