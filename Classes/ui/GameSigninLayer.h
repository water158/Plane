//
//  GameSigninLayer.h
//  ambition
//
//  Created by mini01 on 14-11-24.
//
//

#ifndef __ambition__GameSigninLayer__
#define __ambition__GameSigninLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameDelegate.h"
#include "SmartRes.h"
#include "GlobalEnum.h"
#include "SignManager.h"
#include "UserData.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

#define TAG_SIGIN_DAY 10000
#define TAG_EFFECT_SELECT 100

class GameSigninLayer : public CCLayer
{
public:
    GameSigninLayer();
    ~GameSigninLayer();
    
    CREATE_FUNC(GameSigninLayer);
    
    virtual bool init();
    
    void initUI();
    
private:
    TouchGroup* m_pUILayer;
    
    void refreshReward(SignReward* reward);
    
    void btnCloseCallBack(CCObject* pSender,TouchEventType type);
    
    void btnSignCallBack(CCObject* pSender,TouchEventType type);
    
    void kickOneDayCallBack(CCObject* pSender,TouchEventType type);
};

#endif /* defined(__ambition__GameSigninLayer__) */
