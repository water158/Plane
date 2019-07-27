#ifndef __FERRIS_GAME__UserData__
#define __FERRIS_GAME__UserData__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "MiscDefination.h"
#include "tinyxml.h"

#include "FerrisTools.h"
#include "UserDataMission.h"
#include "EquipmentsData.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

//#include "MobClickCpp.h"
#include "AnalyticsStatID.h"

#endif

USING_NS_CC;
USING_NS_STD;

typedef map<int, UserDataMission> MapUserDataMession;

class UserData
{
    //======================
    // 属性
    //======================
    
    //版本
	CC_SYNTHESIZE(int, m_nVersion, Version);
    
    //体力
    CC_SYNTHESIZE(int, m_nEnergy, EnergyNum);
    
	//金币
	CC_SYNTHESIZE(int, m_nGoldNum, GoldNum);
    
    //代币，水晶
    CC_SYNTHESIZE(int, m_nGemNum, GemNum);
    
    //关卡信息
    CC_SYNTHESIZE(MapUserDataMession, m_mapUserMissions, UserMissions);
    
    //用户情况
    CC_SYNTHESIZE(bool, m_bFirstTime, FirstTime);
    CC_SYNTHESIZE(bool, m_bFirstShop, FirstShop);
    CC_SYNTHESIZE(bool, m_bFinishComplain, FinishComplain);
    
    CC_SYNTHESIZE(bool, m_bIsFirstLotteryUseCoin, IsFirstLotteryUseCoin);
    CC_SYNTHESIZE(bool, m_bIsFirstLotteryUseTenCrystal, IsFirstLotteryUseTenCrystal);
    CC_SYNTHESIZE(bool, m_bIsFirstLotteryUseFourtyCrystal, IsFirstLotteryUseFourtyCrystal);
    CC_SYNTHESIZE(int, m_bLastFreeTimeForCoin, LastFreeTimeForCoin);
    CC_SYNTHESIZE(int, m_bLastFreeTimeForTenCrystal, LastFreeTimeForTenCrystal);
    CC_SYNTHESIZE(int, m_bLastFreeTimeForFourtyCrystal, LastFreeTimeForFourtyCrystal);
    
    //时间记录 用于体力恢复
    CC_SYNTHESIZE(int, m_timeRecord, TimeRecord);
    
    //第一次登陆的时间 精确到毫秒
    CC_SYNTHESIZE(long, m_firstTimeRecord, FirstTimeRecord);
    
    //第一次登录游戏的时间 精确到年月日
    CC_SYNTHESIZE(int, m_lastLoginYear, LastLoginYear);
    CC_SYNTHESIZE(int, m_lastLoginMon, LastLoginMon);
    CC_SYNTHESIZE(int, m_lastLoginDay, LastLoginDay);
    
    //上次日常任务刷新的时间 精确到年月日
    CC_SYNTHESIZE(int, m_lastDailyRefreshYear, LastDailyRefreshYear);
    CC_SYNTHESIZE(int, m_lastDailyRefreshMon, LastDailyRefreshMon);
    CC_SYNTHESIZE(int, m_lastDailyRefreshDay, LastDailyRefreshDay);
    
    //记录已签到的天数
    CC_SYNTHESIZE(int, m_hadeSigned, HadeSigned);
    
    //记录当前使用的签到奖励表编号
    CC_SYNTHESIZE(int, m_curSignReward, CurSignReward);
    
    //签到奖励的更新记录
    CC_SYNTHESIZE(int, m_signRewardUpdateRecord, SignRewardUpdateRecord);
    
    //游戏设定
	CC_SYNTHESIZE(bool, m_bSoundEnable, SoundEnable);
	CC_SYNTHESIZE(bool, m_bMusicEnable, MusicEnable);
	CC_SYNTHESIZE(bool, m_bVibraEnable, VibraEnable);
    CC_SYNTHESIZE(bool, m_bParticleEnable, ParticleEnable);
    
    //用户装备数据
    CC_SYNTHESIZE(EquipmentsData*, m_pEquipmentsData, EquipmentsData);
    
    //===========选择的主炮===========
    CC_SYNTHESIZE(int, m_nSelectedMainGun, SelectedMainGun);
    
    //===========选择的左副炮===========
    CC_SYNTHESIZE(int, m_nSelectedLeftGun, SelectedLeftGun);
    
    //===========选择的右副炮===========
    CC_SYNTHESIZE(int, m_nSelectedRightGun, SelectedRightGun);
    
    //===========选择的僚机===========
    CC_SYNTHESIZE(int, m_nSelectedWing, SelectedWing);
    
public:
	UserData(void);
	virtual ~UserData(void);

	static UserData* shared(void);

	//初始化数据
	void init();

	void check();

	//保存数据
	void save();

	//加载
	void load();

	//检测userdata版本
	void checkVersion();

	//根据userdata版本加载
	void loadDataFromVer(int version);

	//路径
	string m_sUserDataPath;
    
    //===========体力===========
    //增加体力
    void increaseEnergy(int energy);
    
    //减少体力
    void decreaseEnergy(int energy);

	//===========金币===========
	//钱是否够
	bool isGoldEnough(int gold);

	//增加钱
	void increaseGoldNum(int gold);

	//减少钱
	void decreaseGoldNum(int gold);

	//===========水晶===========
	//水晶是否够
	bool isGemEnough(int Gem);

	//增加水晶
	void increaseGemNum(int Gem);

	//减少水晶
	void decreaseGemNum(int Gem);
    
    //
    void increaseGemNumByProductID(int product_id);
    
    //===========道具============
    //道具1
    CC_SYNTHESIZE(int, m_tool_1, NoTool_1);
    
    //道具2
    CC_SYNTHESIZE(int, m_tool_2, NoTool_2);
    
    //道具3
    CC_SYNTHESIZE(int, m_tool_3, NoTool_3);

    //道具4
    CC_SYNTHESIZE(int, m_tool_4, NoTool_4);
    
    //道具5
    CC_SYNTHESIZE(int, m_tool_5, NoTool_5);
    
    //道具6
    CC_SYNTHESIZE(int, m_tool_6, NoTool_6);
    
    //增加道具
    void increaseTool(int tool_id,int num);
    
    //减少道具
    void decreaseTool();
    
    //根据id获取道具的数量
    int getToolNum(int tool_id);

	//===========关卡解锁情况===========
	//获得当前关卡的是否解锁，分数和等级
	UserDataMission* getUserDataMissionByID(int id);

	//根据ID，设定关卡数据
	void setUserDataMissionByID(int id, UserDataMission *&data);

	//得到场景ID，根据内置的selected_mission_id
	int getSceneID();

	//得到场景ID，根据内置的selected_mission_id
	int getSubMissionID();

	void replaceNextMission();

    //===========装备===========
    //导入用户装备数据
    void loadAllEquipments(TiXmlElement* elm);
    
    //更改使用中的装备
    void setEquippedEquipmentsID(int equip_id, int new_id);
    
    
    //===========主机的血量 ====
    CC_SYNTHESIZE(float, m_heroHp, HeroHp);
    
    //===========复活的次数 ====
    CC_SYNTHESIZE(int, m_nReviveTimes, ReviveTimes);
    
private:
	static UserData* s_sharedUserData;

	/*************消费记录*****************/
    /*
public:
	//保存消费记录
	void savePurchraseHistory1(int price, int tradeId);
	//检测未返还的消费记录下标并返回（检测到的第一条）
	int getUnFinishedPurchraseIndex1(std::string& bid);
	//完成返还后更改消费记录,返回价格
	int changePurchraseHistory1(std::string bid);
	//获取所有的消费信息
	string getAllPurchraseHistory1();
*/
};

#endif