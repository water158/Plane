//
//  PaymentIOS.h
//  HelloCpp
//
//  Created by mini01 on 14-10-7.
//
//

#ifndef __FERRIS_GAME__PaymentIOS__
#define __FERRIS_GAME__PaymentIOS__

#include "cocos2d.h"
#include "MiscDefination.h"


USING_NS_CC;

class PaymentIOS
{
public:
    
    static PaymentIOS* shared();
    
    PaymentIOS();
    ~PaymentIOS();
    
    void buy(int product_id);
    
private:
    
    static PaymentIOS* s_shareManager;
};

#endif /* defined(__HelloCpp__PaymentIOS__) */
