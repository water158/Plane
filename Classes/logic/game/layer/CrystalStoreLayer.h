//
//  CrystalStoreLayer.h
//  plane
//
//  Created on 14-5-20.
//
//

#ifndef _CRYSTALSTORELAYER_H_
#define _CRYSTALSTORELAYER_H_

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "SmartRes.h"
#include "StringLibrary.h"
#include "UserData.h"
#include "FerrisTools.h"
#include "Store.h"
#include "FerrisDialog.h"
//#include "MobClickCpp.h"
#include "StatID.h"
#include "GameDelegate.h"
//#include "HttpInstance.h"

#include "cocos-ext.h"

#define CRYSTALSTORE_ITEM_NUM 5
#define kTagCurrentPage 10


USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

class CrystalStoreLayer : public CCLayer
{
public:
	virtual ~CrystalStoreLayer(void);
	static CrystalStoreLayer* create();
/*
	virtual void update(float delta);
*/
	void callbackBuyItem(CCObject* pSender,TouchEventType type);
	void callbackClose(CCObject* pSender,TouchEventType type);
/*	void callbackConfigure(CCNode* target, void* data);

	void callbackRecharge(CCObject* pSender);

	//获取首次充值的节点
	CCNode* getFirstShopNode();

	//获取钻石商店的节点
	CCNode* getCrystalNode();
*/
private:
	CrystalStoreLayer(void);
    
    TouchGroup* m_pUILayer;

	bool init();
/*
	CCLabelTTF *m_pLabelUserGoldNum;

	CCLabelTTF *m_pLabelUserGoldNum1;
*/
	//按钮类型
	//int m_nPurchraseType;

	//StoreManager* m_pStore; //收费类
};

#endif
