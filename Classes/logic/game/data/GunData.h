#ifndef __GUNDATA_H__
#define __GUNDATA_H__

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"

#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class GunData : public CCObject
{
public:
    virtual ~GunData();

    static GunData* create(TiXmlElement *elm);

    bool loadData(TiXmlElement *elm);

    //加载发射子弹的时间间隔
    void loadIntervals(TiXmlElement *elm);

private:

	GunData();

    CC_SYNTHESIZE(string, bullet_id, BulletID);

    CC_SYNTHESIZE(string, id, ID);

    /********************************
     * 通用
     ********************************/
    //X
    CC_SYNTHESIZE(float, x, X);

    //Y
    CC_SYNTHESIZE(float, y, Y);

    //首次延迟
    CC_SYNTHESIZE(float, delay, FirstDelay);

    //枪管指向的角度
    CC_SYNTHESIZE(float, direction_angle, DirectionAngle);

    /********************************
     * 子弹
     ********************************/
    //速度
    CC_SYNTHESIZE(float, speed, Speed);

    //间隔的总个数
    CC_SYNTHESIZE(float, total_interval, TotalInterval)

    //间隔的数据
    CC_SYNTHESIZE(vector<float>, intervals, Intervals);

    //是否是追踪导弹
    CC_SYNTHESIZE(bool, is_trace, IsTrace);

    //是否是穿透子弹
    CC_SYNTHESIZE(bool, is_pass, IsPass);
    
    //是否是导弹
    CC_SYNTHESIZE(bool, is_missile, IsMissile);


    /***************子弹的路径******************/
    CC_SYNTHESIZE(int, m_nTrailGroupID, TrailGroupID);

    //子弹路径组整体偏转角度
    CC_SYNTHESIZE(float, m_fTrailGroupAngle, TrailGroupAngle);

	 /********************************
     * 放射型子弹
     ********************************/
	//放射型子弹之间的偏移夹角
	CC_SYNTHESIZE(float, m_fOffsetAngle, OffsetAngle);
	//放射型子弹爆炸时在屏幕的高度位置
	CC_SYNTHESIZE(float, m_fPercentageHeight, PercentageHeight);
	//子子弹的id
	CC_SYNTHESIZE(int, m_nChildBulletID, ChildBulletID);
    //放射型子弹类型 （目前用1和2区分 1是前一个版本实现的放射效果，2是新版本实现的放射效果）
    CC_SYNTHESIZE(int, m_bRadiateType, RadiateType);
    //放射型子弹2间隔时间
    CC_SYNTHESIZE(float, m_fDelayTime, DelayTime);

};

#endif