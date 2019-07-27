//
//  FerrisScrollMenu.h
//  plane
//
//  Created by mini01 on 14-5-30.
//
//

#ifndef __FERRIS_GAME__FerrisScrollMenu__
#define __FERRIS_GAME__FerrisScrollMenu__

#include "cocos2d.h"
#include "FerrisMenu.h"

USING_NS_CC;

class FerrisScrollMenu : public CCMenu
{
public:
    CCMenuItem* itemForTouch(CCTouch *touch);
    static FerrisScrollMenu* createWithItems(CCMenuItem *firstItem, va_list args);
    static FerrisScrollMenu* create(float s,CCMenuItem* item, ...);
    static FerrisScrollMenu* createWithArray(CCArray* pArrayOfItems);
    float scale_;
    //重写触摸函数
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void ccTouchMoved(CCTouch *touch, CCEvent *event);
    
    //按钮是否滑动
    CC_SYNTHESIZE(bool,move,Move);
    //用于判断ccmenu的可视区域，当超过这个区域按钮不能点击
    CC_SYNTHESIZE(CCRect, visibleRect, VisibleRect);
};

#endif
