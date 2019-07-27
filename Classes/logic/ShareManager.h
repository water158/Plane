//
//  ShareManager.h
//  HelloCpp
//
//  Created by mini01 on 14-10-7.
//
//

#ifndef __FERRIS_GAME__ShareManager__
#define __FERRIS_GAME__ShareManager__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "MiscDefination.h"
#include "SmartRes.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

#include "C2DXShareSDK.h"
using namespace cn::sharesdk;

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "FerrisToolsOC.h"

#endif

USING_NS_CC;
USING_NS_STD;

class ShareManager
{
public:
    static ShareManager* sharedManager();
    
    //调用分享 参数为图片路径
    void callShare(FerrisShareType type, string url_param);
    
private:
    static ShareManager* s_sharedManager;
    
    ShareManager();
    ~ShareManager();
    
    bool init();
    
    void open();
    
    void initPlatforms();
};

#endif /* defined(__HelloCpp__ShareManager__) */
