#ifndef _WEAPON_DATA_H_
#define _WEAPON_DATA_H_

#include "cocos2d.h"
#include "tinyxml.h"

#include "PathGroupData.h"
#include "FerrisTools.h"
#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class PathLibrary
{
public:
    PathLibrary(void);
	virtual ~PathLibrary(void);
    
    static PathLibrary* sharedPathLibrary(void);
    
    //初始化数据
    void init();

	//
	void preload();
    
    //
    CCObject* getPathGroupDataByID(int pg_id);
    CCObject* getPathGroupDataByID(string pg_id);
    
private:
	static PathLibrary* s_sharedPathLibrary;
    
    //===========武器数据组===========
    CC_SYNTHESIZE(CCDictionary*, path_groups, PathGroups);
};

#endif