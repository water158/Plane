//
//  AchieveItem.cpp
//  phoenix
//
//  Created by mini01 on 14-11-6.
//
//

#include "AchieveItem.h"


AchieveItem::AchieveItem()
{

}

AchieveItem::~AchieveItem()
{

}

AchieveItem* AchieveItem::create(TiXmlElement *elm)
{
    AchieveItem* data = new AchieveItem();
    if (data && data->loadData(elm))
    {
        data->autorelease();
        
        return data;
    }
    
    return NULL;
}


bool AchieveItem::loadData(TiXmlElement *elm)
{
    if (!elm)
    {

    }
    
    //id
    if (elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }
    
    //name
    TiXmlElement* elmCursor = elm->FirstChildElement();
    this->setName(elmCursor->FirstChild()->Value());
    
    //describe
    elmCursor = elmCursor->NextSiblingElement();
    this->setDescribe(elmCursor->FirstChild()->Value());
    
    //奖励描述
    elmCursor = elmCursor->NextSiblingElement();
    this->setRewardDes(elmCursor->FirstChild()->Value());
    
    //奖励
    elmCursor = elmCursor->NextSiblingElement();
    if (elmCursor)
    {
        
        BaseReward* reward = BaseReward::create(elmCursor);
        
        reward->retain();
        
        this->setRewardFir(reward);
    }
    
    elmCursor = elmCursor->NextSiblingElement();
    
    if (elmCursor)
    {
        BaseReward* reward = BaseReward::create(elmCursor);
        
        reward->retain();
        
        this->setRewardSec(reward);
    }

    //是否完成
    elmCursor = elmCursor->NextSiblingElement();
    this->setIsFinish(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);
    
    //达成要求
    elmCursor = elmCursor->NextSiblingElement();
    this->setRequestCount(atoi(elmCursor->FirstChild()->Value()));
    
    //类型
    elmCursor = elmCursor->NextSiblingElement();
    if (elmCursor)
    {
        CCLog("type value: %d",atoi(elmCursor->Attribute("value")));
        
        this->setType(atoi(elmCursor->Attribute("value")));
    }
    
    //是否领取
    elmCursor = elmCursor->NextSiblingElement();
    this->setIsGet(atoi(elmCursor->FirstChild()->Value()) == 1 ? true : false);

    
    //图标
    elmCursor = elmCursor->NextSiblingElement();
    
    if (elmCursor)
    {
        if (elmCursor->Attribute("img"))
        {
            this->setIconID(elmCursor->Attribute("img"));
        }
        
        if (elmCursor->Attribute("num"))
        {
            this->setIconNo((elmCursor->Attribute("num")));
        }
    }
    
    //图标数字位置
    elmCursor = elmCursor->NextSiblingElement();
    
    if (elmCursor)
    {
        float pos_x = atof(elmCursor->Attribute("x"));
        
        float pos_y = atof(elmCursor->Attribute("y"));
        
        this->setNoPos(CCPoint(pos_x, pos_y));
    }

    
    return true;
}










