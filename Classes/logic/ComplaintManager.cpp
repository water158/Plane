#include "ComplaintManager.h"

ComplaintManager* ComplaintManager::m_pSharedComplaintManager = NULL;

ComplaintManager::ComplaintManager()
{
    CCLog(" ComplaintManager Created");
}

ComplaintManager::~ComplaintManager()
{
	CCLog(" ComplaintManager Destroyed");
}

ComplaintManager* ComplaintManager::shared()
{
	if(!m_pSharedComplaintManager)
	{
		m_pSharedComplaintManager = new ComplaintManager();
		m_pSharedComplaintManager->init();
	}

	return m_pSharedComplaintManager;
}

bool ComplaintManager::init()
{

	return true;
}

//请求申诉
void ComplaintManager::requestComplaint()
{
	//创建对象
	CCHttpRequest* request = new CCHttpRequest();
    
	//设置请求地址
	request->setUrl(URL_COMPLAINT);
	
    //连接类型为POST
	request->setRequestType(CCHttpRequest::kHttpPost);
	
    //请求完的回调函数
	request->setResponseCallback(this, httpresponse_selector(ComplaintManager::responseComplaint));
    
	//请求的数据
	std::string user_history = PurchaseLogManager::shared()->toString();
	request->setRequestData(user_history.c_str(), strlen(user_history.c_str()));
    
	//tag
	request->setTag("101");
	
    //发送请求
	CCHttpClient::getInstance()->send(request);
	
    //释放发送的数据
	request->release();
}

//返回申诉结果
void ComplaintManager::responseComplaint(CCHttpClient* client, CCHttpResponse* response)
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
		return;
	}

	//接收返回的信息
	std::vector<char> *pBuffer = response->getResponseData();

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

	if(compareString((char *)pBuffer, "ok"))
	{
		m_bReturnComplainResult = true;
	}
    
	m_bReturnComplainResult = true;
}