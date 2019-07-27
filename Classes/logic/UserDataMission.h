#ifndef _USERDATAMISSION_H_
#define _USERDATAMISSION_H_

#include "cocos2d.h"
#include "tinyxml.h"

USING_NS_CC;
using namespace std;

class UserDataMission
{
public:
    UserDataMission(void);
	virtual ~UserDataMission(void);
    
    //加载自身的数据
	void loadData(TiXmlElement *elm);
    
private:
	
    CC_SYNTHESIZE (int, id, ID);
    CC_SYNTHESIZE (bool, enable, Enable);
    CC_SYNTHESIZE (string, score, Score);
    CC_SYNTHESIZE (int, grade, Grade);
};

#endif