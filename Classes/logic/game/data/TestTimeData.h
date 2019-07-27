//
//  TestTimeData.h
//  phoenix
//
//  Created by mini01 on 14-10-13.
//
//

#ifndef __phoenix__TestTimeData__
#define __phoenix__TestTimeData__

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"
#include "TimePlaneData.h"

USING_NS_CC;
using namespace std;

class TestTimeData : public CCObject
{
public:
    virtual ~TestTimeData();
    
    static TestTimeData* create(TiXmlElement *elm);
    
    //加载自身的数据
	bool loadData(TiXmlElement *elm);
private:
    
    TestTimeData();
    
    CC_SYNTHESIZE(CCArray*, time_planes, TimePlanes);
};


#endif /* defined(__phoenix__TestTimeData__) */
