//
//  DropView.h
//  ambition
//
//  Created by mini02 on 15/1/14.
//
//

#ifndef __ambition__DropView__
#define __ambition__DropView__

#include "cocos2d.h"
#include "DropData.h"
#include "SmartRes.h"
#include "GlobalDelegate.h"
#include "MagnetCoinAction.h"
#include "GlobalTagZOrder.h"

USING_NS_CC;

class HeroView;
class DropView : public CCSprite
{

public:
    DropView();
    virtual ~DropView();
    
    static DropView* create(DropData* data);
    
    //获取碰撞矩形 通过类型
    virtual CCRect getCollisionRect(RectType type);
    
    void startUpdateDrop();
    
protected:
    
    bool init(DropData* data);
    
    //切换形态
    void changeShape();
    
    void updateDrop(float dt);
    
public:
    //数据 （形态、碰撞区）
    DropData* dataDrop;
    
    //代理
    DropDelegate* delegateDrop;
    
private:
    //是否与与主机检测框碰撞
    CC_SYNTHESIZE(bool, m_bIsCollisionWithHero, IsCollisionWithHero);
    //是否已经没了
    CC_SYNTHESIZE(bool, is_died, IsDied);
    
    //主机对象
    CC_SYNTHESIZE(HeroView*, heroTmp, HeroTmp);
    
};

#endif /* defined(__ambition__DropView__) */
