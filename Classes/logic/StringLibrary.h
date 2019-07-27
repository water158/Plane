#ifndef __FERRIS_GAME__StringLibrary__
#define __FERRIS_GAME__StringLibrary__

#include "cocos2d.h"
#include "tinyxml.h"
#include "GlobalEnum.h"

#include "FerrisTools.h"

USING_NS_CC;
USING_NS_STD;

class StringLibrary
{
public:
    StringLibrary();
	virtual ~StringLibrary();
    
    static StringLibrary* shared();
    
    //初始化数据
    void init();
    
	//
	void preload();

	//
    CCString* getStringByKey(string key);
    
private:
    static StringLibrary* s_sharedStringLibrary;
    
    CC_SYNTHESIZE(CCDictionary*, dict_strings, DictStrings);
};

#endif