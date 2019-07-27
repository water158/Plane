//
//  GemStoreLayer.h
//  plane
//
//  Created on 14-5-20.
//
//


#ifndef __FERRIS_GAME__GemStoreLayer__
#define __FERRIS_GAME__GemStoreLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

//global
#include "GlobalEnum.h"
#include "GameDelegate.h"

//func
#include "SmartRes.h"
#include "FerrisTools.h"
#include "FerrisDialog.h"

//logic
#include "StringLibrary.h"
#include "VerificationManager.h"
#include "UserData.h"
#include "AnalyticsStatID.h"
#include "PurchaseLogManager.h"
#include "StorePriceData.h"

//payment

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

#include "PaymentAndroid.h"

#endif

#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)

#include "PaymentIOS.h"

#endif

//misc
#include "MiscDefination.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

//#include "MobClickCpp.h"

#endif


#define GEMSTORE_ITEM_NUM 5
#define kTagCurrentPage 10


USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

class GemStoreLayer : public CCLayer
{
public:
	virtual ~GemStoreLayer(void);
	static GemStoreLayer* create();

	//virtual void update(float delta);

	void callbackBuyItem(CCObject* pSender,TouchEventType type);
	void callbackClose(CCObject* pSender,TouchEventType type);
    
    void btnGoCallBack(CCObject* pSender,TouchEventType type);
	//void callbackResponseOfBuyItem(CCNode* target, void* data);

	//void callbackRecharge(CCObject* pSender);

	//获取首次充值的节点
	//CCNode* getFirstShopNode();

	//获取钻石商店的节点
	//CCNode* getGemNode();

private:
	GemStoreLayer(void);

	bool init();
    
    TouchGroup* m_pUILayer;

	//CCLabelTTF *m_pLabelUserGoldNum;

	//CCLabelTTF *m_pLabelUserGoldNum1;

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    
	PaymentAndroid* m_pPaymentAndroid; //收费类
    
#endif
};

#endif
