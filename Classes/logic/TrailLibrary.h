#ifndef _TRAILLIBRARY_H_
#define _TRAILLIBRARY_H_

#include "cocos2d.h"
#include "tinyxml.h"

#include "TrailGroupData.h"
#include "FerrisTools.h"
#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class TrailLibrary
{
public:
	TrailLibrary(void);
	virtual ~TrailLibrary(void);

	static TrailLibrary* sharedTrailLibrary(void);

	//初始化数据
	void init();

	//
	void preload();

	//
	CCObject* getTrailGroupDataByID(int pg_id);
	CCObject* getTrailGroupDataByID(string pg_id);

private:
	static TrailLibrary* s_sharedTrailLibrary;

	//===========武器数据组===========
	CC_SYNTHESIZE(CCDictionary*, trail_groups, TrailGroups);
};

#endif