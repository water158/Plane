//
//  GameSigninLayer.cpp
//  ambition
//
//  Created by mini01 on 14-11-24.
//
//

#include "GameSigninLayer.h"

GameSigninLayer::GameSigninLayer()
{

}

GameSigninLayer::~GameSigninLayer()
{

}

bool GameSigninLayer::init()
{
    CCLayer::init();
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    //读取 成就图标的文件
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/achieve/achieve.plist");
    
    Layout* root_signin = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/signin_layer.ExportJson"));
    m_pUILayer->addWidget(root_signin);
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("Button_23"));
    btn_close->addTouchEventListener(this, toucheventselector(GameSigninLayer::btnCloseCallBack));
    
    
    
    this->initUI();
    
    return true;
}

void GameSigninLayer::initUI()
{
    //通过manager里获得数组，再从userdata中获取已经签到的天数，将已经签到的和未签到的ui做相应的处理
    
    int hadsigned = UserData::shared()->getHadeSigned();
    
    bool issign = false;
    
    if (getCurrentTime(kGetTimeDay) == UserData::shared()->getLastLoginDay()
        && getCurrentTime(kGetTimeMonth) == UserData::shared()->getLastLoginMon())
    {
        issign = true;
    }
    
    for (int i = 0; i < 30; i++)
    {
        CCString* str = CCString::createWithFormat("img_%d",i + 1);
        
        CCLog("str : %s",str->getCString());
        UIImageView* img = static_cast<UIImageView*>(m_pUILayer->getWidgetByName(str->getCString()));
        
        img->setTouchEnabled(true);
        
        img->addTouchEventListener(this, toucheventselector(GameSigninLayer::kickOneDayCallBack));
        
        img->setTag(TAG_SIGIN_DAY + i);
        
        //对颜色进行更改处理
        if (issign)
        {
            if (i < hadsigned)
            {
                img->setColor(ccRED);
            }
            else if (i > hadsigned)
            {
                img->setColor(ccWHITE);
            }
            else
            {
                img->setColor(ccWHITE);
            }
            
            if (i == hadsigned - 1)
            {
                kickOneDayCallBack(img, TOUCH_EVENT_ENDED);
            }
        }
        else
        {
            if (i < hadsigned)
            {
                img->setColor(ccRED);
            }
            else if (i > hadsigned)
            {
                img->setColor(ccWHITE);
            }
            else
            {
                img->setColor(ccORANGE);
                
                kickOneDayCallBack(img, TOUCH_EVENT_ENDED);
            }
        }
    }
    
    //通过userdata的上次登陆游戏的时间做对比 如果当前时间和用户上次保存的时间一致 则将签到按钮置为不可使用 todo:将按钮上的字也置为灰色
    Button* btn_get = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_get"));
    btn_get->addTouchEventListener(this, toucheventselector(GameSigninLayer::btnSignCallBack));
    
    if (issign)
    {
        btn_get->setTouchEnabled(false);
        btn_get->setColor(ccc3(79, 29, 79));
    }
    
    CCArray* rewardArr = SignManager::shared()->getRewardArr();
    
    if (issign)
    {
        SignReward* reward = (SignReward*)rewardArr->objectAtIndex(hadsigned - 1);
        
        refreshReward(reward);
    }
    else
    {
        SignReward* reward = (SignReward*)rewardArr->objectAtIndex(hadsigned);
        
        refreshReward(reward);
    }
}

void GameSigninLayer::kickOneDayCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        for (int i = 0; i < 30; i++)
        {
            CCString* str = CCString::createWithFormat("img_%d",i + 1);
            UIImageView* img = static_cast<UIImageView*>(m_pUILayer->getWidgetByName(str->getCString()));
            
            if (img->getChildByTag(TAG_EFFECT_SELECT))
            {
                img->removeChildByTag(TAG_EFFECT_SELECT,true);
            }
        }
        
        ImageView* img = (ImageView*)pSender;
        
        int tag = img->getTag();
        
        ImageView* img_select = ImageView::create();
        
        img_select->loadTexture("ui/effect/effect_selectSignDay.png");
        
        img->addChild(img_select,0,TAG_EFFECT_SELECT);
        
        //将用户数据加入相对应的奖励，奖励从manager里的数组里获取 下标为hadSigned
        CCArray* rewardArr = SignManager::shared()->getRewardArr();
        
        SignReward* reward = (SignReward*)rewardArr->objectAtIndex(tag - TAG_SIGIN_DAY);
        
        refreshReward(reward);
    }
}

void GameSigninLayer::refreshReward(SignReward* reward)
{
    ImageView* img_reward = static_cast<ImageView*>(m_pUILayer->getWidgetByName("img_reward"));
    
    string iconpath = "";
    
    CCLog("reward id: %s, type:%d,label: %s,count: %d,img: %s",reward->getID().c_str(),reward->getTypeReward(),reward->getStrLabel().c_str(),reward->getValue(),reward->getImgID().c_str());
    
    if (reward)
    {
        
        if (!compareString(reward->getImgID().c_str(), "-1"))
        {
            EquipItem* item = EquipItem::createNewEquip();
            
            item->setImgName(reward->getImgID());
            
            
            switch (reward->getTypeReward())
            {
                case kSignReward_equip_white:
                    item->setGrade(EQUIP_QUALITY_WHITE);
                    break;
                case kSignReward_equip_green:
                    item->setGrade(EQUIP_QUALITY_GREEN);
                    break;
                case kSignReward_equip_blue:
                    item->setGrade(EQUIP_QUALITY_BLUE);
                    break;
                case kSignReward_equip_pupple:
                    item->setGrade(EQUIP_QUALITY_PURPLE);
                    break;
                case kSignReward_equip_orange:
                    item->setGrade(EQUIP_QUALITY_ORRANGE);
                    break;
                default:
                    break;
            }
            
            iconpath = UserEquipBag::shared()->getIconPath(item);
            
            img_reward->loadTexture(iconpath.c_str(),UI_TEX_TYPE_PLIST);
            
        }
        else
        {
            switch (reward->getTypeReward())
            {
                case kSignReward_gold:
                    iconpath = "daily_2.png";
                    break;
                    
                case kSignReward_gem:
                    iconpath = "daily_1.png";
                    break;
                    
                case kSignReward_tool_1:
                    iconpath = "tool_1.png";
                    break;
                    
                case kSignReward_tool_2:
                    iconpath = "tool_2.png";
                    break;
                    
                case kSignReward_tool_3:
                    iconpath = "tool_3.png";
                    break;
                    
                case kSignReward_tool_4:
                    iconpath = "tool_4.png";
                    break;
                    
                case kSignReward_tool_5:
                    iconpath = "tool_5.png";
                    break;
                    
                case kSignReward_tool_6:
                    iconpath = "tool_6.png";
                    break;
                    
                    
                default:
                    break;
            }
            
            img_reward->loadTexture(iconpath.c_str(),UI_TEX_TYPE_PLIST);
        }
        
        Label* label_reward = static_cast<Label*>(m_pUILayer->getWidgetByName("label_reward"));
        
        label_reward->setText(reward->getStrLabel());
    }
}

void GameSigninLayer::btnCloseCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        GameDelegate::shared()->closeSceneSignin();
    }
}

void GameSigninLayer::btnSignCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type != TOUCH_EVENT_ENDED)
    {
        return;
    }
    
    //获取当期系统时间的年月日
    if(getCurrentTime(kGetTimeYear) == UserData::shared()->getLastLoginYear()
       && getCurrentTime(kGetTimeMonth) == UserData::shared()->getLastLoginMon()
       && getCurrentTime(kGetTimeDay) == UserData::shared()->getLastLoginDay())
    {
        return;
    }
    
    //获取已经签到的天数
    int hadSigned = UserData::shared()->getHadeSigned();
    
    CCLog("hadSigned : %d",hadSigned);
    
    //将用户数据加入相对应的奖励，奖励从manager里的数组里获取 下标为hadSigned
    CCArray* rewardArr = SignManager::shared()->getRewardArr();
    
    for (int i = 0; i < 30; i++)
    {
        if (i == hadSigned)
        {
            CCString* str = CCString::createWithFormat("img_%d",i + 1);
            
            CCLog("str : %s",str->getCString());
            
            SignReward* reward = (SignReward*)rewardArr->objectAtIndex(i);
            
            //将这个奖励给用户加上
            if (reward->getTypeReward() == kSignReward_gold)
            {
                UserData::shared()->increaseGoldNum(reward->getValue());
            }
            if (reward->getTypeReward() == kSignReward_gem)
            {
                UserData::shared()->increaseGemNum(reward->getValue());
            }
            
            switch (reward->getTypeReward())
            {
                case kSignReward_tool_1:
                    UserData::shared()->increaseTool(BUY_TOOL_1,reward->getValue());
                    break;
                    
                case kSignReward_tool_2:
                    UserData::shared()->increaseTool(BUY_TOOL_2,reward->getValue());
                    break;
                    
                case kSignReward_tool_3:
                    UserData::shared()->increaseTool(BUY_TOOL_3,reward->getValue());
                    break;
                    
                case kSignReward_tool_4:
                    UserData::shared()->increaseTool(BUY_TOOL_4,reward->getValue());
                    break;
                    
                case kSignReward_tool_5:
                    UserData::shared()->increaseTool(BUY_TOOL_5,reward->getValue());
                    break;
                    
                case kSignReward_tool_6:
                    UserData::shared()->increaseTool(BUY_TOOL_6,reward->getValue());
                    break;
                    
                default:
                    break;
            }
            
            EquipItem* item = NULL;
            
            switch (reward->getTypeReward())
            {
                case kSignReward_equip_white:
                    item = EquipItem::createNewEquip();
                    item->setGrade(EQUIP_QUALITY_WHITE);
                    break;
                    
                case kSignReward_equip_green:
                    item = EquipItem::createNewEquip();
                    item->setGrade(EQUIP_QUALITY_GREEN);
                    break;
                    
                case kSignReward_equip_blue:
                    item = EquipItem::createNewEquip();
                    item->setGrade(EQUIP_QUALITY_BLUE);
                    break;
                    
                case kSignReward_equip_pupple:
                    item = EquipItem::createNewEquip();
                    item->setGrade(EQUIP_QUALITY_PURPLE);
                    break;
                    
                case kSignReward_equip_orange:
                    item = EquipItem::createNewEquip();
                    item->setGrade(EQUIP_QUALITY_ORRANGE);
                    break;
                    
                default:
                    break;
            }
            
            if (item)
            {
                GetRandomEquip::shared()->randHpAndAttack(item);
                
                GetRandomEquip::shared()->randAttribute(item);
                
                GetRandomEquip::shared()->randSkill(item);
                
                item->setID(getStringFromInt(UserEquipBag::shared()->getMinID()));
                
                UserEquipBag::shared()->addEquip(item);
                
                UserEquipBag::shared()->save();
            }
            
            
            
            //将当前的背景颜色更改
            UIImageView* img = static_cast<UIImageView*>(m_pUILayer->getWidgetByName(str->getCString()));
            img->setColor(ccRED);
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESHGOLD);
        }
    }
    
    //更新用户已签到的天数
    UserData::shared()->setHadeSigned(hadSigned + 1);
    
    //更改用户的上次签到信息
    UserData::shared()->setLastLoginYear(getCurrentTime(kGetTimeYear));
    UserData::shared()->setLastLoginMon(getCurrentTime(kGetTimeMonth));
    UserData::shared()->setLastLoginDay(getCurrentTime(kGetTimeDay));
    
    UserData::shared()->save();
    
    //将按钮置为不可使用 将按钮的字置为黑色
    Button* btn = (Button*)pSender;
    btn->setColor(ccc3(79, 29, 79));
    btn->setTouchEnabled(false);
}
