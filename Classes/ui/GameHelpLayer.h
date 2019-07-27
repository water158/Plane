#ifndef __FERRIS_GAME__GameHelpLayer__
#define __FERRIS_GAME__GameHelpLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "GameMainLayer.h"
#include "SoundEffectLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameHelpLayer : public cocos2d::CCLayer,public CCScrollViewDelegate
{
public:
	virtual ~GameHelpLayer(void);
    GameHelpLayer(void);
	//按键处理
	virtual void keyBackClicked();
	virtual void keyMenuClicked();
	static GameHelpLayer* create();
    virtual void scrollViewDidScroll(CCScrollView *view){};
    virtual void scrollViewDidZoom(CCScrollView *view){};

    CCScrollView* tScrollView;
    //label数组用来存放各label
    cocos2d::CCArray* m_labelArr;
private:
	bool init();
	void menu_Return(CCObject* pSender);
};

#endif