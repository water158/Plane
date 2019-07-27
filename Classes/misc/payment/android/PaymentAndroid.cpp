//
//  Store.cpp
//  plane
//
//  Created by mini02 on 13-12-28.
//
//
#include "PaymentAndroid.h"

PaymentAndroid::PaymentAndroid(string group_name)
: m_sGroupName("DefaultGroupName")
{
	m_sGroupName = group_name;
	CCLog("PaymentAndroid::PaymentAndroid Group Name is %s", group_name.c_str());
}

PaymentAndroid::~PaymentAndroid()
{
	CCLog("PaymentAndroid::~PaymentAndroid %s", m_sGroupName.c_str());
    NDKHelper::RemoveSelectorsInGroup(m_sGroupName.c_str());
}

//调用JAVA内的支付函数
// callbackTag  用于在NDKHelper中
// fake_code    购买道具的编号
// target       回调的目标
// callback     回调的方法

void PaymentAndroid::buy(string callbackTag, int product_id, CCNode* target, SEL_CallFuncND callback) //购买道具的类型 由他调用 java
{
	//这里建立同支付接口之间的关系
    
	NDKHelper::AddSelector(m_sGroupName.c_str(),
		callbackTag.c_str(),
		callback,
		target);
    
	int nPrice = getPriceByProductID(product_id);
	string strOrderName = getPayOrderNameByProductID(product_id);
	string strPayCode = getPayCodeByProductID(product_id);

	//参数
	CCDictionary* params = CCDictionary::create();
	//第二个参数是key 第一个参数是value
	params->setObject(CCString::create(callbackTag), "callback_tag");
	params->setObject(CCString::create(strPayCode), "pay_code");
	params->setObject(CCString::createWithFormat("%d", nPrice), "price");
	params->setObject(CCString::create(strOrderName), "order_name");
    
	//调用(方法名为第一个参数，这个方法名是JAVA里面的方法名)，并传递参数(第二个参数)
	SendMessageWithParams(string(PURCHASE_METHOD_NAME_IN_JAVA), params);
}