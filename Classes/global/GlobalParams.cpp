//
//  GlobalParams.cpp
//  plane
//
//  Created by renyi on 14-01-12.
//
//

#include "GlobalParams.h"

GlobalParams* GlobalParams::s_SharedGlobalParams = NULL;

GlobalParams::GlobalParams()
{
    
}

GlobalParams::~GlobalParams()
{
    //CCLog("~GlobalParams");
    
    CC_SAFE_RELEASE(dictFormNames);
    
    if (s_SharedGlobalParams)
    {
        delete s_SharedGlobalParams;
    }
}

GlobalParams* GlobalParams::sharedGlobalParams()
{
    if (!s_SharedGlobalParams)
    {
        s_SharedGlobalParams = new GlobalParams();
        s_SharedGlobalParams->init();
    }
    
    return s_SharedGlobalParams;
}

void GlobalParams::init()
{
    global_uuid = 0;
    
	dictFormNames = CCDictionary::create();
	dictFormNames->retain();
}

int GlobalParams::getUUID()
{
    return ++global_uuid;
}

