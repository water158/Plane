//
//  TestTimeData.cpp
//  phoenix
//
//  Created by mini01 on 14-10-13.
//
//

#include "TestTimeData.h"


TestTimeData::TestTimeData()
{

}

TestTimeData::~TestTimeData()
{

}

TestTimeData* TestTimeData::create(TiXmlElement *elm)
{
    TestTimeData *data = new TestTimeData();
    
    if (data && data->loadData(elm))
    {
        data->autorelease();
        return data;
    }
    
    return NULL;
}

bool TestTimeData::loadData(TiXmlElement *elm)
{
    
    return true;
}