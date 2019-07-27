//
//  HelloCppAppController.h
//  HelloCpp
//
//  Created by mini01 on 14-8-12.
//  Copyright __MyCompanyName__ 2014年. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GADInterstitial.h"

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate, GADInterstitialDelegate>
{
    UIWindow            *window;
    RootViewController  *viewController;
    GADInterstitial     *_interstitial;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;
@property (nonatomic, retain) GADInterstitial *interstitial;

- (void)showInterstitial;

- (void)preloadRequest;

@end