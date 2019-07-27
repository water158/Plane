#include "CoinView.h"

CoinView* CoinView::create(DropData* data)
{
	CoinView *view = new CoinView();

	if(view && view->init(data))
	{
		view->autorelease();
		return view;
	}

	return NULL;
}

bool CoinView::init(DropData* data)
{
    //EnemyView::init(data, dataTargetPlane);
    DropView::init(data);
    
	this->setCurSpeed(1.0f);
	this->setIsSpeedUp(false);
    
    //this->setIsCollisionWithHero(false);

	this->schedule(schedule_selector(CoinView::updateSpeed));
    
    float timeScale = this->getScheduler()->getTimeScale();
    
    this->runAction(CCSequence::create(CCDelayTime::create(3.0f * timeScale),CCFadeOut::create(1.0f * timeScale),CCCallFuncN::create(this, callfuncN_selector(CoinView::callbackRemoveFormParent)),NULL));

	return true;
}

void CoinView::updateSpeed(float dt)
{
	CCSpeed *speed = (CCSpeed*)getActionByTag(kSPEEDUP_ACTION_TAG);

	if(!speed)
	{
		return;
	}

	//向下的过程 一直在加速
	if (speed && isSpeedUp)
	{
		if (speed->getSpeed() < 2.0f)
		{
			speed->setSpeed(speed->getSpeed() + dt);
		}
	}
	//向上的过程 一直在减速
	else if(speed && !isSpeedUp)
	{
		if (speed->getSpeed() > 0.0f)
		{
			speed->setSpeed(speed->getSpeed() - dt);
		}
		else
		{
			isSpeedUp = true;
            this->delegateDrop->dropSpeedUp(this);
			//this->delegateCrashed->coinSpeedUp(this);

			//beginMove();
		}
		//CCLog("------getSpeed-------===----(%f)---",speed->getSpeed());
	}
	
    if (speed)
    {
        //CCLog("CoinView [%d] speed (%f)", this->m_uID, speed->getSpeed());
    }
}

void CoinView::callbackRemoveFormParent()
{
    this->setIsDied(true);
    this->removeAllChildrenWithCleanup(true);
}

////处理与主角的碰撞
//void CoinView::processCraftCollision(CraftView *&craft)
//{
//    
//}
