//
//  GameAccountLayer.h
//  ambition
//
//  Created by mini01 on 14-12-1.
//
//

#ifndef __ambition__GameAccountLayer__
#define __ambition__GameAccountLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalEnum.h"
#include "GameDelegate.h"
#include "ResultCountData.h"
#include "UserData.h"
#include "tinyxml.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;

#define TAG_BTN_BOX 1000

using namespace cocos2d::ui;

using namespace std;

class RewardBox : public CCNode
{
public:
    RewardBox();
    
    ~RewardBox();
    
    virtual bool init(TiXmlElement *elm);
    
    static RewardBox* create(TiXmlElement *elm);
    
private:
    
    CC_SYNTHESIZE(string, m_sID, ID);
    
    CC_SYNTHESIZE(RewardTag, m_tRewardTag, TagOfReward);
    
    CC_SYNTHESIZE(int, m_nType, Type);
    
    CC_SYNTHESIZE(int, m_nValue, Value);
    
    CC_SYNTHESIZE(string, m_sImgID, ImgID);
};

class GameAccountLayer : public CCLayer
{
public:
    GameAccountLayer();
    ~GameAccountLayer();
    
    virtual bool init(ResultCountData* data);
    
    static GameAccountLayer* create(ResultCountData* data);
    
private:
    TouchGroup *m_pUILayer;
    
    void btnCloseCallBack(CCObject* pSender,TouchEventType type);
    
    //void btnShareCallBack(CCObject* pSender,TouchEventType type);
    
    void btnBoxCallBack(CCObject* pSender,TouchEventType type);
    
    void actionFinish(CCObject* pSender);
    
    int m_nBoxPrice;
    
    ResultCountData* m_resultData;
    
    //初始化奖励数组
    void initReward();
    
    //保存每个箱子奖励的数组
    CCArray* m_pArrRewardBox;
};

#endif /* defined(__ambition__GameAccountLayer__) */
