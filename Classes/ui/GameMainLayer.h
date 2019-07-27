//
//  GameMenuLayer.h
//  ambition
//
//  Created by mini01 on 14-11-18.
//
//

#ifndef __ambition__GameMainLayer__
#define __ambition__GameMainLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SmartRes.h"
#include "GameDelegate.h"
#include "GlobalTagZOrder.h"
#include "GlobalEnum.h"
#include "UserData.h"
#include "UserEquipBag.h"
#include "GameStartLayer.h"
#include "SoundEffectManager.h"
#include "FerrisDialog.h"
#include "NDKHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;
using namespace std;


enum
{
    kTagBuyPower = 10,
    kTagBuyGold,
    kTagBuyGem,
};

#define EXITGAME_GROUP_NAME "ExitGameGroupName"
#define EXITGAME_SELECTOR_NAME "ExitGame"

#define TAG_EQUIP_MAXLEVEL 2000


class GameMainLayer : public CCLayer
{
public:
    
    GameMainLayer();
    ~GameMainLayer();
    
    virtual void onEnter();

    static GameMainLayer* create(bool hasAnimation);
    
    virtual bool init(bool hasAnimation);
    
    void refreshButtonState(CCObject* pSender);
    
    virtual void keyBackClicked();
private:
    TouchGroup* m_pUILayer;
    
    bool m_bCanKickEquip;
    
    //滑动到指定页
    void scrollToPage(int page);
    
    //购买按钮回调
    void btnBuyCallBack(CCObject* pSender,TouchEventType type);
    //成就按钮回调
    void btnAchieveCallBack(CCObject* pSender,TouchEventType type);
    //左侧按钮回调
    void btnLeftCallBack(CCObject* pSender,TouchEventType type);
    //右侧按钮回调
    void btnRightCallBack(CCObject* pSender,TouchEventType type);
    //抽奖按钮回调
    void btnLotteryCallBack(CCObject* pSender,TouchEventType type);
    
    //签到按钮回调
    void btnSignInCallBack(CCObject* pSender,TouchEventType type);
    //设置按钮回调
    void btnConfigureCallBack(CCObject* pSender,TouchEventType type);
    
    //装备组节点初始化
    void equipGroupInit(bool runAction);
    
    //装备详细界面初始化
    void equipDetailInit(bool runAction);
    
    //节点移除动作完成回调
    void actionfinish(CCObject* pSender);
    
    //选中动作完成回调
    void actionSelectFinish(CCObject* pSender);
    
    //装备点击回调
    void kickEquipCallBack(CCObject* pSender,TouchEventType type);
    
    //滑动回调
    void pageViewEvent(CCObject *pSender, PageViewEventType type);
    
    //装备详细关闭回调
    void closeEquipDetailCallBack(CCObject* pSender, TouchEventType type);
    
    //主装备的属性显示panel点击回调
    void attributePanelCallBack(CCObject* pSender,TouchEventType type);
    
    //更换装备按钮回调
    void changeEquipCallBack(CCObject* pSender,TouchEventType type);
    
    /*************** 效果 ***************/
    CCSprite* addEffectForChange(CCObject* obj);
    
    /***************计时器 用于体力恢复 *********/
    void updateEnergy(float dt);
    
    int m_currertTime;
    
    /******************道具********************/
    void initToolsUI();
    
    
    void buyToolCallBack(CCObject* pSender,TouchEventType type);
    
    void refreshToolsNum();
    
     /******************装备********************/
    //重铸装备弹出确认框
    void callBackDialogRecast(CCObject* pSender,TouchEventType type);
    
    //弹出装备出售确认框
    void callBackDialogSell(CCObject* pSender,TouchEventType type);
    
    //出售装备
    void sellEquipCallBack(CCObject* pSender,TouchEventType type);
    
    //升级装备
    void upgradeEquipCallBack(CCObject* pSender,TouchEventType type);
    
    //重铸装备
    void recastEquipCallBack(CCObject* pSender,TouchEventType type);
    
    //升级所需金币
    int upgradeRequireGold(int equip_grade,int equip_level);
    
    //出售可获得金币
    int getGoldBySellEquip(int equip_grade,int equip_level);
    
    //取消之前选中的装备
    void unSelectEquip();
    
    //初始化装备背包
    void initEquipBag();
    
    //更新装备背包   不排序
    void refreshEquipBagNoSort();
    
    //更新装备背包   排序
    void refreshEquipBagAndSort();
    
    //添加装备进入背包
    void insertEquipToBag(CCArray* arr);
    
    
    //升级动作完成回调
    void upgradeActionFinish(CCObject* pSender);
    
    //升级后更新装备详细属性    
    void refreshEquipDetail();
    
    /******************刷新金币等等********************/
    
    //刷新金币
    void refreshGoldInfo(CCObject* pSender);
    
    //刷新成就按钮是否有小红点   即：是否有新成就完成 未领取奖励
    void refreshBtnAchieveInfo();

    
    //更新界面装备属性
    void refreshAttributeShow();
    
    //更新签到按钮动画
    void refreshSigninEffect();
    
    //当前页码
    CC_SYNTHESIZE(int, nPage, NPage);
    
    PageView* m_pPageView;
    
    Button* btn_left;
    
    Button* btn_right;
    
    //装备组节点
    Layout* node_equipGroup;
    //装备详细节点
    Layout* node_equipDetail;
    
    //当前选择的装备
    string m_selectEquip;
    
    //是否可更换
    bool m_bCanChange;
    
    //强化次数
    int m_nStrengthEquip;
    
    //出售装备次数
    int m_nSellEquip;
    
    //总攻击加成
    int n_total_attack_add;
    
    //总血量加成
    int n_total_hp_add;
    
    //总道具掉率
    int n_total_toolDrop;
    
    //总血量掉率
    int n_total_bloodDrop;
    
    
    //用于更新界面中总属性的函数
    void refreshTotalAtt(int att_id,int value,float percent);
    
};

#endif /* defined(__ambition__GameMenuLayer__) */
