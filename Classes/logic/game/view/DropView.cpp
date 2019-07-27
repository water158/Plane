//
//  DropView.cpp
//  ambition
//
//  Created by mini02 on 15/1/14.
//
//

#include "DropView.h"
#include "HeroView.h"

DropView::DropView()
{
    delegateDrop = NULL;
    heroTmp = NULL;
}

DropView::~DropView()
{

    CC_SAFE_RELEASE(dataDrop);

    //CC_SAFE_RELEASE(heroTmp);
    
}

DropView* DropView::create(DropData* data)
{
    DropView* drop = new DropView();
    if (drop && drop->init(data))
    {
        drop->autorelease();
        return drop;
    }

    return NULL;
}

bool DropView::init(DropData* data)
{
    CCNode::init();
    
    if (!data)
    {
        return false;
    }
    
    dataDrop = data;
    dataDrop->retain();
    
    this->setIsCollisionWithHero(false);
    this->setIsDied(false);
    
    //改变形态
    this->changeShape();
    
    return true;
}

//切换形态
void DropView::changeShape()
{
    //获取形态数据
    ShapesData* pShapesData = dataDrop->getShapesData();
    CCDictionary* dic = pShapesData->getDictShapesData();
    
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(dic, _ele)
    {
        ShapePartData* pPart = (ShapePartData*)_ele->getObject();
        //是不是动画
        bool bAnimation = pPart->getIsAnimation();
        if(bAnimation)
        {
            //加载图片
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pPart->getPlistPath().c_str());
            
            //Animation
            CCAnimation *anim = CCAnimation::create();
            anim->setDelayPerUnit(pPart->getDelay());
            
            //帧数
            int nNum = pPart->getFrameNum();
            for (int i = 0; i < nNum; i++)
            {
                string strName = pPart->getPlistName();
                
                strName.append("_").append(getStringFromInt(i + 1)).append(".png");
                
                CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strName.c_str());
                anim->addSpriteFrame(frame);
                
                //给飞机设置尺寸
                if(i == 0)
                {
                    this->setContentSize(CCSizeMake(frame->getRect().size.width, frame->getRect().size.height));
                }
                
                this->initWithSpriteFrame(frame);
            }
            
            this->runAction(CCRepeatForever::create(CCAnimate::create(anim)));
            this->setAnchorPoint(ccp(0.5, 0.5));
            this->setPosition(ccp(pPart->getX(), pPart->getY()));
            this->setScaleX(pPart->getScale_x());
            this->setScaleY(pPart->getScale_y());
            this->setRotation(pPart->getRotation());
            
        }
        else
        {
            //加载图片
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pPart->getPlistPath().c_str());
            
            string strName = pPart->getPlistName();
            strName.append(".png");
            
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strName.c_str());
            this->setContentSize(CCSizeMake(frame->getRect().size.width, frame->getRect().size.height));
            
            this->initWithSpriteFrame(frame);
            this->setAnchorPoint(ccp(0.5, 0.5));
            this->setPosition(ccp(pPart->getX(), pPart->getY()));
            this->setScaleX(pPart->getScale_x());
            this->setScaleY(pPart->getScale_y());
            this->setRotation(pPart->getRotation());
            
        }
    }
}

//获取碰撞矩形 通过类型
CCRect DropView::getCollisionRect(RectType type)
{
    //判断碰撞框类型
    int key = 0;
    
    if (type == COLLISIONRECT_TYPE_COMMON)
    {
        key = COLLISIONRECT_TYPE_COMMON;
    }
    else if (type == COLLISIONRECT_TYPE_SPECIAL)
    {
        key = COLLISIONRECT_TYPE_SPECIAL;
    }
    else if (type == COLLISIONRECT_TYPE_CHECKCOIN)
    {
        key = COLLISIONRECT_TYPE_CHECKCOIN;
    }
    else if (type == COLLISIONRECT_TYPE_CHECKTOOL)
    {
        key = COLLISIONRECT_TYPE_CHECKTOOL;
    }
    else if (type == COLLISIONRECT_TYPE_CHECKBLOOD)
    {
        key = COLLISIONRECT_TYPE_CHECKBLOOD;
    }
    
    string keys = getStringFromInt(key);
    
    //所有碰撞数据
    CollisionsData* pCollisionData = this->dataDrop->getCollisionsData();
    //碰撞组数据
    CollisionGroupData* pGroup = (CollisionGroupData*)pCollisionData->getCollisionGroupDataByKeys(keys.c_str());
    
    CCDictionary* pDict = pGroup->getDictCollisionGroupRects();
    
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(pDict, _ele)
    {
        CollisionRectData* pRectData = (CollisionRectData*)_ele->getObject();
        
        float x1 = pRectData->getX();
        float y1 = pRectData->getY();
        float w1 = pRectData->getWidth();
        float h1 = pRectData->getHeight();
        
        return CCRectMake(x1 + this->getPositionX(), y1 + this->getPositionY(), w1, h1);
    }
    
    return CCRect(0,0,0,0);
}

void DropView::updateDrop(float dt)
{
    if (!heroTmp && heroTmp->getIsDied())
    {
        return;
    }
    
    //如果存在动作
    MagnetCoinAction* act = (MagnetCoinAction*)this->getActionByTag(TAG_SPEEDUP_ACTION);
    
    if(act)
    {
        CCPoint pos = heroTmp->getPosition();
        act->setEndpos(pos);
    }
}

void DropView::startUpdateDrop()
{
    this->schedule(schedule_selector(DropView::updateDrop), 1.0f / 60);
}