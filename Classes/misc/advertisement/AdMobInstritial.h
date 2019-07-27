//
//  AdMobInstritial.h
//  RockPaperScissors
//
//  Created by mini01 on 14-7-31.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "GADInterstitial.h"
#import "GADInterstitialDelegate.h"

@interface InterAdmobViewController : UIViewController<GADInterstitialDelegate>

@property(nonatomic, retain) GADInterstitial *interstitial;

@end
