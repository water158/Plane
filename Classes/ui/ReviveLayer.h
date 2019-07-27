//
//  ReviveLayer.h
//  ambition
//
//  Created by mini01 on 14-12-22.
//
//

#ifndef __ambition__ReviveLayer__
#define __ambition__ReviveLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalEnum.h"
#include "UserData.h"
#include "ResultCountData.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

class ReviveLayer : public CCLayer
{
public:
    ReviveLayer();
    ~ReviveLayer();
    
    virtual bool init(ResultCountData* data);
    
    static ReviveLayer* create(ResultCountData* data);
    
    void update(float dt);
    
private:
    
    TouchGroup* m_pUILayer;
    
    void btnReviveCallBack(CCObject* pSender,TouchEventType type);
    
    int m_currentPercentage;
    
    
    ResultCountData* m_accoutData;
};

#endif /* defined(__ambition__ReviveLayer__) */
