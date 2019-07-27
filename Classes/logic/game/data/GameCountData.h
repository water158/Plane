//
//  GameCountData.h
//  phoenix
//
//  Created by mini01 on 14-11-11.
//
//

#ifndef __phoenix__GameCountData__
#define __phoenix__GameCountData__

#include "cocos2d.h"
#include "FerrisTools.h"
#include "GlobalEnum.h"
#include "tinyxml.h"
#include "Achieve.h"
#include "GameDelegate.h"

USING_NS_CC;
using namespace std;

class GameCountData
{
public:
    virtual ~GameCountData();
    
    static GameCountData* sharedInstance();
    
    void load();
    
    void init();
    
    void check();
    
    void save();
    
    string m_sGameCountDataPath;
private:
    GameCountData();
    
    static GameCountData* m_sharedInstance;
    
    void checkVersion();
    
    void loadDataFromVer(int version);
    
    CC_SYNTHESIZE(int, m_nVersion, Version);
    
    /************* 统计条目 **************/
    
    //里程
    CC_SYNTHESIZE(int, m_mileage, Mileage);
    //击毁数
    CC_SYNTHESIZE(int, m_shatter, Shatter);
    //释放道具数
    CC_SYNTHESIZE(int, m_useTools, UseTools);
    //todo:关于类型处理
    //获得优秀装备
    CC_SYNTHESIZE(int, m_greenEquip, GreenEquip);
    //获得精良装备
    CC_SYNTHESIZE(int, m_blueEquip, BlueEquip);
    //获得史诗装备
    CC_SYNTHESIZE(int, m_pupleEquip, PupleEquip);
    //获得传说装备
    CC_SYNTHESIZE(int, m_orangeEquip, OrangeEquip);
    //抽奖次数
    CC_SYNTHESIZE(int, m_lotteryTimes, LotteryTimes);
    //装备强化等级
    CC_SYNTHESIZE(int, m_equipStrength, EquipStrength);
    //充值数
    CC_SYNTHESIZE(int, m_recharge, Recharge);
    //累积消耗钻石
    CC_SYNTHESIZE(int, m_consumeDamond, ConsumeDamond);
    //累积出击次数
    CC_SYNTHESIZE(int, m_attackTimes, AttackTimes);
    //最高分数
    CC_SYNTHESIZE(int, m_bestScore, BestScore);
    //累积获得金币
    CC_SYNTHESIZE(int, m_totalGold, TotalGold);
    //累积获得钻石
    CC_SYNTHESIZE(int, m_totalGem, TotalGem);
    //累积完成任务
    CC_SYNTHESIZE(int, m_totalTask, TotalTask);
    
    
    /***************** 操作 *******************/

    //增加击毁数
    void addShatter();
    
    //增加道具释放数
    void addUseTools();
    
    //增加获得优秀装备数
    void addGreenEquip();
    
    //增加获得精良装备
    void addBlueEquip();
    
    //增加获得史诗装备
    void addPupleEquip();
    
    //增加获得传说装备
    void addOrangeEquip();
    
    //增加出击次数
    void addAttackTime();
    
    //增加金币
    void addTotalGold(int num);
    
    //增加钻石
    void addTotalGem(int num);
    
    //增加完成任务
    void addTotalTask();
    
    //增加消耗钻石
    void addConsumeGem(int num);
    
    /***************** 检查是否有成就完成  ********************/
    
    bool hadAchieveComplete();
    
    void checkAchieve(AchieveItem* item);
    
    /***************** 检查是否有成就未领取 *******************/
    
    bool hadAchieveUnReceive();
    
};

#endif /* defined(__phoenix__GameCountData__) */
