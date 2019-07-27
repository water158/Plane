//
//  GlobalFunc.cpp
//  ambition
//
//  Created by mini02 on 15/3/26.
//
//

#include <stdio.h>
#include "GlobalFunc.h"

//返回掉落道具ID
int getToolShapeID(int type)
{
    int temp = -1;
    
    switch (type)
    {
        case TOOLS_TYPE_INVINCIBLE:        //301  生成一个短时间无敌护罩
            temp = 301;
            break;
            
        case TOOLS_TYPE_RECOVERHEALTH:     //302  回复满格血量
            temp = 302;
            break;
            
        case TOOLS_TYPE_MAKECOIN:          //303  把屏幕中的敌机子弹变成金币
            temp = 303;
            break;
            
        case TOOLS_TYPE_UPDATEATTACK:      //304  升级主机武器攻击（弹道、攻击力）
            temp = 304;
            break;
            
        case TOOLS_TYPE_VERTICALBULLET:             //305  竖向单道激光
            temp = 305;
            break;
            
        case TOOLS_TYPE_DENSEBULLET:       //306  密集子弹
            temp = 306;
            break;
            
        case TOOLS_TYPE_TRACEMISSILE:      //307  追踪导弹
            temp = 307;
            break;
            
        case TOOLS_TYPE_LOCKTARGET:        //308  多道激光定目标攻击
            temp = 308;
            break;
            
        case TOOLS_TYPE_CLEARMISSILE:      //309  清屏导弹
            temp = 309;
            break;
    }
    
    return temp;
}

//根据敌机大小类型返回敌机炮管ID前缀  例：11_1 返回11
int getCannonPrefixID(int type)
{
    int tempID = -1;
    
    switch (type)
    {
        case CRAFT_SIZE_SMALL://小飞机
        {
            int ids[13] = {11,12,13,20,22,23,24,25,26,27,29,33,34};
            
            tempID = ids[getRandom(0,12)];
        }
            break;
        case CRAFT_SIZE_MID://中飞机
        case CRAFT_SIZE_BIG://大飞机
        case CRAFT_SIZE_SMALLBOSS://小boss
        case CRAFT_SIZE_BOSS://大boss
            tempID =getRandom(ENEMYCANNON_MIN_NUM, ENEMYCANNON_MAX_NUM);
            break;
    }
    
    return tempID;
}

//根据游戏波数返回敌机炮管ID后缀
int getCanninSuffixID(int wave)
{
    int tempID = -1;
    
    if (wave <= 10) {
        
        tempID = 1;
    }else if (wave <=20){
        
        tempID = getRandom(1, 2);
    }else if (wave <=30){
        
        tempID = 2;
    }else if (wave <=40){
        
        tempID = getRandom(2, 3);
    }
    else
    {
        tempID = 3;
    }
    
    return tempID;
}

//根据游戏波数返回敌机炮的阶段
int getCannonForPhrase(int wave,int phraseCount)
{
    int phrase = 1;
    
    if (wave <= 10)
    {
        phrase = 1;
    }
    else if (wave <= 30)
    {
        phrase = 2;
    }
    else if (wave <= 60)
    {
        phrase = 3;
    }
    else
    {
        //保证随机出的阶段大于炮管里面 最小的个数
        int nrandom = getRandom(1,(phraseCount - 3));
        phrase = nrandom + 3;
    }
    
    return phrase;
}

int getCraftSizeType(CraftView* craft)
{
    if(!craft)
    {
        return -1;
    }
    
    int nPlaneID = getIntFromS(craft->dataTargetPlane->getPlaneID().c_str());
    int hundred = nPlaneID / 100;
    
    if(hundred == 1)
    {
        return CRAFT_SIZE_SMALL;
    }
    else if(hundred == 2)
    {
        return CRAFT_SIZE_MID;
    }
    else if(hundred == 3)
    {
        return CRAFT_SIZE_BIG;
    }
    else if(hundred == 4)
    {
        return CRAFT_SIZE_SMALLBOSS;
    }
    else if(hundred == 5)
    {
        return CRAFT_SIZE_BOSS;
    }
    else
    {
        return -1;
    }
}

int getChildSizeType(CraftView* craft)
{
    if(!craft)
    {
        return -1;
    }
    
    int nPlaneID = getIntFromS(craft->dataTargetPlane->getPlaneID().c_str());
    int sizeLevel = nPlaneID % 100;
    
    if (sizeLevel == 1) {
        return CRAFT_SIZE_ONE;
    }else if (sizeLevel == 2)
    {
        return CRAFT_SIZE_TWO;
    }else if (sizeLevel == 3)
    {
        return CRAFT_SIZE_THRE;
    }else if (sizeLevel == 4)
    {
        return CRAFT_SIZE_FOUT;
    }else if (sizeLevel == 5)
    {
        return CRAFT_SIZE_FIVE;
    }else
    {
        
        return -1;
    }
    
}

//返回敌机随机移动中心点
CCPoint getEnemyRandomMoveCenter(CraftView* craft)
{
    int enemyType = getCraftSizeType(craft);
    
    CCPoint center = CCPointZero;
    
    switch (enemyType) {
        case CRAFT_SIZE_SMALL://101
            center = ccp(_s_width * 0.5f, _s_height * 0.741f);
            break;
        case CRAFT_SIZE_MID://201 202 203
            center = ccp(_s_width * 0.5f, _s_height * 0.745f);
            break;
        case CRAFT_SIZE_BIG://301 302 303
            center = ccp(_s_width * 0.5f, _s_height * 0.772f);
            break;
        case CRAFT_SIZE_SMALLBOSS://401 402 403
            center = ccp(_s_width * 0.5f, _s_height * 0.783f);
            break;
        case CRAFT_SIZE_BOSS://501 502
            center = ccp(_s_width * 0.5f, _s_height * 0.802f);
            break;
    }
    
    return center;
}

//返回敌机随机移动最大速度
float getEnemyRandomMoveSpeed(CraftView* craft)
{
    int enemyType = getCraftSizeType(craft);
    
    float speed = 0.0f;
    
    switch (enemyType) {
        case CRAFT_SIZE_SMALL://小
//            speed = 150;//60
            speed = 60;
            break;
        case CRAFT_SIZE_MID://中
//            speed = 100;//30
            speed = 30;
            break;
        case CRAFT_SIZE_BIG://大
//            speed = 100;//10
            speed = 10;
            break;
        case CRAFT_SIZE_SMALLBOSS://小boss
//            speed = 60;//10
            speed = 10;
            break;
        case CRAFT_SIZE_BOSS://boss
//            speed = 40;//10
            speed = 10;
            break;
    }
    
    return speed;
}

//返回敌机随机移动速度从0到最大运行的距离
float getEnemyRandomMoveLen(CraftView* craft)
{
    int enemyType = getCraftSizeType(craft);
    
    float len = 0.0f;
    
    switch (enemyType) {
        case CRAFT_SIZE_SMALL:
            len = 38;//38
            break;
        case CRAFT_SIZE_MID:
            len = 38;//38
            break;
        case CRAFT_SIZE_BIG:
            len = 30;//30
            break;
        case CRAFT_SIZE_SMALLBOSS:
            len = 15;//15
            break;
        case CRAFT_SIZE_BOSS:
            len = 10;//10
            break;
    }
    
    return len;
}

//返回敌机随机移动-移动的最小宽高ccpoint(mw,mh)
CCPoint getEnemyRandomMoveForMin(CraftView* craft)
{
    int enemyType = getCraftSizeType(craft);
    int level = getChildSizeType(craft);
    
    //x = minw y = minh
    CCPoint minLen = CCPointZero;
    
    switch (enemyType) {
        case CRAFT_SIZE_SMALL:
            minLen = ccp(90.0f, 90.0f);
            break;
        case CRAFT_SIZE_MID:
        {
            switch (level)
            {
                case CRAFT_SIZE_ONE:
                    minLen = ccp(100.0f, 50.0f);
                    break;
                case CRAFT_SIZE_TWO:
                case CRAFT_SIZE_THRE:
                    minLen = ccp(100.0f, 60.0f);
                    break;
            }
        }
            break;
        case CRAFT_SIZE_BIG:
            minLen = ccp(70.0f, 40.0f);
            break;
        case CRAFT_SIZE_SMALLBOSS:
        {
            switch (level)
            {
                case CRAFT_SIZE_ONE:
                case CRAFT_SIZE_TWO:
                    minLen = ccp(50.0f, 10.0f);
                    break;
                case CRAFT_SIZE_THRE:
                    minLen = ccp(60.0f, 10.0f);
                    break;
            }
        }
            break;
        case CRAFT_SIZE_BOSS:
            minLen = ccp(20.0f, 10.0f);
            break;
    }
    
    return minLen;
}

//返回敌机随机移动-移动的最大宽高ccpoint(mw,mh)
CCPoint getEnemyRandomMoveForMax(CraftView* craft)
{
    int enemyType = getCraftSizeType(craft);
    int level = getChildSizeType(craft);
    
    CCPoint maxLen = CCPointZero;
    
    switch (enemyType) {
        case CRAFT_SIZE_SMALL:
            maxLen = ccp(_s_width * 0.411f, _s_height * 0.145f);
            break;
        case CRAFT_SIZE_MID:
        {
            switch (level)
            {
                case CRAFT_SIZE_ONE:
                    maxLen = ccp(_s_width * 0.381f, _s_height * 0.065f);
                    break;
                case CRAFT_SIZE_TWO:
                    maxLen = ccp(_s_width * 0.315f, _s_height * 0.098f);
                    break;
                case CRAFT_SIZE_THRE:
                    maxLen = ccp(_s_width * 0.304f, _s_height * 0.112f);
                    break;
            }
        }
            break;
        case CRAFT_SIZE_BIG:
        {
            switch (level)
            {
                case CRAFT_SIZE_ONE:
                    maxLen = ccp(_s_width * 0.281f, _s_height * 0.052f);
                    break;
                case CRAFT_SIZE_TWO:
                    maxLen = ccp(_s_width * 0.224f, _s_height * 0.031f);
                    break;
            }
        }
            break;
        case CRAFT_SIZE_SMALLBOSS:
        {
            switch (level)
            {
                case CRAFT_SIZE_ONE:
                    maxLen = ccp(_s_width * 0.152f, _s_height * 0.026f);
                    break;
                case CRAFT_SIZE_TWO:
                    maxLen = ccp(_s_width * 0.139f, _s_height * 0.051f);
                    break;
                case CRAFT_SIZE_THRE:
                    maxLen = ccp(_s_width * 0.115f, _s_height * 0.021f);
                    break;
            }
        }
            break;
        case CRAFT_SIZE_BOSS:
        {
            switch (level)
            {
                case CRAFT_SIZE_ONE:
                    maxLen = ccp(_s_width * 0.030f, _s_height * 0.016f);
                    break;
                case CRAFT_SIZE_TWO:
                    maxLen = ccp(_s_width * 0.017f, _s_height * 0.043f);
                    break;
            }
        }
            break;
    }
    
    return maxLen;
}

//根据敌机类型获取敌机掉落金币的个数
int getCoinNumByType(int craftType)
{
    int nType = craftType;
    
    int nNum = 0;
    
    switch (nType)
    {
        case CRAFT_SIZE_SMALL:
            nNum = getRandom(1, 15);
            break;
        case CRAFT_SIZE_MID:
            nNum = getRandom(1, 25);
            break;
        case CRAFT_SIZE_BIG:
            nNum = getRandom(1, 30);
            break;
        case CRAFT_SIZE_SMALLBOSS:
            nNum = getRandom(1, 45);
            break;
        case CRAFT_SIZE_BOSS:
            nNum = getRandom(10, 50);
            break;
        default:
            nNum = 0;
            break;
    }
    
    return nNum;
}

//根据敌机类型获取敌机爆炸后的光圈缩放值
float getScaleCircleByType(int craftType)
{
    int nType = craftType;
    
    float fScale = 0.0f;
    
    switch (nType)
    {
        case CRAFT_SIZE_SMALL:
        case CRAFT_SIZE_MID:
        case CRAFT_SIZE_BIG:
            fScale = 4.5f;
            break;
        case CRAFT_SIZE_SMALLBOSS:
        case CRAFT_SIZE_BOSS:
            fScale = 18.0f;
            break;
        default:
            break;
    }
    
    return fScale;
}

//根据敌机类型获取敌机爆炸动画1缩放值
float getScaleAnimBoomByType(int craftType)
{
    int nType = craftType;
    
    float fScale = 0.0f;
    
    switch (nType)
    {
        case CRAFT_SIZE_MID:
            fScale = 3.7f;
            break;
        case CRAFT_SIZE_BIG:
            fScale = 4.7f;
            break;
        case CRAFT_SIZE_SMALLBOSS:
            fScale = 5.7f;
            break;
        case CRAFT_SIZE_BOSS:
            fScale = 6.0f;
            break;
        default:
            break;
    }
    
    return fScale;
}
