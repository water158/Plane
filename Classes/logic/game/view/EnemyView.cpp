#include "EnemyView.h"
#include "UserData.h"
#include "FerrisParticle.h"

EnemyView::EnemyView()
{
    batteryPlanes = NULL;
    this->uuid = NEWUUID;
    this->setIsDisplayHp(true);
    this->m_fDelayTime = 0.0f;
}

EnemyView::~EnemyView()
{
    CC_SAFE_RELEASE(batteryPlanes);
    
    if (type == CRAFT_TYPE_ENEMY)
    {
        //CCLog("EnemyView IS DELETED [%d].", this->m_uID);
        //Score::sharedScore()->incrEnemyDied();
    }

	this->unscheduleUpdate();

	if(m_pMotionStreak)
	{
		m_pMotionStreak->removeFromParentAndCleanup(true);
	}

}

EnemyView* EnemyView::create(CraftData* dataCraft, TargetPlaneData* dataTargetPlane)
{
	EnemyView *view = new EnemyView();
	
    if(view && view->init(dataCraft, dataTargetPlane))
	{
		view->autorelease();
		return view;
	}
    
	return NULL;
}

bool EnemyView::init(CraftData* dataCraft, TargetPlaneData* dataTargetPlane)
{

    if(!CraftView::init(dataCraft))
    {
        return false;
    }
    
    this->setType(CRAFT_TYPE_ENEMY);
    this->setBeginAction(false);

    this->setParticle(NULL);
	this->setMotionStreak(NULL);

    this->time = 0.0f;
    
    this->setIsCanInjured(false);
    
    batteryPlanes = CCArray::create();
    batteryPlanes->retain();
    
    if (dataTargetPlane != NULL)
    {
        this->setPlaneID(dataTargetPlane->getPlaneID());
        this->dataTargetPlane = dataTargetPlane;
        this->setTotalHP(dataTargetPlane->getPlaneBlood());
        this->setCurrentHP(dataTargetPlane->getPlaneBlood());
        
        this->setDelayTime(dataTargetPlane->getDelayTime());
        
        //设置后坐力
        this->setRecoil(dataTargetPlane->getRecoil());
        //设置自动旋转
        this->setIsCanRotate(dataTargetPlane->getAutomaticRotate());
        
    }

	return true;
}

void EnemyView::draw()
{
    if (current_hp <= total_hp && current_hp > 0)
    {
        if (isDisplayHp)
        {
            float width = this->getHPWidth();
            float height = this->getContentSize().height;

			float fH = 50.0f;
            
            glLineWidth(5);
            ccDrawColor4B(66, 58, 53, 255);
            ccDrawLine(ccp(0 - width / 2, 0 + height / 2 + fH),
                       ccp(    width / 2, 0 + height / 2 + fH));
            
            glLineWidth(3);
            ccDrawColor4B(100, 255, 114, 255);
            ccDrawLine(ccp(0 - width / 2, 0 + height / 2 + fH),
                       ccp(0 - width / 2 + width * (current_hp * 1.0f / total_hp), 0 + height / 2 + fH));
        }        
    }
    
	 CraftView::draw();
}

//碰撞检测 与子弹之间的碰撞(所有)
void EnemyView::testAllBulletCollision(CCArray *&bullets)
{
    CCObject *pObj = NULL;
    
    CCARRAY_FOREACH(bullets, pObj)
    {
        BulletView *bullet = (BulletView *)pObj;
        
        CCRect rBullet = bullet->getCollisionRect(COLLISIONRECT_TYPE_COMMON);
        //bullet->boundingBox();
        
        if (bullet->getIsLightningBullet())
        {
            continue;
        }
        
        if(!bullet->getIsDied())
        {
            //如果是敌机炮
            if (this->getIsBattery())
            {
                //炮的碰撞区（以相对坐标为基准）
                CCRect rect = this->getCollisionRect(COLLISIONRECT_TYPE_COMMON);
                
                //炮的位置
                CCPoint pos = this->getParent()->getPosition();//ccpAdd(this->getParent()->getPosition(), this->getPosition());

                float x = rect.origin.x;
                float y = rect.origin.y;
                float w = rect.size.width;
                float h = rect.size.width;
                
                //重新设置碰撞区（以绝对坐标为基准）
                rect = CCRectMake(pos.x + x, pos.y + y, w, h);
                
                if (rBullet.intersectsRect(rect))
                {
                    processBulletCollision(bullet);
                }
                
            }
            //与敌机碰撞
            else
            {
                if (rBullet.intersectsRect(this->getCollisionRect(COLLISIONRECT_TYPE_COMMON)))
                {
                    processBulletCollision(bullet);
                }
                
            }
            
        }
    }
    
}

//设置飞机上的炮可以受击
void EnemyView::setBatteryCanInjured()
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(batteryPlanes, obj)
    {
        EnemyView* battery = (EnemyView*)obj;
        battery->setIsCanInjured(true);
    }
}

//添加敌机炮
void EnemyView::addBatteryPlane(CraftView* craft)
{
    batteryPlanes->addObject(craft);
}
//移除敌机炮
void EnemyView::removeBatteryPlane(CraftView* craft)
{
    batteryPlanes->removeObject(craft);
}

void EnemyView::setEnemyBatteryCrashed()
{
    //遍历当前炮数组
    for (int i = 0; i < batteryPlanes->count(); i++)
    {
        EnemyView* battery = (EnemyView*)batteryPlanes->objectAtIndex(i);
        this->delegateCrashed->craftCrashed(battery, 0, true);
    }
}
int EnemyView::getBatteryPlaneSize()
{
    return batteryPlanes->count();
}
void EnemyView::startCheckBatterySize()
{
    this->schedule(schedule_selector(EnemyView::updateCheckBatterySize),1.0f);
}
void EnemyView::updateCheckBatterySize(float dt)
{
    if (this->getIsDied())
    {
        return;
    }
    
    if (!batteryPlanes)
    {
        return;
    }
    
    if (this->getBatteryPlaneSize() < 1)
    {
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(EnemyView::callbackDecreaseHP));
        this->runAction(func);
    }
    
}

void EnemyView::callbackDecreaseHP(CCNode* pSender)
{
    if (this->getIsDied())
    {
        return;
    }
    
    //每次减血为总血量的0.4
    float value = this->getTotalHP() * 0.4f;
    
    this->setCurrentHP(this->getCurrentHP() - value);
    
    //判断血量是否为0
    if (this->getCurrentHP() < 0)
    {
        //敌机死亡
        this->delegateCrashed->craftCrashed(this, 0, true);
        
        int type = getCraftSizeType(this);
        
        if (this->getIsBattery() || type < CRAFT_SIZE_BIG)
        {
            //播放爆炸声音
            SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_SMALL);
        }
        else
        {
            //播放爆炸声音
            SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_BIG);
        }
        
    }

}

//开始检测炮的状态
void EnemyView::startCheckBatteryIsDied()
{
    this->schedule(schedule_selector(EnemyView::updateCheckBatteryIsDied), 0.1f);
}

//检测炮的是否死亡
void EnemyView::updateCheckBatteryIsDied()
{
    
    if (!batteryPlanes || batteryPlanes->count() < 1)
    {
        return;
    }
    
    for (int i = 0; i < batteryPlanes->count(); i++)
    {
        EnemyView* battery = (EnemyView*)batteryPlanes->objectAtIndex(i);
        
        //炮血量少于50%
        if (battery->getCurrentHP() < battery->getTotalHP() * 0.5)
        {
            if (!battery->getIsHaveSmoking1())
            {
                CCPoint pos = getRandomPositionByOffset(ccp(0.0f,0.0f), 10, 10);
                
                FerrisParticle* particle = FerrisParticle::create("game/particles/particle_attrack_small.plist");
                particle->setPosition(pos);
                particle->setAutoRemoveOnFinish(true);
                this->addChild(particle,1000);
                
                battery->setIsHaveSmoking1(true);
            }
            
        }
        
        //炮死亡
        if (battery->getCurrentHP() <= 1)
        {
            if (!battery->getIsHaveSmoking2())
            {
                FerrisParticle* particle = FerrisParticle::create("game/particles/particle_attrack_big.plist");
                particle->setPosition(battery->getPosition());
                particle->setAutoRemoveOnFinish(true);
                this->addChild(particle,1000);
                
                battery->setIsHaveSmoking2(true);
            }
        }

    }
}

//处理子弹的碰撞结果，比如减血啊之类的
void EnemyView::processBulletCollision(BulletView *&bullet)
{
    //甭管子弹怎么碰，肯定是没了
    this->delegateCrashed->bulletCrashed(bullet, true);
    
    //减去子弹的伤害
    this->setCurrentHP(this->getCurrentHP() - bullet->getHarm());

    //判断最终是否没血了
    if(this->getCurrentHP() <= 0)
    {
        this->delegateCrashed->craftCrashed(this, 0, true);
        
        int type = getCraftSizeType(this);
        
        if (this->getIsBattery() || type < CRAFT_SIZE_BIG)
        {
            //播放爆炸声音
            SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_SMALL);
        }
        else
        {
            //播放爆炸声音
            SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_BIG);
        }
    }
}

void EnemyView::changePhrase(int wave)
{
    if (dataTargetPlane)
    {
        this->setPhrase(wave);
        
		unscheduleUpdate();
        
        //清理所有枪管
        gun_details->removeAllObjects();
        
        PhaseData *dataPhrase = dataTargetPlane->getPhaseData("1");
        
        if (!dataPhrase)
        {
            return;
        }
        
        CCArray *phrase_targetgungroups = dataPhrase->getTargetGunGroup();
        
        //准备GunDetail 是每次update的时候 用到的数据
        prepareGunDetail(phrase_targetgungroups);
        
        initWeapon();
    }
}

/* ==============================
 * 发射
 * ============================== */
//初始化武器
void EnemyView::initWeapon()
{
    //开始UPDATE
    this->scheduleUpdateWithPriority(0);
}

//更新时间轴逻辑
void EnemyView::update(float dt)
{
	//更新拖尾位置
	if(this->getMotionStreak())
	{
		m_pMotionStreak->setPosition(this->getPosition());
	}
	if(this->getParticle())
	{
		m_pParticle->setPosition(this->getPosition());
	}
	
	float timeCurrent = time;

	//如果当前枪管的Holder，还没有进入屏幕中，则不开枪，也不运行后面的逻辑
	if (!this->getIsBeginFly())
	{
		return;
	}
    
    //如果当前枪管的Holder，已经死了，也不运行后面的逻辑
	if (this->getIsDied())
	{
		return;
	}
    
    //循环所有的枪管数据
    CCObject* it;
    CCARRAY_FOREACH(gun_details, it)
    {
        GunDetail* detail = (GunDetail *)it;
        
        //设置敌机firstdelay时间 根据炮管计算出来的
        detail->setDelayTime(this->getDelayTime());
  
        if (!detail->getIsBeginFire())
        {
            //当前的时间，大于等于gungroup_delay + gun_delay，就开始工作了
            detail->setIsBeginFire((timeCurrent >= detail->getTimeBeginFire()) ? true : false);
        }
        else
        {
            //如果是普通子弹
            shotBullet(dt, detail);
        }
    }
    
    time += dt;
}

//受到的伤害
void EnemyView::theHurted(float hp)
{
    float currentHP=this->getCurrentHP()-hp;
    this->setCurrentHP(currentHP);
    
    //判断最终是否没血了
    if(this->getCurrentHP() <= 0)
    {
        this->delegateCrashed->craftCrashed(this, 0, true);
        
        int type = getCraftSizeType(this);
        
        if (this->getIsBattery() || type < CRAFT_SIZE_BIG)
        {
            //播放爆炸声音
            SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_SMALL);
        }
        else
        {
            //播放爆炸声音
            SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_BOMB,EFF_BOOM_BIG);
        }
    }
    
}