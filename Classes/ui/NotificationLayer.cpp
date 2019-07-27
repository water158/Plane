#include "NotificationLayer.h"


NotificationLayer::NotificationLayer()
{

}

NotificationLayer::~NotificationLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("ui/icon/achieve/achieve.plist");
}

NotificationLayer* NotificationLayer::create()
{
	NotificationLayer* pLayer = new NotificationLayer();
	if(pLayer && pLayer->init())
	{
		pLayer->autorelease();
		return pLayer;
	}

	return NULL;
}

bool NotificationLayer::init()
{
	CCLayer::init();
    
    b_showEnable = true;
    
    //读取 成就图标的文件
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/icon/achieve/achieve.plist");
    
    spBG = CCSprite::create("ui/panel/panel_notification_bg.png");
    spBG->setAnchorPoint(ccp(0.5, 0));
	spBG->setPosition(ccp(_s_width / 2, _s_height));
    spBG->setVisible(false);
    this->addChild(spBG);
    
    m_fBGWidth = spBG->getContentSize().width;
    m_fBGHeight = spBG->getContentSize().height;
    
    pLabel = CCLabelTTF::create("", "Arial", 32);
    pLabel->setAnchorPoint(ccp(0.5, 0.5));
    pLabel->setPosition(ccp(m_fBGWidth / 2, m_fBGHeight / 2));
    spBG->addChild(pLabel);
    
    m_pUILayer = TouchGroup::create();
    
    addChild(m_pUILayer);
    
    Layout* root_showAchieve = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/achieve_show_layer.ExportJson"));
    
    m_pUILayer->addWidget(root_showAchieve);
    
    return true;
}

void NotificationLayer::show(const char *content)
{
    CCMoveBy *move = CCMoveBy::create(0.5, ccp(0, 0 - m_fBGHeight));
    CCActionInterval *move_back = move->reverse();
    CCDelayTime *delay = CCDelayTime::create(2);
    CCAction *callback = CCCallFunc::create(this, callfunc_selector(NotificationLayer::callBackEnter));
    
    spBG->runAction(CCSequence::create(move, delay, move_back, callback, NULL));
    
	spBG->setVisible(true);
    
    pLabel->setString(content);
}

void NotificationLayer::callBackEnter()
{
	//spBG->setVisible(false);
}

void NotificationLayer::showAchieve(AchieveItem *item,achieve_type type)
{
    if (!b_showEnable)
    {
        return;
    }
    
    b_showEnable = false;
    
    string strpath = "0";
    
    if (type == achieve_game)
    {
        strpath = "achieve_";
    }
    else
    {
        strpath = "daily_";
    }
    
    strpath.append(item->getIconID()).append(".png");
    
    ImageView* img = static_cast<ImageView*>(m_pUILayer->getWidgetByName("img_achieve"));
    
    img->loadTexture(strpath.c_str(),UI_TEX_TYPE_PLIST);
    
    Label* label = static_cast<Label*>(m_pUILayer->getWidgetByName("label_name"));
    
    CCLog("item->getName() : %s",item->getName().c_str());
    
    label->setText(item->getName());
    
    //ActionManager::shareManager()->playActionByName("achieve_show_layer.ExportJson" , "Animation0" );
    
    UIPanel* panel = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("Panel_7"));
    
    float timeScale = this->getScheduler()->getTimeScale();
    
    panel->runAction(CCSequence::create(CCMoveBy::create(0.5 * timeScale, ccp(0, -200)),CCDelayTime::create(1 * timeScale),CCMoveBy::create(0.5 * timeScale, ccp(0, 200)),CCCallFuncN::create(this,callfuncN_selector(NotificationLayer::actionFinish)),NULL));
    
}

void NotificationLayer::actionFinish(cocos2d::CCObject *pSender)
{
    b_showEnable = true;
}