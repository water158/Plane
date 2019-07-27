//
//  GlobalFunc.h
//  ambition
//
//  Created by mini02 on 15/3/26.
//
//

#ifndef ambition_GlobalFunc_h
#define ambition_GlobalFunc_h

#include "GlobalEnum.h"
#include "FerrisTools.h"
#include "CraftView.h"

//返回掉落道具ID
int getToolShapeID(int type);

//根据敌机大小类型返回敌机炮管ID前缀
int getCannonPrefixID(int type);

//根据游戏波数返回敌机炮管ID后缀
int getCanninSuffixID(int wave);

//根据游戏波数和炮管阶段数返回敌机炮的阶段
int getCannonForPhrase(int wave,int phraseCount);

//获取敌机的型号
int getCraftSizeType(CraftView* craft);

//获取各个敌机的子分类
int getChildSizeType(CraftView* craft);

//返回敌机随机移动中心点
CCPoint getEnemyRandomMoveCenter(CraftView* craft);

//返回敌机随机移动最大速度
float getEnemyRandomMoveSpeed(CraftView* craft);

//返回敌机随机移动速度从0到最大运行的距离
float getEnemyRandomMoveLen(CraftView* craft);

//返回敌机随机移动-移动的最小宽高ccpoint(mw,mh)
CCPoint getEnemyRandomMoveForMin(CraftView* craft);

//返回敌机随机移动-移动的最大宽高ccpoint(mw,mh)
CCPoint getEnemyRandomMoveForMax(CraftView* craft);

//根据敌机类型获取敌机掉落金币的个数
int getCoinNumByType(int craftType);

//根据敌机类型获取敌机爆炸后的光圈缩放值
float getScaleCircleByType(int craftType);

//根据敌机类型获取敌机爆炸动画1缩放值
float getScaleAnimBoomByType(int craftType);

#endif
