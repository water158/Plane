#include "HeroView.h"
#include "GameDelegate.h"


HeroView::HeroView()
{

}

HeroView::~HeroView()
{
    
}

//¥¥Ω®
HeroView* HeroView::create(CraftData* data)
{
	HeroView* view = new HeroView();
	if(view && view->init(data))
	{
		view->autorelease();
		return view;
	}

	return NULL;
}

bool HeroView::init(CraftData *data)
{
	if(!CraftView::init(data))
	{
		return false;
	}

    //设置飞机类型
	this->setType(CRAFT_TYPE_HERO);
    
    //设置无敌false
    this->setIsInvincible(false);

    //设置血量
    float healdth = 100;
    //dataMisc->getHealdth();
    this->setTotalHP(healdth);
    this->setCurrentHP(healdth);
    
    //获取动画
    m_pHeroArmature = (CCArmature*)nodeContainer->getChildByTag(kTagShape);
    
	m_pArrayEquipPoints = CCArray::create();
	m_pArrayEquipPoints->retain();

	this->schedule(schedule_selector(HeroView::update), 1.0f/60.0f);
    
	return true;
}

void HeroView::playAnimationFromSkill(int skill,bool isRecover)
{
    if (skill < 1)
    {
        CCLog("skill not found !!!");
        return;
    }
    
    //获取技能id 和 技能阶段
    int skill_id = skill;
    string ani_name = "";
    
    //判断释放哪个技能
    switch (skill_id)
    {
        case MAINGUN_SKILL_TYPE_NULL: //没有技能
            ani_name.append("");
            break;
        case MAINGUN_SKILL_TYPE_SANDS_OF_TIME://时间之沙
            ani_name.append("SANDS_OF_TIME");
            break;
        case MAINGUN_SKILL_TYPE_WAVE_GUN://光波炮
            ani_name.append("WAVE_GUN");
            break;
        case MAINGUN_SKILL_TYPE_SHAPED_GUN://聚能炮
            ani_name.append("SHAPED_GUN");
            break;
        case MAINGUN_SKILL_TYPE_SHADOW_OF_HIDING://影遁
            ani_name.append("SHADOW_OF_HIDING");
            break;
        case MAINGUN_SKILL_TYPE_REPULSION_SHIELD://斥力盾
            ani_name.append("REPULSION_SHIELD");
            break;
        case MAINGUN_SKILL_TYPE_POSITIONING_SHOOTING://定位狙击
            ani_name.append("POSITIONING_SHOOTING");
            break;
            
        default:
            break;
    }
    
    if (ani_name.empty())
    {
        return;
    }
    
    if (isRecover)
    {
        ani_name.append("_BACK");
    }
    
    CCArmatureAnimation* animation = m_pHeroArmature->getAnimation();
    
    //设置Movement回调一定要在play之前 否则第一次播放动画 动画状态不显示START
    animation->setMovementEventCallFunc(this, movementEvent_selector(HeroView::playAnimationFinish));
    
    //播放动画
    animation->play(ani_name.c_str());
    
}

//创建机头动画
void HeroView::createHeadAni()
{
    //主机头部动画效果
    CCSprite* head_1 = CCSprite::create("game/animations/streak_hero_1.png");
    head_1->setPosition(ccp(0,POSY_HERO_HEAD));
    head_1->setOpacity(0);
    head_1->setScale(0.25f);
    this->addChild(head_1,10,kTagHeroHeadSp1);
    
    CCSprite* head_2 = CCSprite::create("game/animations/streak_hero_2.png");
    head_2->setPosition(ccp(0,POSY_HERO_HEAD));
    head_2->setScale(0.25f);
    this->addChild(head_2,10,kTagHeroHeadSp2);
    
    ccBlendFunc func = {GL_SRC_ALPHA,GL_ONE};
    head_2->setBlendFunc(func);
    
    CCSequence* seq1 = CCSequence::create(CCDelayTime::create(1.0f),
                                          CCFadeIn::create(0.5f),
                                          CCFadeOut::create(0.4f),
                                          NULL);
    head_1->runAction(CCRepeatForever::create(seq1));
    
    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(1.0f),
                                          CCFadeOut::create(0.5f),
                                          CCFadeIn::create(0.4f),
                                          NULL);
    head_2->runAction(CCRepeatForever::create(seq2));
}

void HeroView::setHeroHeadUnAni(bool enable)
{
    if (!this->getIsDied())
    {
        if (this->getChildByTag(kTagHeroHeadSp1))
        {
            this->removeChildByTag(kTagHeroHeadSp1);
        }
        
        if (this->getChildByTag(kTagHeroHeadSp2))
        {
            this->removeChildByTag(kTagHeroHeadSp2);
        }
        
        if (enable == true) {
            createHeadAni();
        }
        
    }
}

void HeroView::playAnimationFinish(CCArmature *armature, MovementEventType movementtype, const char *c)
{
    switch (movementtype) {
        case cocos2d::extension::START:
        {
            if (!strstr(c, "BACK")) {
                setHeroHeadUnAni(false);
                this->delegateCrashed->setCraftHeadStreak(false);
            }
        }
            break;
        case cocos2d::extension::COMPLETE:
        {
            if (strstr(c, "BACK")) {
                setHeroHeadUnAni(true);
                this->delegateCrashed->setCraftHeadStreak(true);
            }
        }
            break;
            
        default:
            break;
    }

}

//飞机发射子弹
void HeroView::startFire()
{
	CCLog("HeroView::startFire");

	//清理所有枪管
	gun_details->removeAllObjects();

	//构建枪管
	CCArray* phrase_targetgungroups = dataCraft->getArrayWeaponData();

	//准备GunDetail 是每次update的时候 用到的数据
	prepareGunDetail(phrase_targetgungroups);
}
void HeroView::stopFire()
{
    this->setIsBeginFly(false);
}

void HeroView::draw()
{
    CraftView::draw();
}

void HeroView::update(float dt)
{
	float timeCurrent = time;

	if(!this->getIsBeginFly())
	{
		return;
	}

	if(this->getIsDied())
	{
		return;
	}

	CCObject* it;
	CCARRAY_FOREACH(gun_details, it)
	{
        
		GunDetail* detail = (GunDetail *)it;

		if(!detail->getIsBeginFire())
		{

			detail->setIsBeginFire((timeCurrent >= detail->getTimeBeginFire()) ? true : false);
		}
		else
		{

			detail->setHarmRatio(1.0f);

			shotBullet(dt, detail);
		}
        
	}

	time += dt;
}

void HeroView::testAllBulletCollision(CCArray *&bullets)
{
	CraftView::testAllBulletCollision(bullets);
}

void HeroView::testAllCraftCollision(CCDictionary *&crafts)
{
	CraftView::testAllCraftCollision(crafts);
}

void HeroView::testSingleCraftCollision(CraftView *&craft)
{
	CraftView::testSingleCraftCollision(craft);
}

//处理子弹的碰撞结果，比如减血啊之类的
void HeroView::processBulletCollision(BulletView *&bullet)
{
	//CraftView::processBulletCollision(bullet);
	//甭管子弹怎么碰，肯定是没了
	this->delegateCrashed->bulletCrashed(bullet, true);

	//减去子弹的伤害
	this->setCurrentHP(this->getCurrentHP() - bullet->getHarm());
    
    //播放受击声音
    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_HERO_HURT,EFF_HURT_HERO);

	//判断最终是否没血了
	if(this->getCurrentHP() <= 0)
	{
        this->setIsDied(true);

	}
}
//处理同对手碰撞的结果
void HeroView::processCraftCollision(CraftView *&craft)
{
	CraftView::processCraftCollision(craft);
}

void HeroView::defirmation(int type)
{
    
}