//
//  FerrisDrawLine.h
//  ambition
//
//  Created by mini02 on 15/2/4.
//
//

#ifndef __ambition__FerrisDrawLine__
#define __ambition__FerrisDrawLine__

#include "cocos2d.h"

USING_NS_CC;

class FerrisDrawLine : public CCLayer
{
public:
    static FerrisDrawLine* create(CCPoint start,CCPoint end);
    
    bool init(CCPoint start,CCPoint end);
    
    virtual void draw();
    
private:
    
    //画线点
    CC_SYNTHESIZE(CCPoint, m_startLinePoint, StartLinePoint);
    
    CC_SYNTHESIZE(CCPoint, m_endLinePoint, EndLinePoint);
};

#endif /* defined(__ambition__FerrisDrawLine__) */
