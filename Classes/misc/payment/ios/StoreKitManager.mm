
#import "StoreKitManager.h"
#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>
#import <StoreKit/StoreKit.h>
#import "ASIHTTPRequest.h"
#import "ASIFormDataRequest.h"
#import "GTMBase64.h"

#import "UserData.h"
#import "GlobalEnum.h"
#import "MobClickCpp.h"
#import "VerificationManager.h"

@implementation StoreKitManager

+ (StoreKitManager *)getInstance
{
    static StoreKitManager *mgr = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        mgr = [[self alloc] init];
    });
    return mgr;
}

- (id)init
{
    self = [super init];
    if (self) {
        [self initData];
        [self initStoreKit];
        return self;
    }
    return nil;
}

- (void)initData
{
    _enableGameCenter = NO;
}

//---------------------------------------------------------
#pragma mark - IAP
- (BOOL)canProcessPayments
{
    if ([SKPaymentQueue canMakePayments]) {
        return YES;
    } else {
        return NO;
    }
}


/**
 初始化内消费
 */
- (void)initStoreKit
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

/**
 购买产品
 */
- (void)purchaseItem: (NSString *)identifier
{
    //[self showLoadingView:@"Access Store..."];
    
    if (![self canProcessPayments]) {
        NSLog(@"1.失败-->SKPaymentQueue canMakePayments NO");
        //[self removeLoadingView];
        return;
    }
    NSLog(@"1.成功-->请求产品信息...%@", identifier);
    
    // 使用请求商品信息式购买
    SKProductsRequest *request= [[SKProductsRequest alloc]
                                 initWithProductIdentifiers: [NSSet setWithObject: identifier]];
    request.delegate = self;
    [request start];
}

// SKProductsRequest 的回调
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray *myProduct = response.products;
    
    if (myProduct.count == 0) {
        NSLog(@"2.失败-->无法获取产品信息，购买失败。invalidProductIdentifiers = %@",response.invalidProductIdentifiers);
        //[self removeLoadingView];
        return;
    }
    NSLog(@"2.成功-->获取产品信息成功，正在购买...");
    SKPayment * payment = [SKPayment paymentWithProduct:myProduct[0]];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

// SKPayment 的回调
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    NSLog(@"3.成功-->接收苹果购买数据，正在处理...");
    for (SKPaymentTransaction *transaction in transactions)
    {
        NSLog(@"3.成功-->接收苹果购买数据，正在处理... for");
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                NSLog(@"3.成功-->接收苹果购买数据，正在处理...SKPaymentTransactionStatePurchased");
                [self completeTransaction:transaction];
                break;
                
            case SKPaymentTransactionStateFailed:
                NSLog(@"3.成功-->接收苹果购买数据，正在处理...SKPaymentTransactionStateFailed");
                [self failedTransaction:transaction];
                break;
                
            case SKPaymentTransactionStateRestored:
                NSLog(@"3.成功-->接收苹果购买数据，正在处理...SKPaymentTransactionStateRestored");
                [self restoreTransaction:transaction];
                break;
            case SKPaymentTransactionStatePurchasing:
                NSLog(@"3.商品添加进列表");
                break;
                
            default:
                break;
        }
    }
}

// 结束交易
- (void) completeTransaction: (SKPaymentTransaction*)transaction
{
    //[self removeLoadingView];
	// 记录交易和提供产品 这两方法必须处理
    if ([self verifyReceipt: transaction.transactionReceipt])
    {
        NSLog(@"4.成功-->结束交易 SKPaymentTransactionStatePurchased");
        [self recordTransaction: transaction];
        [self provideContent: transaction.payment.productIdentifier];
    }
    else
    {
        UIAlertView *alerView2 =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                             message:@"交易数据异常，购买失败"
                                                            delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
        
        [alerView2 show];
        [alerView2 release];
    }
    
    // 移除 transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    //VerificationManager::shared()->ok();
}

// 交易失败
- (void) failedTransaction: (SKPaymentTransaction*)transaction
{
    //[self removeLoadingView];
    NSLog(@"4.成功-->交易失败 SKPaymentTransactionStateRestored error.code:%@",transaction.error);
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

// 重置交易
- (void) restoreTransaction: (SKPaymentTransaction*)transaction
{
    NSLog(@"4.成功-->重置交易 SKPaymentTransactionStateRestored");
    [self recordTransaction: transaction];
    [self provideContent: transaction.originalTransaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (BOOL) verifyReceipt:(NSData*)receipt
{
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"%s/IAPAnti", SERVER_DOMAIN]];
    ASIFormDataRequest *request = [ASIFormDataRequest requestWithURL:url];
    [request setPostValue:[GTMBase64 stringByEncodingData:nil] forKey:@"data"];
#ifdef TEST_SANDBOX
    [request setPostValue:@"1" forKey:@"sandbox"];
#else
    [request setPostValue:@"0" forKey:@"sandbox"];
#endif
    
    [request startSynchronous];
    
    NSData* data = [request responseData];
    
    char *test = (char*)[data bytes];
    
    
    NSLog(@"%s",test);
    
    std::string final = test;
    
    
    NSError *err = [request error];
    if (err)
    {
        return  NO;
    }
    
    NSLog(@"%lu",final.find("ok"));
    if (final.find("ok") != -1)
    {
        return YES;
    }
    else if (final.find("invalid_response_data") != -1)
    {
        NSLog(@"%s","wrong value");
        return  NO;
    }
    else if (final.find("invalid_receipt") != -1)
    {
        NSLog(@"%s","ilegole value");
        return  NO;
    }
    
    return NO;
}

// 交易记录
- (void) recordTransaction: (SKPaymentTransaction*)transacation
{
    NSLog(@"4.成功-->交易记录, 可以在此处存储记录");
}

// 提供产品
- (void) provideContent: (NSString*)identifier
{
    NSLog(@"4.成功-->交易成功，请提供产品 identifier = %@", identifier);
    
    if ([identifier isEqualToString:@PAYCODE_PRODUCT_1])
    {
        //购买20金币成功
        UserData::shared()->increaseGoldNum(QUANTITY_PRODUCT_1);
        MobClickCpp::pay(6, 1, ANALYTICS_BUY_PRODUCT_1 , 1, 0);
    }
    else if ([identifier isEqualToString:@PAYCODE_PRODUCT_2])
    {
        //购买48金币成功
        UserData::shared()->increaseGoldNum(QUANTITY_PRODUCT_2);
        MobClickCpp::pay(12, 1, ANALYTICS_BUY_PRODUCT_2, 1, 0);
    }
    else if ([identifier isEqualToString:@PAYCODE_PRODUCT_3])
    {
        //购买84金币成功
        UserData::shared()->increaseGoldNum(QUANTITY_PRODUCT_3);
        MobClickCpp::pay(18, 1, ANALYTICS_BUY_PRODUCT_3, 1, 0);
    }
    else if ([identifier isEqualToString:@PAYCODE_PRODUCT_4])
    {
        //购买128金币成功
        UserData::shared()->increaseGoldNum(QUANTITY_PRODUCT_4);
        MobClickCpp::pay(25, 1, ANALYTICS_BUY_PRODUCT_4, 1, 0);
    }
    else if ([identifier isEqualToString:@PAYCODE_PRODUCT_5])
    {
        //购买180金币成功
        UserData::shared()->increaseGoldNum(QUANTITY_PRODUCT_5);
        MobClickCpp::pay(30, 1, ANALYTICS_BUY_PRODUCT_5, 1, 0);
    }
    
    UserData::shared()->save();
    
    //通知观察者更新金币
    CCNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_UI_4_GEM);
    //[self removeLoadingView];
    //[self showMessage:@"Success" Message:@"You have successfully purchased."];
}

@end