//
//  GameAccountLayer.cpp
//  ambition
//
//  Created by mini01 on 14-12-1.
//
//

#include "GameAccountLayer.h"

RewardBox::RewardBox()
{

}

RewardBox::~RewardBox()
{

}

RewardBox* RewardBox::create(TiXmlElement *elm)
{
    RewardBox* reward = new RewardBox();
    
    if (reward && reward->init(elm))
    {
        reward->autorelease();
        
        return reward;
    }
    
    return NULL;
}

bool RewardBox::init(TiXmlElement *elm)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    //数据初始化
    this->setID("0");
    this->setTagOfReward(kTagRewardToken);
    this->setType(0);
    this->setImgID("-1");  //因为装备的imgid从0开始
    
    if (elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }
    
    if (elm->Attribute("tag"))
    {
        this->setTagOfReward((RewardTag)atoi(elm->Attribute("tag")));
    }
    
    if (elm->Attribute("type"))
    {
        this->setType(atoi(elm->Attribute("type")));
    }
    
    if (elm->Attribute("value"))
    {
        this->setValue(atoi(elm->Attribute("value")));
    }
    
    if (elm->Attribute("img"))
    {
        this->setImgID(elm->Attribute("img"));
    }
    
    return true;
}

GameAccountLayer::GameAccountLayer()
{

}

GameAccountLayer::~GameAccountLayer()
{

}

GameAccountLayer* GameAccountLayer::create(ResultCountData* data)
{
    GameAccountLayer* layer = new GameAccountLayer();
    
    if (layer && layer->init(data))
    {
        layer->autorelease();
        
        return layer;
    }
    
    return NULL;
}

bool GameAccountLayer::init(ResultCountData* data)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/equip/equip_icon.plist");
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/achieve/achieve.plist");
    
    m_resultData = data;
    
    UserData::shared()->increaseGoldNum(m_resultData->m_gold);
    
    GameCountData::sharedInstance()->save();
    
    DailyCountData::sharedInstance()->save();
    
    Achieve::getInstance()->save();
    
    DailyTask::sharedInstance()->save();
    
    UserEquipBag::shared()->save();
    
    UserData::shared()->save();
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(255, 255, 255, 0));
    
    addChild(layer);
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_account = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/end_layer.ExportJson"));
    m_pUILayer->addWidget(root_account);
    
    CCLog("data->m_score : %d",data->m_score);
    
    //积分
    Label* label_score = static_cast<Label*>(m_pUILayer->getWidgetByName("label_score"));
    if (label_score)
    {
        label_score->setText(getStringFromInt(data->m_score));
    }

    //波数
    Label* label_wave = static_cast<Label*>(m_pUILayer->getWidgetByName("label_wave"));
    
    label_wave->setText(getStringFromInt(data->m_wave));
    
    //金币
    Label* label_gold = static_cast<Label*>(m_pUILayer->getWidgetByName("label_gold"));
    
    label_gold->setText(getStringFromInt(data->m_gold));

    //白色装备
    Label* label_white = static_cast<Label*>(m_pUILayer->getWidgetByName("label_white"));
    
    label_white->setText(getStringFromInt(data->m_nEquip_white));
    
    //绿色装备
    Label* label_green = static_cast<Label*>(m_pUILayer->getWidgetByName("label_green"));
    
    label_green->setText(getStringFromInt(data->m_nEquip_green));
    
    //蓝色装备
    Label* label_blue = static_cast<Label*>(m_pUILayer->getWidgetByName("label_blue"));
    
    label_blue->setText(getStringFromInt(data->m_nEquip_blue));
    
    //紫色装备
    Label* label_purple = static_cast<Label*>(m_pUILayer->getWidgetByName("label_purple"));
    
    label_purple->setText(getStringFromInt(data->m_nEquip_puple));
    
    //橙色装备
    Label* label_orange = static_cast<Label*>(m_pUILayer->getWidgetByName("label_orange"));
    label_orange->setText(getStringFromInt(data->m_nEquip_orange));
    
    //箱子 
    
    CCLog("data->m_nBox : %d",data->m_nBox);
    
    for (int i = 1; i <= 9; i++)
    {
        string sBoxName = "btn_";
        sBoxName.append(getStringFromInt(i));
        
        Button* btn = static_cast<Button*>(m_pUILayer->getWidgetByName(sBoxName.c_str()));
        
        btn->setTag(TAG_BTN_BOX + i);
        
        if (i <= data->m_nBox)
        {
            CCLog("sBoxName %s",sBoxName.c_str());
            
            btn->addTouchEventListener(this, toucheventselector(GameAccountLayer::btnBoxCallBack));
        }
        else
        {
            btn->setVisible(false);
        }
    }
    
    //返回按钮
    Button* btn_back = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_back"));
    
    btn_back->addTouchEventListener(this, toucheventselector(GameAccountLayer::btnCloseCallBack));
    
    initReward();
    
    m_nBoxPrice = 0;
    
    return true;
}

void GameAccountLayer::initReward()
{
    string path_reward = "common/CardReward.xml";
    
    TiXmlDocument* xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(path_reward.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    m_pArrRewardBox = CCArray::create();
    
    m_pArrRewardBox->retain();
    
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
    {
        CCArray* arr = CCArray::create();
        
        TiXmlElement* elmitem = elmCursor->FirstChildElement();
        
        while (elmitem)
        {
            RewardBox* reward = RewardBox::create(elmitem);
            
            arr->addObject(reward);
            
            elmitem = elmitem->NextSiblingElement();
        }
        
        m_pArrRewardBox->addObject(arr);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    
    delete xmlDoc;
    
}

void GameAccountLayer::btnCloseCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        GameDelegate::shared()->closeSceneAccount();
        GameDelegate::shared()->openSceneTools(false);
    }
}

void GameAccountLayer::btnBoxCallBack(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        Button* btn = (Button*)pSender;
        
//        CCLog("ori ratition : %f",btn->getRotation());
        
        if (UserData::shared()->isGemEnough(m_nBoxPrice))
        {
            
            GameCountData::sharedInstance()->addConsumeGem(m_nBoxPrice);
            
            Achieve::getInstance()->save();
            
            CCScaleTo* action = CCScaleTo::create(0.5, 0, 1);
            
            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameAccountLayer::actionFinish));
            
            CCScaleTo* action2 = CCScaleTo::create(0.5, 1,1);
            
            CCSequence* seq = CCSequence::create(action,func,action2,NULL);
            
            btn->runAction(seq);
            
            UserData::shared()->decreaseGemNum(m_nBoxPrice);

            
            int tag = btn->getTag() - TAG_BTN_BOX;
            
            m_nBoxPrice += 2;
            
            for (int i = 1; i <= 9; i++)
            {
                string sLabelName = "label_";
                
                sLabelName.append(getStringFromInt(i));
                
                if (i == tag)
                {
                    continue;
                }
                
                Label* label = static_cast<Label*>(m_pUILayer->getWidgetByName(sLabelName.c_str()));
                
                if (label)
                {
                    label->setText(getStringFromInt(m_nBoxPrice));
                }
            }
            
            btn->setTouchEnabled(false);

        }
    }
}

void GameAccountLayer::actionFinish(CCObject* pSender)
{
    Button* btn = (Button*)pSender;
    
    if (btn)
    {
        int tag = btn->getTag();
        
        //从奖励数组中获取一个奖励
        CCArray* arr = (CCArray*)m_pArrRewardBox->objectAtIndex(tag - TAG_BTN_BOX - 1);
        
        RewardBox* reward = (RewardBox*)arr->randomObject();
        
        string path = "label_";

        path.append(getStringFromInt(tag - TAG_BTN_BOX));
        
        Label* label = static_cast<Label*>(m_pUILayer->getWidgetByName(path.c_str()));
        
        label->removeFromParentAndCleanup(true);
        
        
        Label* label_new = Label::create();
        
        label_new->setText(getStringFromInt(reward->getValue()));
        
        label_new->setFontName("font/digifacewide.ttf");
        
        label_new->setPosition(CCPointZero);
        
        label_new->setFontSize(30);
        
        label_new->setColor(ccc3(255, 0, 130));
        
        btn->addChild(label_new);
        
        //根据奖励的不同类型 对按钮图标进行处理
        
        if (reward->getTagOfReward() == kTagRewardToken)
        {
            if (reward->getType() == 1)
            {
                //金币
                btn->loadTextureNormal("daily_2.png",UI_TEX_TYPE_PLIST);
                
                btn->loadTexturePressed("daily_2.png",UI_TEX_TYPE_PLIST);
                
                GameCountData::sharedInstance()->addTotalGold(reward->getValue());
                
                for (int i = 0; i < reward->getValue(); i++)
                {
                    DailyCountData::sharedInstance()->addDailyGetGold();
                }
                
                GameCountData::sharedInstance()->save();
                
                DailyCountData::sharedInstance()->save();
                
                UserData::shared()->increaseGoldNum(reward->getValue());
                
                UserData::shared()->save();
            }
            else if (reward->getType() == 2)
            {
                //钻石
                btn->loadTextureNormal("daily_1.png",UI_TEX_TYPE_PLIST);
                
                btn->loadTexturePressed("daily_1.png",UI_TEX_TYPE_PLIST);
                
                GameCountData::sharedInstance()->addTotalGem(reward->getValue());
                
                GameCountData::sharedInstance()->save();
                
                UserData::shared()->increaseGemNum(reward->getValue());
                
                UserData::shared()->save();
            }
        }
        else
        {
            EquipItem* item = EquipItem::createNewEquip();
            
            switch (reward->getType())
            {
                case kSignReward_equip_white:
                    item->setGrade(EQUIP_QUALITY_WHITE);
                    break;
                    
                case kSignReward_equip_green:
                    item->setGrade(EQUIP_QUALITY_GREEN);
                    GameCountData::sharedInstance()->addGreenEquip();
                    break;
                    
                case kSignReward_equip_blue:
                    item->setGrade(EQUIP_QUALITY_BLUE);
                    GameCountData::sharedInstance()->addBlueEquip();
                    break;
                    
                case kSignReward_equip_pupple:
                    item->setGrade(EQUIP_QUALITY_PURPLE);
                    GameCountData::sharedInstance()->addPupleEquip();
                    break;
                    
                case kSignReward_equip_orange:
                    item->setGrade(EQUIP_QUALITY_ORRANGE);
                    GameCountData::sharedInstance()->addOrangeEquip();
                    break;
                    
                default:
                    break;
            }
            
            item->setImgName(reward->getImgID());
            
            string icon_path = UserEquipBag::shared()->getIconPath(item);
            
            btn->loadTextureNormal(icon_path.c_str(),UI_TEX_TYPE_PLIST);
            
            btn->loadTexturePressed(icon_path.c_str(),UI_TEX_TYPE_PLIST);
            
            GameCountData::sharedInstance()->save();
            
            UserEquipBag::shared()->addEquip(item);
            
            UserEquipBag::shared()->save();
        }
    }
}