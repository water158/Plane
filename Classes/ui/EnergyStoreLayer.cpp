//
//  EnergyStoreLater.cpp
//  ambition
//
//  Created by mini01 on 14-12-9.
//
//

#include "EnergyStoreLayer.h"

EnergyStoreLayer::EnergyStoreLayer()
{

}

EnergyStoreLayer::~EnergyStoreLayer()
{

}

bool EnergyStoreLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_energystore = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/energy_store_layer.ExportJson"));
    m_pUILayer->addWidget(root_energystore);
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    btn_close->addTouchEventListener(this, toucheventselector(EnergyStoreLayer::btnCloseCallBack));
 
    
    for (int i = 1; i < 4; i++)
    {
        string str_id = "1_";
        str_id.append(getStringFromInt(i));
        
        PriceData* data = PriceDataManager::shared()->getPriceData(str_id);
        
        if (data)
        {
            CCString* str_label = CCString::createWithFormat("label_%d",i);
            
            Label* label = static_cast<Label*>(m_pUILayer->getWidgetByName(str_label->getCString()));
            
            label->setText(data->getDesLabel());
            
            CCString* str_btn = CCString::createWithFormat("btn_%d",i);
            
            Button* btn = static_cast<Button*>(m_pUILayer->getWidgetByName(str_btn->getCString()));
            
            btn->setName(data->getID().c_str());
            
            btn->addTouchEventListener(this, toucheventselector(EnergyStoreLayer::btnBuyCallBack));
            
            string str_consume = "    ";
            str_consume.append(getStringFromInt(data->getConsume()));
            
            btn->setTitleText(str_consume);
        }
    }
    
    int interval = time(0) - UserData::shared()->getTimeRecord();
    
    //如果时间大于记录时间 50分钟以上
    if (interval > 10 * 5 * 60)
    {
        //UserData::shared()->setEnergyNum(10);
        if (UserData::shared()->getEnergyNum() < 10)
        {
            UserData::shared()->setEnergyNum(10);
        }
        
        Label* label_min = static_cast<Label*>(m_pUILayer->getWidgetByName("label_min"));
        
        CCString* str_min = CCString::createWithFormat("%02d",0);
        
        label_min->setText(str_min->getCString());
        
        Label* label_sec = static_cast<Label*>(m_pUILayer->getWidgetByName("label_sec"));
        
        CCString* str_sec = CCString::createWithFormat("%02d",0);
        
        label_sec->setText(str_sec->getCString());
    }
    else
    {
        if (interval / (5 * 60) > 0)
        {
            //如果此段时间恢复的体力大于0 更新时间记录
            UserData::shared()->setTimeRecord(time(0));
        }
        
        int energy = UserData::shared()->getEnergyNum();
        
        CCLog("energy is : %d",energy);
        
        int recover = interval / (5 * 60);
        
        CCLog("energy is : %d",energy);
        
        if (energy >= 10)
        {
            
            Label* label_min = static_cast<Label*>(m_pUILayer->getWidgetByName("label_min"));
            
            CCString* str_min = CCString::createWithFormat("%02d",0);
            
            label_min->setText(str_min->getCString());
            
            Label* label_sec = static_cast<Label*>(m_pUILayer->getWidgetByName("label_sec"));
            
            CCString* str_sec = CCString::createWithFormat("%02d",0);
            
            label_sec->setText(str_sec->getCString());
        }
        
        if (energy + recover >= 10)
        {
            Label* label_min = static_cast<Label*>(m_pUILayer->getWidgetByName("label_min"));
            
            CCString* str_min = CCString::createWithFormat("%02d",0);
            
            label_min->setText(str_min->getCString());
            
            Label* label_sec = static_cast<Label*>(m_pUILayer->getWidgetByName("label_sec"));
            
            CCString* str_sec = CCString::createWithFormat("%02d",0);
            
            label_sec->setText(str_sec->getCString());
        }
        else
        {
            //如果体力不满  开启定时器  恢复体力
            m_currTime = 300 - interval % 300;
            
            this->schedule(schedule_selector(EnergyStoreLayer::update), 1.0f);
            
            Label* label_min = static_cast<Label*>(m_pUILayer->getWidgetByName("label_min"));
            
            CCString* str_min = CCString::createWithFormat("%02d",m_currTime / 60);
            
            label_min->setText(str_min->getCString());
            
            Label* label_sec = static_cast<Label*>(m_pUILayer->getWidgetByName("label_sec"));
            
            CCString* str_sec = CCString::createWithFormat("%02d",m_currTime % 60);
            
            label_sec->setText(str_sec->getCString());
        }
        
        UserData::shared()->setEnergyNum(energy);
    }
    UserData::shared()->save();
    
    return true;
}

void EnergyStoreLayer::btnBuyCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {

        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);

        Button* btn = (Button*)pSender;
        
        string btn_id = btn->getName();
        
        PriceData* data = PriceDataManager::shared()->getPriceData(btn_id);
        
        if (UserData::shared()->getGemNum() >= data->getConsume())
        {
            UserData::shared()->decreaseGemNum(data->getConsume());
            
            GameCountData::sharedInstance()->addConsumeGem(data->getConsume());
            
            Achieve::getInstance()->save();
            
            //增加体力 保存数据
            
            UserData::shared()->increaseEnergy(data->getValue());
            
            UserData::shared()->save();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESHGOLD);
        }
        else
        {
            GameDelegate::shared()->closeSceneStoreLayer();
            GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
        }
    }
}

void EnergyStoreLayer::btnCloseCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        GameDelegate::shared()->closeSceneStoreLayer();
    }
}

void EnergyStoreLayer::update(float dt)
{
    m_currTime--;
    
    Label* label_min = static_cast<Label*>(m_pUILayer->getWidgetByName("label_min"));
    
    CCString* str_min = CCString::createWithFormat("%02d",m_currTime / 60);
    
    label_min->setText(str_min->getCString());
    
    Label* label_sec = static_cast<Label*>(m_pUILayer->getWidgetByName("label_sec"));
    
    CCString* str_sec = CCString::createWithFormat("%02d",m_currTime % 60);
    
    label_sec->setText(str_sec->getCString());
    
    if (m_currTime == 0)
    {
        int energy = UserData::shared()->getEnergyNum();
        
        energy++;
        
        UserData::shared()->setTimeRecord(time(0));
        
        if (energy >= 10)
        {
            energy = 10;
            
            this->unscheduleAllSelectors();
        }
        else
        {
            m_currTime = 300;
        }
        
        //可以简化 没必要这么频繁的保存
        UserData::shared()->setEnergyNum(energy);
        
        UserData::shared()->save();        
        
    }
}