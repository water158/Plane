#ifndef _BULLET_VIEW_H_
#define _BULLET_VIEW_H_

#include "cocos2d.h"

#include "GlobalEnum.h"
#include "BulletData.h"

//#include "CraftView.h"
#include "FerrisMotionStreak.h"

#include "TrailGroupData.h"

#include "ArcPathAction.h"

#include "FerrisParticle.h"

#include "LightningBoltNode.h"

USING_NS_CC;
using namespace std;

class CraftView;
class BulletView : public CCSprite
{
public:
    virtual ~BulletView();
    static BulletView* create(BulletData *data);

	//切换形态
    void changeShape();

	//更新位置
	void updatePosition(float dt);

	//创建拖尾效果
	FerrisMotionStreak* createMotionStreak();
    //创建拖尾粒子
    FerrisParticle* createStreakParticle();

	//获得和更改路径
	TrailGroupData* getDataTrailGroup();
	void setDataTrailGroup(TrailGroupData* data);

	//此路径组是否循环完毕
	bool isMoveOver();

	//自转
	void runSelfRotate();

	//是否出界
	bool outOfScreen();
    
    //获取碰撞矩形 通过类型
    CCRect getCollisionRect(RectType type);
    
    //螺旋跟踪
    void callbackCircleTrace();
    
    //自毁回调
    void callbackRemoveSelf(CCNode *pSender);
    
    //更新导弹目标
    void updateMissileTarget(float dt);
    
    //设置导弹目标
    void setMissileTarget(CraftView* _target);
    
private:
    BulletView();
    bool init(BulletData * data);

protected:
	//子弹路径组
	TrailGroupData* m_pDataTrailGroup;

    //当前的飞行器数据 组件 形态 碰撞 武器
    BulletData* dataBullet;
    
    //导弹目标的敌机
    CraftView* missileTarget;
    
    CC_SYNTHESIZE(bool, m_bIsHaveTarget, IsHaveMIssileTarget);

	//ID
	CC_SYNTHESIZE(string, id, ID);
	//伤害值
	CC_SYNTHESIZE(float, harm, Harm);
	//是否已经没了
	CC_SYNTHESIZE(bool, is_died, IsDied);
	//是否自转
	CC_SYNTHESIZE(bool, m_bIsSelfRotate, IsSelfRotate);
    //自转360时间
    CC_SYNTHESIZE(float, m_fSelfRotateTime, SelfRotateTime);

	//IsDied后直接返回其它坐标（追踪动作TraceAction的目标死亡后）
	CC_SYNTHESIZE(bool, isAimDied, IsAimDied);

	//拖尾
	CC_SYNTHESIZE(FerrisMotionStreak*, m_pMotionStreak, MotionStreak);
	//拖尾粒子
	CC_SYNTHESIZE(FerrisParticle*, m_pMissileParticle, MissileParticle);
	//发射动画
	CC_SYNTHESIZE(CCSprite*, m_pSpriteExpand, SpriteExpand);
    //额外效果
    CC_SYNTHESIZE(CCSprite*, m_pSpriteExtra, SpriteExtra);
    //子弹击中的种类
    CC_SYNTHESIZE(int, m_nHitType, HitType);
    //子弹发射的种类
    CC_SYNTHESIZE(int, m_nShootType, ShootType);

	//子弹当前是第几条路径
	CC_SYNTHESIZE(int, m_nCurTrail, CurTrail);
	//子弹速度
	CC_SYNTHESIZE(float, m_fSpeed, Speed);
	//放射子弹的间隔度数
	CC_SYNTHESIZE(float, m_fOffsetAngle, OffsetAngle);
	//子子弹的id
	CC_SYNTHESIZE(int, m_nChildBulletID, ChildBulletID);

	//子弹路径的整体偏转角度
	CC_SYNTHESIZE(float, m_fTrailGroupAngle, TrailGroupAngle);
    
    //放射型子弹类型 （目前用1和2区分 1是前一个版本实现的放射效果，2是新版本实现的放射效果）
    CC_SYNTHESIZE(int, m_bRadiateType, RadiateType);
    
    //放射型子弹2间隔时间
    CC_SYNTHESIZE(float, m_fDelayTime, DelayTime);
    
    //拖尾类型
    CC_SYNTHESIZE(int, m_nStreakType, StreakType);
    
    //子弹拖尾粒子类型
    CC_SYNTHESIZE(int, m_nStreakParticleType, StreakParticleType);
    
    //设置属于那个炮
    CC_SYNTHESIZE(int, belongCount, BelongCount);
    
    //是否是闪电子弹
    CC_SYNTHESIZE(bool, m_bIsLightningBullet, IsLightningBullet);
    //是否悬挂上闪电
    CC_SYNTHESIZE(bool, m_bIsTakeLightning, IsTakeLightning);
    
    //导弹执行圆弧轨迹的次数
    CC_SYNTHESIZE(int, roundPathCount, RooundPathCount);
    
    //半弧运动旋转方向
    CC_SYNTHESIZE(bool, roundIsAnticlockwise, RooundIsAnticlockwise);
    
    //是否是导弹
    CC_SYNTHESIZE(bool, is_missile, IsMissile);
    
    //是否是最小距离
    CC_SYNTHESIZE(bool, is_Arrive, IsArrive);
    
    //闪电
    CC_SYNTHESIZE(LightningBoltNode*,m_lbNode,M_LBNode);

	/*******************路径组重复*********************/
	//当前已重复的次数
	CC_SYNTHESIZE(int, curRepeatTimes, CurRepeatTimes);
	//总的重复的次数
	CC_SYNTHESIZE(int, totalRepeatTimes, TotalRepeatTimes);
	//重复的起始ID
	CC_SYNTHESIZE(int, curRepeatBeginID, CurRepeatBeginID);
	////重复的结束ID
	CC_SYNTHESIZE(int, curRepeatEndID, CurRepeatEndID);
};
#endif