//
//  FerrisScrollMenu.cpp
//  plane
//
//  Created by mini01 on 14-5-30.
//
//

#include "FerrisScrollMenu.h"

FerrisScrollMenu* FerrisScrollMenu::createWithItems(CCMenuItem* item, va_list args)
{
    CCArray* pArray = NULL;
    if( item )
    {
		pArray = CCArray::create(item, NULL);
		CCMenuItem *i = va_arg(args, CCMenuItem*);
		while(i)
		{
			pArray->addObject(i);
			i = va_arg(args, CCMenuItem*);
		}
    }
    
    return FerrisScrollMenu::createWithArray(pArray);
}

FerrisScrollMenu* FerrisScrollMenu::createWithArray(CCArray* pArrayOfItems)
{
    FerrisScrollMenu *pRet = new FerrisScrollMenu();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
		pRet->setMove(false);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}
CCMenuItem* FerrisScrollMenu::itemForTouch(CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();
    
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCMenuItem* pChild = dynamic_cast<CCMenuItem*>(pObject);
            if (pChild && pChild->isVisible() && pChild->isEnabled())
            {
                CCPoint local = pChild->convertToNodeSpace(touchLocation);
                CCRect r = pChild->rect();
                if (!scale_)
                {
                    scale_=1;
                }
                r.origin = ccp((r.size.width-r.size.width*scale_)*0.5,(r.size.height-r.size.height*scale_)*0.5 );
                r.size=CCSizeMake(r.size.width*scale_, r.size.height*scale_);
                if (r.containsPoint(local))
                {
                    return pChild;
                }
            }
        }
    }
    
    return NULL;
}
FerrisScrollMenu * FerrisScrollMenu::create(float s,CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    FerrisScrollMenu *pRet = FerrisScrollMenu::createWithItems(item, args);
    pRet->scale_=s;
    va_end(args);
    
    return pRet;
}

bool FerrisScrollMenu::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCLog("touch point x:%f,y:%f",touch->getLocation().x,touch->getLocation().y);
    
    if (visibleRect.size.width&&visibleRect.size.height)
    {
        if (!visibleRect.containsPoint(touch->getLocation()))
        {
            return false;
        }
    }
    
    CCLog("touch  is successful");
    this->setMove(false);
    return CCMenu::ccTouchBegan(touch, event);
    
}

void FerrisScrollMenu::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    this->setMove(true);
    CCMenu::ccTouchMoved(touch, event);
}

void FerrisScrollMenu::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if (!this->getMove())
    {
        CCMenu::ccTouchEnded(touch, event);
    }
    else
    {
        m_eState = kCCMenuStateWaiting;
        if (m_pSelectedItem)
        {
            m_pSelectedItem->unselected();
        }
    }
}
