//
//  LightningBoltNode.h
//  HelloCpp
//
//  Created by renyi on 14/12/12.
//
//

#ifndef __HelloCpp__LightningBoltNode__
#define __HelloCpp__LightningBoltNode__

#include <stdio.h>
#include "cocos2d.h"
#include "LightningBoltSprite.h"

USING_NS_CC;
using namespace std;

class LightningBoltNode : public CCNode
{
    
    CC_SYNTHESIZE(CCPoint, m_oStartPoint, StartPoint);
    
    CC_SYNTHESIZE(CCPoint, m_oEndPoint, EndPoint);
    
public:
    friend class LightningBoltAction;
    
public:
    LightningBoltNode();
    
    virtual~LightningBoltNode(){}
    
    bool init(const CCPoint&start, const CCPoint&end);
    
    void setStartAndEnd(const CCPoint &start, const CCPoint &end);
    
    void updatePosition(const CCPoint &start, const CCPoint &end);
    
    bool getIsDrawDebug()const
    {
        assert(m_lbSprite->getIsDrawDebug()==m_isDrawDebug);
        assert(m_lbSprite2->getIsDrawDebug()==m_isDrawDebug);
        return m_isDrawDebug;
    }
    void setIsDrawDebug(bool value)
    {
        m_isDrawDebug=value;
        m_lbSprite->setIsDrawDebug(value);
        m_lbSprite2->setIsDrawDebug(value);
    }
    
protected:
    //CCPoint m_start;//parent space start point
    //CCPoint m_end;//parent space end point
    LightningBoltSprite* m_lbSprite;
    LightningBoltSprite* m_lbSprite2;
    bool m_isDrawDebug;
    
    CCSprite * lightningbullet;
    CCSprite * block;
};

#endif /* defined(__HelloCpp__LightningBoltNode__) */
