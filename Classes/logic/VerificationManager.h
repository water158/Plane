#ifndef __FERRIS_GAME__PaymentVerification__
#define __FERRIS_GAME__PaymentVerification__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SmartRes.h"
#include "GlobalEnum.h"
#include "GlobalTagZOrder.h"

#include "FerrisDialog.h"
#include "UserData.h"
#include "StringLibrary.h"
#include "AnalyticsStatID.h"

#include "PurchaseLogManager.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	
//#include "MobClickCpp.h"

#endif


USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_STD;

class VerificationManager : public CCObject
{
public:
	~VerificationManager();

	static VerificationManager* shared();
    
    /**********************************************
     * 检查网络
     **********************************************/
	//检查网络连接
	void requestCheckNetworkAvailable();

	//返回有无网络的结果
	void responseCheckNetworkAvailable(CCHttpClient* client, CCHttpResponse* response);
    
    /**********************************************
     * 验证是否支付
     **********************************************/

	//请求get（向服务器请求付费信息）
	void requestPurchase(string trade_id);

	//返回请求结果
	void responsePurchase(CCHttpClient* client, CCHttpResponse* response);

    /**********************************************
     * 显示是否支付
     **********************************************/
    void ok();
    
private:
	VerificationManager();

	static VerificationManager* m_pSharedHttpInstance;

	bool init();

	//购买是否返回结果
	CC_SYNTHESIZE (bool, m_bReturnPurchraseResult, ReturnPurchraseResult);
	//是否有网络连接
	CC_SYNTHESIZE (bool, m_bNetConnected, NetConnected);
	//是否进行了网络检查
	CC_SYNTHESIZE (bool, m_bNetCheck, NetCheck);
	//是否可以进行SDK的调用
	//CC_SYNTHESIZE (bool, m_bCallSDK, CallSDK);
	//是否开始进行消息刷新
	CC_SYNTHESIZE (bool, m_bRefreshResult, RefreshResult);

	//网络结果请求的时间下标
	CC_SYNTHESIZE (int, m_nRequestIndex, RequestIndex);

	//从进游戏就开检测有没有为完成的交易（时间下标）
	CC_SYNTHESIZE (int, m_nCheckUnFinishedIndex, CheckUnFinishedIndex);

public:
	//当前请求购买的商品bid
	std::string m_strCurrentBid;
};


#endif