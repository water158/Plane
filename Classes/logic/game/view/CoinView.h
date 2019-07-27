#ifndef _COIN_VIEW_H_
#define _COIN_VIEW_H_

#include "EnemyView.h"
#include "DropView.h"

USING_NS_CC;
using namespace std;

class CoinView : public DropView
{
public:

	//创建
	static CoinView* create(DropData* data);
    
    //处理与主角的碰撞
    //virtual void processCraftCollision(CraftView *&craft);
    
    void callbackRemoveFormParent();

private:
	bool init(DropData* data);
	//更新速度变化
	void updateSpeed(float dt);

	CC_SYNTHESIZE(float, curSpeed, CurSpeed);
	CC_SYNTHESIZE(bool, isSpeedUp, IsSpeedUp);
    
    //是否与与主机检测框碰撞
    //CC_SYNTHESIZE(bool, m_bIsCollisionWithHero, IsCollisionWithHero);

};



#endif