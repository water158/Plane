#ifndef _DATAHELPER_H_
#define _DATAHELPER_H_

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "CraftData.h"
#include "GlobalCache.h"
#include "BulletData.h"
#include "TargetPlaneData.h"
#include "DropData.h"
/****************************************
	用来管理所有全局数据（加载、获取、释放等操作）
	单例模式
*****************************************/

class DataManager
{
public:
	DataManager();
	~DataManager();

	static DataManager* sharedDataManager();

	//初始化
	bool init();

/**********加载区********/
	//飞机数据（主机、敌机）
	void loadCraftData(int data_type, int craft_id);

	//加载子弹数据
	void loadAllBulletData();
    
    //加载所有敌机数据
    void loadAllEnemyData();
    
    //加载所有炮管数据
    void loadAllConnonData();
    
    //加载所有targetplane数据
    void loadAllTargetPlaneData();
    
    //加载所有targetConnon数据
    void loadAllTargetConnonData();
    
    //加载所有道具的数据
    void loadAllToolsData();
    
    //加载所有掉落数据
    void loadDropData(int craft_id);

/**********获取区********/
	//获取飞机数据
	CraftData* getCraftData(int data_type, int craft_id);

	//获取子弹数据
	BulletData* getBulletData(int bullet_id);
    
    //获取targetPlane
    TargetPlaneData* getTargetPlane(string plane_id);
    
    //获取targetConnon
    TargetPlaneData* getTargetConnon(string connon_id);
    
    //获取targetTools
    TargetPlaneData* getTargetTool(string tool_id);
    
    //获取掉落数据
    DropData* getDropData(int drop_id);

/**********释放区********/


private:
	static DataManager* m_pSharedDataManager;
    
    //保存targetplane的字典
    CCDictionary* m_dicTarplaneData;

    //保存所欲的炮台数据
    CCDictionary* m_dicTarConnonData;
    
    //保存所有道具的字典
    CCDictionary* m_dicTarToolsData;
};

#endif