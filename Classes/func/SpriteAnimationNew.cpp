#include "SpriteAnimationNew.h"
#include "MyAnimationManager.h"

SpriteAnimationNew::SpriteAnimationNew()
{
	//mysprite = NULL;
    pre = "";
}

SpriteAnimationNew::~SpriteAnimationNew()
{
    
}

SpriteAnimationNew *SpriteAnimationNew::create(const char *animation_name, int loop, float delay,CCActionInterval* action)
{
	SpriteAnimationNew *sa = new SpriteAnimationNew();

	if(sa && sa->init(animation_name, loop, delay,action))
	{
		sa->autorelease();
        return sa;
    }
	return NULL;
}

bool SpriteAnimationNew::init(const char *animation_name, int loop, float delay,CCActionInterval* action)
{
	if(!animation_name)
    {
        return false;
    }
    
	//初始化精灵为第一帧
    pre = animation_name;
	string last = pre + "_1.png";
    
	this->initWithSpriteFrameName(last.c_str());
    this->playAnimation(loop, delay,action);
    
    return true;
}

void SpriteAnimationNew::playAnimation(int loop, float delaytime,CCActionInterval* action)
{
    //通过标记获取动画
    CCAnimationCache *cache = CCAnimationCache::sharedAnimationCache();
    CCAnimation *animation = cache->animationByName(pre.c_str());
    animation->setDelayPerUnit(delaytime);
    
    if(loop < 0)
    {
        this->runAction(CCRepeatForever::create(CCSpawn::create(CCAnimate::create(animation),action,NULL)));
    }
    else
    {
        CCSequence *seq = CCSequence::create(CCRepeat::create(CCSpawn::create(CCAnimate::create(animation),action,NULL), loop),
                                             CCDelayTime::create(0.01f),
                                             CCCallFunc::create(this, callfunc_selector(SpriteAnimationNew::callbackCleanUP)),
                                             NULL);
        this->runAction(seq);
    }
    
}

void SpriteAnimationNew::setFrameByIndex(const char *ani_name,unsigned int index)
{
    //CCLog("SpriteAnimationNew::setFrameByIndex");
	this->stopAllActions();
    
	CCSpriteFrame *spframe = sAnimationManager->getAniFrameByIndex(ani_name, index);
	CCArray *array = CCArray::create();
	array->addObject(spframe);
	CCAnimation *animation = CCAnimation::create(array,0.1f);
	this->runAction(CCAnimate::create(animation));
}

void SpriteAnimationNew::stopAnimation()
{
    //CCLog("SpriteAnimationNew::stopAnimation");
	this->stopAllActions();
}

void SpriteAnimationNew::resumeAnimation()
{
	this->resumeSchedulerAndActions();
}

void SpriteAnimationNew::callbackCleanUP()
{
    //CCLog("SpriteAnimationNew::callbackCleanUP");
    this->removeFromParentAndCleanup(true);
}

/*
void SpriteAnimationNew::setPosition(const CCPoint &position)
{
	CCNode::setPosition(position);
 
	if(mysprite)
	{
        mysprite->setPosition(ccp(getContentSize().width * mysprite->getAnchorPoint().x - mysprite->getContentSize().width / 2 * mysprite->getScaleX(),
                                  getContentSize().height * mysprite->getAnchorPoint().y-mysprite->getContentSize().height/2*mysprite->getScaleY() ) );
    }
 
}

void SpriteAnimationNew::setAnchorPoint(const CCPoint& anchorPoint)
{
	CCNode::setAnchorPoint(anchorPoint);
    if(mysprite)
	{
        mysprite->setAnchorPoint( anchorPoint );
    }
 
}*/

/*
void SpriteAnimationNew::SetOpacity(GLubyte opacity)
{
	//CCNodeRGBA::setOpacity(opacity);
	this->setOpacity(opacity);
}
 */

void SpriteAnimationNew::SetColor(const ccColor3B& color)
{
	//CCNodeRGBA::setColor( color );
	this->setColor(color);
}