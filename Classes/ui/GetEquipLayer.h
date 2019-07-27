//
//  GetEquipLayer.h
//  ambition
//
//  Created by mini01 on 14-12-1.
//
//

#ifndef __ambition__GetEquipLayer__
#define __ambition__GetEquipLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalEnum.h"
#include "GameDelegate.h"
#include "UserEquipBag.h"
#include "StringLibrary.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;
using namespace std;

class GetEquipLayer : public CCLayer
{
public:
    GetEquipLayer();
    ~GetEquipLayer();
    
    virtual bool init(EquipItem* item,bool isShow = false);
    
    static GetEquipLayer* create(EquipItem* item,bool isShow = false);
    
    static GetEquipLayer* createWithGift(SignRewardType type,EquipItem* item = NULL);
    
    bool initWithGift(SignRewardType type,EquipItem* item = NULL);
    
private:
    TouchGroup* m_pUILayer;
    
    EquipItem* m_equipByLottery;
    
    void btnCloseCallBack(CCObject* pSender,TouchEventType type);
    
    void btnGetCallBack(CCObject* pSender,TouchEventType type);
    
    void btnGetGiftCallBack(CCObject* pSender,TouchEventType type);
    
};

#endif /* defined(__ambition__GetEquipLayer__) */
