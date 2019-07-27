#include "CrystalStoreLayer.h"


CrystalStoreLayer::CrystalStoreLayer(void)
{
	//m_pStore = new StoreManager("EditHeroLayer");
}

CrystalStoreLayer::~CrystalStoreLayer(void)
{
    /*
	if(m_pStore)
	{
		delete m_pStore;
		m_pStore = NULL;
	}
     */
}
CrystalStoreLayer* CrystalStoreLayer::create()
{
	CrystalStoreLayer* layer = new CrystalStoreLayer();

	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}

	return NULL;
}

bool CrystalStoreLayer::init()
{
	CCLayer::init();
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_gemstore = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/gem_store_layer.ExportJson"));
    m_pUILayer->addWidget(root_gemstore);
    
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    btn_close->addTouchEventListener(this, toucheventselector(CrystalStoreLayer::callbackClose));
    
/*
	m_nPurchraseType = 0;

	//
	bool bFirst = UserData::shared()->getFirstShop();
	if(bFirst)
	{
		this->addChild(getFirstShopNode(), 0, kTagCurrentPage);
	}
	else
	{
		this->addChild(getCrystalNode(), 0, kTagCurrentPage);
	}

	this->scheduleUpdate();
 */
	return true;
}


void CrystalStoreLayer::callbackClose(cocos2d::CCObject *pSender, TouchEventType type)
{
    if(type == TOUCH_EVENT_ENDED)
    {
        GameDelegate::shared()->closeSceneStoreLayer();
    }
}

/*
//
CCNode* CrystalStoreLayer::getFirstShopNode()
{
	CCNode* pNodeTemp = CCNode::create();

	//
	CCLayerColor* layerColor = CCLayerColor::create(ccc4(0,0,0,128.0f));
	layerColor->setContentSize(CCSizeMake(_s_width, _s_height));
	layerColor->setPosition(ccp(0.0f, 0.0f));
	pNodeTemp->addChild(layerColor, 0);

	//
	CCSprite* spBack = CCSprite::create("menu/store_back_common.png");
	spBack->setAnchorPoint(ccp(0.5f, 0.5f));
	spBack->setPosition(_s_center);
	pNodeTemp->addChild(spBack, 0);

	//
	CCSize sizeBack = spBack->getContentSize();

	CCSize sizeCloseBt = CCSprite::create("menu/store_close_button.png")->getContentSize();
	//
	CCMenuItemImage* btClose = CCMenuItemImage::create("menu/store_close_button.png",
		"menu/store_close_button.png",
		this,
		menu_selector(CrystalStoreLayer::callbackClose));
	btClose->setPosition(ccp(sizeBack.width - sizeCloseBt.width, sizeBack.height - sizeCloseBt.height/2 - 3.0f));

	CCMenuItemImage* btRecharge = CCMenuItemImage::create("menu/button_firstshop.png",
		"menu/button_firstshop.png",
		this,
		menu_selector(CrystalStoreLayer::callbackRecharge));
	btRecharge->setPosition(ccp(sizeBack.width/2.0f, 59.0f));

	CCMenu* pCloseMenu = CCMenu::create(btClose, btRecharge, NULL);
	pCloseMenu->setPosition(CCPointZero);
	spBack->addChild(pCloseMenu, 1);

	//
	CCSprite* spIcon = CCSprite::create("menu/panel_firstshop_icon.png");
	spIcon->setAnchorPoint(ccp(0.5f, 0.0f));
	spIcon->setPosition(ccp(sizeBack.width/2, 0.0f));
	spBack->addChild(spIcon, 0);

	return pNodeTemp;
}

//钻石商店节点
CCNode* CrystalStoreLayer::getCrystalNode()
{
	CCNode* pNodeTemp = CCNode::create();

	//
	CCLayerColor* layerColor = CCLayerColor::create(ccc4(0,0,0,128.0f));
	layerColor->setContentSize(CCSizeMake(_s_width, _s_height));
	layerColor->setPosition(ccp(0.0f, 0.0f));
	pNodeTemp->addChild(layerColor, 0);

	//
	CCSprite* spBack = CCSprite::create("menu/store_back_common.png");
	spBack->setAnchorPoint(ccp(0.5f, 0.5f));
	spBack->setPosition(_s_center);
	pNodeTemp->addChild(spBack, 0);

	//
	CCSprite* spBottom = CCSprite::create("menu/panel_shop_bottom.png");
	spBottom->setAnchorPoint(ccp(0.5f, 0.0f));
	spBottom->setPosition(ccp(spBack->getContentSize().width/2, 0.0f));
	spBack->addChild(spBottom, 0);
	spBottom->setScaleY(1.21f);

	CCSprite* spBottomCover = CCSprite::create("menu/panel_shop_bottom_cover.png");
	spBottomCover->setAnchorPoint(ccp(0.5f, 0.0f));
	spBottomCover->setPosition(ccp(spBack->getContentSize().width/2, 0.0f));
	spBack->addChild(spBottomCover, 1);

	//
	CCSize sizeBack = spBack->getContentSize();

	//
	//
	float fWidth = CCSprite::create("menu/icon_gold.png")->getContentSize().width;
	CCSprite* spCrystal = CCSprite::create("menu/store_crystal_select_bar.png");
	spCrystal->setAnchorPoint(ccp(0.0f, 1.0f));
	spCrystal->setPosition(ccp(20.0f, sizeBack.height - 25.0f));
	spBack->addChild(spCrystal, 1);

	int goldnum = UserData::shared()->getGemNum();
	m_pLabelUserGoldNum = CCLabelTTF::create(CCString::createWithFormat("%d", goldnum)->getCString(),
		"Arial", SIZE_BIG);
	m_pLabelUserGoldNum->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pLabelUserGoldNum->setPosition(ccp(spCrystal->getPositionX() + spCrystal->getContentSize().width/8*5 + 2.0f, spCrystal->getPositionY() - spCrystal->getContentSize().height/2));
	spBack->addChild(m_pLabelUserGoldNum, 1);

	CCSize sizeCloseBt = CCSprite::create("menu/store_close_button.png")->getContentSize();
	//
	CCMenuItemImage* btClose = CCMenuItemImage::create("menu/store_close_button.png",
		"menu/store_close_button.png",
		this,
		menu_selector(CrystalStoreLayer::callbackClose));
	btClose->setPosition(ccp(sizeBack.width - sizeCloseBt.width, sizeBack.height - sizeCloseBt.height/2 - 3.0f));

	CCMenu* pCloseMenu = CCMenu::create(btClose, NULL);
	pCloseMenu->setPosition(CCPointZero);
	spBack->addChild(pCloseMenu, 1);

	CCSize sizeIcon = CCSprite::create("menu/store_crystal_icon.png")->getContentSize();
	CCSize sizeItemBack = CCSprite::create("menu/store_item_back.png")->getContentSize();

	CCScrollView* pScrollView = CCScrollView::create();

	CCNode* pNode = CCNode::create();
	pNode->setContentSize(ccp(sizeBack.width, sizeItemBack.height*14.0f));
	for(int i = 0; i < CRYSTALSTORE_ITEM_NUM; i++)
	{
		//
		CCSprite* spItemBack = CCSprite::create("menu/store_item_back.png");
		spItemBack->setAnchorPoint(ccp(0.5f, 0.5f));
		spItemBack->setPosition(ccp(sizeBack.width/2, sizeBack.height - sizeItemBack.height/2*3 - sizeItemBack.height*(i+1)));
		pNode->addChild(spItemBack);
		//
		CCSprite* spGold = CCSprite::create("menu/store_crystal_icon.png");
		spGold->setAnchorPoint(ccp(0.0f, 0.5f));
		spGold->setPosition(ccp(40.0f, spItemBack->getPositionY()));
		pNode->addChild(spGold);

		//
		CCSprite* spDescribeBack = CCSprite::create("menu/words_describe_gold.png");
		spDescribeBack->setAnchorPoint(ccp(0.0f, 0.0f));
		spDescribeBack->setPosition(ccp(spGold->getPositionX() + spGold->getContentSize().width + 5.0f, spGold->getPositionY()));
		pNode->addChild(spDescribeBack);

		//
		CCLabelTTF* labelGold = CCLabelTTF::create(StringLibrary::shared()->getStringByKey(CCString::createWithFormat("crystalstore_item_%d", i+1)->getCString())->getCString(), "Arial", 16);
		labelGold->setAnchorPoint(ccp(0.0f, 0.0f));
		labelGold->setPosition(ccp(spDescribeBack->getPositionX() + 20.0f, spGold->getPositionY() + 5.0f));
		pNode->addChild(labelGold);

		//
		CCLabelTTF* labelDescribe = CCLabelTTF::create(StringLibrary::shared()->getStringByKey(CCString::createWithFormat("crystalstore_item_extra_%d", i+1)->getCString())->getCString(), "Arial", 16);
		labelDescribe->setAnchorPoint(ccp(0.0f, 1.0f));
		labelDescribe->setPosition(ccp(spDescribeBack->getPositionX() + 20.0f, labelGold->getPositionY() - labelGold->getContentSize().height + 3.0f));
		pNode->addChild(labelDescribe);

	}

	//
	CCMenuItemImage* btBuyGold1 = CCMenuItemImage::create("menu/btn_store_buy_coin_2_1.png",
		"menu/btn_store_buy_coin_2_2.png",
		this,
		menu_selector(CrystalStoreLayer::callbackBuyItem));
	btBuyGold1->setPosition(ccp(sizeBack.width - 70.0f, sizeBack.height - sizeItemBack.height/2*3 - sizeItemBack.height*1));
	btBuyGold1->setTag(0);

	CCMenuItemImage* btBuyGold2 = CCMenuItemImage::create("menu/btn_store_buy_coin_3_1.png",
		"menu/btn_store_buy_coin_3_2.png",
		this,
		menu_selector(CrystalStoreLayer::callbackBuyItem));
	btBuyGold2->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold1->getPositionY() - sizeItemBack.height));
	btBuyGold2->setTag(1);

	CCMenuItemImage* btBuyGold3 = CCMenuItemImage::create("menu/btn_store_buy_coin_6_1.png",
		"menu/btn_store_buy_coin_6_2.png",
		this,
		menu_selector(CrystalStoreLayer::callbackBuyItem));
	btBuyGold3->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold2->getPositionY() - sizeItemBack.height));
	btBuyGold3->setTag(2);

	CCMenuItemImage* btBuyGold4 = CCMenuItemImage::create("menu/btn_store_buy_coin_4_1.png",
		"menu/btn_store_buy_coin_4_2.png",
		this,
		menu_selector(CrystalStoreLayer::callbackBuyItem));
	btBuyGold4->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold3->getPositionY() - sizeItemBack.height));
	btBuyGold4->setTag(3);

	CCMenuItemImage* btBuyGold5 = CCMenuItemImage::create("menu/btn_store_buy_coin_5_1.png",
		"menu/btn_store_buy_coin_5_2.png",
		this,
		menu_selector(CrystalStoreLayer::callbackBuyItem));
	btBuyGold5->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold4->getPositionY() - sizeItemBack.height));
	btBuyGold5->setTag(4);

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
	int nCryStalNum = UserData::shared()->getGemNum();
	m_pLabelUserGoldNum1 = CCLabelTTF::create(CCString::createWithFormat("%d", nCryStalNum)->getCString(),
		"Arial", SIZE_BIG);
	m_pLabelUserGoldNum1->setAnchorPoint(ccp(0.0f, 0.5f));
	m_pLabelUserGoldNum1->setPosition(ccp(_s_center.x + 10.0f, _s_height - 28.0f));
	this->addChild(m_pLabelUserGoldNum1, -1);

	return pNodeTemp;
}

void CrystalStoreLayer::callbackBuyItem(CCObject* pSender)
{
	//点击的购买的钻石类型
	m_nPurchraseType = ((CCMenuItemImage*)pSender)->getTag();

	//m_pStore->newPurchase("BUY_CRYSTAL_2", nType, this, callfuncND_selector(CrystalStoreLayer::callbackConfigure));

	//HttpInstance::sharedHttpInstance()->setCallSDK(true);
	//http检查网络
	//HttpInstance::sharedHttpInstance()->check();
	
    
    //UserData::shared()->savePurchraseHistory(12, 10010);
}

void CrystalStoreLayer::callbackConfigure(CCNode* target, void* data)
{
	CCDictionary* dic = (CCDictionary*)data;

    //用做统计
    int rmb = 0;
    int crystalNum = 0;

    if(!dic->objectForKey("fake_code"))
    {
        CCLog("buyMoneyFromJavaCallBack fake_code is null");
    }

    int paytype = dic->valueForKey("fake_code")->intValue();
	const char* tid = dic->valueForKey("tradeId")->getCString();
	int nCode = dic->valueForKey("code")->intValue();
	int nNum = 0;

	switch (paytype)
	{
	case BUY_CRYSTAL_2:
		nNum = 220;
		rmb = 2;
		break;
	case BUY_CRYSTAL_4:
		nNum = 480;
		rmb = 4;
		break;
	case BUY_CRYSTAL_6:
		nNum = 800;
		rmb = 6;
		break;
	case BUY_CRYSTAL_8:
		nNum = 1200;
		rmb = 8;
		break;
	case BUY_CRYSTAL_12:
		nNum = 2400;
		rmb = 12;
		break;
	}
    
	//购买成功（还没有到账）
	if(nCode == 100)
	{
		//增加消费记录
		//UserData::shared()->savePurchraseHistory(rmb, getIntFromS(tid));
		CCLog("write the massage to the xml file  rmb==(%d), tid==(%s)", rmb, tid);
	}
	else
	{
		CCLog("pay failed   code == (%d)", nCode);
	}
	return;
//////////////////////////////////////////////////////////////////////
 //   crystalNum = nNum;

	//bool bFirst = UserData::shared()->getFirstShop();
	//if(bFirst)
	//{
	//	nNum = nNum * 2;
	//}

 //   if(paytype >= BUY_CRYSTAL_2)
 //   {
	//	//
	//	UserData::shared()->setFirstShop(false);

 //       //
 //       UserData::shared()->increaseCryStalNum(nNum);

 //       //
 //       UserData::shared()->save();

	//	//UserData::shared()->savePurchraseHistory(rmb, tid);
 //       
 //       //统计钻石来源 ID为6
 //       MobClickCpp::bonus(nNum, DIAMOND_GETBY_PAY);

	//	//
	//	int crystalnum = UserData::shared()->getCryStalNum();
	//	m_pLabelUserGoldNum->setString(getStringFromInt(crystalnum).c_str());
	//	m_pLabelUserGoldNum1->setString(getStringFromInt(crystalnum).c_str());
 //       if (bFirst)
 //       {
 //           //统计首次充值
 //           MobClickCpp::event(StatID::getCrystalID(crystalNum));
 //       }
 //       
 //       //统计购买钻石
 //       MobClickCpp::pay(rmb, 10, StatID::getCrystalID(crystalNum), 1, 0);

 //       //÷ß∏∂≥…π¶µƒ∂‘ª∞øÚ
 //       CCNode *nodeDialog = MyDialog::create(DIALOG_TYPE_CLOSE,
 //                                             "buy_crystal_success",
 //                                             this,
 //                                             NULL,
 //                                             NULL);
 //       this->addChild(nodeDialog, GAME_ZORDER_DIALOG + 100, kTagDialog1);
 //   }
 //   else if(paytype == BUY_FAILED)
 //   {
	//	//统计购买失败
	//	MobClickCpp::event("pay_failed");
	//	CCLog("-------store----------MobClickCpp::event(pay_failed);--------------");
 //       //÷ß∏∂ ß∞‹µƒ∂‘ª∞øÚ
 //       CCNode *nodeDialog = MyDialog::create(DIALOG_TYPE_CLOSE,
 //                                             "buy_failed",
 //                                             this,
 //                                             NULL,
 //                                             NULL);
 //       this->addChild(nodeDialog, GAME_ZORDER_DIALOG + 100 , kTagDialog1);
 //   }
 //   else if(paytype == BUY_AGAIN)
 //   {
	//	//统计再次购买
	//	MobClickCpp::event("pay_again");
 //       //‘Ÿ¥Œ÷ß∏∂µƒ∂‘ª∞øÚ
 //       CCNode *nodeDialog = MyDialog::create(DIALOG_TYPE_CLOSE,
 //                                             "buy_again",
 //                                             this,
 //                                             NULL,
 //                                             NULL);
 //       this->addChild(nodeDialog, GAME_ZORDER_DIALOG + 100, kTagDialog1);
 //   }
}

void CrystalStoreLayer::callbackClose(CCObject* pSender)
{
	CCDirector::sharedDirector()->popScene();
}

void CrystalStoreLayer::callbackRecharge(CCObject* pSender)
{
	this->removeChildByTag(kTagCurrentPage);

	this->addChild(getCrystalNode(), 0, kTagCurrentPage);
}

void CrystalStoreLayer::update(float delta)
{
	//时时刷新SDK的调用

}

 */


