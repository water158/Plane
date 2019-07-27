//
//  StorePriceData.h
//  ambition
//
//  Created by mini01 on 15-1-7.
//
//

#ifndef __ambition__StorePriceData__
#define __ambition__StorePriceData__

#include "cocos2d.h"
#include "tinyxml.h"

USING_NS_CC;

using namespace std;

class PriceData : public CCNode
{
public:
    
    PriceData();
    ~PriceData();
    
    virtual bool init(TiXmlElement* elm);
    
    static PriceData* create(TiXmlElement* elm);
    
    CC_SYNTHESIZE(string, m_id, ID);
    CC_SYNTHESIZE(int, m_value, Value);
    CC_SYNTHESIZE(int, m_consume, Consume);
    CC_SYNTHESIZE(string, m_desLabel, DesLabel);
    
};

class PriceDataManager
{
public:
    
    static PriceDataManager* shared();
    
    PriceData* getPriceData(string data_id);
    
private:
    PriceDataManager();
    ~PriceDataManager();
    
    void init();
    
    static PriceDataManager* m_shared;
    
    CCDictionary* m_dcit;
};

#endif /* defined(__ambition__StorePriceData__) */
