#ifndef __FERRIS_GAME__PurchaseLogManager__
#define __FERRIS_GAME__PurchaseLogManager__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "tinyxml.h"

#include "FerrisTools.h"
#include "UserData.h"
#include "VerificationManager.h"

USING_NS_CC;
USING_NS_STD;

#define ATTRIBUTE_TRADE_ID "tid"
#define ATTRIBUTE_PRODUCT_ID "pid"
#define ATTRIBUTE_TIME "time"
#define ATTRIBUTE_RETRY "retry"
#define ATTRIBUTE_FINISH "finish"
#define ATTRIBUTE_GEM_TO_BUY "dtb"
#define ATTRIBUTE_GEM_HAVED "dh"

#define TOTAL_RETRY_TIMES 60

class PurchaseLogManagerItem : public CCObject
{
    //======================
    // 属性
    //======================
    
    //交易ID
    CC_SYNTHESIZE(string, m_sTradeID, TradeID);
    
    //产品ID
    CC_SYNTHESIZE(int, m_sProductID, ProductID);
    
    //时间
    CC_SYNTHESIZE(string, m_sTime, Time);
    
    //重复测试次数
    CC_SYNTHESIZE(int, m_nRetryTimes, RetryTimes);
    
    //是否交易完成
    CC_SYNTHESIZE(bool, m_bFinished, Finished);
    
    //附加信息:当时拥有的钻石数量
    CC_SYNTHESIZE(string, m_sGemHaved, GemHaved);
    
    //======================
    // 方法
    //======================
public:
    //加载自身的数据
	void loadData(TiXmlElement *elm);
    
    void decreaseRetryTimes();
    
    static PurchaseLogManagerItem *create();
};

/*
 
 <?xml version="1.0" encoding="utf-8" ?>
 <PurchaseLogManager>
 <Item price="PRICE" tid="TRADE_ID" time="YYYY.MM.DD HH:MM:SS" finish="1/0" dh="GEM_HAVED">
 </PurchaseLogManager>
 
 
 */

class PurchaseLogManager
{
    //======================
    // 属性
    //======================
    
    //版本
	CC_SYNTHESIZE(int, m_nVersion, Version);
    
    CC_SYNTHESIZE(CCDictionary*, m_dictItems, Items);
    
private:
    //路径
	string m_sPath;
	
private:
	static PurchaseLogManager* s_sharedPurchaseLogManager;
    
public:
	PurchaseLogManager(void);
	virtual ~PurchaseLogManager(void);

	static PurchaseLogManager* shared(void);
    
    //初始化数据
	void init();
    
    //检查文件是否存在
    void check();
    
    //加载
	void load();
    
    //保存
    void save();
    
    //新增消费记录
	void insert(string tradeId, int product_id);
    
    //
    void changeToFinished(string tradeId);
    
    //
    int getProductID(string tradeId);
    
    //
    void syncPurchase();
    
    //获取所有的消费信息
	string toString();
    
    /*
	//检测userdata版本
	void checkVersion();

	//根据userdata版本加载
	void loadDataFromVer(int version);
     */
};

#endif