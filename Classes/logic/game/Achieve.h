//
//  Achieve.h
//  phoenix
//
//  Created by mini01 on 14-11-6.
//
//

#ifndef __phoenix__Achieve__
#define __phoenix__Achieve__

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"
#include "GlobalEnum.h"
#include "AchieveItem.h"

USING_NS_CC;
using namespace std;

class Achieve
{
public:
    virtual ~Achieve();
    
    static Achieve* getInstance();
    
    //初始化数据
    void init();
    
    void check();
    
    //保存数据
    void save();
    
    //检测版本
    void checkVersion();
    
    //根据版本加载
    void loadDataFromVer(int version);
    
    //路径
    string m_sAchieveDataPath;
    
    void load();
    
    //根据成就类型和id号获得成就item
    AchieveItem* getAchieve(AchievType type,int Id);
    
    //里程成就数组
    CCArray* achieveArr_mileage;
    
    //击毁成就数组
    CCArray* achieveArr_shatter;
    
    //道具成就数组
    CCArray* achieveArr_tools;
    
    //装备成就数组
    CCArray* achieveArr_equipment;
    
    //等级成就数组
    CCArray* achieveArr_level;
    
    //充值成就
    CCArray* achieveArr_pay;
    
    //消费成就数组
    CCArray* achieveArr_consume;
    
    
private:
    Achieve();
    
    static Achieve* s_sharedInstance;
    
    CC_SYNTHESIZE(int, m_dataVersion, DataVersion);
    
    
    
    
    
};


#endif /* defined(__phoenix__Achieve__) */
