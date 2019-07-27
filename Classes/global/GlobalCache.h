//
//  GlobalCache.h
//  plane
//
//  Created by renyi on 14-01-12.
//
//

#ifndef __FERRIS_GAME__GlobalCache__
#define __FERRIS_GAME__GlobalCache__

#include "cocos2d.h"
#include "GlobalEnum.h"

#define gCache GlobalCache::sharedGlobalCache()

USING_NS_CC;
USING_NS_STD;

class GlobalCache
{
private:
    static GlobalCache* s_SharedGlobalCache;
    void init();
public:
    
    GlobalCache();
    ~GlobalCache();
    
    static GlobalCache* sharedGlobalCache();
    /***********************
     * bullet
     **********************/
    //cache for bullet add
    void addBulletDataToCache(string key, CCObject *data);
    
    //cache for bullet get
    CCObject* getBulletDataInCache(string key);
    
    //cache for bullet clear
    void clearBulletDataInCache();
    
    /***********************
     * craft
     **********************/
    //cache for craft add
    void addCraftDataToCache(string key, CCObject *data);
    
    //cache for craft get
    CCObject* getCraftDataInCache(string key);
    
    //cache for craft clear
    void clearCraftDataInCache();
    
    /***********************
     * path
     **********************/
    //cache for craft add
    void addPathDataToCache(string key, CCObject *data);
    
    //cache for craft get
    CCObject* getPathDataInCache(string key);
    
    //cache for craft clear
    void clearPathDataInCache();

	 /***********************
     * form
     **********************/
    //cache for craft add
    void addFormDataToCache(string key, CCObject *data);
    
    //cache for craft get
    CCObject* getFormDataInCache(string key);
    
    //cache for craft clear
    void clearFormDataInCache();
    
    /***********************
     * drop
     **********************/
    //cache for drop add
    void addDropDataToCache(string key, CCObject *data);
    
    //cache for drop get
    CCObject* getDropDataInCache(string key);
    
    //cache for drop clear
    void clearDropDataInCache();
    
private:
    
    //cache for bullet
    CCDictionary *cacheBulletData;
    
    //cache for craft
    CCDictionary *cacheCraftData;
    
    //cache for path
    CCDictionary *cachePathData;

	//cache for form
	CCDictionary *cacheFormData;
    
    //cache for drop
    CCDictionary *cacheDropData;
    
};

#endif