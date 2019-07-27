#include "EquipPlaneLayer.h"


EquipPlaneLayer::EquipPlaneLayer(void)
{

}

EquipPlaneLayer::~EquipPlaneLayer(void)
{

}

EquipPlaneLayer* EquipPlaneLayer::create()
{
	EquipPlaneLayer* layer = new EquipPlaneLayer();
	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}

	return NULL;
}

bool EquipPlaneLayer::init()
{
	CCLayer::init();

	CCLabelTTF* pLabelEnter = CCLabelTTF::create("Enter Equiptools", "Arial", 32);
	CCMenuItemLabel* pItem = CCMenuItemLabel::create(pLabelEnter, this, menu_selector(EquipPlaneLayer::callBackEquipTools));
	pItem->setPosition(ccp(_s_width/2, _s_height/3));

	CCMenu* pMenu = CCMenu::create(pItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);

	//预加载飞机数据
	DataManager::sharedDataManager()->loadCraftData(1, 101);
	DataManager::sharedDataManager()->loadCraftData(1, 101);

	//添加飞机节点
	CCNode* pNode = getPlaneNode();
	pNode->setPosition(CCPointZero);
	this->addChild(pNode, kZerorderPlaneNode, kTagPlaneNode);

	//炮台数组
	m_pArrayEquipments = CCArray::create();
	m_pArrayEquipments->retain();

	//添加炮台
	this->createBatteryForHero();

	this->setTouchEnabled(true);
	return true;
}

//为主机创建炮台和僚机
void EquipPlaneLayer::createBatteryForHero()
{
	//获取飞机节点
	CCNode* pNode = this->getChildByTag(kTagPlaneNode);
	if(pNode)
	{
//		//主机图片
//		CCSprite* pHeroIcon = (CCSprite*)pNode->getChildByTag(kTagHero);
//
//		//获取选择的主炮、副炮和僚机ID
//		int nSelectedMainGun = UserData::shared()->getSelectedMainGun();
//		int nSelectedSecondaryLeftGun = UserData::shared()->getSelectedLeftGun();
//		int nSelectedSecondaryRightGun = UserData::shared()->getSelectedRightGun();
//		int nSelectedWingGun = UserData::shared()->getSelectedWing();
//
//		if(nSelectedMainGun >= 0)
//		{
//			EquipItemData* pEquipItemData = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedMainGun));
//			CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData->getPlaneID().c_str()));
//			//主炮的创建
//			EquipView* pMainGun = EquipView::create(dataCraft, pEquipItemData);
//			pMainGun->setPosition(ccp(pHeroIcon->getContentSize().width/2, pHeroIcon->getContentSize().height));
//			pHeroIcon->addChild(pMainGun);
//			m_pArrayEquipments->addObject(pMainGun);
//		}
//
//		if(nSelectedSecondaryLeftGun >= 0)
//		{
//			EquipItemData* pEquipItemData = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedSecondaryLeftGun));
//			CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData->getPlaneID().c_str()));
//			//副炮一的创建
//			EquipView* pSecondaryLeftGun = EquipView::create(dataCraft, pEquipItemData);
//			pSecondaryLeftGun->setPosition(ccp(pHeroIcon->getContentSize().width/2 + 40.0f, pHeroIcon->getContentSize().height / 4*3));
//			pHeroIcon->addChild(pSecondaryLeftGun);
//			m_pArrayEquipments->addObject(pSecondaryLeftGun);
//		}
//
//		if(nSelectedSecondaryRightGun >= 0)
//		{
//			EquipItemData* pEquipItemData = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedSecondaryRightGun));
//			CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData->getPlaneID().c_str()));
//			//副炮二的创建
//			EquipView* pSecondaryRightGun = EquipView::create(dataCraft, pEquipItemData);
//			pSecondaryRightGun->setPosition(ccp(pHeroIcon->getContentSize().width/2 + 80.0f, pHeroIcon->getContentSize().height / 2));
//			pHeroIcon->addChild(pSecondaryRightGun);
//			m_pArrayEquipments->addObject(pSecondaryRightGun);
//		}
//
//		if(nSelectedWingGun >= 0)
//		{
//			EquipItemData* pEquipItemData = UserData::shared()->getEquipmentsData()->getEquipItemDataByKeys(getStringFromInt(nSelectedWingGun));
//			CraftData *dataCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_ENEMY, getIntFromS(pEquipItemData->getPlaneID().c_str()));
//			//僚机的创建
//			EquipView* pWing = EquipView::create(dataCraft, pEquipItemData);
//			pWing->setPosition(ccp(pHeroIcon->getContentSize().width/2, pHeroIcon->getContentSize().height / 4));
//			pHeroIcon->addChild(pWing);
//			m_pArrayEquipments->addObject(pWing);
//		}
//
	}
}

//装备飞机
void EquipPlaneLayer::callBackEquipTools(CCObject* pSender)
{
	//GameDelegate::shared()->openSceneCombat();
}

//获取装备飞机原型节点
CCNode* EquipPlaneLayer::getPlaneNode()
{
	CCNode* pNode = CCNode::create();

	//主机的图片
	CCSprite* pHero = CCSprite::create("gameui/equipplane/hero_1.png");
	pHero->setAnchorPoint(ccp(0.5f, 0.5f));
	pHero->setPosition(ccp(_s_width / 2, _s_height / 3 * 2));
	pNode->addChild(pHero, 0, kTagHero);

	//装备槽的底
	
	//主炮的创建
	CCSprite* pMainGun = CCSprite::create("gameui/equipplane/battery_1.png");
	pMainGun->setPosition(ccp(pHero->getContentSize().width/2, pHero->getContentSize().height - pMainGun->getContentSize().height/2));
	pHero->addChild(pMainGun, 0, kTagMainGun);
	
	//副炮一的创建
	CCSprite* pSecondaryLeftGun = CCSprite::create("gameui/equipplane/battery_1.png");
	pSecondaryLeftGun->setPosition(ccp(pHero->getContentSize().width/2 + 40.0f, pHero->getContentSize().height / 4*3));
	pHero->addChild(pSecondaryLeftGun, 0, kTagSencodaryGun1);
	
	//副炮二的创建
	CCSprite* pSecondaryRightGun = CCSprite::create("gameui/equipplane/battery_1.png");
	pSecondaryRightGun->setPosition(ccp(pHero->getContentSize().width/2 + 80.0f, pHero->getContentSize().height / 2));
	pHero->addChild(pSecondaryRightGun, 0, kTagSencodaryGun2);
	
	//僚机的创建
	CCSprite* pWing = CCSprite::create("gameui/equipplane/battery_1.png");
	pWing->setPosition(ccp(pHero->getContentSize().width/2, pHero->getContentSize().height / 4));
	pHero->addChild(pWing, 0, kTagWing);
	


	string str = StringLibrary::shared()->getStringByKey("attrack")->getCString() + getStringFromInt(999);
	//攻击
	CCLabelTTF* pAttrackLabel = CCLabelTTF::create(str.c_str(), "Arial", 32);
	pAttrackLabel->setAnchorPoint(CCPointZero);
	pAttrackLabel->setPosition(ccp(50.0f, pHero->getPositionY() - pHero->getContentSize().height/2 - pAttrackLabel->getContentSize().height*2));
	pNode->addChild(pAttrackLabel, 0);

	str = StringLibrary::shared()->getStringByKey("blood")->getCString();
	//血量
	CCLabelTTF* pBloodLabel = CCLabelTTF::create(str.c_str(), "Arial", 32);
	pBloodLabel->setAnchorPoint(CCPointZero);
	pBloodLabel->setPosition(ccp(50.0f, pAttrackLabel->getPositionY() - pAttrackLabel->getContentSize().height/2*3));
	pNode->addChild(pBloodLabel, 0);

	str = StringLibrary::shared()->getStringByKey("battery_desc")->getCString();
	//炮管特性
	CCLabelTTF* pBatteryLabel = CCLabelTTF::create(str.c_str(), "Arial", 32);
	pBatteryLabel->setAnchorPoint(CCPointZero);
	pBatteryLabel->setPosition(ccp(_s_width/2 + 50.0f, pAttrackLabel->getPositionY()));
	pNode->addChild(pBatteryLabel, 0);

	str = StringLibrary::shared()->getStringByKey("maingun_desc")->getCString();
	//主炮特性
	CCLabelTTF* pMaingunLabel = CCLabelTTF::create(str.c_str(), "Arial", 32);
	pMaingunLabel->setAnchorPoint(CCPointZero);
	pMaingunLabel->setPosition(ccp(_s_width/2 + 50.0f, pBloodLabel->getPositionY()));
	pNode->addChild(pMaingunLabel, 0);


	return pNode;
}

//按键处理
void EquipPlaneLayer::keyBackClicked()
{

}

void EquipPlaneLayer::keyMenuClicked()
{

}

void EquipPlaneLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();

	//获取飞机节点
	CCNode* pNode = this->getChildByTag(kTagPlaneNode);
	if(pNode)
	{
		//主机图片
		CCSprite* pHeroIcon = (CCSprite*)pNode->getChildByTag(kTagHero);

		for (int i = kTagMainGun; i < 4; i ++)
		{
			EquipView* pIcon = (EquipView*)pHeroIcon->getChildByTag(i);
			if(pIcon)
			{
				CCRect rect = CCRectMake(pIcon->getPositionX() + pHeroIcon->getPositionX() - pHeroIcon->getContentSize().width/2 - 40.0f/2, 
					pIcon->getPositionY() + pHeroIcon->getPositionY() - pHeroIcon->getContentSize().height/2 - 42.0f/2,
					40.0f, 
					42.0f);
				
				//CCLog("rect x = (%f), y = (%f),   pos x =(%f), y = (%f)", rect.getMinX(), rect.getMinY(), location.x, location.y);
				if(rect.containsPoint(location))
				{
					CCLog("=======touch in icon  tag == (%d)", i);
					//GameDelegate::shared()->openSceneEquipBattery();

					break;
				}
			}
		}	
	}

}