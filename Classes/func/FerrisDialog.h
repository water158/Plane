#ifndef __FERRIS_GAME__FerrisDialog__
#define __FERRIS_GAME__FerrisDialog__

#include "cocos2d.h"

#include "GlobalDelegate.h"
#include "GlobalEnum.h"

#include "SoundEffectLayer.h"
#include "FerrisMaskLayer.h"
#include "SmartRes.h"
#include "StringLibrary.h"
#include "cocos-ext.h"
#include "SoundEffectManager.h"


USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;
USING_NS_STD;

class FerrisDialog : public CCNode
{
public:
	virtual ~FerrisDialog();
	static FerrisDialog *createWithLibraryTag(int type, const char *library_tag, CCNode* target, SEL_MenuHandler callbackLT, SEL_MenuHandler callbackRT);
    
    static FerrisDialog *createWithContent(int type, const char *content, CCNode* target, SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT);
    
    static FerrisDialog *createWithStr(const char* str,CCNode* target,SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT,bool canSell = true);
    
    void callbackDialogDoNothing(CCObject* pSender);
    
    void callbackDoNothing(CCObject* pSender,TouchEventType type);

    
protected:
	FerrisDialog();
    
	bool initWithContent(int type, const char *content, CCNode* target, SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT);
    
    bool initWithTag(int type, const char *library_tag, CCNode* target, SEL_MenuHandler callbackLT, SEL_MenuHandler callbackRT);
    
    bool initWithStr(const char* str,CCNode* target,SEL_TouchEvent callbackLT, SEL_TouchEvent callbackRT,bool canSell = true);
    
    TouchGroup* m_puiLayer;
};

#endif