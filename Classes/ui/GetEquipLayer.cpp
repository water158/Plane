//
//  GetEquipLayer.cpp
//  ambition
//
//  Created by mini01 on 14-12-1.
//
//

#include "GetEquipLayer.h"


GetEquipLayer::GetEquipLayer()
{
    //读取 成就图标的文件
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/achieve/achieve.plist");
    
    //读取 装备图标文件
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/equip/equip_icon.plist");
}

GetEquipLayer::~GetEquipLayer()
{

}

GetEquipLayer* GetEquipLayer::create(EquipItem* item,bool isShow)
{
    GetEquipLayer* layer = new GetEquipLayer();
    
    if (layer && layer->init(item,isShow))
    {
        layer->autorelease();
        
        return layer;
    }
    
    return NULL;
}

GetEquipLayer* GetEquipLayer::createWithGift(SignRewardType type,EquipItem* item)
{
    GetEquipLayer* layer = new GetEquipLayer();
    
    if (layer && layer->initWithGift(type,item))
    {
        layer->autorelease();
        
        return layer;
    }
    
    return NULL;
}

bool GetEquipLayer::initWithGift(SignRewardType type,EquipItem* item)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_getequip = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/getequip_layer.ExportJson"));
    m_pUILayer->addWidget(root_getequip);
    
    Button* btn_get = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_get"));
    
    btn_get->addTouchEventListener(this, toucheventselector(GetEquipLayer::btnGetGiftCallBack));
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    
    btn_close->addTouchEventListener(this, toucheventselector(GetEquipLayer::btnCloseCallBack));
    
    ImageView* imag_icon = static_cast<ImageView*>(m_pUILayer->getWidgetByName("Image_27"));
    
    if (type == kSignReward_gold)
    {
        //金币
        imag_icon->loadTexture("daily_2.png",UI_TEX_TYPE_PLIST);
    }
    else if (type == kSignReward_gem)
    {
        //钻石
        imag_icon->loadTexture("daily_1.png",UI_TEX_TYPE_PLIST);
    }
    else if (type >= kSignReward_equip_white && type <= kSignReward_equip_orange)
    {
        //装备
        imag_icon->loadTexture(UserEquipBag::shared()->getIconPath(item).c_str(),UI_TEX_TYPE_PLIST);
    }
    else if (type != kSignReward_noting)
    {
        //道具
        string str = "tool_";
        
        str.append(getStringFromInt((int)(type - kSignReward_equip_orange))).append(".png");
        
        imag_icon->loadTexture(str.c_str(),UI_TEX_TYPE_PLIST);
    }
    
    Label* label_get = static_cast<Label*>(m_pUILayer->getWidgetByName("label_get"));
    
    if (type == kSignReward_gold)
    {
        //金币
        label_get->setText(StringLibrary::shared()->getStringByKey("getGold")->getCString());
    }
    else if (type == kSignReward_gem)
    {
        //钻石
        label_get->setText(StringLibrary::shared()->getStringByKey("getGem")->getCString());
    }
    else if (type >= kSignReward_equip_white && type <= kSignReward_equip_orange)
    {
        //装备
        label_get->setText(StringLibrary::shared()->getStringByKey("getEquip")->getCString());
    }
    else if (type != kSignReward_noting)
    {
        //道具
        label_get->setText(StringLibrary::shared()->getStringByKey("getTool")->getCString());
    }
    
    Label* label_text = static_cast<Label*>(m_pUILayer->getWidgetByName("label_text"));
    
    if (type == kSignReward_gold)
    {
        //金币
        label_text->setText(StringLibrary::shared()->getStringByKey("getGoldText")->getCString());
    }
    else if (type == kSignReward_gem)
    {
        //钻石
        label_text->setText(StringLibrary::shared()->getStringByKey("getGemText")->getCString());
    }
    else if (type >= kSignReward_equip_white && type <= kSignReward_equip_orange)
    {
        //装备
        label_text->setText(StringLibrary::shared()->getStringByKey("getEquipText")->getCString());
    }
    else if (type != kSignReward_noting)
    {
        //道具
        label_text->setText(StringLibrary::shared()->getStringByKey("getToolText")->getCString());
    }
    
    return true;
}

bool GetEquipLayer::init(EquipItem* item,bool isShow)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("ui/icon/equip/equip_icon.plist");
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_getequip = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/getequip_layer.ExportJson"));
    m_pUILayer->addWidget(root_getequip);
    
    m_equipByLottery = item;
    
    //m_equipByLottery->setID(getStringFromInt(UserEquipBag::shared()->getMinID()));
    
    if (!isShow)
    {
        UserEquipBag::shared()->addEquip(m_equipByLottery);
        
        UserEquipBag::shared()->getArrEquipGetRecent()->addObject(m_equipByLottery);
        
        UserEquipBag::shared()->save();
    }
    
    CCLog("m_equipByLottery id : %s",m_equipByLottery->getID().c_str());
    
    Button* btn_get = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_get"));
    
    btn_get->addTouchEventListener(this, toucheventselector(GetEquipLayer::btnGetCallBack));
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    
    btn_close->addTouchEventListener(this, toucheventselector(GetEquipLayer::btnCloseCallBack));
    

    
    ImageView* imag_icon = static_cast<ImageView*>(m_pUILayer->getWidgetByName("Image_27"));
    
    imag_icon->loadTexture(UserEquipBag::shared()->getIconPath(item).c_str(),UI_TEX_TYPE_PLIST);
    
    Label* label_text = static_cast<Label*>(m_pUILayer->getWidgetByName("label_text"));
    
    label_text->setText(StringLibrary::shared()->getStringByKey("getEquipText")->getCString());
    
    return true;
}

void GetEquipLayer::btnCloseCallBack(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        this->removeFromParent();
    }
}

void GetEquipLayer::btnGetGiftCallBack(CCObject* pSender,TouchEventType type)
{

    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        this->removeFromParentAndCleanup(true);
    }
}


void GetEquipLayer::btnGetCallBack(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        this->removeFromParentAndCleanup(true);
    }
}


