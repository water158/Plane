LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/func/FerrisDialog.cpp \
                              ../../Classes/func/FerrisDrawLine.cpp \
                              ../../Classes/func/FerrisMaskLayer.cpp \
                              ../../Classes/func/FerrisMenu.cpp \
                              ../../Classes/func/FerrisMotionStreak.cpp \
                              ../../Classes/func/FerrisParticle.cpp \
                              ../../Classes/func/FerrisScrollMenu.cpp \
                              ../../Classes/func/FerrisTools.cpp \
                              ../../Classes/func/FilterNode.cpp \
                              ../../Classes/func/JumpByAction.cpp \
                              ../../Classes/func/lightning_common/ensDefine.cpp \
                              ../../Classes/func/lightning_common/ensFunc.cpp \
                              ../../Classes/func/lightning_common/ensGLProgramWithUnifos.cpp \
                              ../../Classes/func/lightning_common/ensIndexVBO.cpp \
                              ../../Classes/func/lightning_common/ensMath.cpp \
                              ../../Classes/func/lightning_common/ensMesh.cpp \
                              ../../Classes/func/lightning_common/ensSimpleClasses.cpp \
                              ../../Classes/func/LightningBoltAction.cpp \
                              ../../Classes/func/LightningBoltNode.cpp \
                              ../../Classes/func/LightningBoltSprite.cpp \
                              ../../Classes/func/LineSegment.cpp \
                              ../../Classes/func/MagnetCoinAction.cpp \
                              ../../Classes/func/MyAnimationManager.cpp \
                              ../../Classes/func/OvalPathAction.cpp \
                              ../../Classes/func/RoundPathAction.cpp \
                              ../../Classes/func/SmartRes.cpp \
                              ../../Classes/func/SpriteAnimationNew.cpp \
                              ../../Classes/func/ArcPathAction.cpp \
                              ../../Classes/func/BulletRoundPathAction.cpp \
                              ../../Classes/global/GlobalCache.cpp \
                              ../../Classes/global/GlobalParams.cpp \
                              ../../Classes/global/GlobalFunc.cpp \
                              ../../Classes/logic/DataHelper.cpp \
                              ../../Classes/logic/DataManager.cpp \
                              ../../Classes/logic/EquipNameLibrary.cpp \
                              ../../Classes/logic/game/Achieve.cpp \
                              ../../Classes/logic/game/AchieveItem.cpp \
                              ../../Classes/logic/game/DailyTask.cpp \
                              ../../Classes/logic/game/GunDetail.cpp \
                              ../../Classes/logic/game/HookPoint.cpp \
                              ../../Classes/logic/game/LaserBeam.cpp \
                              ../../Classes/logic/game/SignManager.cpp \
                              ../../Classes/logic/game/StatID.cpp \
                              ../../Classes/logic/game/TraceAction.cpp \
                              ../../Classes/logic/game/data/BaseReward.cpp \
                              ../../Classes/logic/game/data/BatteryData.cpp \
                              ../../Classes/logic/game/data/BatteryPositionData.cpp \
                              ../../Classes/logic/game/data/BatteryPosManager.cpp \
                              ../../Classes/logic/game/data/BulletData.cpp \
                              ../../Classes/logic/game/data/CollisionGroupData.cpp \
                              ../../Classes/logic/game/data/CollisionRectData.cpp \
                              ../../Classes/logic/game/data/CollisionsData.cpp \
                              ../../Classes/logic/game/data/ConditionData.cpp \
                              ../../Classes/logic/game/data/ConditionTargetData.cpp \
                              ../../Classes/logic/game/data/CraftData.cpp \
                              ../../Classes/logic/game/data/DailyCountData.cpp \
                              ../../Classes/logic/game/data/DropData.cpp \
                              ../../Classes/logic/game/data/EquipItem.cpp \
                              ../../Classes/logic/game/data/EquipItemData.cpp \
                              ../../Classes/logic/game/data/GameCountData.cpp \
                              ../../Classes/logic/game/data/GameData.cpp \
                              ../../Classes/logic/game/data/GunData.cpp \
                              ../../Classes/logic/game/data/GunGroupData.cpp \
                              ../../Classes/logic/game/data/MiscData.cpp \
                              ../../Classes/logic/game/data/MissionData.cpp \
                              ../../Classes/logic/game/data/ObjectData.cpp \
                              ../../Classes/logic/game/data/PathData.cpp \
                              ../../Classes/logic/game/data/PathGroupData.cpp \
                              ../../Classes/logic/game/data/PhaseData.cpp \
                              ../../Classes/logic/game/data/PhraseTimeLineData.cpp \
                              ../../Classes/logic/game/data/PositionData.cpp \
                              ../../Classes/logic/game/data/RemoveData.cpp \
                              ../../Classes/logic/game/data/RemoveTargetData.cpp \
                              ../../Classes/logic/game/data/ResultCountData.cpp \
                              ../../Classes/logic/game/data/ShapePartData.cpp \
                              ../../Classes/logic/game/data/ShapesData.cpp \
                              ../../Classes/logic/game/data/SkillData.cpp \
                              ../../Classes/logic/game/data/SkillLevelData.cpp \
                              ../../Classes/logic/game/data/StorePriceData.cpp \
                              ../../Classes/logic/game/data/TargetGunGroupData.cpp \
                              ../../Classes/logic/game/data/TargetPlaneData.cpp \
                              ../../Classes/logic/game/data/TestTimeData.cpp \
                              ../../Classes/logic/game/data/TimeData.cpp \
                              ../../Classes/logic/game/data/TimePlaneData.cpp \
                              ../../Classes/logic/game/data/TrailGroupData.cpp \
                              ../../Classes/logic/game/layer/CrystalStoreLayer.cpp \
                              ../../Classes/logic/game/layer/EquipBatteryLayer.cpp \
                              ../../Classes/logic/game/layer/EquipPlaneLayer.cpp \
                              ../../Classes/logic/game/layer/GameCombatLayer.cpp \
                              ../../Classes/logic/game/layer/SelectPlaneLayer.cpp \
                              ../../Classes/logic/game/layer/lightning.cpp \
                              ../../Classes/logic/game/layer/LaserLayer.cpp \
                              ../../Classes/logic/game/layer/DynamicLaser.cpp \
                              ../../Classes/logic/game/skill/SkillAddedBlood.cpp \
                              ../../Classes/logic/game/skill/SkillDefenseBombs.cpp \
                              ../../Classes/logic/game/skill/SkillNull.cpp \
                              ../../Classes/logic/game/skill/SkillPositioningShooting.cpp \
                              ../../Classes/logic/game/skill/SkillPropsWareHouse.cpp \
                              ../../Classes/logic/game/skill/SkillRepulsionShield.cpp \
                              ../../Classes/logic/game/skill/SkillSandsOfTime.cpp \
                              ../../Classes/logic/game/skill/SkillShadowOfHiding.cpp \
                              ../../Classes/logic/game/skill/SkillShapedGun.cpp \
                              ../../Classes/logic/game/skill/SkillWave.cpp \
                              ../../Classes/logic/game/view/BackgroundView.cpp \
                              ../../Classes/logic/game/view/BloodView.cpp \
                              ../../Classes/logic/game/view/BulletView.cpp \
                              ../../Classes/logic/game/view/CoinView.cpp \
                              ../../Classes/logic/game/view/CraftView.cpp \
                              ../../Classes/logic/game/view/DropView.cpp \
                              ../../Classes/logic/game/view/EnemyView.cpp \
                              ../../Classes/logic/game/view/EquipView.cpp \
                              ../../Classes/logic/game/view/HeroView.cpp \
                              ../../Classes/logic/game/view/ScoreView.cpp \
                              ../../Classes/logic/game/view/ToolsView.cpp \
                              ../../Classes/logic/game/view/WingView.cpp \
                              ../../Classes/logic/LoadHelper.cpp \
                              ../../Classes/logic/MissionDataManager.cpp \
                              ../../Classes/logic/ParticleHelper.cpp \
                              ../../Classes/logic/PathLibrary.cpp \
                              ../../Classes/logic/PurchaseLogManager.cpp \
                              ../../Classes/logic/ShareManager.cpp \
                              ../../Classes/logic/SkillLibrary.cpp \
                              ../../Classes/logic/StringLibrary.cpp \
                              ../../Classes/logic/TrailLibrary.cpp \
                              ../../Classes/logic/ComplaintManager.cpp \
                              ../../Classes/logic/CouponManager.cpp \
                              ../../Classes/logic/VerificationManager.cpp \
                              ../../Classes/logic/AnalyticsStatID.cpp \
                              ../../Classes/logic/SoundEffectManager.cpp \
                              ../../Classes/logic/UserData.cpp \
                              ../../Classes/logic/UserDataMission.cpp \
                              ../../Classes/logic/UserEquip.cpp \
                              ../../Classes/logic/UserEquipBag.cpp \
                              ../../Classes/logic/WeaponLibrary.cpp \
                              ../../Classes/ui/EnergyStoreLayer.cpp \
                              ../../Classes/ui/GameAboutLayer.cpp \
                              ../../Classes/ui/GameAccountLayer.cpp \
                              ../../Classes/ui/GameAchieveLayer.cpp \
                              ../../Classes/ui/GameBattleUILayer.cpp \
                              ../../Classes/ui/GameConfigLayer.cpp \
                              ../../Classes/ui/GameLotteryLayer.cpp \
                              ../../Classes/ui/GameMainLayer.cpp \
                              ../../Classes/ui/GameSigninLayer.cpp \
                              ../../Classes/ui/GameStartLayer.cpp \
                              ../../Classes/ui/GemStoreLayer.cpp \
                              ../../Classes/ui/GetEquipLayer.cpp \
                              ../../Classes/ui/GoldStoreLayer.cpp \
                              ../../Classes/ui/LoadingLayer.cpp \
                              ../../Classes/ui/SplashLayer.cpp \
                              ../../Classes/ui/NotificationLayer.cpp \
                              ../../Classes/ui/PauseLayer.cpp \
                              ../../Classes/ui/ReviveLayer.cpp \
                              ../../Classes/ui/SoundEffectLayer.cpp \
                              ../../Classes/misc/MiscDefination.cpp \
                              ../../Classes/misc/payment/android/PaymentAndroid.cpp \
                              ../../Classes/misc/sharesdk/C2DXShareSDK.cpp \
                              ../../Classes/misc/sharesdk/android/ShareSDKUtils.cpp \
                              ../../Classes/misc/sharesdk/android/JSON/CCJSONConverter.cpp \
                              ../../Classes/misc/sharesdk/android/JSON/cJSON/cJSON.c \
                              ../../Classes/misc/umeng/analytics/MobClickCpp.cpp \
                              ../../Classes/misc/ndkhelper/NDKHelper.cpp \
                              ../../Classes/misc/ndkhelper/NDKCallbackNode.cpp \
                              ../../Classes/misc/ndkhelper/jansson/dump.c \
                              ../../Classes/misc/ndkhelper/jansson/error.c \
                              ../../Classes/misc/ndkhelper/jansson/hashtable.c \
                              ../../Classes/misc/ndkhelper/jansson/load.c \
                              ../../Classes/misc/ndkhelper/jansson/memory.c \
                              ../../Classes/misc/ndkhelper/jansson/pack_unpack.c \
                              ../../Classes/misc/ndkhelper/jansson/strbuffer.c \
                              ../../Classes/misc/ndkhelper/jansson/strconv.c \
                              ../../Classes/misc/ndkhelper/jansson/utf.c \
                              ../../Classes/misc/ndkhelper/jansson/value.c \
                              ../../Classes/misc/tinyxml/tinystr.cpp \
                              ../../Classes/misc/tinyxml/tinyxml.cpp \
                              ../../Classes/misc/tinyxml/tinyxmlerror.cpp \
                              ../../Classes/misc/tinyxml/tinyxmlparser.cpp \
                              ../../Classes/GetRandomEquip.cpp \
                              ../../Classes/GameDelegate.cpp \
                              ../../Classes/AppDelegate.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/func
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/func/lightning_common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/global
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/logic
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/logic/game
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/logic/game/skill
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/logic/game/data
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/logic/game/layer
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/logic/game/view
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/ui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/payment/android
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/sharesdk/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/sharesdk/android
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/sharesdk/android/JSON
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/sharesdk/android/JSON/cJSON
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/umeng/analytics
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/tinyxml
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/ndkhelper
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/misc/ndkhelper/jansson

LOCAL_LDLIBS := $(MTL_ENV)/libs/android_libs/cocos2dx2_libMobClickCpp.a

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)