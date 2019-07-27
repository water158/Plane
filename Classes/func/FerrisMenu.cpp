//
//  FerrisMenu.cpp
//  plane
//
//  Created by mini02 on 14-1-2.
//
//

#include "FerrisMenu.h"

FerrisMenu* FerrisMenu::createWithItems(CCMenuItem* item, va_list args)
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
    
    return FerrisMenu::createWithArray(pArray);
}

FerrisMenu* FerrisMenu::createWithArray(CCArray* pArrayOfItems)
{
    FerrisMenu *pRet = new FerrisMenu();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}
CCMenuItem* FerrisMenu::itemForTouch(CCTouch *touch)
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
FerrisMenu * FerrisMenu::create(float s,CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    FerrisMenu *pRet = FerrisMenu::createWithItems(item, args);
    pRet->scale_=s;
    va_end(args);
    
    return pRet;
}








