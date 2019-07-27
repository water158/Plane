//
//  HelloCppAppController.mm
//  HelloCpp
//
//  Created by mini01 on 14-8-12.
//  Copyright __MyCompanyName__ 2014年. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "RootViewController.h"

//sharesdk
#import <ShareSDK/ShareSDK.h>
#import "WXApi.h"
#import <TencentOpenAPI/QQApiInterface.h>
#import <TencentOpenAPI/TencentOAuth.h>

//admob
#import "GADBannerView.h"
#import "GADRequest.h"

#import "GlobalEnum.h"

@implementation AppController

@synthesize window;
@synthesize viewController;
@synthesize interstitial = _interstitial;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.
    //shareSdk
    //导入微信类型
    [ShareSDK importWeChatClass:[WXApi class]];
    
    //导入QQ类型
    [ShareSDK importQQClass:[QQApiInterface class] tencentOAuthCls:[TencentOAuth class]];
    

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];

    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    
    [__glView setMultipleTouchEnabled:YES];
    
    //------------------- Add Admob
    GADBannerView * bannerView = [[GADBannerView alloc] initWithAdSize:kGADAdSizeSmartBannerPortrait];
    bannerView.rootViewController = viewController;
    bannerView.adUnitID = @ID_ADMOB_BANNER;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    /*
    [bannerView setFrame:CGRectMake(0, viewController.view.frame.size.height - bannerView.frame.size.height, bannerView.frame.size.width, bannerView.frame.size.height)];
    [viewController.view addSubview: bannerView];
    [viewController.view bringSubviewToFront:bannerView];
    
    GADRequest *request = [GADRequest request];
    request.testing = NO;
    
    [bannerView loadRequest: request];
    */
    
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden: YES];
    
    //GGBridge::shared()->view = viewController.view;
    //GGBridge::shared()->controller = viewController;
    //GGBridge::shared()->window = window;

    cocos2d::CCApplication::sharedApplication()->run();
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

//shareSdk
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    return [ShareSDK handleOpenURL:url sourceApplication:nil annotation:nil wxDelegate:nil];
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    return [ShareSDK handleOpenURL:url sourceApplication:sourceApplication annotation:annotation wxDelegate:nil];
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
     cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc
{
    
    _interstitial.delegate = nil;
    [_interstitial release];
    
    [window release];
    
    [super dealloc];
}

#pragma mark -
#pragma Interstitial Delegate

- (void)interstitial:(GADInterstitial *) interstitial didFailToReceiveAdWithError:(GADRequestError *)error
{
    
}

- (void) interstitialDidReceiveAd:(GADInterstitial *) interstitial
{
    
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)ad
{
    [self preloadRequest];
}

- (void)showInterstitial
{
    if (self.interstitial.isReady)
    {
        CCLOG("ready");
        [self.interstitial presentFromRootViewController:viewController];
    }
    else
    {
        CCLOG("not ready");
        [self preloadRequest];
    }
}

#pragma mark GADRequest generation
- (void)initInterstitial
{
    self.interstitial = [[[GADInterstitial alloc] init] autorelease];
    
    self.interstitial.delegate = self;
    
    self.interstitial.adUnitID = @ID_ADMOB_INSTRIRIAL;
}

- (void)preloadRequest
{
    CCLOG("pre load");
    [self initInterstitial];
    [self.interstitial loadRequest: [self createRequest]];
}

- (GADRequest*)createRequest
{
    GADRequest *request = [GADRequest request];
    
    request.testDevices = [NSArray arrayWithObjects: nil];
    
    return request;
}

@end

