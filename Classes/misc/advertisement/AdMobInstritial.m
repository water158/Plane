//
//  AdMobInstritial.m
//  RockPaperScissors
//
//  Created by mini01 on 14-7-31.
//
//

#import "AdMobInstritial.h"

@implementation InterAdmobViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.interstitial = [[GADInterstitial alloc] init];
    
    self.interstitial.delegate = self;
    
    self.interstitial.adUnitID = @"ca-app-pub-4305631875686357/9062669688";
    
    [self.interstitial loadRequest: [self createRequest]];
}

- (GADRequest *)createRequest {
    GADRequest *request = [GADRequest request];
    
    // Make the request for a test ad. Put in an identifier for the simulator as
    // well as any devices you want to receive test ads.
    request.testDevices =
    [NSArray arrayWithObjects:
     // TODO: Add your device/simulator test identifiers here. They are
     // printed to the console when the app is launched.
     nil];
    return request;
}

- (void)interstitialDidReceiveAd:(GADInterstitial *)interstitial {
    [interstitial presentFromRootViewController:self];
}

@end
