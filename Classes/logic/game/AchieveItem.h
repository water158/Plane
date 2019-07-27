//
//  AchieveItem.h
//  phoenix
//
//  Created by mini01 on 14-11-6.
//
//

#ifndef __phoenix__AchieveItem__
#define __phoenix__AchieveItem__

#include "cocos2d.h"
#include "tinyxml.h"
#include "BaseReward.h"

#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class AchieveItem : public CCNode
{
public:
    virtual ~AchieveItem();
    
    static AchieveItem* create(TiXmlElement *elm);
    
    //加载成就信息
    bool loadData(TiXmlElement *elm);
    
private:
    AchieveItem();
    
    //成就信息
    //id
    CC_SYNTHESIZE(string, m_strID, ID);
    //name
    CC_SYNTHESIZE(string, m_strName, Name);
    //describe
    CC_SYNTHESIZE(string, m_strDescribe, Describe);
    
    //奖励1
    CC_SYNTHESIZE(BaseReward*, m_reward1, RewardFir);
    
    //奖励2
    CC_SYNTHESIZE(BaseReward*, m_reward2, RewardSec);
    
    //奖励
    CC_SYNTHESIZE(string, m_rewardDes, RewardDes);
    
    //达成要求
    CC_SYNTHESIZE(int, m_requestCount, RequestCount);
    
    //是否达成
    CC_SYNTHESIZE(bool, m_isFinish, IsFinish);
    
    //是否领取
    CC_SYNTHESIZE(bool, m_isGet, IsGet);
    
    //类型
    CC_SYNTHESIZE(int, m_type, Type);
    
    //图标id
    CC_SYNTHESIZE(string, m_iconID, IconID);
    
    //图标数字
    CC_SYNTHESIZE(string, m_iconNo, IconNo);
    
    //图标数字位置
    CC_SYNTHESIZE(CCPoint, m_NoPos, NoPos);
};


#endif /* defined(__phoenix__AchieveItem__) */
