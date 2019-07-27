#include "CouponManager.h"

CouponManager::CouponManager()
{
    CCLog("CouponManager Created");
}

CouponManager::~CouponManager()
{
	CCLog("CouponManager Destroyed");
    
    m_pFunction = NULL;
}

CouponManager* CouponManager::create(CCNode *target, DECLAIMER_FUNC_CALLBACK function)
{
	CouponManager *manager = new CouponManager();
	if(manager && manager->init(target, function))
	{
		manager->autorelease();
		return manager;
	}
	return NULL;
}

bool CouponManager::init(CCNode *target, DECLAIMER_FUNC_CALLBACK function)
{
    m_pTarget = target;
    m_pFunction = function;
    
    return true;
}

void CouponManager::cleanup()
{
    CCLog("CouponManager::cleanup");
    
    m_pTarget = NULL;
    m_pFunction = NULL;
}

//请求验证验证码是否正确
void CouponManager::requestCoupon(string strCode)
{
	//创建对象
	CCHttpRequest* request = new CCHttpRequest();
    
	//设置请求地址
	request->setUrl(URL_COUPON);
    
    CCLog("Connecting to %s", URL_COUPON);
	
    //连接类型为GET
	request->setRequestType(CCHttpRequest::kHttpGet);
	
    //请求完的回调函数
	request->setResponseCallback(this, httpresponse_selector(CouponManager::responseCoupon));
	
    //请求的数据
	request->setRequestData(strCode.c_str(), strlen(strCode.c_str()));
	
    //tag
	request->setTag("102");
	
    //发送请求
	CCHttpClient::getInstance()->send(request);
	
    //释放发送的数据
	request->release();
}

//返回验证码是否正确的结果
void CouponManager::responseCoupon(CCHttpClient* client, CCHttpResponse* response)
{
    FerrisResult *ret = FerrisResult::create();
    
	if (!response)
	{
        ret->setResult(false);
	}
    else
    {
        //获取请求的类型
        int nRequestType = response->getHttpRequest()->getRequestType();
        CCLog("request type %d", nRequestType);
        
        //返回码statusCode
        int statusCode = response->getResponseCode();
        CCLog("response code: %d", statusCode);
        
        //请求的标记
        if (strlen(response->getHttpRequest()->getTag()) != 0)
        {
            CCLog("response tag %s", response->getHttpRequest()->getTag());
        }
        
        
        //不成功就返回
        if (!response->isSucceed())
        {
            CCLog("response failed");
            CCLog("error buffer: %s", response->getErrorBuffer());
            
            ret->setResult(false);
        }
        else
        {
            //接收返回的信息
            vector<char> *pBuffer = response->getResponseData();
            
            CCLog("--Recv--");
            
            vector<char>::iterator iter = pBuffer->begin();
            for (; iter < pBuffer->end(); iter++)
            {
                CCLog("%c", *iter);
            }
            
            CCLog("--------");
            
            char pChar[100] = "";
            int nSize = pBuffer->size() > 100 ? 100 : pBuffer->size();
            //字符串去空格
            for (int i = 0; i < nSize; i++)
            {
                char c = (*pBuffer)[i];
                if(c != 10)
                {
                    pChar[strlen(pChar)] = (*pBuffer)[i];
                }
            }
            
            if(compareString(pChar, "ok"))
            {
                ret->setResult(true);
            }
            else if(compareString(pChar, "not ok"))
            {
                ret->setResult(false);
            }
        }
        
        
        if (m_pTarget && m_pFunction)
        {
            (m_pTarget->*m_pFunction)(ret);
        }
        else
        {
            CCLog("(m_pTarget->*m_pFunction)(ret); failed");
        }
        
        ret->release();
    }
}