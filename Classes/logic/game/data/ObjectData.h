#ifndef __OBJECT_DATA_H__
#define __OBJECT_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

USING_NS_CC;
using namespace std;


class ObjectData : public CCObject
{
public:
    ObjectData();
	virtual ~ObjectData();
    
	//加载数据
	static bool loadData(TiXmlElement *elm);


   
};

#endif