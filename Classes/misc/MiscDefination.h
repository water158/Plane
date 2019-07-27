#ifndef __FERRIS_GAME__MiscDefination__
#define __FERRIS_GAME__MiscDefination__

/**********************************************
 * PAYMENT
 **********************************************/

/*===================
 * PRODUCT_ID
 ===================*/
enum
{
	//购买钻石 2元
	BUY_PRODUCT_1 = 1,

	//购买钻石 4元
	BUY_PRODUCT_2 = 2,

	//购买钻石 6元
	BUY_PRODUCT_3 = 3,

	//购买钻石 8元
	BUY_PRODUCT_4 = 4,

	//购买钻石 12元
	BUY_PRODUCT_5 = 5,
};

/*===================
 * 以下内容包括
 * 1.Paycode 支付平台后台设定的ID,用于标定某产品
 * 2.Price 花费的人民币
 * 3.Quantity 购买之后增加的数量
 * 4.OrderName 订单名称
 ===================*/

//-----------------Android-----------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

//PayCode
#define PAYCODE_PRODUCT_1 "30000855605201"
#define PAYCODE_PRODUCT_2 "30000855605202"
#define PAYCODE_PRODUCT_3 "30000855605203"
#define PAYCODE_PRODUCT_4 "30000855605204"
#define PAYCODE_PRODUCT_5 "30000855605205"

//Price
#define PRICE_PRODUCT_1 2
#define PRICE_PRODUCT_2 4
#define PRICE_PRODUCT_3 6
#define PRICE_PRODUCT_4 8
#define PRICE_PRODUCT_5 12

//Gem Quantity
#define QUANTITY_PRODUCT_1 20
#define QUANTITY_PRODUCT_2 60
#define QUANTITY_PRODUCT_3 120
#define QUANTITY_PRODUCT_4 260
#define QUANTITY_PRODUCT_5 500

//OrderName
#define ORDERNAME_PRODUCT_1 "宝石20"
#define ORDERNAME_PRODUCT_2 "宝石60"
#define ORDERNAME_PRODUCT_3 "宝石120"
#define ORDERNAME_PRODUCT_4 "宝石260"
#define ORDERNAME_PRODUCT_5 "宝石500"

//-----------------iOS-----------------
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

//PayCode
#define PAYCODE_PRODUCT_1 "gem20"
#define PAYCODE_PRODUCT_2 "gem48"
#define PAYCODE_PRODUCT_3 "gem84"
#define PAYCODE_PRODUCT_4 "gem128"
#define PAYCODE_PRODUCT_5 "gem180"

//Price
#define PRICE_PRODUCT_1 4
#define PRICE_PRODUCT_2 10
#define PRICE_PRODUCT_3 15
#define PRICE_PRODUCT_4 21
#define PRICE_PRODUCT_5 36

//Gem Quantity
#define QUANTITY_PRODUCT_1 60
#define QUANTITY_PRODUCT_2 148
#define QUANTITY_PRODUCT_3 218
#define QUANTITY_PRODUCT_4 298
#define QUANTITY_PRODUCT_5 500

//订单名称
#define ORDERNAME_PRODUCT_1 "宝石60"
#define ORDERNAME_PRODUCT_2 "宝石148"
#define ORDERNAME_PRODUCT_3 "宝石218"
#define ORDERNAME_PRODUCT_4 "宝石298"
#define ORDERNAME_PRODUCT_5 "宝石500"

//-----------------WIN32-----------------
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

//PayCode
#define PAY_CODE_PRODUCT_1 "1"
#define PAY_CODE_PRODUCT_2 "2"
#define PAY_CODE_PRODUCT_3 "3"
#define PAY_CODE_PRODUCT_4 "4"
#define PAY_CODE_PRODUCT_5 "5"

//Price
#define PRICE_PRODUCT_1 1
#define PRICE_PRODUCT_2 2
#define PRICE_PRODUCT_3 3
#define PRICE_PRODUCT_4 4
#define PRICE_PRODUCT_5 5

//Gem Quantity
#define QUANTITY_PRODUCT_1 100
#define QUANTITY_PRODUCT_2 208
#define QUANTITY_PRODUCT_3 308
#define QUANTITY_PRODUCT_4 408
#define QUANTITY_PRODUCT_5 500

//订单名称
#define ORDERNAME_PRODUCT_1 "宝石100"
#define ORDERNAME_PRODUCT_2 "宝石200"
#define ORDERNAME_PRODUCT_3 "宝石300"
#define ORDERNAME_PRODUCT_4 "宝石400"
#define ORDERNAME_PRODUCT_5 "宝石500"

#endif

/*===================
 * 函数们
 ===================*/

//返回支付方提供的产品Paycode
const char* getPayCodeByProductID(int product_id);

//返回花费的RMB
int getPriceByProductID(int product_id);

//返回订单的名称
const char* getPayOrderNameByProductID(int product_id);

//返回获得的钻石
int getGemNumByProductID(int product_id);



/**********************************************
 * UMENG
 **********************************************/

/*===================
 * 虚拟币获取渠道
 ===================*/
//umeng统计中的[虚拟币来源管理]

//打出来的金币
#define ANALYTICS_COIN_GETBY_DROP 2

//奖励的金币
#define ANALYTICS_COIN_GETBY_REWARD 3

//奖励的Gem
#define ANALYTICS_GEM_GETBY_REWARD 4

//购买的金币
#define ANALYTICS_COIN_GETBY_BUY 5

//购买的Gem
#define ANALYTICS_GEM_GETBY_PAY 6

//umeng统计中的[道具管理]-[道具ID]
//这里分开统计IOS和Android两个渠道
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define ANALYTICS_BUY_PRODUCT_1 "and_gem20"
#define ANALYTICS_BUY_PRODUCT_2 "and_gem60"
#define ANALYTICS_BUY_PRODUCT_3 "and_gem120"
#define ANALYTICS_BUY_PRODUCT_4 "and_gem260"
#define ANALYTICS_BUY_PRODUCT_5 "and_gem500"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define ANALYTICS_BUY_PRODUCT_1 "ios_gem60"
#define ANALYTICS_BUY_PRODUCT_2 "ios_gem148"
#define ANALYTICS_BUY_PRODUCT_3 "ios_gem218"
#define ANALYTICS_BUY_PRODUCT_4 "ios_gem298"
#define ANALYTICS_BUY_PRODUCT_5 "ios_gem500"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#define ANALYTICS_BUY_PRODUCT_1 "win_gem1"
#define ANALYTICS_BUY_PRODUCT_2 "win_gem2"
#define ANALYTICS_BUY_PRODUCT_3 "win_gem3"
#define ANALYTICS_BUY_PRODUCT_4 "win_gem4"
#define ANALYTICS_BUY_PRODUCT_5 "win_gem5"

#endif

/**********************************************
 * Share
 **********************************************/
typedef enum
{
    FerrisShareTopic,
    FerrisShareAchieve,
    FerrisShareAccount1,
    FerrisShareAccount0
} FerrisShareType;

#endif