#ifndef _LOADHELPER_H_
#define _LOADHELPER_H_

#include "cocos2d.h"
#include "UserData.h"
#include "FerrisTools.h"
#include "tinyxml.h"
#include "MyAnimationManager.h"

/*
	资源加载器，只加载本关所需要的资源文件
	create at 2014 01 17
*/
typedef std::vector<int>* EnemyVec;

class LoadHelper
{
public:
	//读取关卡资源，获取所需要的敌人飞机id
	static EnemyVec getNeedEnemyVecByLv(int n_lv);

	//根据飞机ID加载资源
	static void preloadCraftResByCraftID(int data_type, int craft_id);

	//清空所有加载的资源（动画帧和动画）
	static void clearAllRes();

	//根据条件加载所需要的资源
	static void preloadAllRes(int n_hero_id, int n_wing_id, int n_mode, int n_properytype, int n_lv, bool bLoad);

};

#endif