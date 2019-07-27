//
//  EnergyStoreLater.h
//  ambition
//
//  Created by mini01 on 14-12-9.
//
//

#ifndef __ambition__EnergyStoreLayer__
#define __ambition__EnergyStoreLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalEnum.h"
#include "GameDelegate.h"
#include "UserData.h"
#include "StorePriceData.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;

using namespace cocos2d::ui;


class EnergyStoreLayer : public CCLayer
{
public:
    EnergyStoreLayer();
    ~EnergyStoreLayer();
    
    CREATE_FUNC(EnergyStoreLayer);
    
    virtual bool init();
    
private:
    
    TouchGroup* m_pUILayer;
    
    void btnBuyCallBack(CCObject* pSender,TouchEventType type);
    
    void btnCloseCallBack(CCObject* pSender,TouchEventType type);
    
    
    
    //定时器用来更新倒计时
    void update(float dt);
    
    int m_currTime;
    
};


#endif /* defined(__ambition__EnergyStoreLater__) */
