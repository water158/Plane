//
//  BaseReward.h
//  ambition
//
//  Created by mini01 on 15-1-29.
//
//

#ifndef __ambition__BaseReward__
#define __ambition__BaseReward__

#include "cocos2d.h"
#include "tinyxml.h"
#include "GlobalEnum.h"

USING_NS_CC;

using namespace std;

class BaseReward : public CCNode
{
public:
    BaseReward();
    ~BaseReward();
    
    static BaseReward* create(TiXmlElement* elm);
    
    bool init(TiXmlElement* elm);
    
    CC_SYNTHESIZE(string, m_sID, ID);
    
    CC_SYNTHESIZE(SignRewardType, m_tTypeReward, TypeReward);
    
    CC_SYNTHESIZE(int, m_nTagReward, TagReward);
    
    CC_SYNTHESIZE(int, m_nValue, Value);
    
    CC_SYNTHESIZE(string, m_sImgID, ImgID);
};

#endif /* defined(__ambition__BaseReward__) */
