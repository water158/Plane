#include "MiscDefination.h"

/**********************************************
 * PAYMENT
 **********************************************/

const char* getPayCodeByProductID(int product_id)
{
    const char* paycode = "";
    
    switch (product_id)
    {
        case BUY_PRODUCT_1:
            paycode = PAYCODE_PRODUCT_1;
            break;
        case BUY_PRODUCT_2:
            paycode = PAYCODE_PRODUCT_2;
            break;
        case BUY_PRODUCT_3:
            paycode = PAYCODE_PRODUCT_3;
            break;
        case BUY_PRODUCT_4:
            paycode = PAYCODE_PRODUCT_4;
            break;
        case BUY_PRODUCT_5:
            paycode = PAYCODE_PRODUCT_5;
            break;
    }
    
    return paycode;
}


//返回花费的RMB
int getPriceByProductID(int product_id)
{
    int price = 0;
    
    switch (product_id)
    {
        case BUY_PRODUCT_1:
            price = PRICE_PRODUCT_1;
            break;
        case BUY_PRODUCT_2:
            price = PRICE_PRODUCT_2;
            break;
        case BUY_PRODUCT_3:
            price = PRICE_PRODUCT_3;
            break;
        case BUY_PRODUCT_4:
            price = PRICE_PRODUCT_4;
            break;
        case BUY_PRODUCT_5:
            price = PRICE_PRODUCT_5;
            break;
    }
    
    return price;
}

//返回订单的名称
const char* getPayOrderNameByProductID(int product_id)
{
    const char* name = "";
    
    switch (product_id)
    {
        case BUY_PRODUCT_1:
            name = ORDERNAME_PRODUCT_1;
            break;
        case BUY_PRODUCT_2:
            name = ORDERNAME_PRODUCT_2;
            break;
        case BUY_PRODUCT_3:
            name = ORDERNAME_PRODUCT_3;
            break;
        case BUY_PRODUCT_4:
            name = ORDERNAME_PRODUCT_4;
            break;
        case BUY_PRODUCT_5:
            name = ORDERNAME_PRODUCT_5;
            break;
    }
    
    return name;
}


//返回获得的钻石
int getGemNumByProductID(int product_id)
{
    int gem_num = 0;
    
    switch (product_id)
    {
        case BUY_PRODUCT_1:
            gem_num = QUANTITY_PRODUCT_1;
            break;
        case BUY_PRODUCT_2:
            gem_num = QUANTITY_PRODUCT_2;
            break;
        case BUY_PRODUCT_3:
            gem_num = QUANTITY_PRODUCT_3;
            break;
        case BUY_PRODUCT_4:
            gem_num = QUANTITY_PRODUCT_4;
            break;
        case BUY_PRODUCT_5:
            gem_num = QUANTITY_PRODUCT_5;
            break;
    }
    
    return gem_num;
};

/**********************************************
 * UMENG
 **********************************************/