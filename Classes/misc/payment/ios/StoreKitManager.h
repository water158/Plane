
#import <GameKit/GameKit.h>
#import <StoreKit/StoreKit.h>

@interface StoreKitManager : NSObject<GKGameCenterControllerDelegate, SKPaymentTransactionObserver, SKProductsRequestDelegate>
{
    UIAlertView *_loadingAlert;
    BOOL _enableGameCenter;
}

+ (StoreKitManager *)getInstance;

//初始化内消费
- (void)initStoreKit;

//购买产品
- (void)purchaseItem: (NSString*)identifier;
@end
