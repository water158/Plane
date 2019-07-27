//
//  GlobalCache.cpp
//  plane
//
//  Created by renyi on 14-01-12.
//
//

#include "GlobalCache.h"

GlobalCache* GlobalCache::s_SharedGlobalCache = NULL;

GlobalCache::GlobalCache()
{
    
}

GlobalCache::~GlobalCache()
{
    //CCLog("~GlobalCache");
    
    CC_SAFE_RELEASE(cacheBulletData);
    CC_SAFE_RELEASE(cacheCraftData);
    CC_SAFE_RELEASE(cacheDropData);
    
    if (s_SharedGlobalCache)
    {
        delete s_SharedGlobalCache;
    }
}

GlobalCache* GlobalCache::sharedGlobalCache()
{
    if (!s_SharedGlobalCache)
    {
        s_SharedGlobalCache = new GlobalCache();
        s_SharedGlobalCache->init();
    }
    
    return s_SharedGlobalCache;
}

void GlobalCache::init()
{
	cacheBulletData = CCDictionary::create();
	cacheBulletData->removeAllObjects();
	cacheBulletData->retain();

    cacheCraftData = CCDictionary::create();
    cacheCraftData->removeAllObjects();
    cacheCraftData->retain();
    
    cachePathData = CCDictionary::create();
    cachePathData->removeAllObjects();
    cachePathData->retain();
    
    cacheDropData = CCDictionary::create();
    cacheDropData->removeAllObjects();
    cacheDropData->retain();
}

/***********************
 * bullet
 **********************/

//cache for bullet add
void GlobalCache::addBulletDataToCache(string key, cocos2d::CCObject *data)
{
    cacheBulletData->setObject(data, key);
}

//cache for bullet get
CCObject* GlobalCache::getBulletDataInCache(string key)
{
    return cacheBulletData->objectForKey(key);
}

//cache for bullet clear
void GlobalCache::clearBulletDataInCache()
{
    cacheBulletData->removeAllObjects();
}

/***********************
 * craft
 **********************/

//cache for craft add
void GlobalCache::addCraftDataToCache(string key, cocos2d::CCObject *data)
{
    cacheCraftData->setObject(data, key);
}

//cache for craft get
CCObject* GlobalCache::getCraftDataInCache(string key)
{
    return cacheCraftData->objectForKey(key);
}

//cache for craft clear
void GlobalCache::clearCraftDataInCache()
{
    cacheCraftData->removeAllObjects();
}

/***********************
 * path
 **********************/

//cache for path add
void GlobalCache::addPathDataToCache(string key, cocos2d::CCObject *data)
{
    cachePathData->setObject(data, key);
}

//cache for path get
CCObject* GlobalCache::getPathDataInCache(string key)
{
    return cachePathData->objectForKey(key);
}

//cache for path clear
void GlobalCache::clearPathDataInCache()
{
    cachePathData->removeAllObjects();
}

/***********************
 * form
 **********************/

//cache for form add
void GlobalCache::addFormDataToCache(string key, cocos2d::CCObject *data)
{
    cacheFormData->setObject(data, key);
}

//cache for form get
CCObject* GlobalCache::getFormDataInCache(string key)
{
    return cacheFormData->objectForKey(key);
}

//cache for form clear
void GlobalCache::clearFormDataInCache()
{
    cacheFormData->removeAllObjects();
}

/***********************
 * drop
 **********************/

//cache for drop add
void GlobalCache::addDropDataToCache(string key, cocos2d::CCObject *data)
{
    cacheDropData->setObject(data, key);
}

//cache for drop get
CCObject* GlobalCache::getDropDataInCache(string key)
{
    return cacheDropData->objectForKey(key);
}

//cache for drop clear
void GlobalCache::clearDropDataInCache()
{
    cacheDropData->removeAllObjects();
}