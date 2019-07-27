//
//  DailyCountData.h
//  phoenix
//
//  Created by mini01 on 14-11-11.
//
//

#ifndef __phoenix__DailyCountData__
#define __phoenix__DailyCountData__

#include "cocos2d.h"
#include "FerrisTools.h"
#include "GlobalEnum.h"
#include "tinyxml.h"
#include "DailyTask.h"
#include "GameDelegate.h"

USING_NS_CC;
using namespace std;

class DailyCountData
{
public:
    virtual ~DailyCountData();
    
    static DailyCountData* sharedInstance();
    
    void load();
    
    void init();
    
    void check();
    
    void save();
    
    string m_sDailyCountDataPath;
private:
    DailyCountData();
    
    static DailyCountData* m_sharedInstance;
    
    void checkVersion();
    
    void loadDataFromVer(int version);
    
    
    CC_SYNTHESIZE(int , m_nVersion, Version);
    
    string m_strengthEquipId;
    
    /******************** 统计项目 ***********************/
    
    //分数累积
    CC_SYNTHESIZE(int, m_dailyTotalScore, DailyTotalScore);
    //一局分数累积
    CC_SYNTHESIZE(int, m_oneMissionTotalScore, OneMissionTotalScore);
    //当日波数累积
    CC_SYNTHESIZE(int, m_dailyTotalWaves, DailyTotalWaves);
    //一局波数累积
    CC_SYNTHESIZE(int, m_oneMissionTotalWaves, OneMissionTotalWaves);
    //强化任一装备到级数
    CC_SYNTHESIZE(int, m_strengthEquip, StrengthEquip);
    //当日累积强化次数
    CC_SYNTHESIZE(int, m_dailyStrengthTimes, DailyStrengthTimes);
    //当日获得的装备数量
    CC_SYNTHESIZE(int, m_dailyGetEquip, DailyGetEquip);
    //当日累积获得金币
    CC_SYNTHESIZE(int, m_dailyGetGold, DailyGetGold);
    //一局累积获得金币
    CC_SYNTHESIZE(int, m_oneMissionGetGold, OneMissionGetGold);
    //当日累积摧毁敌机
    CC_SYNTHESIZE(int, m_dailyDestoryEnemy, DailyDestoryEnemy);
    //一局摧毁敌机数量
    CC_SYNTHESIZE(int, m_oneMissionDestoryEnemy, OneMissionDestoryEnemy);
    //当日累积出击次数
    CC_SYNTHESIZE(int, m_dailyAttackTimes, DailyAttackTimes);
    //当日抽奖次数
    CC_SYNTHESIZE(int, m_dailyLotteryTimes, DailyLotteryTimes);
    //当日出售装备i
    CC_SYNTHESIZE(int, m_dailySellEquip, DailySellEquip);
    //当日分享装备
    CC_SYNTHESIZE(int, m_dailyShareEquip, DailyShareEquip);
    //当日分享成就
    CC_SYNTHESIZE(int, m_dailyShareAchieve, DailyShareAchieve);
    //当日分享成绩
    CC_SYNTHESIZE(int, m_dailyShareAcount, DailyShareAcount);
    
    /************************* 操作 *******************************/
    
    //增加当日波数
    void addDailayTotalWaves();
    
    //增加当日累积摧毁敌机数
    void addDailyDestrotEnemy();
    
    //增加当日获得装备数
    void addDailyGetEquip();
    
    //增加当日累积获得金币
    void addDailyGetGold();
    
    //增加当日出击次数
    void addAttackTime();
    
    //增加当日累积分数
    void addDailyTotalScore(int score);
    
    //增加当日出售装备
    void addDailySellEquip();
    
    //增加当日抽奖
    void addDailyLotteryTime();
    
    /************************ 是否有成就达成 **************************/
    
    bool hadAchieveComplete();
    
    
    void checkAchieve(AchieveItem* item);
    
     /************************ 是否有未领取 **************************/
    bool hadAchieveUnReceive();
    
    void clearData();
};

#endif /* defined(__phoenix__DailyCountData__) */
