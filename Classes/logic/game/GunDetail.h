#ifndef _GUN_DETAIL_H_
#define _GUN_DETAIL_H_

#include "cocos2d.h"
#include "GlobalParams.h"

#include "TargetGunGroupData.h"
#include "GunData.h"
#include "HookPoint.h"

using namespace std;

class PointExt;

class GunDetail : public CCObject
{
    
public:
	virtual ~GunDetail();
    static GunDetail* create(TargetGunGroupData *data1, GunData *data2);
    
    //打印
    void print();
    
private:
    GunDetail();
    bool init(TargetGunGroupData *data1, GunData *data2);
    
    //UUID
    CC_SYNTHESIZE(int, uuid, UUID);
    
    /*********************************************
     * 临时存储
     *********************************************/
    //GunGroup的数据
    CC_SYNTHESIZE(TargetGunGroupData *, dataTargetGunGroup, TargetGunGroupData);
    
    //Gun的数据
    CC_SYNTHESIZE(GunData *, dataGun, GunData);
    
	//GunGroup是否是跟踪模式
	CC_SYNTHESIZE (bool, m_bIsTrack, IsTrack);

	//是否是穿透型
	CC_SYNTHESIZE(bool,is_pass,IsPass);
    
    /*********************************************
     * 系数
     *********************************************/
    //伤害系数
    CC_SYNTHESIZE(float, m_fHarmRatio, HarmRatio);
    
    /*********************************************
     * Refresh Position
     * 用于固定开枪的位置
     * 考虑下面一个场景
     * 当打出圆圈形状的子弹，尤其是连续三次，每次之间间隔短这种情况
     * 会因为飞机的移动，而使得三次不是同心圆
     * Mid循环是中循环 大的循环是RepeatTime
     * 解决方案:
     * 1.在GunDetail存储一个临时的位置，存储子弹初始化的位置
     * 2.在第一次Mid循环的之后，获得开枪的位置，存入GunDetail中
     * 3.第二次及以后，都使用GunDetail中的临时位置
     * 4.直到下一次的第一次，才重新更新位置
     *********************************************/
    bool needRefreshPosition();
    
    CC_SYNTHESIZE(CCPoint, m_oBulletTempPosition, BulletTempPosition);
    
    /*********************************************
     * Time Judgement
     *********************************************/
public:
    //增加重复次数的计数
    void increaseRepeatTimes();
    
    //重复次数是否越界
    bool isRepeatTimeCurrentLargeThenTotal();
    
    //增加Interval次数的计数
    //因为删除了Interval 所以就不要了
    //void increaseIntervalIndex();
    
    //获得当前的Interval总值
    //因为删除了Interval 所以就不要了
    //float getCurrentTotalInterval();
    
    //获得Interval的总个数
    int getIntervalCount();
    
    //获得目标时间
    float getTimeTarget();
    
    //获得激光的目标时间
    float getTimeTargetOfLaser();
    
    //获得初次开枪时间
    float getTimeBeginFire();
    
    //刷新时间的参量
    void refreshRepeatTimes();
    
    //刷新时间的参量
    //因为删除了Interval 所以就不要了
    //void refreshRepeatTimesAndIntervalIndex();
    
    //刷新中循环时间的参量
    void refreshMidRepeatTimesAndIntervalIndex();
    
    //获得子弹发射所用总时间
    float getTimeTotal();
    
    //大循环的次数
	CC_SYNTHESIZE(int, current_repeat_times, CurrentRepeatTimes);
    
    //小循环的次数
    //CC_SYNTHESIZE(int, interval_index, IntervalIndex);
    
    //飞机入场
    CC_SYNTHESIZE(bool, is_begin_fire, IsBeginFire);
    
    //中循环次数
    CC_SYNTHESIZE(int, cur_mid_repeat_times, CurMidRepeatTimes);
    
    //发射延迟
    CC_SYNTHESIZE(float, m_fDelayTime, DelayTime);
    
    //是否固定枪口发射子弹
    CC_SYNTHESIZE(bool, m_bIsFixedGunDetail,IsFixedGunDetail);
   
    /*********************************************
     * Transform
     *********************************************/
public:
    //设定转角
    void setGunGroupRotation(float newRotation);
    
    //获得转角
    float getGunGroupRotation();
    
    //设定XY位移
    void setGunGroupPosition(const CCPoint& newPosition);
    
    //获得XY位移
    const CCPoint& getGunGroupPosition();

	//获得仿射矩阵
	CCAffineTransform getGunGroupTransform();

    //Gun相对GunGroup的位置
    CC_SYNTHESIZE(CCPoint, m_obGunPosition, GunPosition);
    
    //Gun相对GunGroup的转角
    CC_SYNTHESIZE(float, m_fGunRotation, GunRotation);
    
    //Gun相对GunGroup的转角
    CC_SYNTHESIZE(float, m_fGunRotationInit, GunRotationInit);
    
protected:
    //角位移
    float m_fGunGroupRotation;
    
    //XY位移
    CCPoint m_obGunGroupPosition;
    
    //是否发生了transform
    bool m_bGunGroupTransfromDirty;

	//仿射矩阵
	CCAffineTransform m_sGunGroupTransform;

};

class PointExt : public CCObject
{
public:
    ~PointExt();
    static PointExt* create(float x, float y);
    
private:
    PointExt();
    bool init(float x, float y);
    
    CC_SYNTHESIZE(float, x, X);
    CC_SYNTHESIZE(float, y, Y);
};

#endif  // _GUN_DETAIL_H_