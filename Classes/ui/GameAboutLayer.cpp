#include "GameAboutLayer.h"

enum
{
    ZORDER_ABOUT_BG = 0,
    ZORDER_ABOUT_COUPON,
    ZORDER_ABOUT_DIALOG,
};

GameAboutLayer::GameAboutLayer(void)
{
    CCLog("GameAboutLayer Created");
    
    managerCoupon = CouponManager::create(this, CONVERTER_FUNC_CALLBACK(GameAboutLayer::callbackCouponResult));
    managerCoupon->retain();
    
    eClosingCoupon = kCouponClosed;
}

GameAboutLayer::~GameAboutLayer(void)
{
    CC_SAFE_RELEASE(managerCoupon);
    
    CCLog("GameAboutLayer Destroyed");
}

bool GameAboutLayer::init()
{
	CCLayer::init();

	CCNode* pNode = getAboutNode();
	pNode->setPosition(CCPointZero);
	this->addChild(pNode, ZORDER_ABOUT_BG, TagAboutNode);

	this->scheduleUpdate();
	setKeypadEnabled(true);
	return true;
}

GameAboutLayer* GameAboutLayer::create()
{
	GameAboutLayer *layer = new GameAboutLayer();
	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}

/**********************************************
 * 输入法挂接
 **********************************************/

// 要有输出口
bool GameAboutLayer::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
	//CCLOG("onTextFieldAttachWithIME启动输入");
	return false;
	//return true:不启动
}
// 当用户关闭虚拟键盘的时候回调函数
bool GameAboutLayer::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
	//CCLOG("onTextFieldDetachWithIME关闭输入");
	return false;
	//return true:不关闭
}
// 当用户进行输入 虚拟键盘的时候回调函数
bool GameAboutLayer::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
	//CCLOG("onTextFieldInsertText输入字符...");
	return false;
	//return true:不会输入进字符
}
// 当用户进行删除文字 虚拟键盘的时候回调函数
bool GameAboutLayer::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
	//CCLOG("onTextFieldDeleteBackward删除字符");
	return false;
	//return true:不删除
}

/**********************************************
 * 输入法挂接
 **********************************************/

void GameAboutLayer::keyBackClicked()
{
    //todo::关于返回处理
	//GameDelegate::shared()->openSceneMainMenu();
}

void GameAboutLayer::keyMenuClicked()
{
    
}

/**********************************************
 * 其他控制
 **********************************************/

//返回
void GameAboutLayer::menu_Return(CCObject* pSender)
{
    //返回时,对CouponManager的回调函数进行清理,然后再交由系统做release
    managerCoupon->cleanup();
    
    //todo::返回
    //GameDelegate::shared()->openSceneMainMenu();
}


/**********************************************
 * 申诉
 **********************************************/

//发起申诉
void GameAboutLayer::menu_SendComplaint(CCObject* pSender)
{
    ComplaintManager::shared()->requestComplaint();
}

//申诉结果显示

/**********************************************
 * 奖券
 **********************************************/

//开启输入奖券编码的界面
void GameAboutLayer::menu_OpenCoupon(CCObject* pSender)
{
    //避免运行Close的时候 用户误按了Open
    if (eClosingCoupon == kCouponClosed)
    {
        CCNode* pNode = this->getInputControlNode();
        pNode->setPosition(ccp(0.0f, -_s_height));
        pNode->runAction(CCMoveBy::create(0.5f, ccp(0.0f, _s_height)));
        this->addChild(pNode, ZORDER_ABOUT_COUPON, TagInputDialog);
        
        CCNode* pAboutNode = this->getChildByTag(TagAboutNode);
        if(pAboutNode)
        {
            //关闭输入奖券的界面
            CCSequence* seq = CCSequence::create(CCMoveBy::create(0.5f, ccp(0.0f, _s_height)),
                                                 CCCallFuncN::create(this, callfuncN_selector(GameAboutLayer::callbackCouponOpened)),
                                                 NULL);
            pAboutNode->runAction(seq);
            
            //pAboutNode->runAction(CCMoveBy::create(0.5f, ccp(0.0f, _s_height)));
        }
    }
}

//确定关闭输入奖券编码的界面
void GameAboutLayer::callbackCouponOpened(CCNode* pSender)
{
    eClosingCoupon = kCouponOpen;
}

//开始关闭输入奖券编码的界面
void GameAboutLayer::menu_StartCloseCoupon(CCObject* pSender)
{
    //避免再次运行Close
    if (eClosingCoupon == kCouponOpen)
    {
        CCNode* pDialog = this->getChildByTag(TagInputDialog);
        if(pDialog)
        {
            eClosingCoupon = kCouponClosing;
            
            //取消输入法
            CCTextFieldTTF* pInputControl = (CCTextFieldTTF*)pDialog->getChildByTag(TagInputControl);
            
            if(pInputControl)
            {
                pInputControl->detachWithIME();
                
            }
            
            //关闭输入奖券的界面
            CCSequence* seq = CCSequence::create(CCMoveBy::create(0.5f, ccp(0.0f, -_s_height)),
                                                 CCCallFuncN::create(this, callfuncN_selector(GameAboutLayer::callbackCouponClosed)),
                                                 NULL);
            pDialog->runAction(seq);
        }
        
        
        CCNode* pAboutNode = this->getChildByTag(TagAboutNode);
        if(pAboutNode)
        {
            pAboutNode->runAction(CCMoveBy::create(0.5f, ccp(0.0f, -_s_height)));
        }
    }
}

//确定关闭输入奖券编码的界面
void GameAboutLayer::callbackCouponClosed(CCNode* pSender)
{
	this->removeChildByTag(TagInputDialog);
    
    eClosingCoupon = kCouponClosed;
}

//用户的输入框体(实际上是一个按钮),点击后挂接输入法
void GameAboutLayer::menu_StartInputCode(CCObject* pSender)
{
	CCNode* pDialog = this->getChildByTag(TagInputDialog);
	if(pDialog)
	{
		CCTextFieldTTF* pInputControl = (CCTextFieldTTF*)pDialog->getChildByTag(TagInputControl);
        
		if(pInputControl)
		{
			pInputControl->attachWithIME();
		}
	}
}

//发送奖券编码
void GameAboutLayer::menu_SendCoupon(CCObject* pSender)
{
	CCNode* pDialog = this->getChildByTag(TagInputDialog);
    
	if(pDialog)
	{
		CCTextFieldTTF* pInputControl = (CCTextFieldTTF*)pDialog->getChildByTag(TagInputControl);
	
        if(pInputControl)
		{
			const char* pText = pInputControl->getString();
            
			managerCoupon->requestCoupon(pText);
            
			pInputControl->detachWithIME();
		}
	}
}

//发送奖券后,服务器产生的回调
void GameAboutLayer::callbackCouponResult(FerrisResult *result)
{
    if (result && result->getResult())
    {
        CCLog("callbackCouponResult OK");
        
        const char *format = StringLibrary::shared()->getStringByKey("bonus_gem")->getCString();
        const char *content = CCString::createWithFormat(format, result->getAmount())->getCString();
        
//        CCNode *nodeDialog = FerrisDialog::createWithContent(DIALOG_TYPE_ONLY_OK,
//                                                             content,
//                                                             this,
//                                                             menu_selector(GameAboutLayer::callbackCloseCouponAlert),
//                                                             NULL);
//        
//        this->addChild(nodeDialog, ZORDER_ABOUT_DIALOG, TagCouponAlert);
    }
    else
    {
        CCLog("callbackCouponResult Failed");
    }
}

void GameAboutLayer::callbackCloseCouponAlert(CCObject *pSender)
{
    //关闭提示框
    this->removeChildByTag(TagCouponAlert);
    
    if (eClosingCoupon == kCouponOpen)
    {
        menu_StartCloseCoupon(NULL);
    }
}

/**********************************************
 * 创建节点
 **********************************************/

//获得输入框节点
CCNode* GameAboutLayer::getInputControlNode()
{
	CCNode* pNode = CCNode::create();

	//输入框背景
	CCMenuItemImage* pInputBack = CCMenuItemImage::create("ui/text/input_control_back.png", "ui/text/input_control_back.png",
		this,  menu_selector(GameAboutLayer::menu_StartInputCode));
	pInputBack->setPosition(ccp(_s_width/2, _s_height/7*6));

	CCTextFieldTTF *textField = CCTextFieldTTF::textFieldWithPlaceHolder(StringLibrary::shared()->getStringByKey("please_input")->getCString(), "Helvetica", 32);
	textField->setAnchorPoint(ccp(0.0f, 0.5f));
	textField->setPosition(ccp(_s_width/2 - pInputBack->getContentSize().width/2 + 10.0f, pInputBack->getPositionY()));
	pNode->addChild(textField, 11, TagInputControl); 

	//绑定接口
	textField->setDelegate(this);
    
	//开启输入
	textField->attachWithIME();

	//发送按钮
	CCMenuItemImage* item_label = CCMenuItemImage::create("ui/button/btn_coupon_send_1.png", "ui/button/btn_coupon_send_2.png", 
		this,  menu_selector(GameAboutLayer::menu_SendCoupon));
	item_label->setPosition(_s_width/2 - 110.0f, textField->getPositionY() - item_label->getContentSize().height/2*3);

	//关闭按钮
	CCMenuItemImage* item_label1 = CCMenuItemImage::create("ui/button/btn_coupon_close_1.png", "ui/button/btn_coupon_close_2.png",
		this,  menu_selector(GameAboutLayer::menu_StartCloseCoupon));
	item_label1->setPosition(_s_width/2 + 110.0f, item_label->getPositionY());

	CCMenu *ret = CCMenu::create(item_label, item_label1, pInputBack, NULL);
	ret->setPosition(CCPointZero);
	pNode->addChild(ret, 10);

	return pNode;
}

//获取关于节点
CCNode* GameAboutLayer::getAboutNode()
{
	CCNode* pNode = CCNode::create();

	CCSprite *bbb = CCSprite::create("ui/panel/panel_common_bg1.png");
	bbb->setPosition(ccp(_s_width/2, _s_height/2));
	pNode->addChild(bbb,0);
	bbb->setScale(2.0f);

	//添加背景
	CCSprite *sp_back = CCSprite::create("ui/panel/panel_common_bg4.png");
	sp_back->setPosition(ccp(_s_width/2,_s_height/5*3 - 40.0f));
	pNode->addChild(sp_back,0);

	CCMenuItemImage *item_tf = CCMenuItemImage::create("ui/button/btn_common_return_1.png",
		"ui/button/btn_common_return_2.png",
		this,
		menu_selector(GameAboutLayer::menu_Return));
	item_tf->setPosition(ccp(10.0f,item_tf->getContentSize().height));
	item_tf->setAnchorPoint(ccp(0.0f,0.5f));

	CCMenuItemImage* pItemImageComplain = CCMenuItemImage::create("ui/button/btn_complain_1.png", "ui/button/btn_complain_2.png", 
		this,  menu_selector(GameAboutLayer::menu_SendComplaint));
	pItemImageComplain->setPosition(_s_width/2 - 110.0f, sp_back->getPositionY() - sp_back->getContentSize().height/2 + pItemImageComplain->getContentSize().height);

	//确定按钮
	CCMenuItemImage* pItemImageCoupon = CCMenuItemImage::create("ui/button/btn_coupon_start_1.png", "ui/button/btn_coupon_start_2.png",
		this,  menu_selector(GameAboutLayer::menu_OpenCoupon));
	pItemImageCoupon->setPosition(_s_width/2 + 110.0f, pItemImageComplain->getPositionY());

	CCMenu *pMenu = CCMenu::create(item_tf, pItemImageComplain, pItemImageCoupon, NULL);
	pMenu->setPosition(CCPointZero);
	pNode->addChild(pMenu, 1);

	CCString *cstring = StringLibrary::shared()->getStringByKey("abouttitle");
	CCLabelTTF *title = CCLabelTTF::create(cstring->getCString(),"Arial",42);
	title->setPosition(ccp(sp_back->getPositionX(),sp_back->getPositionY()+sp_back->getContentSize().height/2-50.0f));
	title->setAnchorPoint(ccp(0.5f,1.0f));
	pNode->addChild(title,2);

	cstring = StringLibrary::shared()->getStringByKey("about");
	CCLabelTTF *words = CCLabelTTF::create(cstring->getCString(),"Arial",28);
	words->setDimensions(CCSizeMake(sp_back->getContentSize().width/5*4,sp_back->getContentSize().height/5*4));
	words->setPosition(ccp(sp_back->getPositionX(), title->getPositionY() - title->getContentSize().height/2*3));
	words->setAnchorPoint(ccp(0.5f,1.0f));
	pNode->addChild(words,2);

	return pNode;
}


