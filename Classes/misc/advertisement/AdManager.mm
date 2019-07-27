




#import "AdManager.h"
#import <Foundation/Foundation.h>
#import "UserData.h"
#import "AdMobInstritial.h"
#import "AppController.h"

AdManager *AdManager::shared()
{
    static AdManager *manager = NULL;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = new AdManager;
    });
    return manager;
}

void AdManager::preloadAdmobInterstitial()
{
    [(AppController*)[UIApplication sharedApplication].delegate preloadRequest];
}


void AdManager::showAdmobInterstitial()
{
    [(AppController*)[UIApplication sharedApplication].delegate showInterstitial];
}

AdManager::AdManager()
{

}

AdManager::~AdManager()
{

}

/**************************
 触控广告条
 **************************/
void AdManager::showCoCoBanner()
{

}

/**************************
 触控插入广告
 **************************/
void AdManager::showCoCoInterstitial()
{

}



void AdManager::showRandomBanner()
{
    int rand = arc4random() % 2;
    
    if (rand == 0)
    {
        showCoCoBanner();
    }
    else if (rand == 1)
    {

    }
}

void AdManager::showRandomInterstitial()
{
    int rand = arc4random() % 2;
    
    if (rand == 0)
    {
        showCoCoInterstitial();
    }
    else if (rand == 1)
    {

    }
}
