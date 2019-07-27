#include "GoldStoreLayer.h"


GoldStoreLayer::GoldStoreLayer(void)
{

}

GoldStoreLayer::~GoldStoreLayer(void)
{

}

/*
GoldStoreLayer* GoldStoreLayer::create(CCRenderTexture *sqr)
{
	GoldStoreLayer* layer = new GoldStoreLayer();

	if(layer && layer->init(sqr))
	{
		layer->autorelease();
		return layer;
	}

	return NULL;
}

bool GoldStoreLayer::init(CCRenderTexture *sqr)
{
	CCLayer::init();
    
    if (sqr != NULL)
    {
        CCSprite *_spr = CCSprite::createWithTexture(sqr->getSprite()->getTexture());
        _spr->setPosition(ccp(_s_width/2, _s_height/2));
        _spr->setFlipY(true);
        _spr->setColor(ccGRAY);
        this->addChild(_spr, -1);
    }
	

	//
	CCLayerColor* layerColor = CCLayerColor::create(ccc4(0,0,0,128.0f));
	layerColor->setContentSize(CCSizeMake(_s_width, _s_height));
	layerColor->setPosition(ccp(0.0f, 0.0f));
	this->addChild(layerColor, 0);

	//…ÃµÍ±≥æ∞Õº∆¨
	CCSprite* spBack = CCSprite::create("ui/panel/panel_store_bg.png");
	spBack->setAnchorPoint(ccp(0.5f, 0.5f));
	spBack->setPosition(_s_center);
	this->addChild(spBack, 0);

	//±ﬂøÚ’⁄’÷
	CCSprite* spBottom = CCSprite::create("ui/panel/panel_store_bottom.png");
	spBottom->setAnchorPoint(ccp(0.5f, 0.0f));
	spBottom->setPosition(ccp(spBack->getContentSize().width/2, 0.0f));
	spBack->addChild(spBottom, 0);
	spBottom->setScaleY(1.21f);

	CCSprite* spBottomCover = CCSprite::create("ui/panel/panel_store_bottom_cover.png");
	spBottomCover->setAnchorPoint(ccp(0.5f, 0.0f));
	spBottomCover->setPosition(ccp(spBack->getContentSize().width/2, 0.0f));
	spBack->addChild(spBottomCover, 1);

	//±≥æ∞Õº∆¨µƒ≥ﬂ¥Á
	CCSize sizeBack = spBack->getContentSize();

	//◊Û…œΩ«Ω±“
	//Ω±“Õº±Í
	float fWidth = CCSprite::create("ui/icon/icon_store_gold.png")->getContentSize().width;
	CCSprite* spGold = CCSprite::create("ui/panel/panel_store_select_gem.png");
	spGold->setAnchorPoint(ccp(0.0f, 1.0f));
	spGold->setPosition(ccp(20.0f, sizeBack.height - 25.0f));
	spBack->addChild(spGold, 1);

	int goldnum = UserData::shared()->getGoldNum();
	m_pLabelUserGoldNum = CCLabelTTF::create(CCString::createWithFormat("%d", goldnum)->getCString(),
		"Arial", SIZE_BIG);
	m_pLabelUserGoldNum->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pLabelUserGoldNum->setPosition(ccp(spGold->getPositionX() + spGold->getContentSize().width/8*5 + 2.0f, spGold->getPositionY() - spGold->getContentSize().height/2));
	spBack->addChild(m_pLabelUserGoldNum, 1);

	CCSize sizeCloseBt = CCSprite::create("ui/button/btn_store_close.png")->getContentSize();
	//”“…œΩ«πÿ±’∞¥≈•
	CCMenuItemImage* btClose = CCMenuItemImage::create("ui/button/btn_store_close.png",
		"ui/button/btn_store_close.png",
		this,
		menu_selector(GoldStoreLayer::callbackClose));
	btClose->setPosition(ccp(sizeBack.width - sizeCloseBt.width, sizeBack.height - sizeCloseBt.height/2 - 3.0f));

	CCMenu* pCloseMenu = CCMenu::create(btClose, NULL);
	pCloseMenu->setPosition(CCPointZero);
	spBack->addChild(pCloseMenu, 1);

	CCSize sizeIcon = CCSprite::create("ui/icon/icon_store_gold.png")->getContentSize();
	CCSize sizeItemBack = CCSprite::create("ui/panel/panel_store_item_bg.png")->getContentSize();

	CCScrollView* pScrollView = CCScrollView::create();

	CCNode* pNode = CCNode::create();
	pNode->setContentSize(ccp(sizeBack.width, sizeItemBack.height*14.0f));
	for(int i = 0; i < GOLDSTORE_ITEM_NUM; i++)
	{
		//œÓƒøµ◊
		CCSprite* spItemBack = CCSprite::create("ui/panel/panel_store_item_bg.png");
		spItemBack->setAnchorPoint(ccp(0.5f, 0.5f));
		spItemBack->setPosition(ccp(sizeBack.width/2, sizeBack.height - sizeItemBack.height/2*3 - sizeItemBack.height*(i+1)));
		pNode->addChild(spItemBack);
		//Ω±“Õº±Í
		CCSprite* spGold = CCSprite::create("ui/icon/icon_store_gold.png");
		spGold->setAnchorPoint(ccp(0.0f, 0.5f));
		spGold->setPosition(ccp(40.0f, spItemBack->getPositionY()));
		pNode->addChild(spGold);

		//√Ë ˆŒƒ◊÷µƒµ◊
		CCSprite* spDescribeBack = CCSprite::create("ui/panel/panel_store_description.png");
		spDescribeBack->setAnchorPoint(ccp(0.0f, 0.0f));
		spDescribeBack->setPosition(ccp(spGold->getPositionX() + spGold->getContentSize().width + 5.0f, spGold->getPositionY()));
		pNode->addChild(spDescribeBack);

		//ŒÔ∆∑√Ë ˆ
		CCLabelTTF* labelGold = CCLabelTTF::create(StringLibrary::shared()->getStringByKey(CCString::createWithFormat("goldstore_item_%d", i+1)->getCString())->getCString(), "Arial", 16);
		labelGold->setAnchorPoint(ccp(0.0f, 0.0f));
		labelGold->setPosition(ccp(spDescribeBack->getPositionX() + 20.0f, spGold->getPositionY() + 5.0f));
		pNode->addChild(labelGold);

		if(i > 0)
		{
			//ŒÔ∆∑‘˘ÀÕ√Ë ˆ
			CCLabelTTF* labelDescribe = CCLabelTTF::create(StringLibrary::shared()->getStringByKey(CCString::createWithFormat("goldstore_item_extra_%d", i+1)->getCString())->getCString(), "Arial", 16);
			labelDescribe->setAnchorPoint(ccp(0.0f, 1.0f));
			labelDescribe->setPosition(ccp(spDescribeBack->getPositionX() + 20.0f, labelGold->getPositionY() - labelGold->getContentSize().height + 3.0f));
			pNode->addChild(labelDescribe);
		}

	}

		//∞¥≈•
		CCMenuItemImage* btBuyGold1 = CCMenuItemImage::create("ui/button/btn_store_gem_1.png",
			"ui/button/btn_store_gem_2.png",
			this,
			menu_selector(GoldStoreLayer::callbackBuyItem));
		btBuyGold1->setPosition(ccp(sizeBack.width - 70.0f, sizeBack.height - sizeItemBack.height/2*3 - sizeItemBack.height*1));
		btBuyGold1->setTag(GOLDSTORE_ITEM_1);

		CCMenuItemImage* btBuyGold2 = CCMenuItemImage::create("ui/button/btn_store_gem_1.png",
			"ui/button/btn_store_gem_2.png",
			this,
			menu_selector(GoldStoreLayer::callbackBuyItem));
		btBuyGold2->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold1->getPositionY() - sizeItemBack.height));
		btBuyGold2->setTag(GOLDSTORE_ITEM_2);

		CCMenuItemImage* btBuyGold3 = CCMenuItemImage::create("ui/button/btn_store_gem_1.png",
			"ui/button/btn_store_gem_2.png",
			this,
			menu_selector(GoldStoreLayer::callbackBuyItem));
		btBuyGold3->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold2->getPositionY() - sizeItemBack.height));
		btBuyGold3->setTag(GOLDSTORE_ITEM_3);

		CCMenuItemImage* btBuyGold4 = CCMenuItemImage::create("ui/button/btn_store_gem_1.png",
			"ui/button/btn_store_gem_2.png",
			this,
			menu_selector(GoldStoreLayer::callbackBuyItem));
		btBuyGold4->setPosition(ccp(btBuyGold1->getPositionX(), btBuyGold3->getPositionY() - sizeItemBack.height));
		btBuyGold4->setTag(GOLDSTORE_ITEM_4);

		CCMenu* pStoreMenu = CCMenu::create(btBuyGold1, btBuyGold2, btBuyGold3, btBuyGold4, NULL);
		pStoreMenu->setPosition(CCPointZero);
		pNode->addChild(pStoreMenu, 1);

		//º€∏ÒŒƒ◊÷
		CCLabelTTF* labelGem1 = CCLabelTTF::create(CCString::createWithFormat("%d", 200)->getCString(), "Arial", 16);
		labelGem1->setAnchorPoint(ccp(0.0f, 0.5f));
		labelGem1->setPosition(ccp(btBuyGold1->getContentSize().width/2, btBuyGold1->getContentSize().height/2));
		btBuyGold1->addChild(labelGem1);

		CCLabelTTF* labelGem2 = CCLabelTTF::create(CCString::createWithFormat("%d", 450)->getCString(), "Arial", 16);
		labelGem2->setAnchorPoint(ccp(0.0f, 0.5f));
		labelGem2->setPosition(ccp(btBuyGold1->getContentSize().width/2, btBuyGold1->getContentSize().height/2));
		btBuyGold2->addChild(labelGem2);

		CCLabelTTF* labelGem3 = CCLabelTTF::create(CCString::createWithFormat("%d", 1000)->getCString(), "Arial", 16);
		labelGem3->setAnchorPoint(ccp(0.0f, 0.5f));
		labelGem3->setPosition(ccp(btBuyGold1->getContentSize().width/2, btBuyGold1->getContentSize().height/2));
		btBuyGold3->addChild(labelGem3);

		CCLabelTTF* labelGem4 = CCLabelTTF::create(CCString::createWithFormat("%d", 2200)->getCString(), "Arial", 16);
		labelGem4->setAnchorPoint(ccp(0.0f, 0.5f));
		labelGem4->setPosition(ccp(btBuyGold1->getContentSize().width/2, btBuyGold1->getContentSize().height/2));
		btBuyGold4->addChild(labelGem4);


		pScrollView->setContainer(pNode);
		pScrollView->setViewSize(CCSizeMake(sizeBack.width, sizeBack.height - sizeItemBack.height*2));
		pScrollView->setContentSize(CCSizeMake(sizeBack.width, sizeBack.height- sizeItemBack.height*2));
		pScrollView->setDirection(kCCScrollViewDirectionVertical);
		pScrollView->setPosition(ccp(0.0f, sizeItemBack.height));
		pScrollView->setContentOffset(pScrollView->minContainerOffset());
		spBack->addChild(pScrollView, 1);

		//背景上的数量刷新
		//数字黑色遮罩
		CCLayerColor* layerColorLabel = CCLayerColor::create(ccc4(0,0,0,255.0f));
		layerColorLabel->setContentSize(CCSizeMake(110.0f, 30.0f));
		layerColorLabel->setPosition(ccp(_s_width/2 - 201.0f, _s_height - 42.0f));
		this->addChild(layerColorLabel, -1);

		//背景上的金币数量（虚拟）
		int nGoldNum = UserData::shared()->getGoldNum();
		m_pLabelUserGoldNum1 = CCLabelTTF::create(CCString::createWithFormat("%d", nGoldNum)->getCString(),
			"Arial", SIZE_BIG);
		m_pLabelUserGoldNum1->setAnchorPoint(ccp(0.0f, 0.5f));
		m_pLabelUserGoldNum1->setPosition(ccp(_s_center.x - 201.0f, _s_height - 28.0f));
		this->addChild(m_pLabelUserGoldNum1, -1);


		//数字黑色遮罩
		CCLayerColor* layerColorLabel1 = CCLayerColor::create(ccc4(0,0,0,255.0f));
		layerColorLabel1->setContentSize(CCSizeMake(110.0f, 30.0f));
		layerColorLabel1->setPosition(ccp(_s_center.x + 10.0f, _s_height - 42.0f));
		this->addChild(layerColorLabel1, -1);

		//背景上的钻石数量（虚拟）
		int nGemNum = UserData::shared()->getGemNum();
		m_pLabelUserGemNum1 = CCLabelTTF::create(CCString::createWithFormat("%d", nGemNum)->getCString(),
			"Arial", SIZE_BIG);
		m_pLabelUserGemNum1->setAnchorPoint(ccp(0.0f, 0.5f));
		m_pLabelUserGemNum1->setPosition(ccp(_s_center.x + 10.0f, _s_height - 28.0f));
		this->addChild(m_pLabelUserGemNum1, -1);

	return true;
}

void GoldStoreLayer::callbackBuyItem(CCObject* pSender)
{
	int nType = ((CCMenuItemImage*)pSender)->getTag();
	int nNum = 0;
	int nNeedGem = 0;

	switch (nType)
	{
		case GOLDSTORE_ITEM_1:
			nNum = 500;
			nNeedGem = 200;
			break;
		case GOLDSTORE_ITEM_2:
			nNum = 1300;
			nNeedGem = 450;
			break;
		case GOLDSTORE_ITEM_3:
			nNum = 3200;
			nNeedGem = 1000;
			break;
		case GOLDSTORE_ITEM_4:
			nNum = 8000;
			nNeedGem = 2200;
			break;
	}

	bool bGemEnough = UserData::shared()->isGemEnough(nNeedGem);

	if(bGemEnough)
	{
		UserData::shared()->decreaseGemNum(nNeedGem);
		UserData::shared()->increaseGoldNum(nNum);
		UserData::shared()->save();
        
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

		//统计金币来源购买 ID为5
		MobClickCpp::bonus(nNum, ANALYTICS_COIN_GETBY_BUY);
		//统计购买金币
		MobClickCpp::buy(AnalyticsStatID::getCoinID(nNum), 1, nNeedGem);

#endif

		CCNode *nodeDialog = FerrisDialog::createWithLibraryTag(DIALOG_TYPE_CLOSE,
			"buy_gold_success",
			this,
			NULL,
			NULL);
		this->addChild(nodeDialog, KZOrderDialog, kTagDialog);

		int nGoldnum = UserData::shared()->getGoldNum();
		int nGemNum = UserData::shared()->getGemNum();
		m_pLabelUserGoldNum->setString(getStringFromInt(nGoldnum).c_str());
		m_pLabelUserGoldNum1->setString(getStringFromInt(nGoldnum).c_str());
		m_pLabelUserGemNum1->setString(getStringFromInt(nGemNum).c_str());
	}
	else
	{
		CCNode *nodeDialog = FerrisDialog::createWithLibraryTag(DIALOG_TYPE_ONLY_OK,
			"gem_not_enough",
			this,
			menu_selector(GoldStoreLayer::callbackGemStore),
			NULL);
		this->addChild(nodeDialog, KZOrderDialog, kTagDialog);
	}
}

void GoldStoreLayer::callbackClose(CCObject* pSender)
{
    //GameDelegate::shared()->closeSceneGoldStore();
}


void GoldStoreLayer::callbackGemStore(CCObject* pSender)
{
	if(this->getChildByTag(kTagDialog))
	{
		this->removeChildByTag(kTagDialog);
	}

	//GameDelegate::shared()->openSceneGemStore();
}
*/

bool GoldStoreLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    addChild(m_pUILayer);
    
    Layout* root_goldstore = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/gold_store_layer.ExportJson"));
    m_pUILayer->addWidget(root_goldstore);
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    btn_close->addTouchEventListener(this, toucheventselector(GoldStoreLayer::callbackClose));
    
    for (int i = 1; i < 6; i++)
    {
        string str_id = "2_";
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
            
            btn->addTouchEventListener(this, toucheventselector(GoldStoreLayer::callbackBuyItem));
            
            string str_consume = "    ";
            str_consume.append(getStringFromInt(data->getConsume()));
            
            btn->setTitleText(str_consume);
        }
    }
    
    return true;
}

void GoldStoreLayer::callbackClose(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        GameDelegate::shared()->closeSceneStoreLayer();
    }
}

void GoldStoreLayer::callbackBuyItem(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);

        Button* btn = (Button*)pSender;
        
        string btn_id = btn->getName();
        
        PriceData* data = PriceDataManager::shared()->getPriceData(btn_id);
        
        if (UserData::shared()->isGemEnough(data->getConsume()))
        {
            UserData::shared()->decreaseGemNum(data->getConsume());
            
            GameCountData::sharedInstance()->addConsumeGem(data->getConsume());
            
            Achieve::getInstance()->save();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESHGOLD);
            
            GetEquipLayer* layer = GetEquipLayer::createWithGift(kSignReward_gold);
            
            addChild(layer,KZOrderDialog);
            
            //增加金币 保存数据
            
            UserData::shared()->increaseGoldNum(data->getValue());
            
            UserData::shared()->save();
        }
        else
        {
            GameDelegate::shared()->openSceneStoreLayer(kStore_gem);
        }
    }

}

void GoldStoreLayer::callbackGemStore(cocos2d::CCObject *pSender)
{
    
}