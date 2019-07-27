//
//  GoldStoreLayer.h
//  plane
//
//  Created on 14-5-20.
//
//

#ifndef __FERRIS_GAME__GoldStoreLayer__
#define __FERRIS_GAME__GoldStoreLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "FerrisDialog.h"
#include "GameDelegate.h"
#include "GlobalEnum.h"
#include "GlobalTagZOrder.h"
#include "FerrisMaskLayer.h"
#include "FerrisTools.h"
#include "AnalyticsStatID.h"
#include "SmartRes.h"
#include "StringLibrary.h"
#include "UserData.h"
#include "tinyxml.h"
#include "StorePriceData.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

//#include "MobClickCpp.h"

#endif

#define GOLDSTORE_ITEM_NUM 4

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

using namespace extension;

class GoldStoreLayer : public CCLayer
{
public:
	virtual ~GoldStoreLayer(void);
    GoldStoreLayer(void);
    
    CREATE_FUNC(GoldStoreLayer);
    
    bool init();

	void callbackBuyItem(CCObject* pSender,TouchEventType type);
	void callbackClose(CCObject* pSender,TouchEventType type);
	void callbackGemStore(CCObject* pSender);

private:
    
    TouchGroup* m_pUILayer;

};

#endif
