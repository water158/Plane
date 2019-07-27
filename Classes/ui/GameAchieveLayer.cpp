//
//  AchieveLayer.cpp
//  ambition
//
//  Created by mini01 on 14-11-24.
//
//

#include "GameAchieveLayer.h"

GameAchieveLayer::GameAchieveLayer()
{

}

GameAchieveLayer::~GameAchieveLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    m_pUILayer->removeFromParent();
}

bool GameAchieveLayer::init()
{
    CCLayer::init();
    
    //读取 成就图标的文件
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/achieve/achieve.plist");

    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_achieve = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/achieve_layer.ExportJson"));
    m_pUILayer->addWidget(root_achieve);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameAchieveLayer::refreshGoldInfo), MSG_REFRESHGOLD, NULL);
    
    //初始化金币 信息
    LabelAtlas* label_gold = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gold"));
    label_gold->setStringValue(getStringFromInt(UserData::shared()->getGoldNum()).c_str());
    
    LabelAtlas* label_gem = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gem"));
    
    label_gem->setStringValue(getStringFromInt(UserData::shared()->getGemNum()).c_str());
    
    LabelAtlas* label_energy = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_energy"));
    
    label_energy->setStringValue(getStringFromInt(UserData::shared()->getEnergyNum()));
    
    Button* btn_left = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_left"));
    btn_left->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnLeftCallBack));
    
    Button* btn_right = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_right"));
    btn_right->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnRightCallBack));
    
    Button* btn_lottery = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_lottery"));
    btn_lottery->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnLotteryCallBack));
    
    Button* btn_buyenergy = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_buypower"));
    btn_buyenergy->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnBuyCallBack));
    
    Button* btn_buygold = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_buygold"));
    btn_buygold->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnBuyCallBack));
    
    Button* btn_buygem = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_buygem"));
    btn_buygem->addTouchEventListener(this,toucheventselector(GameAchieveLayer::btnBuyCallBack));
    
    ImageView* btn_dailytask = static_cast<ImageView*>(m_pUILayer->getWidgetByName("btn_daily"));
    btn_dailytask->setTouchEnabled(true);
    btn_dailytask->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnAchieveCallBack));

    ImageView* btn_achieve = static_cast<ImageView*>(m_pUILayer->getWidgetByName("btn_achieve"));
    btn_achieve->setTouchEnabled(true);
    btn_achieve->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnAchieveCallBack));
    
    ImageView* btn_countAchieve = static_cast<ImageView*>(m_pUILayer->getWidgetByName("btn_count"));
    btn_countAchieve->setTouchEnabled(true);
    btn_countAchieve->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnAchieveCallBack));
    
    //更新按钮状态 默认显示是成就
    refreshBtnState(0);
    
    //播放抽奖按钮效果
    ActionManager::shareManager()->playActionByName("achieve_layer.ExportJson", "Animation0");
    
    m_pPageview = static_cast<PageView*>(m_pUILayer->getWidgetByName("PageView_achieve"));
    m_pPageview->addEventListenerPageView(this, pagevieweventselector(GameAchieveLayer::pageviewEnent));
    
    m_pPageview->setTouchEnabled(false);
    m_pPageview->getPage(0)->setTouchEnabled(false);
    m_pPageview->getPage(1)->setTouchEnabled(false);
    m_pPageview->getPage(2)->setTouchEnabled(false);
    
    initAchieve();
    initDailyReward();
    initCountReward();
    
    return true;
}

void GameAchieveLayer::initAchieve()
{

    //先遍历各个数组中已完成，领取过得放在最后面的位置
    CCObject* obj = NULL;
    //是否有成就正在显示
    bool isDisplay = false;
    
    //遍历数组，将已完成，未领取的放在最前面
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_mileage, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item,achieve_game);
            
            isDisplay = true;
            
            break;
        }
    }

    if (!isDisplay)
    {
        //遍历数组，将未完成的放在中间的位置
        CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_mileage, obj)
        {
            AchieveItem* item = (AchieveItem*)obj;
            
            if (!item->getIsFinish())
            {
                
                initNewListItem(item,achieve_game);
                
                break;
            }
        }
    }
    
    //击杀成就
    
    isDisplay = false;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_shatter, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item,achieve_game);
            
            break;
        }
    }
    
    if (!isDisplay)
    {
        CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_shatter, obj)
        {
            AchieveItem* item = (AchieveItem*)obj;
            
            if (!item->getIsFinish())
            {
                
                initNewListItem(item,achieve_game);
                
                break;
            }
        }
    }

    //道具成就
    
    isDisplay = false;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_tools, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item,achieve_game);
            
            break;
        }
    }
    
    if (!isDisplay)
    {
        CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_tools, obj)
        {
            AchieveItem* item = (AchieveItem*)obj;
            
            if (!item->getIsFinish())
            {
                
                initNewListItem(item,achieve_game);
                
                break;
            }
        }
    }
    
    //装备成就
    
    isDisplay = false;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_equipment, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item,achieve_game);
            
            break;
        }
    }
    
    if (!isDisplay)
    {
        CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_equipment, obj)
        {
            AchieveItem* item = (AchieveItem*)obj;
            
            if (!item->getIsFinish())
            {
                
                initNewListItem(item,achieve_game);
                
                break;
            }
        }
    }
    
    //强化等级成就
    
    isDisplay = false;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_level, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item,achieve_game);
            
            break;
        }
    }
    
    if (!isDisplay)
    {
        CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_level, obj)
        {
            AchieveItem* item = (AchieveItem*)obj;
            
            if (!item->getIsFinish())
            {
                
                initNewListItem(item,achieve_game);
                
                break;
            }
        }
    }
    
    //充值成就
    
    isDisplay = false;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_pay, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item,achieve_game);
            
            break;
        }
    }
    
    if (!isDisplay)
    {
        CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_pay, obj)
        {
            AchieveItem* item = (AchieveItem*)obj;
            
            if (!item->getIsFinish())
            {
                
                initNewListItem(item,achieve_game);
                
                break;
            }
        }
    }

    //消费成就
    
    isDisplay = false;
    
    CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_consume, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item,achieve_game);
            
            break;
        }
    }
    
    if (!isDisplay)
    {
        CCARRAY_FOREACH(Achieve::getInstance()->achieveArr_consume, obj)
        {
            AchieveItem* item = (AchieveItem*)obj;
            
            if (!item->getIsFinish())
            {
                
                initNewListItem(item,achieve_game);
                
                break;
            }
        }
    }
    
    ListView* root_list = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_achieve"));
    
    if (root_list)
    {
        root_list->removeLastItem();
    }
    
}

void GameAchieveLayer::initNewListItem(AchieveItem *item,achieve_type type)
{
    
    ListView* root_list = NULL;
    
    if (type == achieve_game)
    {
        root_list = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_achieve"));
    }
    else
    {
        root_list = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_daily"));
    }
    
    if (!root_list)
    {
        return;
    }
    
    
    UIPanel* default_item = static_cast<UIPanel*>(root_list->getItem(0));
    
    default_item->setColor(ccRED);
    default_item->setBright(true);
    
    root_list->setItemModel(default_item);
    
    
    root_list->insertDefaultItem(0);
    
    
    UIPanel* list_item = static_cast<UIPanel*>(root_list->getItem(0));
    
    list_item->setTouchEnabled(true);
    
    list_item->addTouchEventListener(this, toucheventselector(GameAchieveLayer::panelRewardCallBack));
    
    UILabel* label_name = static_cast<UILabel*>(list_item->getChildByName("Label_name"));
    
    label_name->setText(item->getName());
    
    UILabel* label_reward = static_cast<UILabel*>(list_item->getChildByName("Label_reward"));
    label_reward->setText(item->getRewardDes());
    
    UILabel* label_descripe = static_cast<UILabel*>(list_item->getChildByName("Label_decripe"));
    
    if (label_descripe)
    {
        label_descripe->setText(item->getDescribe());
    }

    
    ImageView* icon_achieve = static_cast<ImageView*>(list_item->getChildByName("img_icon"));
    
    string strPath = "";
    
    if (type == achieve_game)
    {
        strPath = "achieve_";
    }
    else
    {
        strPath = "daily_";
    }
    
    strPath.append(item->getIconID()).append(".png");
    
    icon_achieve->loadTexture(strPath.c_str(),UI_TEX_TYPE_PLIST);
    

    LabelAtlas* icon_num = static_cast<LabelAtlas*>(icon_achieve->getChildByName("AtlasLabel_num"));
    
    if (icon_num)
    {
        icon_num->setStringValue(item->getIconNo().c_str());
    }
    
    Button* btn_get = static_cast<Button*>(list_item->getChildByName("btn_get_ach"));
    
    //将按钮的tag值设置为 奖励的type * 1000 + id的数值
    int tag_btnGet = item->getType() * 1000 + atoi(item->getID().c_str());
    
    btn_get->setTag(tag_btnGet);
    
    if (item->getIsFinish())
    {
        btn_get->setTouchEnabled(true);
        btn_get->addTouchEventListener(this, toucheventselector(GameAchieveLayer::btnGetCallBack));
    }
    else
    {
        btn_get->setColor(ccc3(49, 0, 99));
    }
    
    if (item->getIsGet())
    {
        btn_get->setColor(ccc3(49, 66, 99));
        
        btn_get->setTouchEnabled(false);
    }
    
    
}

void GameAchieveLayer::initDailyReward()
{
 
    
    //先遍历各个数组中已完成，领取过得放在最后面的位置
    CCObject* obj = NULL;
    CCARRAY_FOREACH(DailyTask::sharedInstance()->arr_dailyTask, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsGet())
        {
            initNewListItem(item, achieve_daily);
        }
    }
    
    //遍历数组，将未完成的放在中间的位置
    CCARRAY_FOREACH(DailyTask::sharedInstance()->arr_dailyTask, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item && !item->getIsFinish())
        {
            initNewListItem(item, achieve_daily);
            
        }
    }
    
    //遍历数组，将已完成，未领取的放在最前面
    CCARRAY_FOREACH(DailyTask::sharedInstance()->arr_dailyTask, obj)
    {
        AchieveItem* item = (AchieveItem*)obj;
        
        if (item->getIsFinish() && !item->getIsGet())
        {
            initNewListItem(item, achieve_daily);
        }
    }
  
    ListView* root_list = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_daily"));
    
    if (root_list)
    {
        root_list->removeLastItem();
    }
}

void GameAchieveLayer::initCountReward()
{
    //出击次数
    Label* label_attackTime = static_cast<Label*>(m_pUILayer->getWidgetByName("number_battle"));
    
    label_attackTime->setText(getStringFromInt(GameCountData::sharedInstance()->getAttackTimes()));
    
    //击毁敌机
    Label* label_shatter = static_cast<Label*>(m_pUILayer->getWidgetByName("number_smarsh"));
    
    label_shatter->setText(getStringFromInt(GameCountData::sharedInstance()->getShatter()));
    
    //最高波数
    Label* label_wave = static_cast<Label*>(m_pUILayer->getWidgetByName("number_best_wave"));
    
    label_wave->setText(getStringFromInt(GameCountData::sharedInstance()->getMileage()));
    
    //最高分数
    Label* label_score = static_cast<Label*>(m_pUILayer->getWidgetByName("number_bestscore"));
    
    label_score->setText(getStringFromInt(GameCountData::sharedInstance()->getBestScore()));
    
    //累积获得金币
    Label* label_gold = static_cast<Label*>(m_pUILayer->getWidgetByName("number_gold"));
    
    label_gold->setText(getStringFromInt(GameCountData::sharedInstance()->getTotalGold()));
    
    //累积获得钻石
    Label* label_gem = static_cast<Label*>(m_pUILayer->getWidgetByName("number_gem"));
    
    label_gem->setText(getStringFromInt(GameCountData::sharedInstance()->getTotalGem()))
    ;
    //完成任务
    Label* label_task = static_cast<Label*>(m_pUILayer->getWidgetByName("number_daily"));
    label_task->setText(getStringFromInt(GameCountData::sharedInstance()->getTotalTask()));
    
    
    
    
    /***************  关于所有成就图标的处理  ******************/
    
    for (int i = 0; i < Achieve::getInstance()->achieveArr_mileage->count(); i++)
    {
        string str = "achieve_1_";
        
        str.append(getStringFromInt(i + 1));
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName(str.c_str()));
        
        string strpath = "achieve_";
        
        AchieveItem* item = (AchieveItem*)Achieve::getInstance()->achieveArr_mileage->objectAtIndex(i);
        
        strpath.append(item->getIconID()).append(".png");
        
        img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
        
        LabelAtlas* icon_num = LabelAtlas::create();
        icon_num->setProperty(item->getIconNo(), "ui/cocostudio/shuzi.png", 15, 22, "0");
        
        icon_num->setPosition(ccp(0, -11));
        
        img->addChild(icon_num);
        
        if (!item->getIsFinish())
        {
            img->setColor(ccc3(95, 95, 95));
        }
        
    }
    
    for (int i = 0; i < Achieve::getInstance()->achieveArr_shatter->count(); i++)
    {
        string str = "achieve_2_";
        
        str.append(getStringFromInt(i + 1));
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName(str.c_str()));
        
        string strpath = "achieve_";
        
        AchieveItem* item = (AchieveItem*)Achieve::getInstance()->achieveArr_shatter->objectAtIndex(i);
        
        strpath.append(item->getIconID()).append(".png");
        
        img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
        
        LabelAtlas* icon_num = LabelAtlas::create();
        icon_num->setProperty(item->getIconNo(), "ui/cocostudio/shuzi.png", 15, 22, "0");
        
        icon_num->setPosition(ccp(0, -11));
        
        img->addChild(icon_num);
        
        if (!item->getIsFinish())
        {
            img->setColor(ccc3(95, 95, 95));
        }
    }
    
    for (int i = 0; i < Achieve::getInstance()->achieveArr_tools->count(); i++)
    {
        string str = "achieve_3_";
        
        str.append(getStringFromInt(i + 1));
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName(str.c_str()));
        
        string strpath = "achieve_";
        
        AchieveItem* item = (AchieveItem*)Achieve::getInstance()->achieveArr_tools->objectAtIndex(i);
        
        strpath.append(item->getIconID()).append(".png");
        
        img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
        
        LabelAtlas* icon_num = LabelAtlas::create();
        icon_num->setProperty(item->getIconNo(), "ui/cocostudio/shuzi.png", 15, 22, "0");
        
        icon_num->setPosition(ccp(0, -11));
        
        img->addChild(icon_num);
        
        if (!item->getIsFinish())
        {
            img->setColor(ccc3(95, 95, 95));
        }
    }
    
    for (int i = 0; i < Achieve::getInstance()->achieveArr_equipment->count(); i++)
    {
        string str = "achieve_4_";
        
        str.append(getStringFromInt(i + 1));
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName(str.c_str()));
        
        string strpath = "achieve_";
        
        AchieveItem* item = (AchieveItem*)Achieve::getInstance()->achieveArr_equipment->objectAtIndex(i);
        
        strpath.append(item->getIconID()).append(".png");
        
        img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
        
        LabelAtlas* icon_num = LabelAtlas::create();
        icon_num->setProperty(item->getIconNo(), "ui/cocostudio/shuzi.png", 15, 22, "0");
        
        icon_num->setPosition(ccp(0, -11));
        
        img->addChild(icon_num);
        
        if (!item->getIsFinish())
        {
            img->setColor(ccc3(95, 95, 95));
        }
    }
    
    for (int i = 0; i < Achieve::getInstance()->achieveArr_level->count(); i++)
    {
        string str = "achieve_5_";
        
        str.append(getStringFromInt(i + 1));
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName(str.c_str()));
        
        string strpath = "achieve_";
        
        AchieveItem* item = (AchieveItem*)Achieve::getInstance()->achieveArr_level->objectAtIndex(i);
        
        strpath.append(item->getIconID()).append(".png");
        
        img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
        
        LabelAtlas* icon_num = LabelAtlas::create();
        icon_num->setProperty(item->getIconNo(), "ui/cocostudio/shuzi.png", 15, 22, "0");
        
        img->addChild(icon_num);
        
        if (!item->getIsFinish())
        {
            img->setColor(ccc3(95, 95, 95));
        }
    }
    
    for (int i = 0; i < Achieve::getInstance()->achieveArr_pay->count(); i++)
    {
        
        
        string str = "achieve_6_";
        
        str.append(getStringFromInt(i + 1));
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName(str.c_str()));
        
        string strpath = "achieve_";
        
        AchieveItem* item = (AchieveItem*)Achieve::getInstance()->achieveArr_pay->objectAtIndex(i);
        
        strpath.append(item->getIconID()).append(".png");
        
        img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
        
        LabelAtlas* icon_num = LabelAtlas::create();
        icon_num->setProperty(item->getIconNo(), "ui/cocostudio/shuzi.png", 15, 22, "0");
        
        icon_num->setPosition(ccp(0, -11));
        
        img->addChild(icon_num);
        
        if (!item->getIsFinish())
        {
            img->setColor(ccc3(95, 95, 95));
        }
    }
    
    for (int i = 0; i < Achieve::getInstance()->achieveArr_consume->count(); i++)
    {
        
        string str = "achieve_7_";
        
        str.append(getStringFromInt(i + 1));
        
        ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName(str.c_str()));
        
        if(!img)
        {
            CCLog("img %s is null",str.c_str());
        }
        
        string strpath = "achieve_";
        
        AchieveItem* item = (AchieveItem*)Achieve::getInstance()->achieveArr_consume->objectAtIndex(i);
        
        strpath.append(item->getIconID()).append(".png");
        
        img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
        
        LabelAtlas* icon_num = LabelAtlas::create();
        icon_num->setProperty(item->getIconNo(), "ui/cocostudio/shuzi.png", 15, 22, "0");
        
        icon_num->setPosition(ccp(0, -11));
        
        img->addChild(icon_num);
        
        if (!item->getIsFinish())
        {
            img->setColor(ccc3(95, 95, 95));
        }
    }
    
}

void GameAchieveLayer::panelRewardCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {

    }
}

void GameAchieveLayer::btnGetCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        
        Button* btn = (Button*)pSender;
        
        int tag = btn->getTag();
        
        AchievType type = (AchievType)(tag / 1000);
        
        int id = tag % 1000;
        
        CCLog("id is : %d",id);
        
        AchieveItem* item = NULL;
        
        //如果是日常任务
        if (tag / 1000 >= task_day_score)
        {
            item = DailyTask::sharedInstance()->getTaskByID(id);
            
            item->setIsGet(true);
            
            DailyTask::sharedInstance()->save();
        }
        else  //如果是成就领取
        {
            item = Achieve::getInstance()->getAchieve(type, id);
            
            item->setIsGet(true);
            
            Achieve::getInstance()->save();
        }
        
        if (!item)
        {
            return;
        }
        
        
        //将奖励 加给玩家
        //奖励1
        if (item->getRewardFir())
        {
            BaseReward* reward = item->getRewardFir();
            
            if (reward->getTypeReward() == kSignReward_gold)
            {
                UserData::shared()->increaseGoldNum(reward->getValue());
                
                GetEquipLayer* layer = GetEquipLayer::createWithGift(kSignReward_gold);
                
                addChild(layer,KZOrderDialog);
            }
            
            if (reward->getTypeReward() == kSignReward_gem)
            {
                UserData::shared()->increaseGemNum(reward->getValue());
                
                GetEquipLayer* layer = GetEquipLayer::createWithGift(kSignReward_gem);
                
                addChild(layer,KZOrderDialog);
            }
            
            UserData::shared()->save();
        }
        
        //奖励2
        if (item->getRewardSec())
        {
            BaseReward* reward = item->getRewardSec();
            
            EquipItem* equip = NULL;
            
            switch (reward->getTypeReward())
            {
                case kSignReward_equip_white:
                    equip = EquipItem::createNewEquip();
                    equip->setGrade(EQUIP_QUALITY_WHITE);
                    break;
                    
                case kSignReward_equip_green:
                    equip = EquipItem::createNewEquip();
                    equip->setGrade(EQUIP_QUALITY_GREEN);
                    break;
                    
                case kSignReward_equip_blue:
                    equip = EquipItem::createNewEquip();
                    equip->setGrade(EQUIP_QUALITY_BLUE);
                    break;
                    
                case kSignReward_equip_pupple:
                    equip = EquipItem::createNewEquip();
                    equip->setGrade(EQUIP_QUALITY_PURPLE);
                    break;
                    
                case kSignReward_equip_orange:
                    equip = EquipItem::createNewEquip();
                    equip->setGrade(EQUIP_QUALITY_ORRANGE);
                    break;
                    
                default:
                    break;
            }
            
            if (equip)
            {
                equip->setImgName(reward->getImgID());
                GetRandomEquip::shared()->randHpAndAttack(equip);
                GetRandomEquip::shared()->randAttribute(equip);
                GetRandomEquip::shared()->randSkill(equip);
                
                int wave = GameCountData::sharedInstance()->getMileage();
                
                //等级上限属性
                int level_limit = 0;
                
                if (wave <= 30)
                {
                    level_limit = ((int)(wave / 10.0) + 1) * 5;
                }
                else
                {
                    level_limit = ((int)((wave - 30) / 20.0) + 1) * 5 + 15;
                }
                
                equip->setLevelLimit(level_limit);
                
                equip->setID(getStringFromInt(UserEquipBag::shared()->getMinID()));
                
                GetEquipLayer* layer = GetEquipLayer::createWithGift(kSignReward_equip_white,equip);
                
                addChild(layer,KZOrderDialog);
                
                UserEquipBag::shared()->addEquip(equip);
                
                UserEquipBag::shared()->save();
            }
            
            btn->setTouchEnabled(false);
            
            btn->setColor(ccc3(49, 66, 99));
            
            refreshGoldInfo();
            
            refreshUI();
        }
        
    }
}

void GameAchieveLayer::refreshUI()
{
//    ListView* root_listDaily = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_daily"));
//    
//    if (root_listDaily)
//    {
//        UIPanel* default_item = static_cast<UIPanel*>(root_listDaily->getItem(0));
//        
//        root_listDaily->setItemModel(default_item);
//        
//        root_listDaily->removeAllItems();
//        
//        root_listDaily->pushBackDefaultItem();
//    }
//    
    
    ListView* root_listAchieve = static_cast<ListView*>(m_pUILayer->getWidgetByName("ListView_achieve"));
    
    if (root_listAchieve)
    {
        UIPanel* default_item = static_cast<UIPanel*>(root_listAchieve->getItem(0));
        
        root_listAchieve->setItemModel(default_item);
        
        root_listAchieve->removeAllItems();
        
        root_listAchieve->pushBackDefaultItem();
    }
    
    initAchieve();
    
    //initDailyReward();
}


//刷新金币
void GameAchieveLayer::refreshGoldInfo()
{
    
    LabelAtlas* label_energy = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_energy"));
    label_energy->setStringValue(getStringFromInt(UserData::shared()->getEnergyNum()));
    
    LabelAtlas* label_gold = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gold"));
    label_gold->setStringValue(getStringFromInt(UserData::shared()->getGoldNum()));
    
    
    LabelAtlas* label_gem = static_cast<LabelAtlas*>(m_pUILayer->getWidgetByName("AtlasLabel_gem"));
    label_gem->setStringValue(getStringFromInt(UserData::shared()->getGemNum()));
}

void GameAchieveLayer::btnBuyCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        Button* btn = (Button*)pSender;
        
        if (compareString(btn->getName(), "btn_buygem"))
        {
             SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
            
            //打开购买钻石界面
            GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
        }
        
        if (compareString(btn->getName(), "btn_buypower"))
        {
             SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
            
            //打开购买体力界面
            GameDelegate::shared()->openSceneStoreLayer(kStore_energy);
        }
        
        if (compareString(btn->getName(), "btn_buygold"))
        {
             SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
            
            //打开购买金币界面
            GameDelegate::shared()->openSceneStoreLayer(kStore_gold);
        }
    }
}

void GameAchieveLayer::btnLeftCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{

}

void GameAchieveLayer::btnRightCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        GameDelegate::shared()->openSceneTools(false);
    }
}

void GameAchieveLayer::btnLotteryCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        GameDelegate::shared()->openSceneLottery();
    }
}

void GameAchieveLayer::refreshBtnState(int tag)
{
    ImageView* btn_dailytask = static_cast<ImageView*>(m_pUILayer->getWidgetByName("btn_daily"));
    btn_dailytask->loadTexture("btn_daily.png",UI_TEX_TYPE_PLIST);
    
    ImageView* btn_achieve = static_cast<ImageView*>(m_pUILayer->getWidgetByName("btn_achieve"));
    btn_achieve->loadTexture("btn_achieve.png",UI_TEX_TYPE_PLIST);
    
    ImageView* btn_countAchieve = static_cast<ImageView*>(m_pUILayer->getWidgetByName("btn_count"));
    btn_countAchieve->loadTexture("btn_count.png",UI_TEX_TYPE_PLIST);
    
    if (tag == 0)
    {
        btn_achieve->loadTexture("btn_achieve_1.png",UI_TEX_TYPE_PLIST);
    }
    else if (tag == 1)
    {
        btn_dailytask->loadTexture("btn_daily_1.png",UI_TEX_TYPE_PLIST);
    }
    else
    {
        btn_countAchieve->loadTexture("btn_count_1.png",UI_TEX_TYPE_PLIST);
    }
    
}

void GameAchieveLayer::btnAchieveCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
         SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        PageView* pageview = static_cast<PageView*>(m_pUILayer->getWidgetByName("PageView_achieve"));
        
        Button* btn = (Button*)pSender;
        
        if (compareString(btn->getName(), "btn_daily"))
        {
            refreshBtnState(1);
            
            pageview->scrollToPage(1);
        }
        
        if (compareString(btn->getName(), "btn_achieve"))
        {
            refreshBtnState(0);
            
            pageview->scrollToPage(0);
        }
        
        if (compareString(btn->getName(), "btn_count"))
        {
            refreshBtnState(2);
            
            pageview->scrollToPage(2);
        }
    }
}

void GameAchieveLayer::pageviewEnent(cocos2d::CCObject *pSender, PageViewEventType type)
{
    if (type == PAGEVIEW_EVENT_TURNING)
    {
        PageView* pageview = static_cast<PageView*>(m_pUILayer->getWidgetByName("PageView_achieve"));
        
        //更新按钮状态
        refreshBtnState(pageview->getCurPageIndex());
    }
}
