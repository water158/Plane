//
//  PaymentIOS.cpp
//  HelloCpp
//
//  Created by mini01 on 14-10-7.
//
//

#include "PaymentIOS.h"
#include "StoreKitManager.h"


PaymentIOS* PaymentIOS::s_shareManager = NULL;

PaymentIOS* PaymentIOS::shared()
{
    if (!s_shareManager)
    {
        s_shareManager = new PaymentIOS();
        
    }
    
    return s_shareManager;
}

PaymentIOS::PaymentIOS()
{

}

PaymentIOS::~PaymentIOS()
{

}

void PaymentIOS::buy(int product_id)
{
    const char* pay_code = getPayCodeByProductID(product_id);
    
    NSString* pay_code_utf8 = [NSString stringWithCString: pay_code encoding:NSUTF8StringEncoding];
    
    [[StoreKitManager getInstance]purchaseItem:pay_code_utf8];
    
    [pay_code_utf8 release];
}