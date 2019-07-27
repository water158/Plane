//
//  PauseLayer.h
//  ambition
//
//  Created by mini01 on 14-11-26.
//
//

#ifndef __ambition__PauseLayer__
#define __ambition__PauseLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalEnum.h"
#include "SoundEffectManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

class PauseLayer : public CCLayer
{
public:
    PauseLayer();
    ~PauseLayer();
    
    virtual bool init();
    
    CREATE_FUNC(PauseLayer);
    
private:
    TouchGroup* m_pUILayer;
    
    void btnContinueCallBack(CCObject* pSender,TouchEventType type);
    
    void btnExitCallBacl(CCObject* pSender,TouchEventType type);
};

#endif /* defined(__ambition__PauseLayer__) */
