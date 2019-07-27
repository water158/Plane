#ifndef _TARGETGUNGROUPDATA_H_
#define _TARGETGUNGROUPDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"

#include "GlobalEnum.h"


USING_NS_CC;
using namespace std;

class TargetGunGroupData : public CCObject
{
public:
    virtual ~TargetGunGroupData(void);

    static TargetGunGroupData* create(TiXmlElement *elm);

    //加载自身的数据
    bool loadData(TiXmlElement *elm);

    void print();

    TargetGunGroupData(void);

private:


    CC_SYNTHESIZE(string, id, ID);
    CC_SYNTHESIZE(string, gungroup_id, GunGroupID);
    CC_SYNTHESIZE(float, x, X);
    CC_SYNTHESIZE(float, y, Y);

    //首次延迟
    CC_SYNTHESIZE(float, first_delay, FirstDelay);
    //循环延迟
    CC_SYNTHESIZE(float, repeat_delay, RepeatDelay);
    //循环次数
    CC_SYNTHESIZE(int, repeat_times, RepeatTimes);
    
    //中间循环延迟
    CC_SYNTHESIZE(float, mid_delay, MidDelay);
    //中间循环次数
    CC_SYNTHESIZE(int, mid_repeat, MidRepeat);

    //伤害值
    CC_SYNTHESIZE(float, harm, Harm);

    //是否是追踪型
    CC_SYNTHESIZE(bool, is_track, IsTrack);

    //TODO:用于激光旋转的 但是现在没有被用到 应该会被用到
    CC_SYNTHESIZE(float, m_fRotationInit, RotationInit);
    CC_SYNTHESIZE(float, m_fRotationOffset, RotationOffset);
    
    //初始延迟
    CC_SYNTHESIZE(float, m_fDelayTime, DelayTime);
    
    //是否固定枪口发射子弹
    CC_SYNTHESIZE(bool, m_bIsFixedGunDetail,IsFixedGunDetail);
};

#endif