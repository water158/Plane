#ifndef __EQUIPPLANE_LAYER_H__
#define __EQUIPPLANE_LAYER_H__

#include "cocos2d.h"
#include "tinyxml.h"
#include "GameDelegate.h"
#include "SmartRes.h"
#include "StringLibrary.h"
#include "FerrisMaskLayer.h"

USING_NS_CC;

class EquipPlaneLayer : public CCLayer
{
public:
	EquipPlaneLayer(void);
	virtual ~EquipPlaneLayer(void);

	static EquipPlaneLayer* create();

	//按键处理
	virtual void keyBackClicked();
	virtual void keyMenuClicked();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);

private:
	bool init();

	//装备飞机
	void callBackEquipTools(CCObject* pSender);

	//获取装备飞机原型节点
	CCNode* getPlaneNode();

	//为主机创建炮台和僚机
	void createBatteryForHero();

private:
	//主机上炮台的数组
	CC_SYNTHESIZE(CCArray*, m_pArrayEquipments, ArrayEquipments)

};

#endif