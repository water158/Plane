#ifndef __FERRIS_GAME__ComplaintManager__
#define __FERRIS_GAME__ComplaintManager__

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


class ComplaintManager : public CCObject
{
    CC_SYNTHESIZE(bool, m_bReturnComplainResult, ReturnComplainResult);
    
    
public:
	~ComplaintManager();

	static ComplaintManager* shared();

	//请求申诉
	void requestComplaint();

	//返回申诉结果
	void responseComplaint(CCHttpClient* client, CCHttpResponse* response);

private:
	ComplaintManager();

	static ComplaintManager* m_pSharedComplaintManager;

	bool init();
};


#endif