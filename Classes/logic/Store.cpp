//
//  Store.cpp
//  plane
//
//  Created by mini02 on 13-12-28.
//
//
#include "Store.h"

StoreManager::StoreManager(string group_name)
: m_sGroupName("DefaultGroupName")
{
	m_sGroupName = group_name;
	CCLog("StoreManager::StoreManager Group Name is %s", group_name.c_str());
}

StoreManager::~StoreManager()
{
	CCLog("StoreManager::~StoreManager %s", m_sGroupName.c_str());
    //NDKHelper::RemoveSelectorsInGroup(m_sGroupName.c_str());
}

//调用JAVA内的支付函数
// callbackTag  用于在NDKHelper中
// fake_code    购买道具的编号
// target       回调的目标
// callback     回调的方法

void StoreManager::newPurchase(string callbackTag, int fake_code, CCNode* target, SEL_CallFuncND callback) //购买道具的类型 由他调用 java
{
	//这里建立同支付接口之间的关系
	//NDKHelper::AddSelector(m_sGroupName.c_str(),
	//	callbackTag.c_str(),
	//	callback,
	//	target);

    //2014-11-14
    /*
	int price = 2;
	string strOrderName = "";
	switch (fake_code) {
	case BUY_CRYSTAL_2:
		price = 2;
		strOrderName = PAY_ORDER_NAME_CRYSTAL_220;
		break;
	case BUY_CRYSTAL_4:
		price = 4;
		strOrderName = PAY_ORDER_NAME_CRYSTAL_480;
		break;
	case BUY_CRYSTAL_6:
		price = 6;
		strOrderName = PAY_ORDER_NAME_CRYSTAL_800;
		break;
	case BUY_CRYSTAL_8:
		price = 8;
		strOrderName = PAY_ORDER_NAME_CRYSTAL_1200;
		break;
	case BUY_CRYSTAL_12:
		price = 12;
		strOrderName = PAY_ORDER_NAME_CRYSTAL_2400;
		break;
	default:
		break;
	}

	//参数
	CCDictionary* params = CCDictionary::create();
	//第二个参数是key 第一个参数是value
	params->setObject(CCString::createWithFormat("%d", fake_code), "fake_code");
	params->setObject(CCString::create(callbackTag), "callback_tag");
	params->setObject(CCString::createWithFormat("%d", price), "price");
	params->setObject(CCString::create(strOrderName), "order_name");
     
     */

	//调用(方法名为第一个参数，这个方法名是JAVA里面的方法名)，并传递参数(第二个参数)
	//SendMessageWithParams(string(PURCHASE_METHOD_NAME_IN_JAVA), params);
}