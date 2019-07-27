#ifndef __FERRIS_GAME__NotificationLayer__
#define __FERRIS_GAME__NotificationLayer__

#include "cocos2d.h"
#include "SmartRes.h"
#include "GameDelegate.h"
#include "cocos-ext.h"
#include "AchieveItem.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;

using namespace cocos2d::ui;

class NotificationLayer : public CCLayer
{
public:
	~NotificationLayer();

	static NotificationLayer* create();
    
    void show(const char* content);
    
    void showAchieve(AchieveItem *item,achieve_type type);

	void callBackEnter();

private:
	NotificationLayer();

	bool init();
    
    //动画播放结束回调
    void actionFinish(CCObject* pSender);
    
    //开关控制动画播放
    bool b_showEnable;
    
    CCSprite* spBG;
    
    CCLabelTTF *pLabel;
    
    float m_fBGHeight;
    
    float m_fBGWidth;
    
    TouchGroup* m_pUILayer;
};


#endif