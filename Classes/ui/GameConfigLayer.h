#ifndef __FERRIS_GAME__GameConfigLayer__
#define __FERRIS_GAME__GameConfigLayer__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SmartRes.h"
#include "cocos-ext.h"
#include "GameDelegate.h"
#include "GlobalEnum.h"


USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;
using namespace CocosDenshion;

class GameConfigLayer : public CCLayer
{
public:
	virtual ~GameConfigLayer(void);

	static GameConfigLayer* create();
	//按键处理
	virtual void keyBackClicked();
	virtual void keyMenuClicked();
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);

	void effectCallBack();    //音效
	void musicCallBack();//音乐
	void vibrationCallBack();//震动
    
	CCRect effect_left;         //选择矩形
	CCRect effect_right;
	CCRect music_left;
	CCRect music_right;
	CCRect vibration_left;
	CCRect vibration_right;
	
private:
    TouchGroup* m_pUILayer;
    
	GameConfigLayer(void);
	bool init();
	void menu_close(CCObject* pSender,TouchEventType type);

	int curSelectItem;      //当前选择的设置
    
    CCLayerColor* m_pBgColorLayer;
    
    Button* btn_sound_on;
    Button* btn_sound_off;
    
    Button* btn_music_on;
    Button* btn_music_off;
    
    Button* btn_particle_on; //画面粒子设置
    Button* btn_particle_off;
    
    void musicSettingCallBack(CCObject* pSender,TouchEventType type);//声音设置回调
    
    void soundSettingCallBack(CCObject* pSender,TouchEventType type);//音效设置回调
    
    void particlesSettingCallBack(CCObject* pSender,TouchEventType type);//画面设置回调
    
    void actionFinish(CCObject* pSender);
};

enum
{
	None,
	Effect,
	Music,
	Vibration,
};
#endif