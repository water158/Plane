//
//  BackgroundView.h
//  planeGame
//
//  Created by mini01 on 14-9-17.
//
//

#ifndef __planeGame__BackgroundView__
#define __planeGame__BackgroundView__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "SmartRes.h"
#include "FerrisTools.h"
#include "GlobalDelegate.h"
#include "GlobalTagZOrder.h"

USING_NS_CC;

class BackgroundView : public CCLayer
{
public:
    BackgroundView();
    ~BackgroundView();
    
    virtual bool init();
    
    CREATE_FUNC(BackgroundView);
    
    //更新上层地图
    void update(float dt);
    
    //底层地图逻辑更新
    void updateMap(float dt);
    
    //切换地图
    void switchingMap();
    
    //开启地图移动
    void moveStart();
    
    //map代理
    MapDelegate* mapdalegate;
private:
    //当前地图的tag值
    CC_SYNTHESIZE(int, currentMapTag, CurrentMapTag);
    
    //上层地图的移动速度
    CC_SYNTHESIZE(float, upMapSpeed, UpMapSpeed);
    
    //底层地图的移动速度
    CC_SYNTHESIZE(float, downMapSpeed, DownMapSpeed);
    
    //切换底层地图
    CC_SYNTHESIZE(bool, isSwitchng, IsSwitching);
    
    //地图整体移动速度
    CC_SYNTHESIZE(float, allMapSpeed, AllMapSpeed);
    
    //上层地图出现频率
    CC_SYNTHESIZE(float, upMapTime, UpMapTime);
    
    //控制上层地图显示
    bool isRun;
    
    //底层地图精灵
    CCSprite* map1;
    CCSprite* map2;
    
    //数组存放所有上层地图的精灵
    CC_SYNTHESIZE(CCArray*, m_pArrayMapUp, ArrayMapUp);
    
    //地图缩放值
    CC_SYNTHESIZE(float, mapScale, MapScale);
    
    //底层地图显示时间间隔
    float currentTime;
    
    //上层地图显示时间间隔
    float timeup;
};

#endif /* defined(__planeGame__BackgroundView__) */
