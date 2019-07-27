//
//  SignManager.cpp
//  ambition
//
//  Created by mini01 on 14-11-26.
//
//

#include "SignManager.h"

SignReward::SignReward()
{

}

SignReward::~SignReward()
{

}


SignReward* SignReward::create(TiXmlElement* elm)
{
    SignReward* reward = new SignReward();
    if (reward && reward->init(elm))
    {
        reward->autorelease();
        return reward;
    }
    
    return NULL;
}

bool SignReward::init(TiXmlElement* elm)
{
    CCNode::init();
    
    //初始化数据
    this->setID("0");
    this->setTypeReward(kSignReward_noting);
    this->setValue(0);
    this->setStrLabel("");
    this->setImgID("-1");
    
    if (elm->Attribute("day"))
    {
        this->setID(elm->Attribute("day"));
    }
    
    if (elm->Attribute("type"))
    {
        this->setTypeReward((SignRewardType)atoi(elm->Attribute("type")));
    }
    
    if (elm->Attribute("count"))
    {
        this->setValue(atoi(elm->Attribute("count")));
    }
    
    if (elm->Attribute("label"))
    {
        this->setStrLabel(elm->Attribute("label"));
    }
    
    if (elm->Attribute("img"))
    {
        this->setImgID(elm->Attribute("img"));
    }
    
    return true;
}

SignManager::SignManager()
{
    
}

SignManager::~SignManager()
{
    CC_SAFE_DELETE_ARRAY(m_rewardArr);
}

SignManager* SignManager::m_sharedInstance = NULL;

SignManager* SignManager::shared()
{
    if (!m_sharedInstance)
    {
        m_sharedInstance = new SignManager();
        m_sharedInstance->init();
    }
    
    return m_sharedInstance;
}

bool SignManager::init()
{
    m_rewardArr = CCArray::create();
    m_rewardArr->retain();
    
    //获取用户的安装游戏时间
    long firstTimeRecord = UserData::shared()->getFirstTimeRecord();
    
    //获取当前系统时间
    long curtime = getCurrentSystemTime();
    
    //时间间隔
    long interval = curtime - firstTimeRecord;
    
    //把时间间隔转换为周期
    int circle = interval / (30 * 24 * 60 * 60);
    
    CCLog("circle : %d",circle);
    
    CCLog("UserData::shared()->getSignRewardUpdateRecord() : %d",UserData::shared()->getSignRewardUpdateRecord());
    
    //对比周期与用户的签到更新记录
    if (circle != UserData::shared()->getSignRewardUpdateRecord())
    {
        //更改用户签到更新记录
        UserData::shared()->setSignRewardUpdateRecord(circle);
        //同时将用户已签到的天数清空
        UserData::shared()->setHadeSigned(0);
    }
    
    //要读取的奖励编号 1----12
    int flag = circle % 12 + 1;
    
    //todo：测试用  记得删除
    flag = 1;
    
    //读取奖励的xml文件
    TiXmlDocument *xmlDoc = new TiXmlDocument();
    unsigned long nLength = 0;
    
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData("common/signin_reward.xml", "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获取数据的根元素
    TiXmlElement* elmRoot = xmlDoc->RootElement();
    
    TiXmlElement* elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
    {
        if (atoi(elmCursor->Attribute("id")) == flag)
        {
            loadRewardData(elmCursor);
        }
        
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    return true;
}

void SignManager::loadRewardData(TiXmlElement *elm)
{
    TiXmlElement* elmCursor = elm->FirstChildElement();
    
    while (elmCursor)
    {
        SignReward* reward = SignReward::create(elmCursor);
        m_rewardArr->addObject(reward);
        
        elmCursor = elmCursor->NextSiblingElement();
    }
}