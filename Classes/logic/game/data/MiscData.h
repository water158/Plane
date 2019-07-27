//
//  MiscData.h
//  ambition
//
//  Created by mini02 on 14/11/28.
//
//

#ifndef __ambition__MiscData__
#define __ambition__MiscData__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"

#include "FerrisTools.h"


#include <stdio.h>

USING_NS_CC;
using namespace std;

class MiscData : public CCObject
{
public:
    virtual ~MiscData();
    
    static MiscData* create(TiXmlElement *elm);
    
    bool loadData(TiXmlElement *elm);
    
private:
    MiscData();
    
    //血量
    CC_SYNTHESIZE(float, m_healdth, Healdth);
    
    //子弹拖尾
    CC_SYNTHESIZE(int, m_nStreakType, StreakType);
    
    //子弹拖尾粒子
    CC_SYNTHESIZE(int, m_nStreakParticleType, StreakParticleType);
    
    //子弹集中效果类型
    CC_SYNTHESIZE(int, m_nBulletHitType, BulletHitType);
    
    //子弹发射效果类型
    CC_SYNTHESIZE(int, m_nBylletShootType, BulletShootType);
    
    //子弹是否自转
    CC_SYNTHESIZE(bool, m_isBulletSelfRotate, IsBulletSelfRotate);
    
    //子弹自转360时间
    CC_SYNTHESIZE(float, m_fBulletSelfRotateTime, BulletSelfRotateTime);
};

#endif /* defined(__ambition__MiscData__) */
