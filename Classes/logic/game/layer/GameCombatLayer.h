#ifndef __GAME_COMBAT_LAYER_H__
#define __GAME_COMBAT_LAYER_H__

#include "cocos2d.h"
#include "tinyxml.h"
#include "GameDelegate.h"
#include "GlobalEnum.h"
#include "DataManager.h"
#include "GlobalDelegate.h"
#include "DataManager.h"
#include "TrailLibrary.h"
#include "PathLibrary.h"
#include "SkillLibrary.h"
#include "GlobalTagZOrder.h"
#include "GlobalFunc.h"

//extend
#include "FerrisTools.h"
#include "FerrisMotionStreak.h"
#include "FerrisParticle.h"
#include "LightningBoltNode.h"
#include "LightningBoltSprite.h"
#include "LightningBoltAction.h"
#include "LineSegment.h"
#include "MagnetCoinAction.h"
#include "FerrisDrawLine.h"

#include "RoundPathAction.h"
#include "TraceAction.h"
#include "SmartRes.h"
#include "JumpByAction.h"
#include "SpriteAnimationNew.h"
#include "LaserBeam.h"
#include "OvalPathAction.h"
#include "GetRandomEquip.h"
#include "UserEquipBag.h"

//view
#include "BulletView.h"
#include "CraftView.h"
#include "HeroView.h"
#include "EnemyView.h"
#include "EquipView.h"
#include "LaserBeam.h"
#include "BackgroundView.h"
#include "CoinView.h"
#include "WingView.h"
#include "GameBattleUILayer.h"
#include "ToolsView.h"
#include "BloodView.h"
#include "ScoreView.h"

//data
#include "GunDetail.h"
#include "GunData.h"
#include "BulletData.h"
#include "CraftData.h"
#include "TrailGroupData.h"
#include "TimePlaneData.h"
#include "TargetPlaneData.h"
#include "EquipItemData.h"
#include "MissionDataManager.h"
#include "SkillData.h"
#include "SkillLevelData.h"
#include <stdlib.h>
#include "BatteryPosManager.h"
#include "ResultCountData.h"
#include "DropData.h"

//skill
#include "SkillNull.h"
#include "SkillSandsOfTime.h"
#include "SkillWave.h"
#include "SkillShapedGun.h"
#include "SkillShadowOfHiding.h"
#include "SkillRepulsionShield.h"
#include "SkillPositioningShooting.h"
#include "SkillDefenseBombs.h"
#include "SkillAddedBlood.h"
#include "SkillPropsWareHouse.h"

//laser
#include "LaserLayer.h"

//count
#include "DailyCountData.h"
#include "GameCountData.h"

USING_NS_CC;


class GameCombatLayer : public CCLayer, public BulletDelegate, public CrashedDelegate, public MapDelegate, public DropDelegate
{
public:
    GameCombatLayer(void);
	virtual ~GameCombatLayer(void);
    
    static GameCombatLayer* create();
	
	//按键处理
	virtual void keyBackClicked();
	virtual void keyMenuClicked();

	//循环
	virtual void update(float delta);
    
    //飞出敌机
    virtual void enemyFly();
    
    //创建子弹
    virtual void createCraftBullet(CraftView *craft, GunDetail* detail);
    //子弹撞了
    virtual void bulletCrashed(BulletView *&bullet, bool is_play_animation);
    //设置飞机机头拖尾是否显示
    virtual void setCraftHeadStreak(bool enable);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

public:
    /************************
     * 游戏开始
     ************************/
    void gameStart();
    void startFireBatteryAndWing();
    
    void gameFlyNormalWave();                      //正常游戏
    void gameFlyOverTwentyWave();                  //冲过二十波
    void callbackRemoveSkipAni(CCNode *pSender);   //移除冲过二十波动画
    void callbackAddGameWaveAndScore();            //冲过二十波时 波数，积分，翻牌数增加
    
	/************************
     * 时间轴
     ************************/
    //时间轴滚动
    void updateTime(float dt);
    
    //更新激光
    void updateLaserTest(float delta);

	/************************
     * 轨迹
     ************************/
    //飞机轨迹
    void callBackNextPathAction(CCNode* pSender);
    //子弹轨迹
    void callBackNextTrailAction(CCNode* pSender);
	//随机并移动到该点
	void callBackRandAndMove(CCNode* pSender);
	
	 /************************
     * 子弹
     ************************/
    //即将移除
    void bulletRemoveCallBack(CCNode* pSender);
    //放射型子弹的爆炸回调（发射一圈子弹）
    void callbackRadiateBulletBoom(CCNode* pSender);
    //放射型子弹2爆炸回调（发射两个相对的子弹）
    void callbackRadiaeBulletType2Boom(CCNode* pSender);
    
    MissionData* mission;

	/************************
     * TRACE BULLET
     ************************/
    virtual bool getAimedCraftPosition(int aimed_craft_id, CCPoint &position);
    CraftView* getAimedCraft();

    CCPoint getAimedConnonPosition();
    
    CraftView* getAimedConnon();

    /***********************
     * 僚机技能 防御敌弹
     ***********************/
    BulletView* getNearestBullet();
    
    void attackBullet();
	/************************
    * 移除
    ************************/
    //清理已消除项目
    void updateRemove(float dt);
    //飞机起飞回调
    void callbackSetCraftBegin(CCNode* pSender);
    //飞机坠毁回调
    void callbackSetCraftRemove(CCNode* pSender);
    //飞机坠毁回调
    void callbackSetBulletRemove(CCNode* pSender);
    
    //移除node
    void removeNode(CCNode* pSender);
    
    //移除Craft
    void callBackRemoveCraft(CCNode* pSender);
    
    //移除drop
    void callbackRemoveDrop(CCNode* pSender);
    
    //移除bullet
    void callbackRemoveBullet(CCNode* pSender);

public:
	bool init();
    
    /********* 复活 ***********/
    void revive(CCObject* pSender);
    
    //复活之后
    void afterRevive(CCObject* pSender);
    
/******************属性*******************/
    //道具掉率
    float f_toolDrop;
    //血量掉率
    float f_bloodDrop;
    //血量加成
    float f_hpAdd;
    //是否跳过前20波
    bool b_skipTwenty;
    //是否可复活
    bool b_canRevive;
    //是否三倍金币
    bool b_tripleCoin;
    
/******************道具*******************/
    
    //使用道具
    void useTool(bool isTouch);
    //使用强化道具
    void useInvincibleTool();
    //void useTool(int grid);
    //301  生成一个短时间无敌护罩
    void useToolForInvincible();
    //302  回复满格血量
    void useToolForRecoverHealth();
    //303  把屏幕中的敌机子弹变成金币
    void useToolForMakeCoin();
    //305  竖向单道激光
    void useToolForVerticalBullet();
    //306  密集子弹
    void useToolForDenseBullet();
    //307  追踪导弹
    void useToolForTraceMissile();
    //308  多道激光定目标攻击
    void useToolForLockTarget();
    //309  清屏导弹
    void useToolForClearMissile();
    //创建闪电
    void createLightningBolt();
    //更新清屏导弹道具（闪电）
    void updateLightningBolt();
    
    //子弹变金币
    void addCoinFromBullet(BulletView* bullet);

    //创建道具飞机
    void createToolPlane(TargetPlaneData* dataTargetPlane);
 
    //根据type创建在主机前面飞的道具
    void createToolWithHeroFront(int type);
    
    //检测血球是否出界
    void CheckBloodOutofScreen();
    
    //检测道具飞机子弹是否全部打完
    void checkToolPlaneBulletofDied();
    //删除道具飞机
    void removeToolPlaneFromHero(CraftView *craft);
    
    //移除无敌盾
    void removeShiled(CCArmature *arm, MovementEventType type, const char * name);
    
    //延时调用自动吸附回调
    void callbackAutoFindHero(CCNode* pSender);
    
    //初始强化道具掉落回调
    void callbackDropSevenToolsForInvincible();
    void dropToolsForInvincible();
    
/******************飞机*******************/
    //创建主角
    void createHero();
    //获取主角位置
    virtual CCPoint getHeroPosition();
    void checkHeroHP();
    
    //获取僚机位置
    CCPoint getWingPosition();
    
    //主机取消无敌
    void callbackCancelInvincible();
    
	//飞机撞了
	virtual void craftCrashed(CraftView *craft, int reason, bool is_play_animation);
	//enemy恢复之前的动作
	virtual void resumeEnemyPreAction(EnemyView *craft);

	//创建普通敌人
	void createEnemy(TargetPlaneData *dataTargetPlane, TimePlaneData *dataTimePlane, bool is_pause);
	//为飞机创建炮台
    void createBatteryForCraft(EnemyView* craft, int connonCount,TimePlaneData *dataTimePlane);
    //炮台自动转向
    void batteryAutomaticRotate(CraftView* craft);
    //敌机可以受击回调
    void callbackSetEnemyInjured(CCNode* pSender);
    
	//为主机创建炮台
	void createBatteryForHero();
    //设置主角攻击状态
    void setHeroAttrackState(bool bEnable);
    
    //掉落
    //掉落金币
    void dropCoin(CraftView* craft);
    //金币加速运动
    virtual void dropSpeedUp(DropView* drop);
    //掉落普通道具
    void dropToolsForNormal(CraftView* craft);
    //掉落强化道具
    void dropToolsForInvincible(CraftView* craft);
    //掉落装备
    void dropEquip(CraftView* craft);
	//掉落积分
    void dropScore(CraftView* craft);
    //掉落血球
    void dropBlood(CraftView* craft);
    //创建血球
    void createBloodCell(CCPoint pos);
    //道具回调
    void callbackToolMagnet(CCNode* node);
    //敌机炮子弹变积分
    void enemyBulletToScore(CraftView *craft);
    //创建积分
    void createScore(CCPoint pos);
    
    //爆炸
    //给炮爆炸效果
    void addBoomForBattery(CraftView* craft);
	//给飞机添加小型爆炸效果
	void addSmallBoomForCraft(CraftView* craft);
	//给飞机添加大型爆炸效果
	void addBigBoomForCraft(CraftView* craft);
    //给大飞机、BOSS额外的爆炸效果
    void addExtraBoom(CCNode* pSender,void* craftType);
    //大爆炸后期的小爆炸
    void addSmallBoomsForBigBoom(int craftType,CCPoint targetPos,CCPoint range);
    //void callBackRemoveSmallBomm(CCNode* pSender);
    void callbackCircleForSmall(CCNode* pSender);
    //给小飞机爆炸添加光圈
    void addCircleForSmallPlane(CCPoint pos,float opacity,float scale,float dt);
    //给大飞机爆炸添加光圈
    void addCircleForBigPlane(CCPoint pos,float opacity,float scale,float dt);
    //给BOSS添加屏幕的晃动
    void addExtraShaking();
    //Boss飞机爆炸后屏幕晃动
    void addShakingScreen(float dt);

/******************碰撞*******************/
	//所有碰撞
	void updateCollisions(float dt);
	//主角子弹和敌机的碰撞
	void collisionHeroBulletsWithEnemys();
	//主机和敌机的碰撞
	void collisionHeroWithEnemys();
    //主机和金币碰撞
    void collsiionHeroWithCoin(float dt);
    //主机和道具碰撞
    void collsiionHeroWithCommonTool();
    //主机和强化道具碰撞
    void collsiionHeroWithInvinibleTool();
    //主机和血球碰撞
    void collisionHeroWithBlood();
    //主机和积分碰撞
    void collisionHeroWithScore();
    //主机检测碰撞
    void collisionHeroPreCheck();
    
    //僚机子弹与敌机碰撞
    void collisionWingBulletsWithEnemys();
    
	//敌机子弹和主机的碰撞
	void collisionEnemyBulletsWithHero();
	//检测子弹是否出界
	void checkBulletsOutofScreen();
	//敌机子弹和飞机与光波炮的碰撞
	void collisionEnemyAndBulletsWithWaveGun();
	//敌机与聚能炮的碰撞
	void collisionEnemyWithShapedGun();
	//敌机子弹与斥力盾的碰撞
	void collisionEnemyBulletsWithRepulsionShiled();

    //无敌护罩道具与子弹碰撞
    void collisionToolsInvincibleWithBullet();
    
    //道具飞机子弹与敌机碰撞
    void collisionToolPlaneBulletsWithEnemys();
    
/**************激光****************/
    LaserDelegate* m_pLaser;
    
    //主炮是否激光
    bool m_bIsMainLaser;
    
    //肩炮是否激光
    bool m_bIsShoulderLaser;
    
    //翼炮是否激光
    bool m_bIsPinionLaser;
    
    //僚机是否激光
    bool m_bIsWingLaser;

/******************装备技能释放*******************/
    SkillDelegate *m_pMainSkill;
    SkillDelegate *m_pWingSkill;
    
    void releaseShadowOfHiding();
    //主机恢复最初状态
    void callbackHeroRecover(CCNode* pSender);
    //计算影遁技能持续时间
    void calculateDurationOfSkill();
    
	//技能更新
	void updateSkills(float dt);
	//技能CD回复回调
	void callBackEnableSkill(CCNode* pSender);
	//技能CD回复回调(定位狙击)
	void callBackEnableTraceBullet(CCNode* pSender);
	//技能开关
	bool m_bSkillEnable;
	//僚机技能开关
	bool m_bWingSkillEnable;
    //技能正在使用
    bool m_bIsSkillRunning;

	//光波炮当前蓄力时间
	float m_fCurrentPrepareTime;
	//是否蓄满
	bool m_bFull;
	//聚能炮当前聚能的阶段
	int m_nShapedGunCurrentPhrase;
	//进入聚能炮本阶段后的时间
	float m_fShapedGunCurrentPhraseTime;
	//斥力盾当前持续的时间
	float m_fRepulsionShiledLastTime;
	//定位狙击的持续时间
	float m_fPositionShootingLastTime;
	//是否开启追踪子弹
	bool m_bTraceBulletEnable;
    
    //技能准备的进度条
    CCProgressTimer* m_pProgressForPrapare;
    
    //主机的影遁之前的位置
    CCPoint m_pPosOri;

/******************主机操作*******************/
	//更新主机位置
	void updateHeroPosition(CCPoint new_position);
	//是否触摸了屏幕
	bool m_bIsTouch;
	//记录前一个触摸点
	CCPoint m_posPrePoint;

public:
    //主机
    HeroView* m_pHeroView;
    //主炮
    EquipView* m_pMainGun;
    //肩炮1、2
    EquipView* m_pBearLeftGun;
    EquipView* m_pBearRightGun;
    //翼炮1、2
    EquipView* m_pWingLeftGun;
    EquipView* m_pWingRightGun;
    //僚机
    WingView* m_pWingView;
    
	//敌机
	CCDictionary *m_pDictEnemy;
	CCArray *m_pArrayTaskEnemy;
	//子弹
	CCArray *m_pArrayHeroBullet;
	CCArray *m_pArrayWingBullet;
	CCArray *m_pArrayEnemyBullet;
    CCArray* m_pArrayToolBullets;
	//需要移除的东西
	CCArray *m_pArrayRemove;
    //金币容器
    CCArray *m_pArrayCoin;
    //普通道具容器（屏幕移动）
    CCArray *m_pArrayTool;
    //强化道具容器（屏幕移动）
    CCArray *m_pArrInvincibleTool;
    //道具容器（主机前面飞）
    CCArray *m_pArrayFlyTool;
    //道具飞机容器
    CCArray *m_pArrayToolPlane;
	//粒子（血球）
	CCArray* m_pArrayBloodParticles;
    CCArray* m_pArrayBlood;
    //积分容器
    CCArray *m_pArrayScore;
    
    //闪电容器
    CCArray* m_pArrayBolts;
    
    /* ==============================
     * batchnode
     * ============================== */
    
	//子弹容器
	CCSpriteBatchNode* m_pHeroBullets;
	CCSpriteBatchNode* m_pEnemyBullets;
    //掉落物品容器
    CCSpriteBatchNode* m_pDropBatchNode;
    //子弹发射动画节点
    CCSpriteBatchNode *m_pSpriteExpands;

	//战机节点
//	CCNode* m_pCrafts;
	//粒子节点
	CCNode* m_pParticleNode;

    //游戏是否结束
    bool gameOver;
    
	//时间轴
	int m_nGameTime;
    
	//游戏波数
    int m_nGameWave;
    
    //摧毁敌机数
    int m_nShatter;
    
    //金币数
    int m_nCoin;
    
    //积分数
    int m_nScore;

    //飞机组id
    int m_nPlaneGroupId;
    //敌机是否可以飞入
    bool isEnemyFly;
    
    //强化道具出现次数
    int m_nStrengtimes;
    
    //炮管血量增加量
    int m_nBloodAddedValue;
    
    //敌机攻击基础值
    float m_enemyAttackBase;
    
    //是否使用了道具
    bool m_bIsUseToolFinish;
    
    //主机道具槽个数
    int m_nToolGridNum;
    
    //强化道具出现次数
    CC_SYNTHESIZE(int, m_nInvincibleTimes, InvincibleTimes);
    
    //结算数据
    ResultCountData* m_resultCountData;
    
    //炮个数计数
    int batteryNumCount;
    
    //子弹出现时间计数
    float bulletAppearTime;
    
    //道具飞机个数计数
    int toolPlaneNumCount;
    
    //当前主炮攻击阶段
    int curMainGunPhrase;
    
    //当前副炮1攻击阶段
    int curDeputy1GunPhrase;
    
    //当前副炮2攻击阶段
    int curDeputy2GunPhrase;
    
    //之前的道具掉率
    int preNormalToolRate;
    
    //敌机飞出计数
    int flyPlaneCount;
    
    /******************UI*******************/
    GameBattleUILayer* m_battleUI;
    
    
    /****************** 数据 ******************/
    
    float m_heroMaxHp;
    
    /****************延迟时间  用于同一波炮的反射延迟****************/
    float m_fDelayTime;
    
    /*********** GameCombatLayer自定义scheduler actionmanager *************/
    //时间管理器 scheduler
    CCScheduler* m_pCombatScheduler;
    
    //动作管理器
    CCActionManager* m_pCombatActionManager;
};

#endif
