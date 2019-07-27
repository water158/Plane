//
//  PaymentAndroid.h
//  plane
//
//  Created by mini02 on 13-12-28.
//
//

#ifndef __FERRIS_GAME__PaymentAndroid__
#define __FERRIS_GAME__PaymentAndroid__

#include <iostream>
#include "cocos2d.h"
#include "GlobalEnum.h"
#include "MiscDefination.h"
#include "NDKHelper.h"

USING_NS_CC;
USING_NS_STD;

class PaymentAndroid
{
    
public:
	PaymentAndroid(string group_name);
	~PaymentAndroid();
    
    //调用JAVA内的支付函数
    // callbackTag  用于在NDKHelper中
    // fake_code    购买道具的编号
    // target       回调的目标
    // callback     回调的方法
	//最新的付费（统一购买用代币）（分为 2、4、6、8、12元）
	void buy(string callbackTag, int product_id, CCNode* target, SEL_CallFuncND callback);
    
private:
    
    string m_sGroupName;
};

#endif