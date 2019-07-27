#include "EquipBatteryLayer.h"


EquipBatteryLayer::EquipBatteryLayer()
{

}

EquipBatteryLayer::~EquipBatteryLayer()
{

}

EquipBatteryLayer* EquipBatteryLayer::create()
{
	EquipBatteryLayer* pLayer = new EquipBatteryLayer();
	if(pLayer && pLayer->init())
	{
		pLayer->autorelease();
		return pLayer;
	}

	return NULL;
}

bool EquipBatteryLayer::init()
{
	CCLayer::init();

	//为选择任何一栏
	m_nSelectedEquip = -1;
	m_nSelectedOwnedEquip = -1;
	m_nSelectedShopEquip = -1;

	m_bClickEquipButton = false;
	m_bTouch = false;
	m_posPreOffset = CCPointZero;

	CCNode* pNode = this->getEquipBatteryNode();
	pNode->setPosition(CCPointZero);
	this->addChild(pNode, 0, kTagEquipNode);

	m_pDictEquippedEquipments = CCDictionary::create();
	m_pDictEquippedEquipments->retain();

	m_pDictRemainingEquipments = CCDictionary::create();
	m_pDictRemainingEquipments->retain();

	//创建装备
	this->createEquipment();

	this->setTouchEnabled(true);
	return true;
}

//按键处理
void EquipBatteryLayer::keyBackClicked()
{

}
void EquipBatteryLayer::keyMenuClicked()
{

}

//为装备槽装载装备
void EquipBatteryLayer::createEquipment()
{
//	CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, 101);
//	EquipItemData* pEquipItemData = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys("1");
//
//	//获取飞机节点
//	CCNode* pNode = this->getChildByTag(kTagEquipNode);
//	if(pNode)
//	{
//		//获取选择的主炮、副炮和僚机ID
//		int nSelectedMainGun = UserData::shared()->getSelectedMainGun();
//		int nSelectedSecondaryLeftGun = UserData::shared()->getSelectedLeftGun();
//		int nSelectedSecondaryRightGun = UserData::shared()->getSelectedRightGun();
//		int nSelectedWingGun = UserData::shared()->getSelectedWing();
//
//		CCMenu* pEquipMenu = CCMenu::create();
//		pEquipMenu->setPosition(CCPointZero);
//		pNode->addChild(pEquipMenu, -1);
//
//		if(nSelectedMainGun >= 0)
//		{
//			EquipItemData* pEquipItemData1 = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedMainGun));
//			CraftData *dataCraft1 = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData1->getPlaneID().c_str()));
//			//主炮的创建
//			EquipView* pMainGun = EquipView::create(dataCraft1, pEquipItemData1);
//			pMainGun->setPosition(ccp(_s_width/5 , _s_height/5*4));
//			pNode->addChild(pMainGun);
//			m_pDictEquippedEquipments->setObject(pMainGun, kTagEquipItem + 0);
//
//			//底图
//			CCMenuItemImage* pItem = CCMenuItemImage::create("gameui/equipplane/equip_back.png", "gameui/equipplane/equip_back.png", 
//				this, menu_selector(EquipBatteryLayer::callBackClickEquipment));
//			pItem->setTag(kTagEquipItem + 0);
//			pItem->setPosition(pMainGun->getPosition());
//			pEquipMenu->addChild(pItem);
//		}
//
//		if(nSelectedSecondaryLeftGun >= 0)
//		{
//			//副炮一的创建
//			EquipItemData* pEquipItemData1 = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedSecondaryLeftGun));
//			CraftData *dataCraft1 = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData1->getPlaneID().c_str()));
//			EquipView* pSecondaryLeftGun = EquipView::create(dataCraft1, pEquipItemData1);
//			pSecondaryLeftGun->setPosition(ccp(_s_width/5*2 , _s_height/5*4));
//			pNode->addChild(pSecondaryLeftGun);
//			m_pDictEquippedEquipments->setObject(pSecondaryLeftGun, kTagEquipItem + 1);
//
//			//底图
//			CCMenuItemImage* pItem = CCMenuItemImage::create("gameui/equipplane/equip_back.png", "gameui/equipplane/equip_back.png", 
//				this, menu_selector(EquipBatteryLayer::callBackClickEquipment));
//			pItem->setTag(kTagEquipItem + 1);
//			pItem->setPosition(pSecondaryLeftGun->getPosition());
//			pEquipMenu->addChild(pItem);
//		}
//
//		if(nSelectedSecondaryRightGun >= 0)
//		{
//			//副炮二的创建
//			EquipItemData* pEquipItemData1 = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedSecondaryRightGun));
//			CraftData *dataCraft1 = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData1->getPlaneID().c_str()));
//			EquipView* pSecondaryRightGun = EquipView::create(dataCraft1, pEquipItemData1);
//			pSecondaryRightGun->setPosition(ccp(_s_width/5*3 , _s_height/5*4));
//			pNode->addChild(pSecondaryRightGun);
//			m_pDictEquippedEquipments->setObject(pSecondaryRightGun, kTagEquipItem + 2);
//
//			//底图
//			CCMenuItemImage* pItem = CCMenuItemImage::create("gameui/equipplane/equip_back.png", "gameui/equipplane/equip_back.png", 
//				this, menu_selector(EquipBatteryLayer::callBackClickEquipment));
//			pItem->setTag(kTagEquipItem + 2);
//			pItem->setPosition(pSecondaryRightGun->getPosition());
//			pEquipMenu->addChild(pItem);
//		}
//
//		if(nSelectedWingGun >= 0)
//		{
//			//僚机的创建
//			EquipItemData* pEquipItemData1 = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedWingGun));
//			CraftData *dataCraft1 = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData1->getPlaneID().c_str()));
//			EquipView* pWing = EquipView::create(dataCraft1, pEquipItemData1);
//			pWing->setPosition(ccp(_s_width/5*4 , _s_height/5*4));
//			pNode->addChild(pWing);
//			m_pDictEquippedEquipments->setObject(pWing, kTagEquipItem + 3);
//
//			//底图
//			CCMenuItemImage* pItem = CCMenuItemImage::create("gameui/equipplane/equip_back.png", "gameui/equipplane/equip_back.png", 
//				this, menu_selector(EquipBatteryLayer::callBackClickEquipment));
//			pItem->setTag(kTagEquipItem + 3);
//			pItem->setPosition(pWing->getPosition());
//			pEquipMenu->addChild(pItem);
//		}
//
//	}
//
//	//购买的所有装备，除去已经装备上的(包裹中的装备数量)
//	CCDictionary* pDictEquipInPackage = CCDictionary::create();
//	this->getEquipNumInPackage(pDictEquipInPackage);
//
//	//装备ScrollView
//	CCScrollView* pScrollView = CCScrollView::create();
//	CCNode* pTempNode = CCNode::create();
//	pTempNode->setContentSize(CCSizeMake(VIEW_WIDTH, VIEW_HEIGHT));
//
//	//装备底图（做成按钮）
//	CCMenu* pMenu = CCMenu::create();
//	pMenu->setPosition(CCPointZero);
//	pTempNode->addChild(pMenu, -1);
//
//	int index = 0;
//	CCDictElement *ele1;
//	CCDICT_FOREACH(pDictEquipInPackage, ele1)
//	{
//		EquipItemData* pEquipItemData1 = (EquipItemData*)ele1->getObject();
//		CraftData *dataCraft1 = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData1->getPlaneID().c_str()));
//		int nRow = index / 5 % 2;
//		int nCol = index % 5;
//		int nPage = index / 10;
//		//装备
//		EquipView* pEquip = EquipView::create(dataCraft1, pEquipItemData1);
//		pEquip->setPosition(ccp(_s_width/6 * (nCol + 1) + nPage * _s_width, /*_s_height/5*4*/80 - nRow * 43.0f));
//		pTempNode->addChild(pEquip);
//		m_pDictRemainingEquipments->setObject(pEquip, kTagHaveItem + index);
//
//		//底图
//		CCMenuItemImage* pItem = CCMenuItemImage::create("gameui/equipplane/equip_back.png", "gameui/equipplane/equip_back.png", 
//			this, menu_selector(EquipBatteryLayer::callBackClickEquipment));
//		pItem->setTag(kTagHaveItem + index);
//		pItem->setPosition(pEquip->getPosition());
//		pMenu->addChild(pItem);
//
//		index ++;
//	}
//
//	pScrollView->setContainer(pTempNode);
//	pScrollView->setViewSize(CCSizeMake(VIEW_WIDTH, VIEW_HEIGHT));
//	pScrollView->setContentSize(CCSizeMake(VIEW_WIDTH*5, VIEW_HEIGHT));
//	pScrollView->setDirection(kCCScrollViewDirectionHorizontal);
//	pScrollView->setPosition(ccp(0, _s_height/5*3 + 40.0f));
//	pScrollView->setDelegate(this);
//	pScrollView->setContentOffset(ccp(0, 0.0f));
//	pNode->addChild(pScrollView, 0, kTagScrollView);
//
//	int nNum1 = 10;
//	//商店的装备
//	for (int i = 0; i < nNum1; i ++)
//	{
//		int nRow = i / 5;
//		int nCol = i % 5;
//		//装备
//		EquipView* pEquip = EquipView::create(dataCraft, pEquipItemData);
//		pEquip->setPosition(ccp(_s_width/6 * (nCol + 1), _s_height/5*4 - nRow * (42.0f + 3.0f) - 42.0f * 5));
//		pNode->addChild(pEquip);
//	}
}

//获取包裹中的装备的数量,剔除已经装备的
void EquipBatteryLayer::getEquipNumInPackage(CCDictionary* &equip_in_package)
{
	//获取选择的主炮、副炮和僚机ID
	int nSelectedMainGun = UserData::shared()->getSelectedMainGun();
	int nSelectedSecondaryLeftGun = UserData::shared()->getSelectedLeftGun();
	int nSelectedSecondaryRightGun = UserData::shared()->getSelectedRightGun();
	int nSelectedWingGun = UserData::shared()->getSelectedWing();

	//将装备拷贝到新数组里
	CCDictionary* pDict = UserData::shared()->getEquipmentsData()->getDictEquipmentsData();
	CCDictElement *ele1;
	CCDICT_FOREACH(pDict, ele1)
	{
		equip_in_package->setObject(ele1->getObject(), ele1->getStrKey());
	}

	//对已经装备上的进行剔除
	if(nSelectedMainGun >= 0)
	{
		equip_in_package->removeObjectForKey(getStringFromInt(nSelectedMainGun));
	}

	if(nSelectedSecondaryLeftGun >= 0)
	{
		equip_in_package->removeObjectForKey(getStringFromInt(nSelectedSecondaryLeftGun));
	}

	if(nSelectedSecondaryRightGun >= 0)
	{
		equip_in_package->removeObjectForKey(getStringFromInt(nSelectedSecondaryRightGun));
	}

	if(nSelectedWingGun >= 0)
	{
		equip_in_package->removeObjectForKey(getStringFromInt(nSelectedWingGun));
	}
}

//切换装备后刷新操作
void EquipBatteryLayer::refreshEquipment()
{
	//根据当前点击的装备的ID获取切换的装备
	EquipView* pTempEquip = (EquipView*)m_pDictEquippedEquipments->objectForKey(kTagEquipItem + m_nSelectedEquip);
	EquipView* pTempOwnedEquip = (EquipView*)m_pDictRemainingEquipments->objectForKey(kTagHaveItem + m_nSelectedOwnedEquip);

	pTempEquip->removeFromParent();
	pTempOwnedEquip->removeFromParent();
	
	//设置到变换的位置上
	pTempEquip->setPosition(pTempEquip->getPosChange());
		
	pTempOwnedEquip->setPosition(pTempOwnedEquip->getPosChange());

	CCNode* pNode = this->getChildByTag(kTagEquipNode);
	if(pNode)
	{
		pNode->addChild(pTempEquip);

		CCScrollView* pScrollView = (CCScrollView*)pNode->getChildByTag(kTagScrollView);
		if(pScrollView)
		{
			CCNode* pTempNode = pScrollView->getContainer();
			pTempNode->addChild(pTempOwnedEquip);
		}
	}
}

void EquipBatteryLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();

	//获取飞机节点
	CCNode* pNode = this->getChildByTag(kTagEquipNode);
	//点击到的装备的TAG
	int nClickEquipTag = 100;
	if(pNode)
	{
		//主机滑动
		CCScrollView* pScrollView = (CCScrollView*)pNode->getChildByTag(kTagScrollView);
		if(pScrollView)
		{
			m_posPreOffset = pScrollView->getContentOffset();
		}

		//int nNum1 = 10;
		////商店的装备
		//for (int i = 0; i < nNum1; i ++)
		//{
		//	CCSprite* pEquip = (CCSprite*)pNode->getChildByTag(kTagShopItem + i);
		//	if(pEquip)
		//	{
		//		CCRect rect = CCRectMake(pEquip->getPositionX() - pEquip->getContentSize().width / 2, 
		//			pEquip->getPositionY() - pEquip->getContentSize().height / 2, 
		//			pEquip->getContentSize().width, 
		//			pEquip->getContentSize().height);

		//		if(rect.containsPoint(location))
		//		{
		//			nClickEquipTag = kTagShopItem + i;
		//			m_nSelectedShopEquip = i;
		//			CCLog("333333333333333333  tag == (%d)", nClickEquipTag);
		//			onClickShopItem();
		//			break;
		//		}
		//	}
		//}

	}
}

void EquipBatteryLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	
}

void EquipBatteryLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	//主机滑动
	CCNode* pNode = this->getChildByTag(kTagEquipNode);
	//点击到的装备的TAG
	int nClickEquipTag = 100;
	if(pNode)
	{
		CCScrollView* pScrollView = (CCScrollView*)pNode->getChildByTag(kTagScrollView);
		CCPoint endOffset = pScrollView->getContentOffset();
		adjustPosition(m_posPreOffset, endOffset);
		m_bTouch = false;
	}
	
}

void EquipBatteryLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}

void EquipBatteryLayer::scrollViewDidScroll(extension::CCScrollView *view)
{
	return;
}

void EquipBatteryLayer::scrollViewDidZoom(extension::CCScrollView* view)
{
	return;
}

//调整位置
void EquipBatteryLayer::adjustPosition(CCPoint posPreOffset, CCPoint posEndOffset)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	int nPage = abs((int) posPreOffset.x / (int)(VIEW_WIDTH));

	int nAdjustPage = 0;

	int nDis = posEndOffset.x - posPreOffset.x;

	if(nDis < -visibleSize.width / 20)
	{
		nAdjustPage = nPage + 1;
	}
	else if(nDis > visibleSize.width / 20)
	{
		nAdjustPage = nPage - 1;
	}
	else
	{
		nAdjustPage = nPage;
	}

	//确保值不能超出范围
	nAdjustPage = MIN(nAdjustPage, 4);

	nAdjustPage = MAX(nAdjustPage, 0);

	scrollToPage(nAdjustPage , 0.3f);
}

//滚动到某一页
void EquipBatteryLayer::scrollToPage(int npage, float delay)
{
	CCNode* pNode = this->getChildByTag(kTagEquipNode);
	if(!pNode)
	{
		return;
	}

	CCScrollView* pScrollView = (CCScrollView*)pNode->getChildByTag(kTagScrollView);

	if(!pScrollView)
	{
		return;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//关闭ccscrollview中的自调整
	pScrollView->unscheduleAllSelectors();
	// 调整位置
	CCPoint adjustPos = ccp(-VIEW_WIDTH * npage, 0.0f);

	// 调整位置
	pScrollView->setContentOffsetInDuration(adjustPos, delay);
}

//装备飞机
void EquipBatteryLayer::callBackEquipPlane(CCObject* pSender)
{
	//GameDelegate::shared()->openSceneEquipPlane();
}

//获取切换炮台的节点
CCNode* EquipBatteryLayer::getEquipBatteryNode()
{
	CCNode* pNode = CCNode::create();

	//返回按钮
	CCLabelTTF* pLabelEnter = CCLabelTTF::create("return", "Arial", 32);
	CCMenuItemLabel* pItem = CCMenuItemLabel::create(pLabelEnter, this, menu_selector(EquipBatteryLayer::callBackEquipPlane));
	pItem->setPosition(ccp(pItem->getContentSize().width, 30.0f));

	//出售按钮
	m_pItemSell = CCMenuItemImage::create("gameui/equipplane/button_equip.png", 
		"gameui/equipplane/button_equip.png",
		this, 
		menu_selector(EquipBatteryLayer::callBackSellEquip));
	m_pItemSell->setPosition(ccp(_s_width/2 - m_pItemSell->getContentSize().width - 20.0f, _s_height / 4));

	int nGold = 522;
	string str = StringLibrary::shared()->getStringByKey("sell")->getCString() + getStringFromInt(nGold) + StringLibrary::shared()->getStringByKey("gold")->getCString();
	m_pSellLabel = CCLabelTTF::create(str.c_str(), "Arial", 12);
	m_pSellLabel->setPosition(m_pItemSell->getPosition());
	pNode->addChild(m_pSellLabel);
	//装备
	m_pItemEquip = CCMenuItemImage::create("gameui/equipplane/button_equip.png", 
		"gameui/equipplane/button_equip.png",
		this, 
		menu_selector(EquipBatteryLayer::callBackEquip));
	m_pItemEquip->setPosition(ccp(_s_width/2, _s_height / 4));

	str = StringLibrary::shared()->getStringByKey("equip")->getCString();
	m_pEquipLabel = CCLabelTTF::create(str.c_str(), "Arial", 12);
	m_pEquipLabel->setPosition(m_pItemEquip->getPosition());
	pNode->addChild(m_pEquipLabel);

	//购买
	m_pItemBuy = CCMenuItemImage::create("gameui/equipplane/button_equip.png", 
		"gameui/equipplane/button_equip.png",
		this, 
		menu_selector(EquipBatteryLayer::callBackBuy));
	m_pItemBuy->setPosition(ccp(_s_width/2, _s_height / 4));

	int nDiamond = 256;
	str = StringLibrary::shared()->getStringByKey("buy")->getCString() + getStringFromInt(nDiamond) + StringLibrary::shared()->getStringByKey("diamond")->getCString();
	m_pDiamondLabel = CCLabelTTF::create(str.c_str(), "Arial", 12);
	m_pDiamondLabel->setPosition(m_pItemBuy->getPosition());
	pNode->addChild(m_pDiamondLabel);

	//升级
	m_pItemUpgrade = CCMenuItemImage::create("gameui/equipplane/button_equip.png", 
		"gameui/equipplane/button_equip.png",
		this, 
		menu_selector(EquipBatteryLayer::callBackUpgradeEquip));
	m_pItemUpgrade->setPosition(ccp(_s_width/2 + m_pItemUpgrade->getContentSize().width + 20.0f, _s_height / 4));

	nGold = 999;
	str = StringLibrary::shared()->getStringByKey("upgrade")->getCString() + getStringFromInt(nGold) + StringLibrary::shared()->getStringByKey("gold")->getCString();
	m_pUpgradeLabel = CCLabelTTF::create(str.c_str(), "Arial", 12);
	m_pUpgradeLabel->setPosition(m_pItemUpgrade->getPosition());
	pNode->addChild(m_pUpgradeLabel);

	//设置按钮和文字均不可见
	m_pItemSell->setVisible(false);
	m_pItemEquip->setVisible(false);
	m_pItemBuy->setVisible(false);
	m_pItemUpgrade->setVisible(false);

	m_pSellLabel->setVisible(false);
	m_pEquipLabel->setVisible(false);
	m_pDiamondLabel->setVisible(false);
	m_pUpgradeLabel->setVisible(false);

	CCMenu* pMenu = CCMenu::create(pItem, m_pItemSell, m_pItemEquip, m_pItemBuy, m_pItemUpgrade, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);

	//攻击
	CCLabelTTF* pAttrackLabel = CCLabelTTF::create(StringLibrary::shared()->getStringByKey("attrack")->getCString(), "Arial", 32);
	pAttrackLabel->setAnchorPoint(CCPointZero);
	pAttrackLabel->setPosition(ccp(_s_width / 3, m_pItemSell->getPositionY() - pAttrackLabel->getContentSize().height * 2));
	pNode->addChild(pAttrackLabel, 0);
	//血量
	CCLabelTTF* pBloodLabel = CCLabelTTF::create(StringLibrary::shared()->getStringByKey("blood")->getCString(), "Arial", 32);
	pBloodLabel->setAnchorPoint(CCPointZero);
	pBloodLabel->setPosition(ccp(_s_width / 3, pAttrackLabel->getPositionY() - pAttrackLabel->getContentSize().height * 2));
	pNode->addChild(pBloodLabel, 0);
	//炮管特性
	CCLabelTTF* pBatteryLabel = CCLabelTTF::create(StringLibrary::shared()->getStringByKey("battery_desc")->getCString(), "Arial", 32);
	pBatteryLabel->setAnchorPoint(CCPointZero);
	pBatteryLabel->setPosition(ccp(_s_width / 3 * 2, pAttrackLabel->getPositionY()));
	pNode->addChild(pBatteryLabel, 0);
	//主炮特性
	CCLabelTTF* pMaingunLabel = CCLabelTTF::create(StringLibrary::shared()->getStringByKey("maingun_desc")->getCString(), "Arial", 32);
	pMaingunLabel->setAnchorPoint(CCPointZero);
	pMaingunLabel->setPosition(ccp(_s_width / 3 * 2, pBloodLabel->getPositionY()));
	pNode->addChild(pMaingunLabel, 0);

	//等级
	str = "LV: ";
	int nLV = 11;
	int nLVAll = 30;
	str += getStringFromInt(nLV) + "/" + getStringFromInt(nLVAll);

	CCLabelTTF* pLVLabel = CCLabelTTF::create(str.c_str(), "Arial", 40);
	pLVLabel->setAnchorPoint(CCPointZero);
	pLVLabel->setPosition(ccp(30.0f, pAttrackLabel->getPositionY() - pAttrackLabel->getContentSize().height));
	pNode->addChild(pLVLabel);

	return pNode;
}

//出售装备
void EquipBatteryLayer::callBackSellEquip(CCObject* pSender)
{

}

//穿上装备
void EquipBatteryLayer::callBackEquip(CCObject* pSender)
{
	if(m_nSelectedEquip > -1 || m_nSelectedOwnedEquip > -1)
	{
		m_bClickEquipButton = true;
	}
}

//购买装备
void EquipBatteryLayer::callBackBuy(CCObject* pSender)
{
	CCLog("callBackBuy equip  id == (%d)", m_nSelectedShopEquip + 300);
}

//升级装备
void EquipBatteryLayer::callBackUpgradeEquip(CCObject* pSender)
{

}

//点击装备
void EquipBatteryLayer::callBackClickEquipment(CCObject* pSender)
{
	CCMenuItemImage* pItem = (CCMenuItemImage*)(pSender);
	int nTag = pItem->getTag();
	//已经装备上的（准备槽）
	if(nTag < kTagHaveItem)
	{
		m_nSelectedEquip = nTag - 100;
		this->onClickEquipItem();
	}
	//背包里的所有装备
	else if(nTag < kTagShopItem)
	{
		m_nSelectedOwnedEquip = nTag - 200;
		this->onClickHaveItem();
	}
	//商店里的装备
	else
	{
		m_nSelectedShopEquip = nTag - 300;
		this->onClickShopItem();
	}
	
}

//点击了装备栏
void EquipBatteryLayer::onClickEquipItem()
{
	m_pItemSell->setVisible(true);
	m_pItemEquip->setVisible(true);
	m_pItemBuy->setVisible(false);
	m_pItemUpgrade->setVisible(true);

	m_pSellLabel->setVisible(true);
	m_pEquipLabel->setVisible(true);
	m_pDiamondLabel->setVisible(false);
	m_pUpgradeLabel->setVisible(true);

	if(m_nSelectedOwnedEquip > -1 && m_bClickEquipButton)
	{
		//切换装备
		CCLog("change equip  id == (%d)  to equip pos == (%d)", m_nSelectedOwnedEquip + 200, m_nSelectedEquip + 100);
		
		//切换装备槽的装备
		EquipView* pTempEquip = (EquipView*)m_pDictEquippedEquipments->objectForKey(kTagEquipItem + m_nSelectedEquip);
		EquipView* pTempOwnedEquip = (EquipView*)m_pDictRemainingEquipments->objectForKey(kTagHaveItem + m_nSelectedOwnedEquip);
		//记录变换的位置
		pTempEquip->setPosChange(pTempOwnedEquip->getPosition());
		pTempOwnedEquip->setPosChange(pTempEquip->getPosition());
		//变换所在的数组
		m_pDictEquippedEquipments->setObject(pTempOwnedEquip, kTagEquipItem + m_nSelectedEquip);
		m_pDictRemainingEquipments->setObject(pTempEquip, kTagHaveItem + m_nSelectedOwnedEquip);

		//刷新装备位置
		this->refreshEquipment();

		//更改用户数据并保存
		UserData::shared()->setEquippedEquipmentsID(m_nSelectedEquip, getIntFromS(pTempOwnedEquip->getEquipItemData()->getID().c_str()));
		UserData::shared()->save();
		
		//边为未选择状态
		m_bClickEquipButton = false;
		m_nSelectedEquip = -1;
		m_nSelectedOwnedEquip = -1;
	}
	//定义为第一次选择
	else if(!m_bClickEquipButton)
	{
		m_nSelectedOwnedEquip = -1;
	}
}

//点击了拥有装备栏
void EquipBatteryLayer::onClickHaveItem()
{
	m_pItemSell->setVisible(true);
	m_pItemEquip->setVisible(true);
	m_pItemBuy->setVisible(false);
	m_pItemUpgrade->setVisible(true);

	m_pSellLabel->setVisible(true);
	m_pEquipLabel->setVisible(true);
	m_pDiamondLabel->setVisible(false);
	m_pUpgradeLabel->setVisible(true);

	if(m_nSelectedEquip > -1 && m_bClickEquipButton)
	{
		//切换装备
		CCLog("change equip  id == (%d) to equip pos == (%d)", m_nSelectedOwnedEquip + 200, m_nSelectedEquip + 100);

		//切换装备槽的装备
		EquipView* pTempEquip = (EquipView*)m_pDictEquippedEquipments->objectForKey(kTagEquipItem + m_nSelectedEquip);
		EquipView* pTempOwnedEquip = (EquipView*)m_pDictRemainingEquipments->objectForKey(kTagHaveItem + m_nSelectedOwnedEquip);
		//记录变换的位置
		pTempEquip->setPosChange(pTempOwnedEquip->getPosition());
		pTempOwnedEquip->setPosChange(pTempEquip->getPosition());
		//变换所在的数组
		m_pDictEquippedEquipments->setObject(pTempOwnedEquip, kTagEquipItem + m_nSelectedEquip);
		m_pDictRemainingEquipments->setObject(pTempEquip, kTagHaveItem + m_nSelectedOwnedEquip);

		//刷新装备位置
		this->refreshEquipment();

		//更改用户数据并保存
		UserData::shared()->setEquippedEquipmentsID(m_nSelectedEquip, getIntFromS(pTempOwnedEquip->getEquipItemData()->getID().c_str()));
		UserData::shared()->save();

		m_bClickEquipButton = false;
		m_nSelectedEquip = -1;
		m_nSelectedOwnedEquip = -1;
	}
	//定义为第一次选择
	else if(!m_bClickEquipButton)
	{
		m_nSelectedEquip = -1;
	}
}

//点击了商店装备栏
void EquipBatteryLayer::onClickShopItem()
{
	m_pItemSell->setVisible(false);
	m_pItemEquip->setVisible(false);
	m_pItemBuy->setVisible(true);
	m_pItemUpgrade->setVisible(false);

	m_pSellLabel->setVisible(false);
	m_pEquipLabel->setVisible(false);
	m_pDiamondLabel->setVisible(true);
	m_pUpgradeLabel->setVisible(false);


	m_bClickEquipButton = false;
	m_nSelectedEquip = -1;
	m_nSelectedOwnedEquip = -1;

}