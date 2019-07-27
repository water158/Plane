//
//  DailyTask.h
//  phoenix
//
//  Created by mini01 on 14-11-10.
//
//

#ifndef __phoenix__DailyTask__
#define __phoenix__DailyTask__

#include "cocos2d.h"
#include "AchieveItem.h"
#include "GlobalEnum.h"
#include "tinyxml.h"
#include "FerrisTools.h"
#include "UserData.h"

USING_NS_CC;
using namespace std;

class DailyTask
{
public:
    virtual ~DailyTask();
    
    static DailyTask* sharedInstance();
    
    //初始化数据
    void init();
    
    void check();
    
    //保存数据
    void save();
    
    //保存elm
    void saveElm(TiXmlElement* elm, AchieveItem* item);
    
    void load();
    
    //日常任务数组
    CCArray* arr_dailyTask;
    
private:
    //路径
    string m_sDailyTaskDataPath;
    
    static DailyTask* s_sharedInstance;
    
    void checkVersion();
    
    void loadDataFromVer(int version);
    
    CC_SYNTHESIZE(int, m_nVersion, Version);
    
    DailyTask();
    
    //获取时间信息
    CC_SYNTHESIZE(int, m_date, Date);
    
    //四个普通日常任务id
    CC_SYNTHESIZE(string, m_firstTaskID, FirstTaskID);
    CC_SYNTHESIZE(string, m_secTaskID, SecTaskID);
    CC_SYNTHESIZE(string, m_thirdTaskID, ThirdTaskID);
    CC_SYNTHESIZE(string, m_fourTaskID, FourTaskID);
    
    //分享任务ID
    CC_SYNTHESIZE(string, m_shareTaskID, ShareTaskID);
    
    
    //根据id获取日常任务
    AchieveItem* getTaskByID(int nId);
    
    //清空数据
    void clearData();
    
    
    
    //更新日常任务
    void refreshTask();
    
};

#endif /* defined(__phoenix__DailyTask__) */
