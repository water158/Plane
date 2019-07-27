//
//  StatID.h
//  plane
//
//  Created by mini01 on 14-4-16.
//  用于友盟统计生成id
//

#ifndef __plane__StatID__
#define __plane__StatID__

#include "cocos2d.h"
#include <iostream>

USING_NS_CC;

class StatID
{
public:
    StatID();
    ~StatID();
    //获取关卡字符串
    static const char* getMissionID (int missionID);
    //获取S级关卡字符串
    static const char* getSMissionID (int missionID);
    //通过主机类型和等级获取飞机升级ID字符串
    static const char* getHeroUpgradeUseCoin (int type,int level);
    //升级主机用钻石
    static const char* getHeroUpgradeUseCrystal(int type,int level);
    //升级僚机用金币
    static const char* getWingUpgtadeUseCoin(int type,int level);
    //通过僚机类型和等级获取僚机ID字符串
    static const char* getWingUpgradeUseCryStal (int type,int level);
    //解锁主机
    static const char* getUnlockHero(int type);
    //解锁僚机
    static const char* getUnlockWing(int type);
    //获取道具ID
    static const char* getToolsID (int toolType);
    //获取复活ID
    static const char* getReBornID ();
    //获取强化50%
    static const char* getAttributes50 ();
    //获取购买金币ID 参数为购买金币的量
    static const char* getCoinID (int coinNum);
    //获取购买钻石ID 参数为购买钻石的量
    static const char* getCrystalID(int crystalNum);
    //获取购买永久双倍金币id
    static const char* getPerDoubleCoin();
    //获得购买永久自动收集金币id
    static const char* getMagnet();
    //购买英雄小礼包
    static const char* getHeroSmallGift();
    //购买三倍金币
    static const char* getTripleCoin();
    //购买英雄大礼包
    static const char* getHeroBigGift();
    //购买僚机大礼包
    static const char* getWingGift();
    //购买道具上限
    static const char* getToolTopLimit();
};

#endif /* defined(__plane__StatID__) */