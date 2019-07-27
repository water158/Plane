#ifndef __FERRIS_GAME__GlobalTagZOrder__
#define __FERRIS_GAME__GlobalTagZOrder__

/***************************************************
 * GameDelegate
 * 在其他类中,不要再用了
 ***************************************************/

// 在GameDelegate中,用于区分不同的层
typedef enum
{
    kTagSplashLayer = 101,
    kTagStartLayer,
    kTagGameMenuLayer,
    kTagToolsLayer,
    kTagGameCombatLayer,
    kTagLoadingLayer,
    kTagGameConfigLayer,
    kTagGameSigninLayer,
    kTagGameAboutLayer,
    kTagGameHelpLayer,
    kTagGameAchieveLayer,
    kTagGameLotteryLayer,
    kTagReviveLayer,
    kTagAccountLayer,
    kTagBlureNode,
    kTagGameStoreLayer,
    kTagGamePauseLayer,
    kTagSoundLayer,
    kTagNetworkLayer,
    kTagNotificationLayer,
} SceneTag;

//在游戏中各个层的前后关系
//最下面是网络层和声音层
//上面是游戏运行的层(菜单界面啊,游戏界面啊,配置界面等等)
//再上面是商店层
//再上面是对话框层
//最上面是全局提示层
enum
{
    kZOrderNetworkLayer,
    kZOrderSoundLayer,
    kZOrderRunningLayer,    //是指当前正在运行的Layer
    kZOrderBlureNode,
    kZOrderAchieveLayer,
    kZOrderSigninLayer,
    kZOrderConfigureLayer,
    kZOrderLotteryLayer,
    kZOrderReviveLayer,
    kZOrderAccountLayer,
    kZOrderPauseLayer,
    kZOrderGameStoreLayer,
    kZOrderDialogLayer,
    kZOrderNotificationLayer,
};

/***************************************************
 * COMMON TAG ZORDER
 ***************************************************/
//gamemain tag
enum
{
    kTagBackground = 300,
    kTagDialog,
    kTagEffectSelectPanel,
    kTagEffectSelectEquip,
    kTagEffectOverEquip,
    kTagEffectChange,
};
//gamemain zorder
enum
{
    kZOrderBackground = -1,
    kZOrderBaseUI,
    kZOrderEffectSelect = 10,
    kZOrderEffectOverEquip,
    KZOrderDialog = 300,
    kZOrderGetEquipLayer,
};
//GameCombat zorder
enum
{
    GAME_ZORDER_BACKGROUND = -100,
//    GAME_ZORDER_CLOUD_LOW,
//    GAME_ZORDER_CLOUD_HIGH,
//    GAME_ZORDER_SPEEDLINE,
//    GAME_ZORDER_BIG_FIRE,
//   // GAME_ZORDER_HERO_BULLET,
//    GAME_ZORDER_CRAFT,
//    GAME_ZORDER_DEBRIS,
//    GAME_ZORDER_BOOM_EXTRA,
//    GAME_ZORDER_BOOM,
//    GAME_ZORDER_BOOM_PARTICLE,
    GAME_ZORDER_ENEMY,
    GAME_ZORDER_SHOOT_EFFECT,
    GAME_ZORDER_BOOM_EFFECT,
    GAME_ZORDER_HIT_EFFECT,
    GAME_ZORDER_HERO_BULLET,
    GAME_ZORDER_TOOLS,
    GAME_ZORDER_HERO,//主机、血球、技能
    GAME_ZORDER_ENEMY_BULLET,
    GAME_ZORDER_UI_COVER,
    GAME_ZORDER_DIALOG,
    GAME_ZORDER_STORELAYER,
};

//飞机层各种飞机的 ZORDER
enum
{
    CRAFT_ZORDER_SCORE,
    CRAFT_ZORDER_ENEMY,
    GAME_ZORDER_LASER,
    CRAFT_ZORDER_HERO,
    CRAFT_ZORDER_WING,
    CRAFT_ZORDER_COIN,
    CRAFT_ZORDER_TOOL,
    CRAFT_ZORDER_BLOOD,
};

////  TAG  ///

//金币道具血球加速action的tag
#define TAG_SPEEDUP_ACTION 10001

//主机机头拖尾tag
#define TAG_HERO_HEAD 10101

//当前目标身上的标记
#define kTagAimedCraftRemark 1005

//用于处理道具界面的pageview中显示的页数标志
enum
{
    pageTools = 100,
    pageEquip,
};

//游戏中的图片tag
enum
{
    //无敌护盾道具
    kTagToolInvincible = 2000,
    //子弹变金币道具
    kTagToolMakeCoin,
    //主机复活护盾
    kTagReviveInvincible = 8090,
    //影遁的残影
    kTagHidingShadowsSpr = 500,
    //冲过20波气流动画
    kTagSkipWaveAnimation = 100111,
};

//GameCombat tag
enum
{
    GAME_TAG_BACKGROUND = 100,
};

//主机机头动画tag
enum
{
    kTagHeroHeadSp1,
    kTagHeroHeadSp2,
};

//技能TAG
enum
{
    //光波炮
    kTagSkillWaveGun = 1001,
    //聚能炮
    kTagSkillShapedGun = 1002,
    //斥力盾
    kTagSkillRepulsionShiled = 1003,
};

#endif