//
//  BackgroundView.cpp
//  planeGame
//
//  Created by mini01 on 14-9-17.
//
//

#include "BackgroundView.h"

BackgroundView::BackgroundView()
{
    //加载上层地图资源
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/map/background_up.plist");
}

BackgroundView::~BackgroundView()
{
    CC_SAFE_RELEASE_NULL(m_pArrayMapUp);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("game/map/background_up.plist");
}

bool BackgroundView::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    //初始化数据
    isRun = true;
    isSwitchng = false;
    
    currentMapTag = 1;
    currentTime = 0.0f;
    timeup = 0.0f;
    
    allMapSpeed = 0.0f;
    downMapSpeed = MAP_DOWN_SPEED;
    upMapSpeed = MAP_UP_SPEED;
    upMapTime = MAP_APPEAR_RATE;
    
    //初始化地图数组
    m_pArrayMapUp = CCArray::create();
    m_pArrayMapUp->retain();
    
    this->setMapScale((_s_width + 60.0f) / 540.0f);
    
    //加载底层地图资源
    CCString* mapPath = CCString::createWithFormat("game/map/map_%d.jpg",currentMapTag);

    map1 = CCSprite::create(mapPath->getCString());
    map1->setPosition(_s_center);
    map1->setScale(mapScale);
    map1->retain();
    addChild(map1);
    
    map2 = CCSprite::create(mapPath->getCString());
    map2->setPosition(ccp(_s_center.x, map1->getPositionY() + map1->getContentSize().height*map1->getScaleY()));
    map2->setScale(mapScale);
    map2->retain();
    addChild(map2);
    
    return true;
}

void BackgroundView::moveStart()
{

    this->schedule(schedule_selector(BackgroundView::updateMap));
    
    this->scheduleUpdate();
}

void BackgroundView::update(float dt)
{
    if (isRun) {
        
        timeup += dt;
        
        if (timeup >= upMapTime) {
            
            int id = 0;
            
            switch (currentMapTag)
            {
                case 1:
                {
                    int arr_1[] = {11,12,13,14,15};
                    id = arr_1[getRandom(0, 4)];
                }
                    break;
                case 3:
                {
                    int arr_2[] = {1,2,3,4,16,17};
                    id = arr_2[getRandom(0, 5)];
                }
                    break;
                case 5:
                {
                    int arr_3[] = {5,6,7,8,9};
                    id = arr_3[getRandom(0, 4)];
                }
                    break;
                case 7:
                {
                    int arr_4[] = {10,18,19,20};
                    id = arr_4[getRandom(0, 3)];
                }
                    break;
            }
            
            if (id > 0)
            {
                CCString* str = CCString::createWithFormat("bg_up_%d.png",id);
                
                CCSprite* map = CCSprite::createWithSpriteFrameName(str->getCString());
                map->setScaleX(mapScale);
                map->setPosition(ccp(_s_center.x, _s_top + map->getContentSize().height));
                addChild(map);
                
                m_pArrayMapUp->addObject(map);
                
                timeup = 0.0f;
                
            }

        }
        
    }
    
    if (m_pArrayMapUp->count()>0)
    {
        for (int i = 0; i < m_pArrayMapUp->count(); i++)
        {
            CCSprite* map  = (CCSprite*)m_pArrayMapUp->objectAtIndex(i);
            map->setPosition(ccp(map->getPositionX(), map->getPositionY() - (upMapSpeed + allMapSpeed)*this->getScheduler()->getTimeScale()));
            
            if (map->getPositionY() + map->getContentSize().height <= _s_bottom)
            {
                map->removeFromParent();
                
            }
        }
    }
}

void BackgroundView::updateMap(float dt)
{
    
    float pos_y_map1 = map1->getPositionY();
    float pos_y_map2 = map2->getPositionY();
    
    map1->setPosition(ccp(map1->getPositionX(), pos_y_map1 - (downMapSpeed + allMapSpeed)*this->getScheduler()->getTimeScale()));
    map2->setPosition(ccp(map2->getPositionX(), pos_y_map2 - (downMapSpeed + allMapSpeed)*this->getScheduler()->getTimeScale()));
    
    if (!isRun && map1->getPositionY() < -map1->getContentSize().height/2*map1->getScaleY()) {
        isRun = true;
        
        //设置敌机飞机可以飞出
        mapdalegate->enemyFly();
        
        downMapSpeed = MAP_DOWN_SPEED;
    }
    
    CCString* mapPath = CCString::createWithFormat("game/map/map_%d.jpg",currentMapTag);
    
    if(map1->getPositionY() < -map1->getContentSize().height/2*map1->getScaleY())
    {
        //CCLog("++++++++++++++++++++++%d",currentMapTag);
        CCSprite* s = CCSprite::create(mapPath->getCString());
        //s->setScale(1.4);
        map1->setTexture(s->getTexture());
        
        if (currentMapTag % 2 == 0)
        {
            isRun = false;
            
            currentMapTag++;
            
            if (currentMapTag > 8)
            {
                currentMapTag = 1;
            }
        }
        
		map1->setPosition(ccp(_s_center.x,map2->getPositionY()+map2->getContentSize().height*map2->getScaleY()));

    }
    
    if (!isRun && map2->getPositionY() < -map2->getContentSize().height/2*map2->getScaleY()) {
        isRun = true;
        
        //设置敌机飞机可以飞出
        mapdalegate->enemyFly();
        
        downMapSpeed = MAP_DOWN_SPEED;
    }
    
    mapPath = CCString::createWithFormat("game/map/map_%d.jpg",currentMapTag);
    
    
	if(map2->getPositionY() < -map2->getContentSize().height/2*map1->getScaleY())
    {
        //CCLog("---------------------%d",currentMapTag);
        CCSprite* s = CCSprite::create(mapPath->getCString());
        map2->setTexture(s->getTexture());
        
        if (currentMapTag % 2 == 0)
        {
            isRun = false;
            
            currentMapTag++;
            if (currentMapTag > 8)
            {
                currentMapTag = 1;
            }
        }
        
		map2->setPosition(ccp(map1->getPositionX(),map1->getPositionY()+map1->getContentSize().height*map1->getScaleY()));
    }
    
}

void BackgroundView::switchingMap()
{
    if (!isSwitchng && currentMapTag % 2 != 0) {
        currentMapTag++;
        
        downMapSpeed = MAP_CHANGE_SPEED;
        isSwitchng = true;
    }
}













