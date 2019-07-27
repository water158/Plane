//
//  Store.h
//  plane
//
//  Created by mini02 on 13-12-28.
//
//

#ifndef __plane__Store__
#define __plane__Store__

#include <iostream>
#include "cocos2d.h"

#include "GlobalEnum.h"
#include "NDKHelper.h"

USING_NS_CC;
using namespace std;

class StoreManager
{
    
public:
	StoreManager(string group_name);
	~StoreManager();
    
    //调用JAVA内的支付函数
    // callbackTag  用于在NDKHelper中
    // fake_code    购买道具的编号
    // target       回调的目标
    // callback     回调的方法
	//最新的付费（统一购买用代币）（分为 2、4、6、8、12元）
	void newPurchase(string callbackTag, int fake_code, CCNode* target, SEL_CallFuncND callback);
    
private:
    
    string m_sGroupName;
};

#endif /* defined(__plane__Store__) */