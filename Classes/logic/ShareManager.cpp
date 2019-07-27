//
//  ShareManager.cpp
//  HelloCpp
//
//  Created by mini01 on 14-10-7.
//
//

#include "ShareManager.h"

ShareManager* ShareManager::s_sharedManager = NULL;

/*
 * 分享回调, 该回调不是某个类的成员函数， 而是一个普通的函数, 具体使用参考HelloWorldScene的例子
 * @param platform 要分享到的目标平台
 * @param stCode 返回码, 200代表分享成功, 100代表开始分享
 * @param errorMsg 分享失败时的错误信息,android平台没有错误信息
 */
void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state)
    {
        case C2DXResponseStateSuccess:
            CCLog("分享成功");
            break;
        case C2DXResponseStateFail:
            CCLog("分享失败");
            break;
        default:
            break;
    }
}

ShareManager::ShareManager()
{

}

ShareManager::~ShareManager()
{

}

bool ShareManager::init()
{
    open();
    
    initPlatforms();
    
    return true;
}

ShareManager* ShareManager::sharedManager()
{
    if (!s_sharedManager)
    {
        s_sharedManager = new ShareManager();
        
        s_sharedManager->init();
    }
    
    return s_sharedManager;
}

void ShareManager::callShare(FerrisShareType type, string url_param)
{
    CCString* title = NULL;
    std::string url = SHARE_TARGET_URL;
    
    switch(type)
    {
        case FerrisShareTopic:
            title = CCString::createWithFormat("大哥，大嫂这题怎么做啊，来帮帮我吧！");
            url.append("help?words=").append(url_param);
            break;
            
        case FerrisShareAchieve:
            title = CCString::createWithFormat("看看小爷的成绩，你也来试试！");
            url.append(url_param);
            break;
            
        case FerrisShareAccount1:
            title = CCString::createWithFormat("看我多牛X，这种题都能答对，你行吗！");
            url.append("win?words=").append(url_param);
            break;
            
        case FerrisShareAccount0:
            title = CCString::createWithFormat("大哥，大嫂这题怎么做啊，来帮帮我吧！");
            url.append("help?words=").append(url_param);
            break;
            
        default:
            break;
    }
    
    CCSprite* sp_path = CCSprite::create("word/cang.jpg");;
    CCPoint p = sp_path->getAnchorPoint();
    sp_path->setAnchorPoint(ccp(0, 0));
    CCRenderTexture* render = CCRenderTexture::create(sp_path->getContentSize().width, sp_path->getContentSize().height);
    
    render->begin();
    sp_path->visit();
    render->end();
    sp_path->setAnchorPoint(p);
    
    bool success = render->saveToFile("screenshot.jpg", kCCImageFormatJPEG);
    
    if(!success)
    {
        return;
    }
    
    std::string fullpath = CCFileUtils::sharedFileUtils()->getWritablePath() + "screenshot.jpg";
    
    CCLOG("url == %s", url.c_str());
    
    CCDictionary *dicContent = CCDictionary::create();
    dicContent-> setObject(CCString::createWithFormat("%d", C2DXContentTypeImage), "type");
    dicContent-> setObject(CCString::create(""), "content");
    dicContent-> setObject(CCString::create(title->getCString()), "title");
    dicContent-> setObject(CCString::create(fullpath.c_str()), "image");
    dicContent-> setObject(CCString::create(url.c_str()), "url");
    dicContent-> setObject(CCString::create(url.c_str()), "siteUrl");
    dicContent-> setObject(CCString::create("反了反了"), "site");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    C2DXShareSDK::showShareMenu(NULL, dicContent, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    if(getDeviceType() == 1)
    {
        C2DXShareSDK::showShareMenu(NULL,
                                    dicContent,
                                    ccp(_s_center.x, _s_center.y + 160),
                                    C2DXMenuArrowDirectionDown,
                                    shareResultHandler);
    }
    else
    {
        C2DXShareSDK::showShareMenu(NULL,
                                    dicContent,
                                    ccp(100, 100),
                                    C2DXMenuArrowDirectionLeft,
                                    shareResultHandler);
    }
    
#endif
}

void ShareManager::open()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

    //初始化ShareSDK
    C2DXShareSDK::open(CCString::create(ID_SHARESDK), false);

#endif
}

void ShareManager::initPlatforms()
{
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    
    //QQ空间
    CCDictionary *qzConfigDict = CCDictionary::create();
    qzConfigDict -> setObject(CCString::create(ID_SHARE_QQ_APPID), "app_id");
    qzConfigDict -> setObject(CCString::create(ID_SHARE_QQ_KEY), "app_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQZone, qzConfigDict);
    
    //微信
    CCDictionary *wcConfigDict = CCDictionary::create();
    wcConfigDict -> setObject(CCString::create(ID_SHARE_WEIXIN_ID), "app_id");
    wcConfigDict -> setObject(CCString::create(ID_SHARE_WEIXIN_SECRET), "app_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);
    
    //QQ
    CCDictionary *qqConfigDict = CCDictionary::create();
    qqConfigDict -> setObject(CCString::create(ID_SHARE_QQ_APPID), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQQ, qqConfigDict);
    
    //短信
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSMS, NULL);
    
    //邮件
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeMail, NULL);

#endif
}

