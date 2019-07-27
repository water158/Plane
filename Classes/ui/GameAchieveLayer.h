//
//  AchieveLayer.h
//  ambition
//
//  Created by mini01 on 14-11-24.
//
//

#ifndef __ambition__AchieveLayer__
#define __ambition__AchieveLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameDelegate.h"
#include "SmartRes.h"
#include "GlobalEnum.h"
#include "Achieve.h"
#include "DailyTask.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

class GameAchieveLayer : public CCLayer
{
public:
    GameAchieveLayer();
    ~GameAchieveLayer();
    
    virtual bool init();
    CREATE_FUNC(GameAchieveLayer);
    
private:
    TouchGroup* m_pUILayer;
    
    //购买按钮回调
    void btnBuyCallBack(CCObject* pSender,TouchEventType type);
    
    //左侧按钮回调
    void btnLeftCallBack(CCObject* pSender,TouchEventType type);
    //右侧按钮回调
    void btnRightCallBack(CCObject* pSender,TouchEventType type);
    //抽奖按钮回调
    void btnLotteryCallBack(CCObject* pSender,TouchEventType type);
    
    //更新按钮状态
    void refreshBtnState(int tag);
    
    //成就按钮回调
    void btnAchieveCallBack(CCObject* pSender,TouchEventType type);
    
    //滑动页回调
    void pageviewEnent(CCObject* pSender,PageViewEventType type);
    
    void initAchieve();
    
    void initDailyReward();
    
    void initCountReward();
    
    //领取按钮回调
    void btnGetCallBack(CCObject* pSender,TouchEventType type);
    
    //奖励项的panel回调
    void panelRewardCallBack(CCObject* pSender,TouchEventType type);
    
    //添加一个新的listitem 用成就
    void initNewListItem(AchieveItem* item,achieve_type type);
    
    //更新成就界面
    void refreshUI();
    
    //刷新金币
    void refreshGoldInfo();
    
    PageView* m_pPageview;
};

#endif /* defined(__ambition__AchieveLayer__) */
