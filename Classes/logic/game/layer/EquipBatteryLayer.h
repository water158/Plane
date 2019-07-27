#ifndef __EQUIPBATTERY_LAYER_H__
#define __EQUIPBATTERY_LAYER_H__

#include "cocos2d.h"
#include "tinyxml.h"
#include "GameDelegate.h"
#include "SmartRes.h"
#include "StringLibrary.h"
#include "FerrisMaskLayer.h"
#include "cocos-ext.h"

//滑动窗口大小
#define VIEW_WIDTH 640
#define VIEW_HEIGHT 200
#define kTagScrollView 50

USING_NS_CC;
using namespace extension;

class EquipBatteryLayer : public CCLayer, public CCScrollViewDelegate
{
public:
	EquipBatteryLayer(void);
	virtual ~EquipBatteryLayer(void);

	static EquipBatteryLayer* create();

	//按键处理
	virtual void keyBackClicked();
	virtual void keyMenuClicked();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	virtual void scrollViewDidScroll(extension::CCScrollView *view);
	virtual void scrollViewDidZoom(extension::CCScrollView* view);

private:
	bool init();

	//装备飞机
	void callBackEquipPlane(CCObject* pSender);

	//出售装备
	void callBackSellEquip(CCObject* pSender);

	//穿上装备
	void callBackEquip(CCObject* pSender);

	//购买装备
	void callBackBuy(CCObject* pSender);

	//升级装备
	void callBackUpgradeEquip(CCObject* pSender);

	//获取切换炮台的节点
	CCNode* getEquipBatteryNode();

	//点击了装备栏
	void onClickEquipItem();

	//点击了拥有装备栏
	void onClickHaveItem();

	//点击了商店装备栏
	void onClickShopItem();

	//为装备槽装载装备
	void createEquipment();

	//切换装备后刷新操作
	void refreshEquipment();

	//获取包裹中的装备的数量
	void getEquipNumInPackage(CCDictionary* &equip_in_package);

	//点击装备
	void callBackClickEquipment(CCObject* pSender);


private:
	//按钮
	CCMenuItemImage* m_pItemSell;
	CCMenuItemImage* m_pItemEquip;
	CCMenuItemImage* m_pItemBuy;
	CCMenuItemImage* m_pItemUpgrade;

	//按钮的文字
	CCLabelTTF* m_pSellLabel;
	CCLabelTTF* m_pEquipLabel;
	CCLabelTTF* m_pDiamondLabel;
	CCLabelTTF* m_pUpgradeLabel;

	//选择的装备栏下标（从0开始）
	int m_nSelectedEquip;
	//选择的拥有的装备（从0开始）
	int m_nSelectedOwnedEquip;
	//选择的商店里的装备的下标（从0开始）
	int m_nSelectedShopEquip;

	//是否点击了装备按钮
	bool m_bClickEquipButton;

	//装载中的装备
	CC_SYNTHESIZE(CCDictionary*, m_pDictEquippedEquipments, DictEquippedEquipments)

	//装载后剩余的装备
	CC_SYNTHESIZE(CCDictionary*, m_pDictRemainingEquipments, DictRemainingEquipments)

	/**************背包栏滑动***************/
	//调整位置
	void adjustPosition(CCPoint posPreOffset, CCPoint posEndOffset);
	//滚动到某一页
	void scrollToPage(int npage, float delay);

	//时候开始滑动了
	bool m_bTouch;
	//滑动前的位移
	CCPoint m_posPreOffset;
};

#endif