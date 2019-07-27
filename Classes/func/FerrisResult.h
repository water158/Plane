//
//  FerrisDialogResult.h
//  plane
//
//  Created by renyi on 14-01-12.
//
//

#ifndef __FERRIS_GAME__FerrisDialogResult__
#define __FERRIS_GAME__FerrisDialogResult__

#include "cocos2d.h"
#include "GlobalEnum.h"

USING_NS_CC;
USING_NS_STD;

class FerrisResult  : public CCObject
{
    CC_SYNTHESIZE(bool, m_bResult, Result);
    CC_SYNTHESIZE(int, m_nAmount, Amount);
    
public:
    
    static FerrisResult* create()
    {
        FerrisResult* retsult = new FerrisResult();
        
        if(retsult)
        {
            retsult->autorelease();
            return retsult;
        }
        return NULL;
    }
    
    FerrisResult()
    {
        CCLog("FerrisResult Created");
        
        m_bResult = false;
        m_nAmount = 0;
    };
    
    ~FerrisResult()
    {
        CCLog("FerrisResult Destroyed");
    };
};

#endif