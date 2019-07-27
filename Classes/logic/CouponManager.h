#ifndef __FERRIS_GAME__CouponManager__
#define __FERRIS_GAME__CouponManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalEnum.h"
#include "GlobalTagZOrder.h"
#include "SmartRes.h"

#include "FerrisDialog.h"
#include "FerrisResult.h"
#include "UserData.h"
#include "StringLibrary.h"
#include "AnalyticsStatID.h"


#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	
//#include "MobClickCpp.h"

#endif

USING_NS_CC;
USING_NS_CC_EXT;


class CouponManager : public CCObject
{
    
public:
    static CouponManager* create(CCNode *target, DECLAIMER_FUNC_CALLBACK function);
    
	~CouponManager();

	//请求申诉
	void requestCoupon(string strCode);

	//返回申诉结果
	void responseCoupon(CCHttpClient* client, CCHttpResponse* response);
    
    void cleanup();

private:
	CouponManager();

	bool init(CCNode *target, DECLAIMER_FUNC_CALLBACK function);
    
    CCNode *m_pTarget;
    
    DECLAIMER_FUNC_CALLBACK m_pFunction;
};


#endif