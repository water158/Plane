#include "FerrisDialog.h"

FerrisDialog::FerrisDialog()
{
}

FerrisDialog::~FerrisDialog()
{
}

FerrisDialog *FerrisDialog::createWithLibraryTag(int type, const char *library_tag, CCNode* target, SEL_MenuHandler callbackLT, SEL_MenuHandler callbackRT)
{
	FerrisDialog *sa = new FerrisDialog();
	sa->autorelease();
	if(sa && sa->initWithTag(type, library_tag, target, callbackLT, callbackRT))
	{
        return sa;
    }
	return NULL;
}

FerrisDialog *FerrisDialog::createWithContent(int type, const char *content, CCNode* target, SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT)
{
	FerrisDialog *sa = new FerrisDialog();
	sa->autorelease();
	if(sa && sa->initWithContent(type, content, target, callbackLT, callbackRT))
	{
        return sa;
    }
	return NULL;
}

FerrisDialog *FerrisDialog::createWithStr(const char *str, cocos2d::CCNode *target, SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT,bool canSell)
{
    FerrisDialog *sa = new FerrisDialog();
    sa->autorelease();
    if (sa && sa->initWithStr(str, target, callbackLT, callbackRT,canSell))
    {
        return sa;
    }
    
    return NULL;
}

bool FerrisDialog::initWithTag(int type, const char *library_tag, CCNode* target, SEL_MenuHandler callbackLT, SEL_MenuHandler callbackRT)
{
//    return initWithContent(type,
//                           StringLibrary::shared()->getStringByKey(library_tag)->getCString(),
//                           target,
//                           callbackLT,
//                           callbackRT);
    return false;
}

bool FerrisDialog::initWithContent(int type, const char *content, CCNode* target, SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT)
{
    m_puiLayer = TouchGroup::create();
    
    addChild(m_puiLayer);
    
    Layout* root_dialog = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/recast_layer.ExportJson"));
    
    m_puiLayer->addWidget(root_dialog);
    
    Button* btn_yes = static_cast<Button*>(m_puiLayer->getWidgetByName("btn_yes"));
    btn_yes->addTouchEventListener(target, callbackLT);
    
    Button* btn_no = static_cast<Button*>(m_puiLayer->getWidgetByName("btn_no"));
    btn_no->addTouchEventListener(this, toucheventselector(FerrisDialog::callbackDoNothing));
    
    if (type == DIALOG_TYPE_CONFIRM_CANCEL)
    {

    }
    else if (type == DIALOG_TYPE_LEFT_RIGHT)
    {
       
    }
    else if (type == DIALOG_TYPE_CLOSE)
    {
        //确认按钮
        CCMenuItemImage * miBtnLT = CCMenuItemImage::create("ui/button/btn_common_yes_1.png",
                                                            "ui/button/btn_common_yes_2.png",
                                                            this,
                                                            menu_selector(FerrisDialog::callbackDialogDoNothing));
        miBtnLT->setAnchorPoint(ccp(0.5f, 0.5f));
        miBtnLT->setPosition(ccp(_s_center.x, _s_center.y - 50));
        
        CCMenu* menu = CCMenu::create(miBtnLT, NULL);
        menu->setPosition(CCPointZero);
        
        this->addChild(menu);
    }
    else if (type == DIALOG_TYPE_ONLY_OK)
    {
        
    }
    
    return true;
}

bool FerrisDialog::initWithStr(const char *str, cocos2d::CCNode *target, SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT,bool canSell)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    m_puiLayer = TouchGroup::create();
    
    addChild(m_puiLayer);
    
    Layout* root_dialog = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/confirm_layer.ExportJson"));
    
    m_puiLayer->addWidget(root_dialog);
    
    Label* label_style = static_cast<Label*>(m_puiLayer->getWidgetByName("label_style"));
    
    if (canSell)
    {
        label_style->setText(StringLibrary::shared()->getStringByKey("sellEquip")->getCString());
    }
    else
    {
        
    }
    
    Label* label_name = static_cast<Label*>(m_puiLayer->getWidgetByName("label_name"));
    if (canSell)
    {
        label_name->setText(str);
    }
    else
    {
        label_name->setText(StringLibrary::shared()->getStringByKey("cannotsellEquip")->getCString());
    }
    
    Button* btn_confirm = static_cast<Button*>(m_puiLayer->getWidgetByName("Button_19"));
    
    if (canSell)
    {
        btn_confirm->addTouchEventListener(target, callbackLT);
    }
    else
    {
        btn_confirm->addTouchEventListener(this, toucheventselector(FerrisDialog::callbackDoNothing));
    }
    
    
    Button* btn_cancel = static_cast<Button*>(m_puiLayer->getWidgetByName("Button_20"));
    
    btn_cancel->addTouchEventListener(this, toucheventselector(FerrisDialog::callbackDoNothing));
    
    return true;
}

void FerrisDialog::callbackDialogDoNothing(CCObject* pSender)
{
    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
    
    this->removeFromParentAndCleanup(true);
}

void FerrisDialog::callbackDoNothing(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        this->removeFromParentAndCleanup(true);
    }
}

