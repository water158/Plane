//
//  ArcPathAction.h
//  ambition
//
//  Created by mini02 on 15/2/27.
//
//

#ifndef __ambition__ArcPathAction__
#define __ambition__ArcPathAction__

#include <stdio.h>

#include "cocos2d.h"

USING_NS_CC;

class ArcPathAction : public CCActionInterval
{
public:
    virtual ~ArcPathAction();
    
    static ArcPathAction* create(float duration, float r, float b_angle, float e_angle, bool is_rotate=true,bool is_anticlockwise = false);
    
protected:
    bool init(float duration, float r, float b_angle, float e_angle, bool is_rotate=true,bool is_anticlockwise = false);
    
    virtual void startWithTarget(CCNode* pTarget);
    
    virtual void update(float dt);
    
private:
    ArcPathAction();
    
    //CCPoint m_endPosition;
    CCPoint m_startPosition;
    //CCPoint m_oldPosition;
    CCPoint m_centerPosition;
    
    //半径
    float radius;
    
    //起始角度
    float angleBegin;
    
    //结束角度
    float angleEnd;
    
    float angleDelta;
    
    float angleRotationArmBegin;
    
    float angleRotationArmEnd;
    
    bool m_bIsRotate;
    
    bool _anticlockwise;//是否是逆时针
    
};

#endif /* defined(__ambition__ArcPathAction__) */
