#ifndef __SELECTPLANE_LAYER_H__
#define __SELECTPLANE_LAYER_H__

#include "cocos2d.h"
#include "tinyxml.h"
#include "GameDelegate.h"

USING_NS_CC;

class SelectPlaneLayer : public CCLayer
{
public:
    SelectPlaneLayer(void);
	virtual ~SelectPlaneLayer(void);
	
	static SelectPlaneLayer* create();

	//按键处理
	virtual void keyBackClicked();
	virtual void keyMenuClicked();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
private:
	bool init();

	//装备飞机
	void callBackEquipPlane(CCObject* pSender);
};

#endif