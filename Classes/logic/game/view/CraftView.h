#ifndef _CRAFT_VIEW_H_
#define _CRAFT_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "FerrisTools.h"
#include "SmartRes.h"
#include "BulletView.h"
#include "SoundEffectManager.h"

//global
#include "GlobalParams.h"
#include "GlobalEnum.h"
#include "GlobalDelegate.h"

//data
#include "CraftData.h"
#include "ShapesData.h"
#include "ShapePartData.h"
#include "CollisionsData.h"
#include "CollisionGroupData.h"
#include "PathData.h"
#include "PathGroupData.h"
#include "TargetPlaneData.h"
#include "GunDetail.h"
#include "GunGroupData.h"
#include "WeaponLibrary.h"
#include "GunData.h"
#include "MiscData.h"

#define kTagBattery 100
#define kTagShape 10


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace gui;


class CraftView : public CCNode
{

public:
    //构造 不支持显式的构造 只支持create
    CraftView();

    //创建
    static CraftView* create(CraftData* data);

    //析构
    virtual ~CraftView();

	//绘制
	virtual void draw();

	//切换阶段
	virtual void changePhrase(int phrase);
    
	/*****************************
     * 碰撞检测
     *****************************/
    //碰撞检测 与子弹之间的碰撞(所有)
    virtual void testAllBulletCollision(CCArray *&bullets);
    //碰撞检测 与对手之间的碰撞(数组)
    virtual void testAllCraftCollision(CCDictionary *&crafts);
    //碰撞检测 与对手之间的碰撞(单一)
    virtual void testSingleCraftCollision(CraftView *&craft);
    
    //获取碰撞矩形 通过类型
    virtual CCRect getCollisionRect(RectType type);

	/*****************************
     * 碰撞结果处理
     *****************************/
	//处理子弹的碰撞结果，比如减血啊之类的
	virtual void processBulletCollision(BulletView *&bullet);
	//处理同对手碰撞的结果
	virtual void processCraftCollision(CraftView *&craft);

	//获得父辈的坐标 用于转换成世界坐标（专用于飞机本身为BOSS附属飞机）
	CCPoint getHolderPosition();

	//此路径组是否循环完毕
	bool isMoveOver();

	//恢复后坐动作状态
	void callBackResumeRecoil(CCNode* node);

	//主机预览界面的时候将主机机身隐身
	void hideBody();

	//飞机渐隐
	void craftFadeIn(float dt);

	//子弹发射的状态回复
	void callbackResumeRotate();

	//回复与光波炮的碰撞
	void resumeAction();
	void callBackResumeCollisionWithWaveGun(CCNode* pSender);

	//回复与聚能炮的碰撞
	void resumeShapedGunAction();
	void callBackResumeCollisionWithShapedGun(CCNode* pSender);
    
    //获取敌机的型号
//    int getCraftSizeType(CraftView* craft);
    
    //获取各个敌机的子分类
//    int getChildSizeType(CraftView* craft);

protected:
	//初始化
    bool init(CraftData *data);
	//切换形态
    void changeShape();

	//准备GunDetail 是每次update的时候 用到的数据
	void prepareGunDetail(CCArray* targetgungroups);

	//清除
	virtual void callbackClean(CCNode *pSender);
    
public:
	//回调
	BulletDelegate* delegateBullet;
	CrashedDelegate* delegateCrashed;
	/*************
     *数据
     *************/
    //当前的飞行器数据 HP Phrase信息等等
    TargetPlaneData* dataTargetPlane;

    //当前的形态数据
    ShapesData* dataShape;

    //当前的飞行器数据 组件 形态 碰撞 武器
    CraftData* dataCraft;
    
    //当前的附加数据
//    MiscData* dataMisc;

    //Sprite们的容器
    CCNode* nodeContainer;

	//粒子特效的容器
	CCNode* nodeParticleContainer;

	//武器的位置
	CCDictionary* dictPosWeapon;

	//被追踪的中心点(偏移量)
    CC_SYNTHESIZE(CCPoint, m_oAimedCenter, AimedCenter);

    //生命总值
    CC_SYNTHESIZE(float, total_hp, TotalHP);
    //生命当前值
    CC_SYNTHESIZE(float, current_hp, CurrentHP);
    //UUID
    CC_SYNTHESIZE(int, uuid, UUID);
    //Type(HERO BOSS ENEMY WING)
    CC_SYNTHESIZE(int, type, Type);
    //宽度
    CC_SYNTHESIZE(float, width, Width);
    //高度
    CC_SYNTHESIZE(float, height, Height);
    //血条宽度
    CC_SYNTHESIZE(float, hp_width, HPWidth);
    //阶段
    CC_SYNTHESIZE(int, phrase, Phrase);
    //飞机入场
    CC_SYNTHESIZE(bool, is_begin_fly, IsBeginFly)
	//是否已连接激光线
	CC_SYNTHESIZE(bool, m_bLinkLaser, LinkLaser)
    //飞机死亡
    CC_SYNTHESIZE(bool, is_died, IsDied);
	//飞机是否进行攻击的开关
	CC_SYNTHESIZE(bool, m_bGunSwitch, GunSwitch);
    //当前战机的目标
    CC_SYNTHESIZE(CraftView*, craft_target, CraftTarget);
    //当前飞机挂的武器
    CC_SYNTHESIZE(CCArray*, gun_details, GunDetails);
	//当前飞机粒子特效的位置信息
	CC_SYNTHESIZE(CCArray*, m_pArrParticlePosition, ArrParticlePosition);

    //是否绘制枪管位置
    CC_SYNTHESIZE(bool, is_draw_weapon_position, IsDrawWeaponPosition);
    //是否绘制碰撞矩形和碰撞线
    CC_SYNTHESIZE(bool, is_draw_collision_rect, IsDrawCollisionRect);

    //此飞机是否作为一个导弹的目标
    CC_SYNTHESIZE(bool, isBullletAim, IsBullletAim);
    //此飞机是否是其它飞机的附属飞机
    CC_SYNTHESIZE(bool, isAffiliatedPlane, IsAffiliatedPlane);
    //此飞机是否有附属飞机
    CC_SYNTHESIZE(bool, isExistBattery, IsExistBattery);

    //此飞机是否在飞机身上
    CC_SYNTHESIZE(bool, m_bIsBossPart, IsBossPart);
    //飞机进入发射子弹状态
    CC_SYNTHESIZE(bool, m_bIsCreatingBullet, IsCreatingBullet);
    //炮台是否可以旋转
    CC_SYNTHESIZE(bool, m_bIsCanRotate, IsCanRotate);
    
    //飞机变形动画name
    CC_SYNTHESIZE(string, m_sTransformName, TransformName);
    //飞机变形动画path
    CC_SYNTHESIZE(string, m_sTransformPath, TransformPath);
    
    //炮的类型  用于区分主炮还是副炮
    CC_SYNTHESIZE(int, m_connonType, ConnonType);

    /*******************路径组重复*********************/
    //当前已重复的次数
    CC_SYNTHESIZE(int, curRepeatTimes, CurRepeatTimes);
    //总的重复的次数
    CC_SYNTHESIZE(int, totalRepeatTimes, TotalRepeatTimes);
    //重复的起始ID
    CC_SYNTHESIZE(int, curRepeatBeginID, CurRepeatBeginID);
    ////重复的结束ID
    CC_SYNTHESIZE(int, curRepeatEndID, CurRepeatEndID);

    //当前运行到第几个轨迹
    CC_SYNTHESIZE(int, curPath, CurPath);
    //飞机当前轨迹
    CC_SYNTHESIZE(float, speed, Speed);
	//轨迹的下一个目标点
	CC_SYNTHESIZE(CCPoint, m_posNextAimPos, NextAimPos);
	//轨迹的下一个目标点的下一个目标点
	CC_SYNTHESIZE(CCPoint, m_posSecondAimPos, SecondAimPos);


    //boss是否正在爆炸
    CC_SYNTHESIZE(bool, isBossBooming, IsBossBooming);

    //销毁后是否掉落金币
    CC_SYNTHESIZE(bool, isDropCoin, IsDropCoin);
    
    //销毁后是否掉落积分
    CC_SYNTHESIZE(bool, isDropScore, IsDropScore);
    
    //销毁后是否掉落装备
    CC_SYNTHESIZE(bool, isDropEquip, IsDropEquip);
    
    //销毁后是否掉落普通道具
    CC_SYNTHESIZE(bool, isDropNormalTools, IsDropNormalTools);
    
    //销毁后是否掉落强化道具
    CC_SYNTHESIZE(bool, isDropInvincibleTools, IsDropInvincibleTools);
    
    //销毁后是否掉落血球
    CC_SYNTHESIZE(bool, isDropBloodCell, IsDropBloodCell);

    //拿到父飞行器的ID
    CC_SYNTHESIZE(int, m_nHolderID, HolderID);

    //碰撞时间延迟，此时为不可撞击状态
    CC_SYNTHESIZE(bool, m_bCollisionLess, CollisionLess);

	//粒子数量
	CC_SYNTHESIZE (int, m_nParticleNum, ParticleNum);

	//爆炸次数
	CC_SYNTHESIZE(int, m_nCraftBoomNum, CraftBoomNum)

	//爆炸次数
	CC_SYNTHESIZE(int, m_nCurBoomNum, CurBoomNum)

	//是否是炮台
	CC_SYNTHESIZE(bool, m_bIsBattery, IsBattery);

	//是否有后坐力
	CC_SYNTHESIZE(bool, m_bRecoil, Recoil);

	//是否正在进行后坐力动作
	CC_SYNTHESIZE(bool, m_bRecoilAction, RecoilAction);

	//炮台旋转速度（度/s）
	CC_SYNTHESIZE(float, m_fBatteryRotateSpeed, BatteryRotateSpeed);

	//是否与光波炮碰过了
	CC_SYNTHESIZE(bool, m_bCollisionWithWaveGun, CollisionWithWaveGun);

	//是否与聚能炮碰过了
	CC_SYNTHESIZE(bool, m_bCollisionWithShapedGun, CollisionWithShapedGun);
    
    //子弹计数 用于判断子弹归属那个炮  或者  归属于那个道具飞机
    CC_SYNTHESIZE(int, belongCount, BelongCount);
    
    //着火1是否存在
    CC_SYNTHESIZE(bool, m_bIsHaveSmoking1, IsHaveSmoking1);
    //着火2是否存在
    CC_SYNTHESIZE(bool, m_bIsHaveSmoking2, IsHaveSmoking2);
    
    //判断是左是右 0：无 1：主炮 僚机 2：主机副炮左 3 主机副炮右
    CC_SYNTHESIZE(int, m_nBatteryDirection, BatteryDirection);
    
    CC_SYNTHESIZE(float, m_pStagnateX, StagnatePointX);
    CC_SYNTHESIZE(float, m_pStagnateY, StagnatePointY);

	/* ==============================
     * 粒子batchnode
     * ============================== */
	 CC_SYNTHESIZE(CCParticleBatchNode*, m_pParticleBatchNode, ParticleBatchNode);
	 CC_SYNTHESIZE(CCParticleBatchNode*, m_pParticleBatchNode1, ParticleBatchNode1);
	 CC_SYNTHESIZE(CCParticleBatchNode*, m_pParticleBatchNode2, ParticleBatchNode2);

public:
	PathGroupData* dataPathGroup;

public:
	void setDataPathGroup(PathGroupData *path_group_data);

	PathGroupData* getDataPathGroup();

	    /*********************************************
     * Transform
     *********************************************/
public:
    //设置转角，只是让nodeContainer旋转，自身不旋转
    virtual void setRotation(float newRotation);
    //获得转角
    virtual float getRotation();
    //设定XY位移
    virtual void setPosition(const CCPoint& newPosition);
    //获得仿射矩阵
    CCAffineTransform getCraftTransform();
    //获得内部放射矩阵
    CCAffineTransform getCraftInnerTransform();

    //是否发生了transform
    bool m_bCraftTransfromDirty;
    bool m_bCraftInnerTransfromDirty;

private:
    //仿射矩阵
    CCAffineTransform m_sCraftTransform;
    //内部仿射矩阵
    CCAffineTransform m_sCraftInnerTransform;

    /*********************************************
     * 出界判断
     *********************************************/
public:
    bool checkOutOfScreen();
    bool checkOutOfScreenForBoom();
    bool checkIsAutoCollected();

    /* ==============================
     * 发射
     * ============================== */
    //存储当前飞机的时间 用于发射子弹用
    float time;

    //发射子弹
    void shotBullet(float dt, GunDetail *detail);


};

#endif
