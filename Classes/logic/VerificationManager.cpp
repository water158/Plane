#include "VerificationManager.h"

VerificationManager* VerificationManager::m_pSharedHttpInstance = NULL;

VerificationManager::VerificationManager()
{
}

VerificationManager::~VerificationManager()
{
	CCLog(" VerificationManager is destroy");
}

VerificationManager* VerificationManager::shared()
{
	if(!m_pSharedHttpInstance)
	{
		m_pSharedHttpInstance = new VerificationManager();
		m_pSharedHttpInstance->init();
	}

	return m_pSharedHttpInstance;
}

bool VerificationManager::init()
{
	m_bReturnPurchraseResult = false;
	m_bNetConnected = false;
	m_bNetCheck = false;
	//m_bCallSDK = false;
	m_bRefreshResult = false;
	m_nRequestIndex = 0;
	m_nCheckUnFinishedIndex = 0;
	m_strCurrentBid = "";

	return true;
}

/**********************************************
 * 检查网络
 **********************************************/

//检查网络连接
void VerificationManager::requestCheckNetworkAvailable()
{
	//创建对象
	CCHttpRequest* request = new CCHttpRequest();
    
	//设置请求地址
	request->setUrl(URL_CHECK_NETWORK_AVAILABLE);
	
    //连接类型为GET
	request->setRequestType(CCHttpRequest::kHttpGet);
	
    //请求完的回调函数
	request->setResponseCallback(this, httpresponse_selector(VerificationManager::responseCheckNetworkAvailable));
	
    //请求的数据
	request->setRequestData("", 0);
	
    //tag
	request->setTag("100");
    
	//发送请求
	CCHttpClient::getInstance()->send(request);
	
    //释放发送的数据
	request->release();

}
//返回有无网络的结果
void VerificationManager::responseCheckNetworkAvailable(CCHttpClient* client, CCHttpResponse* response)
{
	//接收返回的消息
	if (!response)
	{
		return;
	}
    
	//获取请求的类型
	int nRequestType = response->getHttpRequest()->getRequestType();
	CCLog("request type %d", nRequestType);

	//请求的标记
	if (0 != strlen(response->getHttpRequest()->getTag())) 
	{
		CCLog("%s ------>oked", response->getHttpRequest()->getTag());
	}

	//返回码statusCode
	int statusCode = response->getResponseCode();
	CCLog("response code: %d", statusCode);

	m_bNetCheck = true;

	//不成功就返回
	if (!response->isSucceed()) 
	{
		CCLog("response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		m_bNetConnected = false;
		return;
	}

	//接收返回的信息
	std::vector<char> *pBuffer = response->getResponseData();
	CCLog("-----%s-------",pBuffer);

	//到这说明可以联网
	m_bNetConnected = true;
}

/**********************************************
 * 验证是否支付
 **********************************************/

//请求购买结果
void VerificationManager::requestPurchase(string trade_id)
{
	//创建对象
	CCHttpRequest* request = new CCHttpRequest();
    
	string strURL = URL_PAYMENG_VERIFICATION + m_strCurrentBid;
	
    //设置请求地址
	request->setUrl(strURL.c_str());
	//连接类型为GET
	request->setRequestType(CCHttpRequest::kHttpGet);
	//请求完的回调函数
	request->setResponseCallback(this, httpresponse_selector(VerificationManager::responsePurchase));
	//请求的数据
	request->setRequestData("", 0);
	//tag
	request->setTag(trade_id.c_str());
	//发送请求
	
    CCHttpClient::getInstance()->send(request);
	//释放发送的数据
    request->release();
}

//返回购买结果
void VerificationManager::responsePurchase(CCHttpClient* client, CCHttpResponse* response)
{
	//接收返回的消息
	if (!response)
	{
		return;
	}
    
	//获取请求的类型
	int nRequestType = response->getHttpRequest()->getRequestType();
	CCLog("request type %d", nRequestType);

	//请求的标记
	if (0 != strlen(response->getHttpRequest()->getTag())) 
    {
        CCLog("%s ------>oked", response->getHttpRequest()->getTag());
    }

	//返回码statusCode
	int statusCode = response->getResponseCode();
	CCLog("response code: %d", statusCode);

	//不成功就返回
	if (!response->isSucceed()) 
    {
		CCLog("response failed");
		CCLog("error buffer: %s", response->getErrorBuffer());
		m_bNetConnected = false;
		return;
	}
	 
	//接收返回的信息
	std::vector<char> *pBuffer = response->getResponseData();
	CCLog("-----%s----%d---", pBuffer, (int)pBuffer->size());

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

	//截取字符串
	char* result = strtok(pChar, ",");
	char* tradeID = strtok(NULL, ",");

	if(compareString(result, "ok"))
	{
		//m_bReturnPurchraseResult = true;
		//返回结果后直接停止刷新
		this->setRefreshResult(false);
		this->setRequestIndex(0);
        
		//更改配置文件
        PurchaseLogManager::shared()->changeToFinished(tradeID);
        
		int product_id = PurchaseLogManager::shared()->getProductID(tradeID);
        
        UserData::shared()->increaseGemNumByProductID(product_id);
        
		CCLog("getResponseData  is  ok");
	}
	else
	{
		CCLog("getResponseData  is  not  ok");
	}
}

void VerificationManager::ok()
{
    CCLog("OK");
}
