#include "GemStoreLayer.h"


GemStoreLayer::GemStoreLayer(void)
{
    
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	
    m_pPaymentAndroid = new PaymentAndroid("GemStoreLayer");

#endif
    
}

GemStoreLayer::~GemStoreLayer(void)
{
    
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	
    if(m_pPaymentAndroid)
	{
		delete m_pPaymentAndroid;
		m_pPaymentAndroid = NULL;
	}
    
#endif
    
}
GemStoreLayer* GemStoreLayer::create()
{
	GemStoreLayer* layer = new GemStoreLayer();

	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}

	return NULL;
}

bool GemStoreLayer::init()
{
	CCLayer::init();

	//
	bool bFirst = UserData::shared()->getFirstShop();
    /*
	if(bFirst)
	{
		this->addChild(getFirstShopNode(), 0, kTagCurrentPage);
	}
	else
	{
		this->addChild(getGemNode(), 0, kTagCurrentPage);
	}

	this->scheduleUpdate();
     */
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_gemstore = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/cocostudio/gem_store_layer.ExportJson"));
    m_pUILayer->addWidget(root_gemstore);
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    btn_close->addTouchEventListener(this, toucheventselector(GemStoreLayer::callbackClose));
    
    Button* btn_go = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_go"));
    
    btn_go->addTouchEventListener(this, toucheventselector(GemStoreLayer::btnGoCallBack));
    
    for (int i = 1; i <= 4; i++)
    {
        string str_id = "3_";
        str_id.append(getStringFromInt(i));
        
        PriceData* data = PriceDataManager::shared()->getPriceData(str_id);
        
        if (data)
        {
            CCString* str_label = CCString::createWithFormat("label_%d",i);
            
            Label* label = static_cast<Label*>(m_pUILayer->getWidgetByName(str_label->getCString()));
            
            label->setText(data->getDesLabel());
            
            CCString* str_btn = CCString::createWithFormat("btn_buy_%d",i);
            
            Button* btn = static_cast<Button*>(m_pUILayer->getWidgetByName(str_btn->getCString()));
            
            btn->setName(data->getID().c_str());
            
            btn->addTouchEventListener(this, toucheventselector(GemStoreLayer::callbackBuyItem));
            
            string str_consume = "";
            str_consume.append("￥").append(getStringFromInt(data->getConsume()));
            
            btn->setTitleText(str_consume);
        }
    }
    
    if (!UserData::shared()->getFirstShop())
    {
        UIPanel* panel_ad = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_ad"));
        
        panel_ad->removeFromParentAndCleanup(true);
    }
    else
    {
        ActionManager::shareManager()->playActionByName("gem_store_layer.ExportJson", "Animation0");
    }

    
    
	return true;
}

void GemStoreLayer::callbackBuyItem(cocos2d::CCObject *pSender, TouchEventType type)
{

    if (type == TOUCH_EVENT_ENDED)
    {
        
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        Button* btn = (Button*)pSender;
        
        string btn_id = btn->getName();
        
        PriceData* data = PriceDataManager::shared()->getPriceData(btn_id);
        
        //todo: 消费处理
    }

}

void GemStoreLayer::callbackClose(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        GameDelegate::shared()->closeSceneStoreLayer();
    }
}

void GemStoreLayer::btnGoCallBack(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        CCLog("btn_go clicked!");
        
        UIPanel* panel_ad = static_cast<UIPanel*>(m_pUILayer->getWidgetByName("panel_ad"));
        
        panel_ad->removeFromParentAndCleanup(true);
    }
}

/*
//
CCNode* GemStoreLayer::getFirstShopNode()
{
	CCNode* pNodeTemp = CCNode::create();

	//
	CCLayerColor* layerColor = CCLayerColor::create(ccc4(0,0,0,128.0f));
	layerColor->setContentSize(CCSizeMake(_s_width, _s_height));
	layerColor->setPosition(ccp(0.0f, 0.0f));
	pNodeTemp->addChild(layerColor, 0);

	//
	CCSprite* spBack = CCSprite::create("ui/panel/panel_store_bg.png");
	spBack->setAnchorPoint(ccp(0.5f, 0.5f));
	spBack->setPosition(_s_center);
	pNodeTemp->addChild(spBack, 0);

	//
	CCSize sizeBack = spBack->getContentSize();

	CCSize sizeCloseBt = CCSprite::create("ui/button/btn_store_close.png")->getContentSize();
	//
	CCMenuItemImage* btClose = CCMenuItemImage::create("ui/button/btn_store_close.png",
		"ui/button/btn_store_close.png",
		this,
		menu_selector(GemStoreLayer::callbackClose));
	btClose->setPosition(ccp(sizeBack.width - sizeCloseBt.width, sizeBack.height - sizeCloseBt.height/2 - 3.0f));

	CCMenuItemImage* btRecharge = CCMenuItemImage::create("ui/button/btn_store_firstshop.png",
		"ui/button/btn_store_firstshop.png",
		this,
		menu_selector(GemStoreLayer::callbackRecharge));
	btRecharge->setPosition(ccp(sizeBack.width/2.0f, 59.0f));

	CCMenu* pCloseMenu = CCMenu::create(btClose, btRecharge, NULL);
	pCloseMenu->setPosition(CCPointZero);
	spBack->addChild(pCloseMenu, 1);

	//
	CCSprite* spIcon = CCSprite::create("ui/panel/panel_firstshop.png");
	spIcon->setAnchorPoint(ccp(0.5f, 0.0f));
	spIcon->setPosition(ccp(sizeBack.width/2, 0.0f));
	spBack->addChild(spIcon, 0);

	return pNodeTemp;
}

//钻石商店节点
CCNode* GemStoreLayer::getGemNode()
{
	CCNode* pNodeTemp = CCNode::create();

	//
	CCLayerColor* layerColor = CCLayerColor::create(ccc4(0,0,0,128.0f));
	layerColor->setContentSize(CCSizeMake(_s_width, _s_height));
	layerColor->setPosition(ccp(0.0f, 0.0f));
	pNodeTemp->addChild(layerColor, 0);

	//
	CCSprite* spBack = CCSprite::create("ui/panel/panel_store_bg.png");
	spBack->setAnchorPoint(ccp(0.5f, 0.5f));
	spBack->setPosition(_s_center);
	pNodeTemp->addChild(spBack, 0);

	//
	CCSprite* spBottom = CCSprite::create("ui/panel/panel_store_bottom.png");
	spBottom->setAnchorPoint(ccp(0.5f, 0.0f));
	spBottom->setPosition(ccp(spBack->getContentSize().width/2, 0.0f));
	spBack->addChild(spBottom, 0);
	spBottom->setScaleY(1.21f);

	CCSprite* spBottomCover = CCSprite::create("ui/panel/panel_store_bottom_cover.png");
	spBottomCover->setAnchorPoint(ccp(0.5f, 0.0f));
	spBottomCover->setPosition(ccp(spBack->getContentSize().width/2, 0.0f));
	spBack->addChild(spBottomCover, 1);

	//
	CCSize sizeBack = spBack->getContentSize();

	//
	//
	float fWidth = CCSprite::create("ui/icon/icon_gold.png")->getContentSize().width;
	CCSprite* spGem = CCSprite::create("ui/panel/panel_store_select_gem.png");
	spGem->setAnchorPoint(ccp(0.0f, 1.0f));
	spGem->setPosition(ccp(20.0f, sizeBack.height - 25.0f));
	spBack->addChild(spGem, 1);

	int goldnum = UserData::shared()->getGemNum();
	m_pLabelUserGoldNum = CCLabelTTF::create(CCString::createWithFormat("%d", goldnum)->getCString(),
		"Arial", SIZE_BIG);
	m_pLabelUserGoldNum->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pLabelUserGoldNum->setPosition(ccp(spGem->getPositionX() + spGem->getContentSize().width/8*5 + 2.0f, spGem->getPositionY() - spGem->getContentSize().height/2));
	spBack->addChild(m_pLabelUserGoldNum, 1);

	CCSize sizeCloseBt = CCSprite::create("ui/button/btn_store_close.png")->getContentSize();
	//
	CCMenuItemImage* btClose = CCMenuItemImage::create("ui/button/btn_store_close.png",
		"ui/button/btn_store_close.png",
		this,
		menu_selector(GemStoreLayer::callbackClose));
	btClose->setPosition(ccp(sizeBack.width - sizeCloseBt.width, sizeBack.height - sizeCloseBt.height/2 - 3.0f));

	CCMenu* pCloseMenu = CCMenu::create(btClose, NULL);
	pCloseMenu->setPosition(CCPointZero);
	spBack->addChild(pCloseMenu, 1);

	CCSize sizeIcon = CCSprite::create("ui/icon/icon_store_gem.png")->getContentSize();
	CCSize sizeItemBack = CCSprite::create("ui/panel/panel_store_item_bg.png")->getContentSize();

	CCScrollView* pScrollView = CCScrollView::create();

	CCNode* pNode = CCNode::create();
	pNode->setContentSize(ccp(sizeBack.width, sizeItemBack.height*14.0f));
	for(int i = 0; i < GEMSTORE_ITEM_NUM; i++)
	{
		//
		CCSprite* spItemBack = CCSprite::create("ui/panel/panel_store_item_bg.png");
		spItemBack->setAnchorPoint(ccp(0.5f, 0.5f));
		spItemBack->setPosition(ccp(sizeBack.width/2, sizeBack.height - sizeItemBack.height/2*3 - sizeItemBack.height*(i+1)));
		pNode->addChild(spItemBack);
		//
		CCSprite* spGold = CCSprite::create("ui/icon/icon_store_gem.png");
		spGold->setAnchorPoint(ccp(0.0f, 0.5f));
		spGold->setPosition(ccp(40.0f, spItemBack->getPositionY()));
		pNode->addChild(spGold);

		//
		CCSprite* spDescribeBack = CCSprite::create("ui/panel/panel_store_description.png");
		spDescribeBack->setAnchorPoint(ccp(0.0f, 0.0f));
		spDescribeBack->setPosition(ccp(spGold->getPositionX() + spGold->getContentSize().width + 5.0f, spGold->getPositionY()));
		pNode->addChild(spDescribeBack);

		//
		CCLabelTTF* labelGold = CCLabelTTF::create(StringLibrary::shared()->getStringByKey(CCString::createWithFormat("gemstore_item_%d", i+1)->getCString())->getCString(), "Arial", 16);
		labelGold->setAnchorPoint(ccp(0.0f, 0.0f));
		labelGold->setPosition(ccp(spDescribeBack->getPositionX() + 20.0f, spGold->getPositionY() + 5.0f));
		pNode->addChild(labelGold);

		//
		CCLabelTTF* labelDescribe = CCLabelTTF::create(StringLibrary::shared()->getStringByKey(CCString::createWithFormat("gemstore_item_extra_%d", i+1)->getCString())->getCString(), "Arial", 16);
		labelDescribe->setAnchorPoint(ccp(0.0f, 1.0f));
		labelDescribe->setPosition(ccp(spDescribeBack->getPositionX() + 20.0f, labelGold->getPositionY() - labelGold->getContentSize().height + 3.0f));
		pNode->addChild(labelDescribe);

	}

	//
	CCMenuItemImage* btBuyGold1 = CCMenuItemImage::create("ui/button/btn_store_buy_coin_2_1.png",
		"ui/button/btn_store_buy_coin_2_2.png",
		this,
		menu_selector(GemStoreLayer::callbackBuyItem));
	btBuyGold1->setPosition(ccp(sizeBack.width - 70.0f, sizeBack.height - sizeItemBack.height/2*3 - sizeItemBack.height*1));
	btBuyGold1->setTag(BUY_PRODUCT_1);

	CCMenuItemImage* btBuyGold2 = CCMenuItemImage::create("ui/button/btn_store_buy_coin_3_1.png",
		"ui/button/btn_store_buy_coin_3_2.png",
		this,
		menu_selector(GemStoreLayer::callbackBuyItem));
	btBuyGold2->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold1->getPositionY() - sizeItemBack.height));
	btBuyGold2->setTag(BUY_PRODUCT_2);

	CCMenuItemImage* btBuyGold3 = CCMenuItemImage::create("ui/button/btn_store_buy_coin_6_1.png",
		"ui/button/btn_store_buy_coin_6_2.png",
		this,
		menu_selector(GemStoreLayer::callbackBuyItem));
	btBuyGold3->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold2->getPositionY() - sizeItemBack.height));
	btBuyGold3->setTag(BUY_PRODUCT_3);

	CCMenuItemImage* btBuyGold4 = CCMenuItemImage::create("ui/button/btn_store_buy_coin_4_1.png",
		"ui/button/btn_store_buy_coin_4_2.png",
		this,
		menu_selector(GemStoreLayer::callbackBuyItem));
	btBuyGold4->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold3->getPositionY() - sizeItemBack.height));
	btBuyGold4->setTag(BUY_PRODUCT_4);

	CCMenuItemImage* btBuyGold5 = CCMenuItemImage::create("ui/button/btn_store_buy_coin_5_1.png",
		"ui/button/btn_store_buy_coin_5_2.png",
		this,
		menu_selector(GemStoreLayer::callbackBuyItem));
	btBuyGold5->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold4->getPositionY() - sizeItemBack.height));
	btBuyGold5->setTag(BUY_PRODUCT_5);

	CCMenu* pStoreMenu = CCMenu::create(btBuyGold1, btBuyGold2, btBuyGold3, btBuyGold4, btBuyGold5, NULL);
	pStoreMenu->setPosition(CCPointZero);
	pNode->addChild(pStoreMenu, 1);


	pScrollView->setContainer(pNode);
	pScrollView->setViewSize(CCSizeMake(sizeBack.width, sizeBack.height - sizeItemBack.height*2));
	pScrollView->setContentSize(CCSizeMake(sizeBack.width, sizeBack.height- sizeItemBack.height*2));
	pScrollView->setDirection(kCCScrollViewDirectionVertical);
	pScrollView->setPosition(ccp(0.0f, sizeItemBack.height));
	pScrollView->setContentOffset(pScrollView->minContainerOffset());
	spBack->addChild(pScrollView, 1);

	//数字黑色遮罩
	CCLayerColor* layerColorLabel = CCLayerColor::create(ccc4(0,0,0,255.0f));
	layerColorLabel->setContentSize(CCSizeMake(110.0f, 30.0f));
	layerColorLabel->setPosition(ccp(_s_center.x + 10.0f, _s_height - 42.0f));
	this->addChild(layerColorLabel, -1);

	//背景上的钻石数量（虚拟）
	int nGemNum = UserData::shared()->getGemNum();
	m_pLabelUserGoldNum1 = CCLabelTTF::create(CCString::createWithFormat("%d", nGemNum)->getCString(),
		"Arial", SIZE_BIG);
	m_pLabelUserGoldNum1->setAnchorPoint(ccp(0.0f, 0.5f));
	m_pLabelUserGoldNum1->setPosition(ccp(_s_center.x + 10.0f, _s_height - 28.0f));
	this->addChild(m_pLabelUserGoldNum1, -1);

	return pNodeTemp;
}


void GemStoreLayer::callbackBuyItem(CCObject* pSender)
{
	//点击的购买的钻石类型
	int nProductID = ((CCMenuItemImage*)pSender)->getTag();
    
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    
    m_pPaymentAndroid->buy("BUY_PRODUCT", nProductID, this, callfuncND_selector(GemStoreLayer::callbackResponseOfBuyItem));
    
#endif
    
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    
    PaymentIOS::shared()->buy(nProductID);
    
#endif

	//

	//VerificationManager::shared()->setCallSDK(true);
	//http检查网络
	//VerificationManager::shared()->check();
	//PurchaseLogManager::shared()->savePurchraseHistory(12, 10010);
}

void GemStoreLayer::callbackResponseOfBuyItem(CCNode* target, void* data)
{
	CCDictionary* dic = (CCDictionary*)data;

    //用做统计
    if(!dic->objectForKey("fake_code"))
    {
        CCLog("buyMoneyFromJavaCallBack fake_code is null");
    }

    int nCode = dic->valueForKey("code")->intValue();
    int nProductID = dic->valueForKey("product_id")->intValue();
    int nPrice = getPriceByProductID(nProductID);
	const char* sTradeID = dic->valueForKey("trade_id")->getCString();
    
	//购买成功（还没有到账）
	if(nCode == 100)
	{
		//增加消费记录
        PurchaseLogManager::shared()->insert(sTradeID, nProductID);
        
		CCLog("write the massage to the xml file rmb=(%d), tid=(%s)", nPrice, sTradeID);
	}
	else
	{
		CCLog("pay failed code == (%d)", nCode);
	}
    
	return;
//////////////////////////////////////////////////////////////////////
 //   gemNum = nNum;

	//bool bFirst = UserData::shared()->getFirstShop();
	//if(bFirst)
	//{
	//	nNum = nNum * 2;
	//}

 //   if(paytype >= BUY_PRODUCT_1)
 //   {
	//	//
	//	UserData::shared()->setFirstShop(false);

 //       //
 //       UserData::shared()->increaseGemNum(nNum);

 //       //
 //       UserData::shared()->save();

	//	//UserData::shared()->savePurchraseHistory(rmb, tid);
 //       
 //       //统计钻石来源 ID为6
 //       MobClickCpp::bonus(nNum, ANALYTICS_GEM_GETBY_PAY);

	//	//
	//	int gemnum = UserData::shared()->getGemNum();
	//	m_pLabelUserGoldNum->setString(convertIntToString(gemnum).c_str());
	//	m_pLabelUserGoldNum1->setString(convertIntToString(gemnum).c_str());
 //       if (bFirst)
 //       {
 //           //统计首次充值
 //           MobClickCpp::event(AnalyticsStatID::getGemID(gemNum));
 //       }
 //       
 //       //统计购买钻石
 //       MobClickCpp::pay(rmb, 10, AnalyticsStatID::getGemID(gemNum), 1, 0);

 //       //÷ß∏∂≥…π¶µƒ∂‘ª∞øÚ
 //       CCNode *nodeDialog = FerrisDialog::createWithLibraryTag(DIALOG_TYPE_CLOSE,
 //                                             "buy_gem_success",
 //                                             this,
 //                                             NULL,
 //                                             NULL);
 //       this->addChild(nodeDialog, GAME_ZORDER_DIALOG + 100, kTagDialogStore);
 //   }
 //   else if(paytype == BUY_FAILED)
 //   {
	//	//统计购买失败
	//	MobClickCpp::event("pay_failed");
	//	CCLog("-------store----------MobClickCpp::event(pay_failed);--------------");
 //       //÷ß∏∂ ß∞‹µƒ∂‘ª∞øÚ
 //       CCNode *nodeDialog = FerrisDialog::createWithLibraryTag(DIALOG_TYPE_CLOSE,
 //                                             "buy_failed",
 //                                             this,
 //                                             NULL,
 //                                             NULL);
 //       this->addChild(nodeDialog, GAME_ZORDER_DIALOG + 100 , kTagDialogStore);
 //   }
 //   else if(paytype == BUY_AGAIN)
 //   {
	//	//统计再次购买
	//	MobClickCpp::event("pay_again");
 //       //‘Ÿ¥Œ÷ß∏∂µƒ∂‘ª∞øÚ
 //       CCNode *nodeDialog = FerrisDialog::createWithLibraryTag(DIALOG_TYPE_CLOSE,
 //                                             "buy_again",
 //                                             this,
 //                                             NULL,
 //                                             NULL);
 //       this->addChild(nodeDialog, GAME_ZORDER_DIALOG + 100, kTagDialogStore);
 //   }
}

void GemStoreLayer::callbackClose(CCObject* pSender)
{
}

void GemStoreLayer::callbackRecharge(CCObject* pSender)
{
	this->removeChildByTag(kTagCurrentPage);

	this->addChild(getGemNode(), 0, kTagCurrentPage);
}

void GemStoreLayer::update(float delta)
{
	//时时刷新SDK的调用
	//if(VerificationManager::shared()->getCallSDK())
	//{
		//m_pPaymentAndroid->buy("BUY_PRODUCT_1", m_nPurchraseType, this, callfuncND_selector(GemStoreLayer::callbackResponseOfBuyItem));
		//VerificationManager::shared()->setCallSDK(false);
	//}

}
*/


