//
//  BaseReward.cpp
//  ambition
//
//  Created by mini01 on 15-1-29.
//
//

#include "BaseReward.h"

BaseReward::BaseReward()
{

}

BaseReward::~BaseReward()
{

}

BaseReward* BaseReward::create(TiXmlElement *elm)
{
    BaseReward* reward = new BaseReward();
    
    if (reward && reward->init(elm))
    {
        reward->autorelease();
        
        return reward;
    }
    
    return NULL;
}

bool BaseReward::init(TiXmlElement *elm)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    //初始化数据
    this->setID("0");
    this->setTypeReward(kSignReward_noting);
    this->setTagReward(0);
    this->setValue(0);
    this->setImgID("-1");
    
    if (elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }
    
    if (elm->Attribute("tag"))
    {
        this->setTagReward(atoi(elm->Attribute("tag")));
    }
    
    if (elm->Attribute("type"))
    {
        this->setTypeReward((SignRewardType)atoi(elm->Attribute("type")));
    }
    
    if (elm->Attribute("value"))
    {
        this->setValue(atoi(elm->Attribute("value")));
    }
    
    if (elm->Attribute("img"))
    {
        this->setImgID(elm->Attribute("img"));
    }
    
    return true;
}