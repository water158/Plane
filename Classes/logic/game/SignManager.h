//
//  SignManager.h
//  ambition
//
//  Created by mini01 on 14-11-26.
//
//

#ifndef __ambition__SignManager__
#define __ambition__SignManager__

#include "cocos2d.h"
#include "UserData.h"
#include "GlobalEnum.h"
#include "tinyxml.h"

#include "FerrisTools.h"

USING_NS_CC;
using namespace std;

class SignReward : public CCNode
{
public:
    SignReward();
    ~SignReward();
    
    static SignReward* create(TiXmlElement* elm);
    
    bool init(TiXmlElement* elm);
    
    CC_SYNTHESIZE(string, m_sID, ID);
    CC_SYNTHESIZE(SignRewardType, m_tTypeReward, TypeReward);
    CC_SYNTHESIZE(int, m_nValue, Value);
    CC_SYNTHESIZE(string, m_sLabel, StrLabel);
    CC_SYNTHESIZE(string, m_sImgID, ImgID);
    
};

class SignManager
{
public:
    SignManager();
    ~SignManager();
    
    static SignManager* shared();
    
private:
    
    static SignManager* m_sharedInstance;
    
    virtual bool init();
    
    void loadRewardData(TiXmlElement* elm);
    
    CC_SYNTHESIZE(CCArray*, m_rewardArr, RewardArr);
    
    //根据更新周期初始化数组
    void initRewardArr();
};

#endif /* defined(__ambition__SignManager__) */
