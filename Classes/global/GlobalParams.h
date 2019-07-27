//
//  GlobalParams.h
//  plane
//
//  Created by renyi on 14-01-12.
//
//

#ifndef __FERRIS_GAME__GlobalParams__
#define __FERRIS_GAME__GlobalParams__

#include "cocos2d.h"

#include "GlobalEnum.h"

USING_NS_CC;
USING_NS_STD;

#define NEWUUID GlobalParams::sharedGlobalParams()->getUUID()

class GlobalParams
{
private:
    static GlobalParams* s_SharedGlobalParams;
    void init();
public:
    
    GlobalParams();
    ~GlobalParams();
    
    static GlobalParams* sharedGlobalParams();
    
    //uuid
    int getUUID();
    
private:
    //uuid
    int global_uuid;
    
    //key->ObjectStatus value->FormName
    CCDictionary *dictFormNames;
};

#endif