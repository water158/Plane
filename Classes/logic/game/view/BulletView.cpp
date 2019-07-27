#include "BulletView.h"
#include "FerrisTools.h"
#include "CraftView.h"

BulletView::BulletView()
{
}

BulletView::~BulletView()
{
    if(dataBullet)
    {
        CC_SAFE_RELEASE(dataBullet);
    }
    if(m_pMotionStreak)
    {
        m_pMotionStreak->removeFromParentAndCleanup(true);
    }
    
    if(m_pMissileParticle)
    {
        m_pMissileParticle->removeFromParent();
        m_pMissileParticle = NULL;
    }
    
    if(m_pSpriteExpand)
    {
        m_pSpriteExpand->removeFromParent();
    }
    
    if (m_pSpriteExtra)
    {
        m_pSpriteExtra->removeFromParent();
    }
    
    if (missileTarget) {
        CC_SAFE_RELEASE(missileTarget);
    }
    
}

BulletView* BulletView::create(BulletData *data)
{
    BulletView *view = new BulletView();
    if(view && view->init(data))
    {
        view->autorelease();
        return view;
    }
    return NULL;
}

bool BulletView::init(BulletData *data)
{
    dataBullet = data;
    dataBullet->retain();
    
    missileTarget = NULL;
    
    this->setHarm(0.0f);
    this->setIsDied(false);
    this->setIsAimDied(false);
    this->setMotionStreak(NULL);
    this->setDataTrailGroup(NULL);
    this->setCurTrail(1);
    this->setSpeed(0.0f);
    this->setCurRepeatBeginID(0);
    this->setCurRepeatEndID(0);
    this->setTotalRepeatTimes(0);
    this->setCurRepeatBeginID(0);
    this->setCurRepeatTimes(0);
    this->setOffsetAngle(0.0f);
    this->setChildBulletID(1);
    this->setTrailGroupAngle(0.0f);
    this->setHitType(dataBullet->getMiscData()->getBulletHitType());
    this->setShootType(dataBullet->getMiscData()->getBulletShootType());
    this->setIsSelfRotate(dataBullet->getMiscData()->getIsBulletSelfRotate());
    this->setSelfRotateTime(dataBullet->getMiscData()->getBulletSelfRotateTime());
    this->setID("1");
    this->setStreakType(dataBullet->getMiscData()->getStreakType());
    this->setStreakParticleType(dataBullet->getMiscData()->getStreakParticleType());
    this->setBelongCount(0);
    this->setRadiateType(1);
    this->setDelayTime(0.0f);
    this->setIsLightningBullet(false);
    this->setIsTakeLightning(false);
    
    this->setRooundPathCount(1);
    this->setRooundIsAnticlockwise(false);
    this->setIsMissile(false);
    
    this->setSpriteExtra(NULL);
    this->setSpriteExpand(NULL);
    this->setMotionStreak(NULL);
    this->setMissileParticle(NULL);
    //this->setMissileTarget(NULL);
    this->setIsArrive(false);
    this->setIsHaveMIssileTarget(false);
    
    //初始化形态
    this->changeShape();
    
    //更新拖尾 粒子位置
    schedule(schedule_selector(BulletView::updatePosition));
    
//    schedule(schedule_selector(BulletView::updateMissileTarget));
    
    return true;
}

void BulletView::setMissileTarget(CraftView* _target)
{
//    _target->retain();
    
    missileTarget = _target;
    missileTarget->retain();
}

void BulletView::changeShape()
{
    //获取形态数据
    ShapesData* pShapesData = dataBullet->getShapesData();
    CCDictionary* dic = pShapesData->getDictShapesData();
    
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(dic, _ele)
    {
        ShapePartData* pPart = (ShapePartData*)_ele->getObject();
        //是不是动画
        bool bAnimation = pPart->getIsAnimation();
        if(bAnimation)
        {
            //加载图片
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pPart->getPlistPath().c_str());
            
            //Animation
            CCAnimation *anim = CCAnimation::create();
            anim->setDelayPerUnit(pPart->getDelay());
            
            //帧数
            int nNum = pPart->getFrameNum();
            for (int i = 0; i < nNum; i++)
            {
                string strName = pPart->getPlistName();
                
                strName.append("_").append(getStringFromInt(i + 1)).append(".png");
                
                CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strName.c_str());
                anim->addSpriteFrame(frame);
                
                //给飞机设置尺寸
                if(i == 0)
                {
                    this->setContentSize(CCSizeMake(frame->getRect().size.width, frame->getRect().size.height));
                }
                
                this->initWithSpriteFrame(frame);
            }
            
            this->runAction(CCRepeatForever::create(CCAnimate::create(anim)));
            this->setAnchorPoint(ccp(0.5, 0.5));
            this->setPosition(ccp(pPart->getX(), pPart->getY()));
            this->setScaleX(pPart->getScale_x());
            this->setScaleY(pPart->getScale_y());
            this->setRotation(pPart->getRotation());
            
        }else
        {
            //加载图片
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pPart->getPlistPath().c_str());
            
            string strName = pPart->getPlistName();
            strName.append(".png");
            
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strName.c_str());
            this->setContentSize(CCSizeMake(frame->getRect().size.width, frame->getRect().size.height));
            
            this->initWithSpriteFrame(frame);
            this->setAnchorPoint(ccp(0.5, 0.5));
            this->setPosition(ccp(pPart->getX(), pPart->getY()));
            this->setScaleX(pPart->getScale_x());
            this->setScaleY(pPart->getScale_y());
            this->setRotation(pPart->getRotation());
            
        }
    }
    
}

//自转
void BulletView::runSelfRotate()
{
    float dt = this->getSelfRotateTime();
    this->runAction(CCRepeatForever::create(CCRotateBy::create(dt, 360.0f)));
}

//创建拖尾
FerrisMotionStreak* BulletView::createMotionStreak()
{
    int type = this->getStreakType();
    string path = "";
    float delay = 0.0;
    float width = 0.0f;
    float length = 0.0f;
    
    switch (type) {
        case BULLET_STREAK_TYPE1://11
        {
            path.append("game/streak/streak_1.png");
            delay = 1.5f;
            width = 20.0f;
            length = 2.0f;
        }
            break;
        case BULLET_STREAK_TYPE2://12
        {
            path.append("game/streak/streak_2.png");
            delay = 1.4f;
            width = 18.0f;
            length = 2.0f;
        }
            break;
        case BULLET_STREAK_TYPE3://18
        {
            path.append("game/streak/streak_3.png");
            delay = 3.0f;
            width = 40.0f;
            length = 2.0f;
        }
            break;
        case BULLET_STREAK_TYPE4://20
        {
            path.append("game/streak/streak_4.png");
            delay = 1.4f;
            width = 18.0f;
            length = 1.0f;
        }
            break;
        case BULLET_STREAK_TYPE5://wing
        {
            path.append("game/streak/streak_5.png");
            delay = 1.2f;
            width = 7.0f;
            length = 2.0f;
        }
            break;
        case BULLET_STREAK_TYPE6://17
        {
            path.append("game/streak/streak_6.png");
            delay = 5.2f;
            width = 105.0f;
            length = 5.0f;
        }
            break;
        default:
            break;
    }
    
    //混合加亮（源 = 源的alpha * 源  目标 = 目标）
    ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE};
    
    FerrisMotionStreak* pStreak = NULL;
    
    //如果是拖尾type是1 颜色需要使用黄色
    if (type == BULLET_STREAK_TYPE1)
    {
        pStreak = FerrisMotionStreak::create(delay, length, width, ccYELLOW, path.c_str());
    }
    else//否则使用图片原来的颜色
    {
        pStreak =FerrisMotionStreak::create(delay, length, width, ccWHITE, path.c_str());
    }
    
    pStreak->setBlendFunc(blend);
    pStreak->setFastMode(true);
    
    return pStreak;
}

FerrisParticle* BulletView::createStreakParticle()
{
    int type = this->getStreakParticleType();
    string path = "";
    float scale = 0.0f;
    
    switch (type) {
        case BULLET_STREAK_PARTICLE_TYPE1://11
        {
            path = "game/particles/streak_particle_1.plist";
            scale = 0.7f;
        }
            break;
        case BULLET_STREAK_PARTICLE_TYPE2://17
        {
            path = "game/particles/streak_particle_2.plist";
            scale = 1.0f;
        }
            break;
        case BULLET_STREAK_PARTICLE_TYPE3://20
        {
            path = "game/particles/streak_particle_3.plist";
            scale = 0.5f;
        }
            break;
        case BULLET_STREAK_PARTICLE_TYPE4://21
        {
            path = "game/particles/streak_particle_4.plist";
            scale = 1.0f;
        }
            break;
        case BULLET_STREAK_PARTICLE_TYPE5://9
        {
            path = "game/particles/streak_particle_5.plist";
            scale = 1.0f;
        }
            break;
        default:
            break;
    }
    
    FerrisParticle* particle = FerrisParticle::create(path.c_str());
    particle->setScale(scale);
    
    //如果粒子效果是type1 or 3 5设置位置跟随类型是随着粒子发射器节点移动而动
    if (type == BULLET_STREAK_PARTICLE_TYPE1 || type == BULLET_STREAK_PARTICLE_TYPE3 || type == BULLET_STREAK_PARTICLE_TYPE5)
    {
        particle->setPositionType(kCCPositionTypeGrouped);
    }
    
    return particle;
}
void BulletView::updatePosition(float dt)
{
    if (this->getIsDied())
    {
        return;
    }
    
    if(m_pMotionStreak)
    {
        //如果拖尾type是6 那么拖尾位置放到子弹头部
        if (this->getStreakType() == BULLET_STREAK_TYPE6)
        {
            m_pMotionStreak->setPosition(ccpAdd(this->getPosition(), ccp(0,this->getContentSize().height)));
        }
        else
        {
            m_pMotionStreak->setPosition(this->getPosition());
        }
        
    }
    
    if(m_pMissileParticle)
    {
        m_pMissileParticle->setPosition(this->getPosition());
    }
    
    if(m_pSpriteExpand)
    {
        m_pSpriteExpand->setPosition(this->getPosition());
        m_pSpriteExpand->setRotation(this->getRotation() + 90.0f);
    }
    
    if (m_pSpriteExtra)
    {
        if (getIntFromS(this->getID().c_str()) == 17)
        {
            m_pSpriteExtra->setPosition(ccpAdd(this->getPosition(), ccp(0,-this->getContentSize().height - 50)));
        }
        else
        {
            m_pSpriteExtra->setPosition(this->getPosition());
        }
        m_pSpriteExtra->setRotation(this->getRotation() + 90.0f);
    }
}

TrailGroupData* BulletView::getDataTrailGroup()
{
    return m_pDataTrailGroup;
}

void BulletView::setDataTrailGroup(TrailGroupData* data)
{
    m_pDataTrailGroup = data;
}

bool BulletView::isMoveOver()
{
    int n_cur = this->getCurRepeatTimes();
    int n_total = this->getTotalRepeatTimes();
    int n_bid = this->getCurRepeatBeginID();
    int n_eid = this->getCurRepeatEndID();
    
    //总次数小于0则可继续重复此路径组（- 1表示无限循环）
    if(n_total < 0)
    {
        return false;
    }
    
    //循环次数未达到总次数可继续重复
    if(n_cur < n_total)
    {
        return false;
    }
    
    return true;
}

//是否出界
bool BulletView::outOfScreen()
{
    float xx = this->getPositionX();
    float yy = this->getPositionY();
    
    int n_offset_x = 30;
    int n_offset_y_bottom = 30;
    
    if(xx < - n_offset_x || xx > _s_width + n_offset_x || yy < - n_offset_y_bottom || yy > _s_height + n_offset_y_bottom)
    {
        return true;
    }
    
    return false;
}
//获取碰撞矩形 通过类型
CCRect BulletView::getCollisionRect(RectType type)
{
    //判断碰撞框类型
    int key = 0;
    
    if (type == COLLISIONRECT_TYPE_COMMON)
    {
        key = COLLISIONRECT_TYPE_COMMON;
    }
    else if (type == COLLISIONRECT_TYPE_SPECIAL)
    {
        key = COLLISIONRECT_TYPE_SPECIAL;
    }
    else if (type == COLLISIONRECT_TYPE_CHECKCOIN)
    {
        key = COLLISIONRECT_TYPE_CHECKCOIN;
    }
    else if (type == COLLISIONRECT_TYPE_CHECKTOOL)
    {
        key = COLLISIONRECT_TYPE_CHECKTOOL;
    }
    else if (type == COLLISIONRECT_TYPE_CHECKBLOOD)
    {
        key = COLLISIONRECT_TYPE_CHECKBLOOD;
    }
    
    string keys = getStringFromInt(key);
    
    //所有碰撞数据
    CollisionsData* pCollisionData = this->dataBullet->getCollisionsData();
    //碰撞组数据
    CollisionGroupData* pGroup = (CollisionGroupData*)pCollisionData->getCollisionGroupDataByKeys(keys.c_str());
    
    CCDictionary* pDict = pGroup->getDictCollisionGroupRects();
    
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(pDict, _ele)
    {
        CollisionRectData* pRectData = (CollisionRectData*)_ele->getObject();
        
        float x1 = pRectData->getX();
        float y1 = pRectData->getY();
        float w1 = pRectData->getWidth();
        float h1 = pRectData->getHeight();
        
        return CCRectMake(x1 + this->getPositionX(), y1 + this->getPositionY(), w1, h1);
    }
    
    return CCRect(0,0,0,0);
}

void BulletView::callbackCircleTrace()
{
    
    if (this->getIsDied())
    {
        return;
    }
    
//    if (missileTarget == NULL) {
//        return;
//    }
    
    if (!missileTarget->getIsDied()) {
        //获取目标坐标
        CCPoint tmp_target_pos = missileTarget->getPosition();

        //半径
        float r = 150 - (15 * roundPathCount);
        
        //计算起始坐标 与 目标坐标角度
        float angle = calcTwoPointAngle2(this->getPosition(),tmp_target_pos);
        
        //起始目标所构成直线的切线角度
        float tangent = angle - 90;
        
        //设置正反半弧
        bool anticlockwise = roundIsAnticlockwise;
        if (roundPathCount % 2 == 0)
        {
            if (anticlockwise)
            {
                anticlockwise = false;
            }else
            {
                anticlockwise = true;
            }
        }
        
        //计算起始坐标 与 目标坐标 距离
        float distence = tmp_target_pos.getDistance(this->getPosition());
        
        //获取子弹坐标
        CCPoint tmp_bullet_pos = this->getPosition();
        
        //距离小于r 直接攻击敌机
        if (distence < r)
        {
            if (!is_Arrive)
            {
                if (tmp_bullet_pos.y > tmp_target_pos.y) {
                    anticlockwise = !anticlockwise;
                }
            }
            
            this->setIsArrive(true);
            
            ArcPathAction* moveCircleBy = ArcPathAction::create(0.25f, distence / 2, tangent, 180,false,anticlockwise);
            this->runAction(moveCircleBy);
            
            return;
        }
        
        if (tmp_bullet_pos.y > tmp_target_pos.y) {
            
            //保持最后一次的方向
            anticlockwise = !anticlockwise;
        }
        
        //圆弧动作
        ArcPathAction* moveCircleBy = ArcPathAction::create(0.35f, r, tangent, 180,true,anticlockwise);
        this->runAction(CCSequence::create(moveCircleBy,
                                           CCCallFuncN::create(this, callfuncN_selector(BulletView::callbackCircleTrace)),
                                           NULL));
        
        
        //改变圆心点
        roundPathCount++;
    }
    else
    {
        this->setIsDied(true);
//        //飞向屏幕的左右上角
//        CCPoint p_aim = CCPointZero;
//        if(roundIsAnticlockwise)
//        {
//            p_aim = ccp(_s_left, _s_top);
//        }
//        else
//        {
//            p_aim = ccp(_s_right, _s_top);
//        }
//        
//        //起始坐标
//        CCPoint p_origin = this->getPosition();
//        
//        //弧度
//        float degress = calcTwoPointAngle2(p_origin, p_aim);
//        
//        //角度
//        this->setRotation(degress);
//        
//        CCMoveTo *temp_ac2 = CCMoveTo::create(1.0f, p_aim);
//        this->runAction(CCSequence::create(temp_ac2,CCCallFuncN::create(this, callfuncN_selector(BulletView::callbackRemoveSelf)),NULL));
    }
}

//自毁回调
void BulletView::callbackRemoveSelf(CCNode *pSender)
{
    BulletView* bullet =  (BulletView*)pSender;
    bullet->setIsDied(true);
    
    bullet->removeFromParent();
}
