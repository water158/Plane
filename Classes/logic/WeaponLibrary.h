#ifndef _WEAPON_LIBRARY_H_
#define _WEAPON_LIBRARY_H_

#include "cocos2d.h"
#include "tinyxml.h"

#include "FerrisTools.h"
#include "GlobalEnum.h"

#include "GunGroupData.h"


USING_NS_CC;
using namespace std;

class WeaponLibrary
{
public:
    WeaponLibrary(void);
	virtual ~WeaponLibrary(void);
    
    static WeaponLibrary* sharedWeaponLibrary(void);
    
    //初始化数据
    void init();

	//
	void preload();
    
    //
    CCObject* getGunGroupDataByID(int gg_id);
    CCObject* getGunGroupDataByID(string gg_id);
    
private:
	static WeaponLibrary* s_sharedWeaponLibrary;
    
    //===========武器数据组===========
    CC_SYNTHESIZE(CCDictionary*, groups, Groups);
};

#endif