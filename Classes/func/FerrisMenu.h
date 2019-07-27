//
//  FerrisMenu.h
//  plane
//
//  Created by mini02 on 14-1-2.
//
//

#ifndef __FERRIS_GAME__FerrisMenu__
#define __FERRIS_GAME__FerrisMenu__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FerrisMenu :public CCMenu
{
public:
    CCMenuItem* itemForTouch(CCTouch *touch);
    static FerrisMenu* createWithItems(CCMenuItem *firstItem, va_list args);
    static FerrisMenu* create(float s,CCMenuItem* item, ...);
    static FerrisMenu* createWithArray(CCArray* pArrayOfItems);
    float scale_;
};

#endif
