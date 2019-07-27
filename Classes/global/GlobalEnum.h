#ifndef __FERRIS_GAME__GlobalEnum__
#define __FERRIS_GAME__GlobalEnum__

#include "cocos2d.h"

USING_NS_CC;

/***************************************************
 * NAMESPACE
 ***************************************************/
#define USING_NS_STD using namespace std

/***************************************************
 * CALLBACK
 ***************************************************/
class FerrisResult;

typedef void (CCObject::*DECLAIMER_FUNC_CALLBACK)(FerrisResult*);
#define CONVERTER_FUNC_CALLBACK(_FUNCTION_NAME) (DECLAIMER_FUNC_CALLBACK)(&_FUNCTION_NAME)

/***************************************************
 * STD VECTOR的循环函数
 ***************************************************/

#define VECTOR_FOREACH(__type__, __vector__, __object__)             \
if (__vector__.size() > 0)                                           \
for(__type__::iterator ite = __vector__.begin();                     \
    ite != __vector__.end() && (__object__ = *ite);                  \
    ite++)
/***************************************************
 * 音效播放优先级
 ***************************************************/

//普通音效，指持续播放的音效优先级
#define SOUND_NORMAL_BULLET 1
//激光音效优先级
#define SOUND_LASER 5
//导弹音效优先级
#define SOUND_MISSLE 10
//爆炸音效优先级
#define SOUND_BOMB  25
//连续爆炸优先级
#define SOUND_BIG_BOOM 27
//主机受伤害优先级
#define SOUND_HERO_HURT 28
//获得金币和积分的音效优先级
#define SOUND_GET_DROP 30
//道具获得优先级
#define SOUND_GET_TOOL 31
//使用道具 技能的音效优先级
#define SOUND_USE_PROP 32
//点击按钮和选择优先级
#define SOUND_CLICK 40

/***************************************************
 * 音效文件
 ***************************************************/

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//todo::如果是ANDROID系统使用资源名称
/* 音效 */
//按钮点击效果1
#define EFF_CLICK_HEAVY "sound/buttonEffect1.mp3"
//按钮点击效果2
#define EFF_CLICK_LIGHT "sound/buttonEffect2.mp3"
//开门音效
#define EFF_OPENDOOR "sound/doorEffect.mp3"
//技能释放效果
#define EFF_SKILL "sound/skillEffect.mp3"
//道具释放效果
#define EFF_USE_TOOL_1 "sound/toolEffect1.mp3" //
#define EFF_USE_TOOL_2 "sound/toolEffect2.mp3" //强化道具 其他道具
#define EFF_USE_TOOL_3 "sound/toolEffect3.mp3" //无敌道具
//道具获得音效
#define EFF_GET_TOOL "sound/getToolEffect.mp3"
//吃金币释放效果
#define EFF_GET_COIN "sound/coinEffect.mp3"
//吃积分释放效果
#define EFF_GET_SCORE "sound/scoreEffect.mp3"
//普通子弹音效
#define EFF_NORMAL_BULLET "sound/bulletEffect.mp3"
//导弹音效
#define EFF_MISSLE "sound/missleEffect.mp3"
//激光音效
#define EFF_LAYSER "sound/layserEffect.mp3"
//闪电音效
#define EFF_LIGHTNING "sound/lightningEffect.mp3"
//爆炸音效
#define EFF_BOOM_SMALL "sound/smallExplodeEffect.mp3"
//连续爆炸音效
#define EFF_BOOM_BIG "sound/bigExplodeEffect.mp3"
//主机受攻击音效
#define EFF_HURT_HERO "sound/hurtHeroEffect.mp3"

/* 音乐 */
//主背景音乐
#define MIS_MAIN_BG "sound/mainMainMusic.mp3"
//战斗场景背景音乐
#define MIS_BATTLE_BG "sound/bgMusic.mp3"
#endif

#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
///todo::如果是IOS系统使用资源名称
/* 音效 */
//按钮点击效果1
#define EFF_CLICK_HEAVY "sound/buttonEffect1.mp3"
//按钮点击效果2
#define EFF_CLICK_LIGHT "sound/buttonEffect2.mp3"
//开门音效
#define EFF_OPENDOOR "sound/doorEffect.mp3"
//技能释放效果
#define EFF_SKILL "sound/skillEffect.mp3"
//道具释放效果
#define EFF_USE_TOOL_1 "sound/toolEffect1.mp3" //
#define EFF_USE_TOOL_2 "sound/toolEffect2.mp3" //强化道具 其他道具
#define EFF_USE_TOOL_3 "sound/toolEffect3.mp3" //无敌道具
//道具获得音效
#define EFF_GET_TOOL "sound/getToolEffect.mp3"
//吃金币释放效果
#define EFF_GET_COIN "sound/coinEffect.mp3"
//吃积分释放效果
#define EFF_GET_SCORE "sound/scoreEffect.mp3"
//普通子弹音效
#define EFF_NORMAL_BULLET "sound/bulletEffect.mp3"
//导弹音效
#define EFF_MISSLE "sound/missleEffect.mp3"
//激光音效
#define EFF_LAYSER "sound/layserEffect.mp3"
//闪电音效
#define EFF_LIGHTNING "sound/lightningEffect.mp3"
//爆炸音效
#define EFF_BOOM_SMALL "sound/smallExplodeEffect.mp3"
//连续爆炸音效
#define EFF_BOOM_BIG "sound/bigExplodeEffect.mp3"
//主机受攻击音效
#define EFF_HURT_HERO "sound/hurtHeroEffect.mp3"

/* 音乐 */
//主背景音乐
#define MIS_MAIN_BG "sound/mainMainMusic.mp3"
//战斗场景背景音乐
#define MIS_BATTLE_BG "sound/bgMusic.mp3"
#endif

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
//todo::如果是win32系统使用资源名称
/* 音效 */
//按钮点击效果1
#define EFF_CLICK_HEAVY "sound/buttonEffect1.mp3"
//按钮点击效果2
#define EFF_CLICK_LIGHT "sound/buttonEffect2.mp3"
//开门音效
#define EFF_OPENDOOR "sound/doorEffect.mp3"
//技能释放效果
#define EFF_SKILL "sound/skillEffect.mp3"
//道具释放效果
#define EFF_USE_TOOL_1 "sound/toolEffect1.mp3" //
#define EFF_USE_TOOL_2 "sound/toolEffect2.mp3" //强化道具 其他道具
#define EFF_USE_TOOL_3 "sound/toolEffect3.mp3" //无敌道具
//道具获得音效
#define EFF_GET_TOOL "sound/getToolEffect.mp3"
//吃金币释放效果
#define EFF_GET_COIN "sound/coinEffect.mp3"
//吃积分释放效果
#define EFF_GET_SCORE "sound/scoreEffect.mp3"
//普通子弹音效
#define EFF_NORMAL_BULLET "sound/bulletEffect.mp3"
//导弹音效
#define EFF_MISSLE "sound/missleEffect.mp3"
//激光音效
#define EFF_LAYSER "sound/layserEffect.mp3"
//闪电音效
#define EFF_LIGHTNING "sound/lightningEffect.mp3"
//爆炸音效
#define EFF_BOOM_SMALL "sound/smallExplodeEffect.mp3"
//连续爆炸音效
#define EFF_BOOM_BIG "sound/bigExplodeEffect.mp3"
//主机受攻击音效
#define EFF_HURT_HERO "sound/hurtHeroEffect.mp3"

/* 音乐 */
//主背景音乐
#define MIS_MAIN_BG "sound/mainMainMusic.mp3"
//战斗场景背景音乐
#define MIS_BATTLE_BG "sound/bgMusic.mp3"
#endif

/***************************************************
 * 游戏中用到的联网地址
 ***************************************************/
#define SERVER_DOMAIN "http://pay.ferrisgame.com"

#define URL_COMPLAINT "http://pay.ferrisgame.com/Appeal/create"

#define URL_COUPON "http://pay.ferrisgame.com/Coupon/"

#define URL_CHECK_NETWORK_AVAILABLE "http://www.baidu.com"

#define URL_PAYMENG_VERIFICATION "http://pay.ferrisgame.com/Upay/inquiry/?bid="

/***************************************************
 * 游戏中用到的id
 ***************************************************/
//UMENG ID
#define ID_UMENG "53a113a456240be1e108f2f6"

//ShareSDK
#define ID_SHARESDK "3df5508271cf"

//QQ
#define ID_SHARE_QQ_APPID "1103292314"
#define ID_SHARE_QQ_KEY "9eDiVkwX2UxFI2Cn"

//Weixin
#define ID_SHARE_WEIXIN_ID "wxe107521dbb2791cf"
#define ID_SHARE_WEIXIN_SECRET "107a827edf6401000d4511643aa95050"

//ShareContent
#define SHARE_TARGET_URL "http://www.ferrisgame.com/GameAnti/"
#define SHARE_CONTENT "快来看看我的成就，想要超越我，不光需要脑力，还需要一定的运气。"

//ADS ID
#define ID_ADMOB_BANNER "ca-app-pub-4305631875686357/1539402887"
#define ID_ADMOB_INSTRIRIAL "ca-app-pub-4305631875686357/9062669688"
#define ID_COCO_AD "100032-4CE817-ABA2-5B48-14D009296720"

/***************************************************
 * 游戏用的宏
 ***************************************************/

//版本号
#define VERSION "v1.0.1"

//std::string的值是空
#define NOT_SET "NOT_SET"

//字体大小
#define SIZE_BIG 30
#define SIZE_SMALL 24

//PI
#define PI 3.14f

//游戏中的最大距离 由
#define MAX_DISTANCE 1360.0f

//CRAFT的初始方向
#define CRAFT_FLY_DIR 90.0f

//在JAVA中 接收消费请求的方法名
#define PURCHASE_METHOD_NAME_IN_JAVA "ProcessPurchaseRequest"

//刷新游戏中的宝石
#define REFRESH_UI_4_GEM "refresh_gem"

//道具库最大容量
#define TOOL_MAX_CAPACITY 2

//复活消息
#define MSG_REVIVE "msg_revive"

//更新金币
#define MSG_REFRESHGOLD "msg_refreshGold"

//基础的道具掉率
#define BASE_TOOL_DROP 10.0f

//基础的血球掉率
#define BASE_BLOOD_DROP 14.0f

//上层地图的速度
#define MAP_UP_SPEED 7.0f
//下层地图的速度
#define MAP_DOWN_SPEED 1.5f
//切换一套地图时变速速度
#define MAP_COVER_CHANGE_SPEED 35.0f
//上层地图出现频率
#define MAP_APPEAR_RATE 1.2f

//切换地图时地图的速度
#define MAP_CHANGE_SPEED 15.0f

//机头位置
#define POSY_HERO_HEAD 77

//机头拖尾位置
#define POSY_HERO_HEAD_STREAK 65

//光圈持续时间
#define CIRCLE_INTERVAL 0.0f

//爆炸持续时间
#define BOOM_INTERVAL 0.03f

//敌机延时飞入时间
#define ENEMYFLY_INTERVAL 100

//敌机炮最低序号
#define ENEMYCANNON_MIN_NUM 11
#define ENEMYCANNON_MAX_NUM 35

//掉落积分数据ID
#define DROP_DATA_ID 201

//敌机炮轨迹ID
#define ENEMY_ROTE_ID 105

//敌机受攻击延时时间
#define ENEMY_INJURED_DELEAY 0.2f 

//初始随机掉落的强化道具个数
#define INIT_DROPTOOL_NUM 7

/***************************************************
 * UI UI UI UI UI UI UI UI UI UI UI UI UI UI
 ***************************************************/
//Dialog Type
enum
{
    DIALOG_TYPE_ONLY_OK,
    DIALOG_TYPE_CLOSE,
    DIALOG_TYPE_LEFT_RIGHT,
    DIALOG_TYPE_CONFIRM_CANCEL,
};

/***************************************************
 * LOGIC LOGIC LOGIC LOGIC LOGIC LOGIC LOGIC
 ***************************************************/

//UserData的版本
enum
{
    USERDATA_VERSION_1,//version 1
    USERDATA_VERSION_2,
    USERDATA_VERSION_3,
    USERDATA_VERSION_4,
    USERDATA_VERSION_5,
};

//商店类型
typedef enum
{
    kStore_energy = 0,
    kStore_gold,
    kStore_gem,
} StoreType;

//金币商店的按钮标记
enum
{
	GOLDSTORE_ITEM_1 = 1,
	GOLDSTORE_ITEM_2,
	GOLDSTORE_ITEM_3,
	GOLDSTORE_ITEM_4,
	GOLDSTORE_ITEM_5,
};

//钻石购买
enum
{
    //充值成功 请再次购买
    BUY_CRYSTAL_AGAIN = -2,
    
    //支付失败
    BUY_CRYSTAL_FAILED = -1,
    
    //购买钻石 1元
    BUY_CRYSTAL_2,
    
    //购买钻石 1元
    BUY_CRYSTAL_4,
    
    //购买钻石 1元
    BUY_CRYSTAL_6,
    
    //购买钻石 1元
    BUY_CRYSTAL_8,
    
    //购买钻石 1元
    BUY_CRYSTAL_12,
    
    
};

//道具购买
enum
{
    //购买道具1
    BUY_TOOL_1 = 1,
    
    //购买道具2
    BUY_TOOL_2,
    
    //购买道具3
    BUY_TOOL_3,
    
    
    //购买道具4
    BUY_TOOL_4,
    
    //购买道具5
    BUY_TOOL_5,
    
    //购买道具6
    BUY_TOOL_6,
};

//抽奖类型是使用金币还是使用的钻石 使用了多少钻石
typedef enum
{
    LOTTERY_USE_COIN,                  //使用金币
    LOTTERY_USE_TEN_CRYSTAL,           //使用10钻
    LOTTERY_USE_FOURTY_CRYSTAL,        //使用40钻
} LotteryType;

//签到奖励的类型
typedef enum
{
    kSignReward_noting = 0,
    kSignReward_gold,
    kSignReward_gem,
    kSignReward_equip_white,
    kSignReward_equip_green,
    kSignReward_equip_blue,
    kSignReward_equip_pupple,
    kSignReward_equip_orange,
    kSignReward_tool_1,
    kSignReward_tool_2,
    kSignReward_tool_3,
    kSignReward_tool_4,
    kSignReward_tool_5,
    kSignReward_tool_6,
} SignRewardType;


//奖励的tag 1为代币 2为装备
typedef enum
{
    kTagRewardToken = 1,
    kTagRewardEquip,
} RewardTag;

/*******************
	最新的付费
 ********************/
#define PAY_ORDER_NAME_CRYSTAL_220 "钻石220"
#define PAY_ORDER_NAME_CRYSTAL_480 "钻石480"
#define PAY_ORDER_NAME_CRYSTAL_800 "钻石800"
#define PAY_ORDER_NAME_CRYSTAL_1200 "钻石1200"
#define PAY_ORDER_NAME_CRYSTAL_2400 "钻石2400"



#define PRICE_REVIVE 20

 

/********************
 * 虚拟币获取渠道
 ********************/
#define DIAMOND_GETBY_PAY 6
#define COIN_GETBY_BUY 5
#define DIAMOND_GETBY_REWARD 4
#define COIN_GETBY_REWARD 3
#define COIN_GETBY_DROP 2


//可变速子弹的TAG 估计要删除
#define kSPEEDUP_ACTION_TAG 112

//飞机大小类型
enum
{
    CRAFT_SIZE_SMALL,        //小型飞机
    CRAFT_SIZE_MID,          //中型飞机
    CRAFT_SIZE_BIG,          //大型飞机
    CRAFT_SIZE_SMALLBOSS,    //头目
    CRAFT_SIZE_BOSS,         //boss
};

//飞机子类型
enum
{
    CRAFT_SIZE_ONE,     //1
    CRAFT_SIZE_TWO,     //2
    CRAFT_SIZE_THRE,    //3
    CRAFT_SIZE_FOUT,    //4
    CRAFT_SIZE_FIVE,    //5
};

//飞机数据类型
enum
{
    CRAFT_DATA_TYPE_HERO = 0,   //主机数据
    CRAFT_DATA_TYPE_ENEMY,		//敌机数据
    CRAFT_DATA_TYPE_CONNON,     //炮台数据
    CRAFT_DATA_TYPE_DROP,       //掉落数据
    CRAFT_DATA_TYPE_WING,       //僚机数据
};

//飞机类型
enum
{
    CRAFT_TYPE_HERO,                //主角 0
    CRAFT_TYPE_WING,                //僚机 1
    CRAFT_TYPE_ENEMY,               //敌人 2
    CRAFT_TYPE_BATTERY,				//主机装备炮 3
    CRAFT_TYPE_BATTERY_TOOL,        //道具炮 4
    CRAFT_TYPE_BOSS,                //BOSS 5
    CRAFT_TYPE_STONE,               //宝石 6
    CRAFT_TYPE_COIN,                //金币 7
    CRAFT_TYPE_MISSILE,             //导弹 8
    CRAFT_TYPE_TOOL,                //道具 9
    CRAFT_TYPE_SCORE,                //积分 10
};

//PATH的枚举
enum
{
    ACTION_MOVETO,      //移动到哪
    ACTION_MOVEBY,      //移动多少
    ACTION_MOVEARC,     //圆弧运动
    ACTION_STAYAT,      //停留
    ACTION_ADDHP,       //加血
    ACTION_REPEAT,      //重复
    ACTION_DIED,        //销毁
};

//僚机位置状态
enum
{
    WING_POSITION_STATE_NULL,
    WING_POSITION_STATE_LIFT,
    WING_POSITION_STATE_RIGHT,
};

//要获取的时间类型
typedef enum
{
    kGetTimeYear,
    kGetTimeMonth,
    kGetTimeDay,
    kGetTimeHour,
    kGetTimeMin,
    kGetTimeSec,
} GetTimeType;

/*********************
    主界面中的tag
 *********************/
#define TAG_TOOL_SELECT 10010
#define TAG_EQUIP_SELECT 10011
#define TAG_PANEL_SELECT 10012


/***************************************************
	成就
 ****************************************************/
//成就类型
typedef enum
{
    ach_mileage = 1,      //里程成就
    ach_shatter,          //击毁成就
    ach_tools,            //道具成就
    ach_equipment,        //装备成就
    ach_level,            //等级成就
    ach_pay,              //充值成就
    ach_consume,          //消费成就
} AchievType;

//日常任务类型
typedef enum
{
    task_day_score = 8,  //日累积分数
    task_one_score,      //一局累积分数
    task_day_wave,       //日累积波数
    task_one_wave,       //一局波数累积
    task_streng_level,   //强化任一装备到等级
    task_streng_time,    //强化次数
    task_get_equip,      //累积获得装备
    task_get_gold,       //累积获得金币
    task_get_goldOneTime,//一局累积获得金币
    task_day_shatter,    //累积摧毁敌机
    task_one_shatter,    //一局摧毁敌机
    task_day_attack,     //累积出击次数
    task_day_lottery,    //累积抽奖次数
    task_sell_equip,     //累积出售装备
    task_share_equip,    //分享装备
    task_share_achieve,  //分享成就
    task_share_account,  //分享成绩
} DailyTaskType;

typedef enum
{
    achieve_game = 1, //成就
    achieve_daily,
    
} achieve_type;

/***************************************************
	装备主机装备的界面 EquipPlaneLayer
 ****************************************************/
//显示飞机的界面层级
#define kZerorderPlaneNode 0
//显示装备的界面层级
#define kZerorderEquipNode 1
//显示飞机的界面标记
#define kTagPlaneNode 2
//显示装备的界面标记
#define kTagEquipNode 3
//显示飞机装备组的标记
#define kTagEquipGroup 301
//显示飞机装备详细的标记
#define kTagEquipDetail 217

//主炮标记
#define kTagHero 0
//主炮标记
#define kTagMainGun 0
//僚机标记
#define kTagWing 3
//副炮1标记
#define kTagSencodaryGun1 1
//副炮2标记
#define kTagSencodaryGun2 2

//装备栏装备起始标记
#define kTagEquipItem 100
//已拥有装备起始标记
#define kTagHaveItem 200
//商城装备起始标记
#define kTagShopItem 300

//主炮装备强化阶段上限
#define equipment_main_strengthen_upper 2
//副炮装备强化阶段上限
#define equipment_deputy_strengthen_upper 5

//装备的类型
enum
{
    EQUIPMENT_TYPE_MAIN_GUN = 1,        //主炮
    EQUIPMENT_TYPE_LEFT_GUN,  //副炮1
    EQUIPMENT_TYPE_RIGHT_GUN, //副炮2
    EQUIPMENT_TYPE_WING,                //僚机
};

//装备分类
enum
{
    EQUIPMENT_KIND_MAIN_GUN = 0, //主炮
    EQUIPMENT_KIND_MAIN_BEAR,    //肩炮
    EQUIPMENT_KIND_MAIN_WING,    //翼炮
};

//主炮装备技能类型
enum
{
    MAINGUN_SKILL_TYPE_NULL = 0,          //没有技能
    MAINGUN_SKILL_TYPE_SANDS_OF_TIME,     //时间之沙
    MAINGUN_SKILL_TYPE_WAVE_GUN,	          //光波炮
    MAINGUN_SKILL_TYPE_SHAPED_GUN,            //聚能炮
    MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING,      //影遁
    MAINGUN_SKILL_TYPE_REPULSION_SHIELD,      //斥力盾
    MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING,  //定位狙击
};

//僚机技能类型
enum
{
    WING_SKILL_TYPE_DEFENSE_BOMBS = 7,    //防御敌弹
    WING_SKILL_TYPE_PROPS_WAREHOUSE,   //道具仓
    WING_SKILL_TYPE_ADDED_BLOOD,       //血球
};

//装备的品级
enum
{
    EQUIP_QUALITY_WHITE = 1,    //白色
    EQUIP_QUALITY_GREEN,        //绿色
    EQUIP_QUALITY_BLUE,         //蓝色
    EQUIP_QUALITY_PURPLE,       //紫色
    EQUIP_QUALITY_ORRANGE,      //橙色
};


//装备附加属性
typedef enum
{
    eEquip_extAttribute_nothing = 0,  //没有附加属性
    eEquip_extAttribute_addAttack,    //攻击加成
    eEquip_extAttribute_addHp,        //血量加成
    eEquip_extAttribute_addToolDrop,  //道具掉率
    eEquip_extAttribute_addHpDrop,    //血量掉率
} EQUIP_EXT_ATTRIBUTE;

//道具类型
enum
{
    TOOLS_TYPE_INVINCIBLE,        //301  生成一个短时间无敌护罩
    TOOLS_TYPE_RECOVERHEALTH,     //302  回复满格血量
    TOOLS_TYPE_MAKECOIN,          //303  把屏幕中的敌机子弹变成金币
    TOOLS_TYPE_UPDATEATTACK,      //304  升级主机武器攻击（弹道、攻击力）
    TOOLS_TYPE_VERTICALBULLET,             //305  竖向单道子弹
    TOOLS_TYPE_DENSEBULLET,       //306  密集子弹
    TOOLS_TYPE_TRACEMISSILE,      //307  追踪导弹
    TOOLS_TYPE_LOCKTARGET,        //308  多道激光定目标攻击
    TOOLS_TYPE_CLEARMISSILE,      //309  清屏导弹
};
//碰撞矩形的类型
typedef enum
{
    COLLISIONRECT_TYPE_COMMON = 1,     //与子弹碰撞检测的矩形
    COLLISIONRECT_TYPE_SPECIAL,        //与特殊物体碰撞检测的矩形
    COLLISIONRECT_TYPE_CHECKCOIN,      //金币检测框
    COLLISIONRECT_TYPE_CHECKTOOL,      //道具检测框
    COLLISIONRECT_TYPE_CHECKBLOOD,     //血球检测框
}RectType;

//激光的类型
typedef enum
{
    LASER_LINE = 1,    //线型激光
    LASER_IMAGE,       //图片型激光
    LASER_SHADER,      //shader型激光
}LaserType;

//子弹击中效果类型
enum
{
    BULLET_HIT_TYPE1 = 1,//图片效果：子弹1-10 12 20使用
    BULLET_HIT_TYPE2,    //图片效果：子弹13使用
    BULLET_HIT_TYPE3,    //粒子效果：子弹16 19 14 15使用
    BULLET_HIT_TYPE4,    //图片效果：子弹18使用
    BULLET_HIT_TYPE5,    //图片效果：子弹17使用
    BULLET_HIT_TYPE6,    //
    BULLET_HIT_TYPE7,    //
};
//子弹发射效果类型
enum
{
    BULLET_SHOOT_SHOOT1 = 1,
    BULLET_SHOOT_SHOOT2,
};
//子弹拖尾效果类型
enum
{
    BULLET_STREAK_TYPE1 = 1, //子弹11使用
    BULLET_STREAK_TYPE2,     //子弹12使用
    BULLET_STREAK_TYPE3,     //子弹18使用
    BULLET_STREAK_TYPE4,     //子弹20使用
    BULLET_STREAK_TYPE5,     //子弹僚机使用
    BULLET_STREAK_TYPE6,     //子弹17使用
};
//子弹拖尾粒子效果类型
enum
{
    BULLET_STREAK_PARTICLE_TYPE1 = 1, //子弹11使用
    BULLET_STREAK_PARTICLE_TYPE2,     //子弹17使用
    BULLET_STREAK_PARTICLE_TYPE3,     //子弹20使用
    BULLET_STREAK_PARTICLE_TYPE4,     //子弹21使用
    BULLET_STREAK_PARTICLE_TYPE5,     //子弹9使用
};
//标示游戏界面
enum
{
    GAME_SCENE_TYPE_MAIN,
    GAME_SCENE_TYPE_BATTLE,
};
#endif