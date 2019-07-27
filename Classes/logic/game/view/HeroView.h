#ifndef _HERO_VIEW_H_
#define _HERO_VIEW_H_

#include "cocos2d.h"
#include "CraftView.h"
#include "CraftData.h"
#include "GlobalEnum.h"

#include "SpriteAnimationNew.h"

USING_NS_CC;
using namespace std;

//class ObjectPoint : public CCObject
//{
//public:
//	ObjectPoint(int tx, int ty)
//	{
//		m_fOX = tx;
//		m_fOY = ty;
//	}
//
//	float m_fOX;
//	float m_fOY;
//};

class HeroView : public CraftView
{
public:
	HeroView();

	static HeroView* create(CraftData* data);

	virtual ~HeroView();

	virtual void startFire();
	virtual void stopFire();


    virtual void testAllBulletCollision(CCArray *&bullets);

    virtual void testAllCraftCollision(CCDictionary *&crafts);

    virtual void testSingleCraftCollision(CraftView *&craft);


	virtual void processBulletCollision(BulletView *&bullet);

	virtual void processCraftCollision(CraftView *&craft);


    void update(float dt);
    void updatePosition(float dt);

    void defirmation(int type);
    
    //根据技能播放动画（参数一 技能id，参数二播放的是否是复原动画）
    void playAnimationFromSkill(int skill,bool isRecover = false);
    
    void playAnimationFinish(CCArmature *armature, MovementEventType movementtype, const char *c);
    
    void setHeroHeadUnAni(bool enable);
    
    //创建机头动画
    void createHeadAni();
    
private:
    
    //绘制
    virtual void draw();

	bool init(CraftData *data);

    //装备位置
    CC_SYNTHESIZE(CCArray* , m_pArrayEquipPoints, ArrayEquipPoints);
    
    //是否无敌
    CC_SYNTHESIZE(bool, m_pIsInvincible, IsInvincible);
    
    //主机动画
    CC_SYNTHESIZE(CCArmature*, m_pHeroArmature, HeroArmature);

};

#endif