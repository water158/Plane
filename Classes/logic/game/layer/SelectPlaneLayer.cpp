#include "SelectPlaneLayer.h"


SelectPlaneLayer::SelectPlaneLayer(void)
{

}

SelectPlaneLayer::~SelectPlaneLayer(void)
{

}

SelectPlaneLayer* SelectPlaneLayer::create()
{
	SelectPlaneLayer* layer = new SelectPlaneLayer();
	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}

	return NULL;
}

bool SelectPlaneLayer::init()
{
	CCLayer::init();

	CCLabelTTF* pLabel = CCLabelTTF::create("SelectPlaneLayer", "Arial", 64);
	pLabel->setPosition(ccp(_s_width/2, _s_height/2));
	this->addChild(pLabel, 0);

	CCLabelTTF* pLabelEnter = CCLabelTTF::create("Enter EquipPlane", "Arial", 32);
	CCMenuItemLabel* pItem = CCMenuItemLabel::create(pLabelEnter, this, menu_selector(SelectPlaneLayer::callBackEquipPlane));
	pItem->setPosition(ccp(_s_width/2, _s_height/3));

	CCMenu* pMenu = CCMenu::create(pItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);

	return true;
}

//装备飞机
void SelectPlaneLayer::callBackEquipPlane(CCObject* pSender)
{
	//GameDelegate::shared()->openSceneEquipPlane();
}

//按键处理
void SelectPlaneLayer::keyBackClicked()
{

}

void SelectPlaneLayer::keyMenuClicked()
{

}

void SelectPlaneLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}