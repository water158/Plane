#include "CraftView.h"
#include "BulletView.h"

CraftView::CraftView()
{
    this->uuid = NEWUUID;
    delegateBullet = NULL;
    delegateCrashed = NULL;
}

CraftView::~CraftView()
{
    if(dataCraft)
    {
        CC_SAFE_RELEASE(dataCraft);
    }
    if(gun_details)
    {
        CC_SAFE_RELEASE(gun_details);
    }
    if(dictPosWeapon)
    {
        CC_SAFE_RELEASE(dictPosWeapon);
    }
    if(dataPathGroup)
    {
        //CC_SAFE_RELEASE(dataPathGroup);
        //dataPathGroup = NULL;
    }
//    if(dataMisc)
//    {
//        CC_SAFE_RELEASE(dataMisc);
//    }
}

//创建
CraftView* CraftView::create(CraftData* data)
{
    CraftView* view = new CraftView();
    if(view && view->init(data))
    {
        view->autorelease();
        return view;
    }
    
    return NULL;
}

bool CraftView::init(CraftData *data)
{
    CCNode::init();
    if(!data)
    {
        return false;
    }
    
    dataCraft = data;
    dataCraft->retain();
    
//    dataMisc = dataCraft->getMiscData();
    
    this->setIsDied(false);
    
    
    nodeContainer = CCNode::create();
    nodeContainer->setPosition(CCPointZero);
    this->addChild(nodeContainer, 0);
    
    nodeParticleContainer = CCNode::create();
    nodeParticleContainer->setPosition(CCPointZero);
    nodeParticleContainer->setRotation(0.0f);
    addChild(nodeParticleContainer, 0);
    
    //注：一个奇怪的问题（粒子和opgl绘制时，必须有精灵作为媒介，并且，精灵在粒子上面）
    nodeParticleContainer->addChild(CCSprite::create(), 1);
    
    //ParticleBatchNode初始化
    m_pParticleBatchNode = CCParticleBatchNode::create("game/particles/particle_attrack_big.plist");
    nodeParticleContainer->addChild(m_pParticleBatchNode);
    
    //ParticleBatchNode初始化
    m_pParticleBatchNode1 = CCParticleBatchNode::create("game/particles/particle_attrack_small.plist");
    nodeParticleContainer->addChild(m_pParticleBatchNode1);
    
    //ParticleBatchNode初始化
    m_pParticleBatchNode2 = CCParticleBatchNode::create("game/particles/mission_bear.plist");
    nodeParticleContainer->addChild(m_pParticleBatchNode2);
    
    this->setIsBullletAim(false);
    this->setIsAffiliatedPlane(false);
    this->setIsBossPart(false);
    this->setIsExistBattery(false);
    this->setCurRepeatTimes(0);
    this->setTotalRepeatTimes(0);
    this->setCurRepeatBeginID(0);
    this->setCurRepeatEndID(0);
    this->setSpeed(0.0f);
    this->setHolderID(-1);
    this->setIsCreatingBullet(false);
    this->setIsCanRotate(false);
    this->setParticleNum(0);
    this->setCraftBoomNum(1);
    this->setCurBoomNum(0);
    this->setIsBattery(false);
    this->setRecoil(false);
    this->setBatteryRotateSpeed(90.0f);
    this->setNextAimPos(CCPointZero);
    this->setSecondAimPos(CCPointZero);
    this->setRecoilAction(false);
    this->setHPWidth(50.0f);
    this->setLinkLaser(false);
    this->setConnonType(data->getConnonType());
    this->setIsHaveSmoking1(false);
    this->setIsHaveSmoking2(false);
    this->setBatteryDirection(0);
    this->setCurrentHP(0.0f);
    this->setTotalHP(0.0f);
    
    //掉落
    this->setIsDropCoin(false);
    this->setIsDropScore(false);
    this->setIsDropEquip(false);
    this->setIsDropNormalTools(false);
    this->setIsDropInvincibleTools(false);
    this->setIsDropBloodCell(false);
    
    //初始化状态
    this->setIsBossBooming(false);
    this->setIsBeginFly(false);
    this->setIsDied(false);
    this->setCollisionLess(false);
    this->setGunSwitch(true);
    this->setCollisionWithWaveGun(false);
    this->setCollisionWithShapedGun(false);
    
    //调试
    this->setIsDrawWeaponPosition(false);
    this->setIsDrawCollisionRect(false);
    
    //炮type
    this->setBelongCount(0);
    
    //初始化时间
    this->time = 0.0f;
    
    this->setStagnatePointX(0.0f);
    this->setStagnatePointY(0.0f);
    
    //武器
    gun_details = CCArray::create();
    gun_details->retain();
    
    //武器位置
    dictPosWeapon = CCDictionary::create();
    dictPosWeapon->retain();
    
    this->setType(dataCraft->getType());
    //初始化形态
    this->changeShape();
    
    return true;
}

//切换形态
void CraftView::changeShape()
{
    //获取形态数据
    ShapesData* pShapesData = dataCraft->getShapesData();
    CCDictionary* dic = pShapesData->getDictShapesData();
    
    CCDictElement* _ele = NULL;
    CCDICT_FOREACH(dic, _ele)
    {
        ShapePartData* pPart = (ShapePartData*)_ele->getObject();
        
        bool bTrandform = pPart->getIsTransform();
        
        if (dataCraft->getType() == CRAFT_DATA_TYPE_HERO || dataCraft->getType() == CRAFT_DATA_TYPE_WING)
        {
            //如果带有变形动画
            if (bTrandform)
            {
                
                //获取变形name
                this->setTransformName(pPart->getTransformName());
                
                //获取变形path
                this->setTransformPath(pPart->getTransformPath());

                //加载动画
                CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(pPart->getTransformPath().c_str());
                
                CCArmature* armation = CCArmature::create(pPart->getTransformName().c_str());
                
                armation->setAnchorPoint(ccp(0.5, 0.5));
                armation->setPosition(ccp(pPart->getX(), pPart->getY()));
                armation->setScaleX(pPart->getScale_x());
                armation->setScaleY(pPart->getScale_y());
                CCLog("pPart->getRotation() : %f",pPart->getRotation());
                armation->setRotation(pPart->getRotation());
                
                this->setContentSize(CCSizeMake(armation->getContentSize().width * pPart->getScale_x(), armation->getContentSize().height) * pPart->getScale_y());
                
                nodeContainer->addChild(armation, 0, kTagShape);
            }
        }
        else
        {
            
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
                    
                    //CCLog("------strName---=-=----%s--------------", strName.c_str());
                    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strName.c_str());
                    anim->addSpriteFrame(frame);
                    
                    //给飞机设置尺寸
                    if(i == 0)
                    {
                        this->setContentSize(CCSizeMake(frame->getRect().size.width, frame->getRect().size.height));
                    }
                }
                
                CCSprite* sp = CCSprite::create();
                sp->runAction(CCRepeatForever::create(CCAnimate::create(anim)));
                sp->setAnchorPoint(ccp(0.5, 0.5));
                sp->setPosition(ccp(pPart->getX(), pPart->getY()));
                sp->setScaleX(pPart->getScale_x());
                sp->setScaleY(pPart->getScale_y());
                sp->setRotation(pPart->getRotation());
                
                nodeContainer->addChild(sp, 0, kTagShape);
            }else
            {

                CCLog("************* %s ************",pPart->getPlistPath().c_str());
                
                if (compareString(pPart->getPlistPath().c_str(), ""))
                {
                    return;
                }
                
                //加载图片
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pPart->getPlistPath().c_str());
                
                string strName = pPart->getPlistName();
                //getPlistName();
                strName.append(".png");
                
                CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strName.c_str());
                
                if (frame)
                {
                    this->setContentSize(CCSizeMake(frame->getRect().size.width, frame->getRect().size.height));
                    
                    CCSprite* sp = CCSprite::createWithSpriteFrame(frame);
                    
                    sp->setAnchorPoint(ccp(0.5, 0.5));
                    sp->setPosition(ccp(pPart->getX(), pPart->getY()));
                    sp->setScaleX(pPart->getScale_x());
                    sp->setScaleY(pPart->getScale_y());
                    sp->setRotation(pPart->getRotation());
                    
                    nodeContainer->addChild(sp, 0, kTagShape);
                }
                
            }
            
        }
        
    }
}
/*****************************
 * 碰撞检测
 *****************************/
//碰撞检测 与子弹之间的碰撞(所有)
void CraftView::testAllBulletCollision(CCArray *&bullets)
{
    CCObject *pObj = NULL;
    
    CCARRAY_FOREACH(bullets, pObj)
    {
        BulletView *bullet = (BulletView *)pObj;
        
        //读取xml中配置的碰撞区
        CCRect rBullet = bullet->getCollisionRect(COLLISIONRECT_TYPE_COMMON);
        //bullet->boundingBox();
        
        if(!bullet->getIsDied())
        {
            if (rBullet.intersectsRect(this->getCollisionRect(COLLISIONRECT_TYPE_COMMON)))
            {
                processBulletCollision(bullet);
            }
            
        }
    }
}
//碰撞检测 与对手之间的碰撞(数组)
void CraftView::testAllCraftCollision(CCDictionary *&crafts)
{
    CCDictElement *_ele = NULL;
    CCDICT_FOREACH(crafts, _ele)
    {
        CraftView *craft = (CraftView *)_ele->getObject();
        testSingleCraftCollision(craft);
    }
}
//碰撞检测 与对手之间的碰撞(单一)
void CraftView::testSingleCraftCollision(CraftView *&craft)
{
    if(!craft->getIsDied())
    {
        //所有碰撞数据
        CollisionsData* pCollisionData = this->dataCraft->getCollisionsData();
        //碰撞组数据
        CollisionGroupData* pGroup = (CollisionGroupData*)pCollisionData->getCollisionGroupDataByKeys("1");
        
        CCDictionary* pDict = pGroup->getDictCollisionGroupRects();
        
        CCDictElement* _ele = NULL;
        CCDICT_FOREACH(pDict, _ele)
        {
            CollisionRectData* pRectData = (CollisionRectData*)_ele->getObject();
            
            float x = pRectData->getX();
            float y = pRectData->getY();
            float w = pRectData->getWidth();
            float h = pRectData->getHeight();
            
            CCRect rEnemy = CCRectMake(x + this->getPositionX(), y + this->getPositionY(), w, h);
            
            //所有碰撞数据
            CollisionsData* pCollisionData1 = craft->dataCraft->getCollisionsData();
            //碰撞组数据
            CollisionGroupData* pGroup1 = (CollisionGroupData*)pCollisionData1->getCollisionGroupDataByKeys("1");
            
            CCDictionary* pDict1 = pGroup1->getDictCollisionGroupRects();
            
            CCDictElement* _ele1 = NULL;
            CCDICT_FOREACH(pDict1, _ele1)
            {
                CollisionRectData* pRectData1 = (CollisionRectData*)_ele1->getObject();
                
                float x1 = pRectData1->getX();
                float y1 = pRectData1->getY();
                float w1 = pRectData1->getWidth();
                float h1 = pRectData1->getHeight();
                
                CCRect rEnemy1 = CCRectMake(x1 + craft->getPositionX(), y1 + craft->getPositionY(), w1, h1);
                
                if(rEnemy.intersectsRect(rEnemy1))
                {
                    processCraftCollision(craft);
                }
            }
        }
    }
    
}
//获取碰撞矩形 通过类型
CCRect CraftView::getCollisionRect(RectType type)
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
    CollisionsData* pCollisionData = this->dataCraft->getCollisionsData();
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
//处理子弹的碰撞结果，比如减血啊之类的
void CraftView::processBulletCollision(BulletView *&bullet)
{
    //甭管子弹怎么碰，肯定是没了
    this->delegateCrashed->bulletCrashed(bullet, true);
    
    //减去子弹的伤害
    this->setCurrentHP(this->getCurrentHP() - bullet->getHarm());
    
    //判断最终是否没血了
    if(this->getCurrentHP() <= 0)
    {
        
        this->delegateCrashed->craftCrashed(this, 0, true);
    
    }
}
//处理同对手碰撞的结果
void CraftView::processCraftCollision(CraftView *&craft)
{
    //CCLog("processCraftCollision   -----");
}

//绘制
void CraftView::draw()
{
    if(this->getIsDied())
    {
        return;
    }

//    //绘制碰撞区域
//    
//    //所有碰撞数据
//    CollisionsData* pCollisionData = dataCraft->getCollisionsData();
//    //碰撞组数据
//    CollisionGroupData* pGroup = (CollisionGroupData*)pCollisionData->getCollisionGroupDataByKeys(getStringFromInt(COLLISIONRECT_TYPE_COMMON));
//    
//    CCDictionary* pDict = pGroup->getDictCollisionGroupRects();
//    
//    CCDictElement* _ele = NULL;
//    CCDICT_FOREACH(pDict, _ele)
//    {
//        CollisionRectData* pRectData = (CollisionRectData*)_ele->getObject();
//        
//        float x = pRectData->getX();
//        float y = pRectData->getY();
//        float w = pRectData->getWidth();
//        float h = pRectData->getHeight();
//        
//        glLineWidth(2.0f);
//        ccDrawColor4B(0, 255, 0, 255);
//        ccDrawRect(ccp(x, y), ccp(x + w, y + h));
//    }

}

/*********************************
 * 阶段及子弹
 *********************************/

//切换阶段
void CraftView::changePhrase(int phrase)
{
    //CCLog("CraftView Change Phrase");
}

//准备GunDetail 是每次update的时候 用到的数据
void CraftView::prepareGunDetail(CCArray* targetgungroups)
{
    CCLog("CraftView CHARGE GunDetail %d",targetgungroups->count());
    
    CCObject *pObj1 = NULL;
    CCARRAY_FOREACH(targetgungroups, pObj1)
    {
        TargetGunGroupData* dataTargetGG = (TargetGunGroupData *)pObj1;
        
        CCLog("dataTargetGG id %s",dataTargetGG->getGunGroupID().c_str());
        
        CCLog("dataTargetGG gungroup id %s",dataTargetGG->getGunGroupID().c_str());
        
        //根据TargetGunGroupData里面的id，获得具体的数值
        GunGroupData *dataGunGroup = (GunGroupData*)WeaponLibrary::sharedWeaponLibrary()->getGunGroupDataByID(dataTargetGG->getGunGroupID());
        
        //遍历所有的枪
        CCArray *gundatas = dataGunGroup->getGuns();
        CCObject *pObj2 = NULL;
        CCARRAY_FOREACH(gundatas, pObj2)
        {
            GunData *dataGun = (GunData *)pObj2;
            
            //描述枪的位置 开枪的次序等等
            GunDetail *detail = GunDetail::create(dataTargetGG, dataGun);
            
            /********************************
             * Transform for gun group
             ********************************/
            
            //GunGroup Position
            detail->setGunGroupPosition(ccp(dataTargetGG->getX(), dataTargetGG->getY()));
            
            //GunGroup Rotation <--- 如果GunGroup自转 修正这个值
            detail->setGunGroupRotation(dataTargetGG->getRotationInit());
            
            //Gun Position
            detail->setGunPosition(ccp(dataGun->getX(), dataGun->getY()));
            
            //Gun Rotation
            detail->setGunRotation(dataGun->getDirectionAngle());
            
            detail->setGunRotationInit(dataGun->getDirectionAngle());
            
            /********************************
             * Pass
             *********************************/
            //Pass 是否穿透
            detail->setIsPass(dataGun->getIsPass());
            
            /********************************
             * Track
             *********************************/
            //Track 是否追踪
            detail->setIsTrack(dataTargetGG->getIsTrack());
            
            /********************************
             * Repeat
             *********************************/
            //以下是辅助型参数
            
            //去掉Interval了所以注释掉下面行
            //Interval 当前打出了一梭子子弹中的第index颗子弹
            //detail->setIntervalIndex(0);
            
            //RepeatTimes 重复开枪的次数
            detail->setCurrentRepeatTimes(0);
            
            //是否已经开了枪
            detail->setIsBeginFire(false);
            
            gun_details->addObject(detail);
        }
    }
}

/*********************************
 * 其他
 *********************************/
//获得父辈的坐标 用于转换成世界坐标（专用于飞机本身为BOSS附属飞机）
CCPoint CraftView::getHolderPosition()
{
    if (this->getParent())
    {
        return this->getParent()->getPosition();
    }
    
    return CCPointZero;
}

bool CraftView::isMoveOver()
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

void CraftView::hideBody()
{
    m_pParticleBatchNode->removeAllChildren();
    m_pParticleBatchNode1->removeAllChildren();
    nodeContainer->setVisible(false);
    
    
    int tag = kTagBattery;
    while (this->getChildByTag(tag))
    {
        CraftView* view = (CraftView*)(this->getChildByTag(tag));
        view->setVisible(false);
        view->setIsDied(true);
        
        tag ++;
    }
}

//飞机渐隐
void CraftView::craftFadeIn(float dt)
{
    CCSprite* sp = (CCSprite*)nodeContainer->getChildByTag(kTagShape);
    sp->runAction(CCFadeIn::create(dt));
}

/*********************************************
 * Transform
 *********************************************/

//设定XY位移
void CraftView::setPosition(const CCPoint& newPosition)
{
    CCPoint pos = this->getPosition();
    
    if (pos.x != newPosition.x || pos.y != newPosition.y)
    {
        CCNode::setPosition(newPosition);
        m_bCraftTransfromDirty = true;
    }
}

//设置转角，只是让nodeContainer旋转，自身不旋转
void CraftView::setRotation(float newRotation)
{
    if (nodeContainer->getRotation() != newRotation)
    {
        nodeContainer->setRotation(newRotation);
        
        m_bCraftTransfromDirty = true;
        m_bCraftInnerTransfromDirty = true;
    }
}

//获得转角
float CraftView::getRotation()
{
    return nodeContainer->getRotation();
}

//获得仿射矩阵
CCAffineTransform CraftView::getCraftTransform()
{
    if (m_bCraftTransfromDirty)
    {
        float craft_rotation = this->getRotation();
        float cos_angle = 1, sin_angle = 0;
        
        if (craft_rotation)
        {
            float radian = CC_DEGREES_TO_RADIANS(craft_rotation);
            
            cos_angle = cosf(radian);
            sin_angle = sinf(radian);
        }
        
        m_sCraftTransform = CCAffineTransformMake(cos_angle,
                                                  sin_angle,
                                                  -sin_angle,
                                                  cos_angle,
                                                  m_obPosition.x,
                                                  m_obPosition.y);
        
        m_bCraftTransfromDirty = false;
    }
    
    return m_sCraftTransform;
}

//获得内部放射矩阵
CCAffineTransform CraftView::getCraftInnerTransform()
{
    if (m_bCraftInnerTransfromDirty)
    {
        float craft_rotation = this->getRotation();
        float cos_angle = 1, sin_angle = 0;
        
        if (craft_rotation)
        {
            float radian = CC_DEGREES_TO_RADIANS(craft_rotation);
            
            cos_angle = cosf(radian);
            sin_angle = sinf(radian);
        }
        
        m_sCraftInnerTransform = CCAffineTransformMake(cos_angle,
                                                       sin_angle,
                                                       -sin_angle,
                                                       cos_angle,
                                                       0,
                                                       0);
        
        m_bCraftInnerTransfromDirty = false;
    }
    
    return m_sCraftInnerTransform;
}

/*********************************************
 * 出界判断
 *********************************************/
//飞机出界检查 如果出界 则不参与碰撞计算
bool CraftView::checkOutOfScreen()
{
    float xx = this->getPositionX();
    float yy = this->getPositionY();
    
    if(this->getIsAffiliatedPlane())
    {
        xx += this->getHolderPosition().x;
        yy += this->getHolderPosition().y;
    }
    
    int n_offset_x = 0;
    int n_offset_y_top = -50;
    int n_offset_y_bottom = 0;
    
    if(xx < - n_offset_x || xx > _s_width + n_offset_x || yy < - n_offset_y_bottom || yy > _s_height + n_offset_y_top)
    {
        return true;
    }
    
    return false;
}
/*********************************************
 * 检查金币是否执行自动收集的动作
 *********************************************/
bool CraftView::checkIsAutoCollected()
{
    float yy = this->getPositionY();
    CCLog("%f",yy);
    if (yy < _s_height * 0.90f)
    {
        return true;
    }
    return false;
}

//飞机出界检查 如果出界 则不参与碰撞计算
bool CraftView::checkOutOfScreenForBoom()
{
    float xx = this->getPositionX();
    float yy = this->getPositionY();
    
    if(this->getIsAffiliatedPlane())
    {
        xx += this->getHolderPosition().x;
        yy += this->getHolderPosition().y;
    }
    
    if(xx < 0 || xx > _s_width || yy < 0 || yy > _s_height)
    {
        return true;
    }
    
    return false;
}

/*********************************************
 * Path
 *********************************************/

void CraftView::setDataPathGroup(PathGroupData *path_group_data)
{
    dataPathGroup = path_group_data;
    dataPathGroup->retain();
}

PathGroupData* CraftView::getDataPathGroup()
{
    return dataPathGroup;
}

//发射子弹
void CraftView::shotBullet(float dt, GunDetail *detail)
{
    //如果RepeatTime越界了
    if (detail->isRepeatTimeCurrentLargeThenTotal())
    {
        return;
    }
    
    //当前时间与延迟时间进行比较
    //如果当前时间略微的超过延迟时间，则创建子弹
    
    if (time > detail->getTimeTarget())
    {
        //********************************
        //通用的 不管是否追踪
        //********************************
        
        if (detail->getIsTrack())
        {
            //********************************
            //若是追踪型
            //
            //其实就是重新计算出GunGroup的角度 然后交由外面的方法 来计算实际的发射角度
            //外面的方法 会重新计算GunGroup的Transfrom 继而就会发生奇妙的事儿了
            //********************************
            
            //*******************
            //计算开枪角度
            //每打完一组子弹之后，才进行更新
            //*******************
            
            //Hero的锚点位置，相对于整体坐标系
            CCPoint pos_hero = this->delegateBullet->getHeroPosition();
            
            //GunGroup的位置，相对于整体坐标系
            CCPoint pos_gungroup = CCPointApplyAffineTransform(detail->getGunGroupPosition(),
                                                               this->getCraftTransform());
            
            //把加载boss身上的飞机的坐标更新到世界坐标
            if(this->getIsAffiliatedPlane())
            {
                pos_gungroup = ccpAdd(pos_gungroup, this->getParent()->getPosition());
            }
            
            //计算开枪角度 从pos_gungroup_abs到pos_hero的方向
            float rot_gungroup = calcAngleFromOriginToTarget(pos_gungroup, pos_hero);
            
            //Dr.Ren开始讲解 这里为什么需要减去this->getRotation
            //因为pos_hero和pos_gungroup两者连线角度 是在世界坐标系中的
            //但是我这里需要的是gungroup自身的转动方向 相对于craft本身的
            //所以减去了craft自身的角度
            //如果还不懂 请拨打电话400-638-3399转脑部发育迟缓科室
            detail->setGunGroupRotation(rot_gungroup - this->getRotation());
        }
        else
        {
            //********************************
            //若不是追踪型
            //********************************
            
            //什么都不用干了
        }
        
        //绘制枪口
        if (this->getIsDrawWeaponPosition())
        {
            CCAffineTransform transform = CCAffineTransformConcat(detail->getGunGroupTransform(), this->getCraftInnerTransform());
            CCPoint pos_gun = CCPointApplyAffineTransform(detail->getGunPosition(), transform);
            
            //CCLog("POS GUN      (%4.2f, %4.2f) Angle=(%4.2f)", pos_gun.x, pos_gun.y, detail->getGunGroupRotation());
            
            dictPosWeapon->setObject(PointExt::create(pos_gun.x, pos_gun.y), detail->m_uID);
        }
        
        //创建子弹
        //调用接口
        if (delegateBullet)
        {
            //开始发子弹之前不可以再旋转
            if(this->getIsCanRotate() && this->getRecoil())
            {
                this->setIsCreatingBullet(true);
                CCSequence* seq = CCSequence::create(CCDelayTime::create(0.4f),
                                                     CCCallFunc::create(this, callfunc_selector(CraftView::callbackResumeRotate)),
                                                     NULL);
                this->runAction(seq);
                
                //->也许不用，不知道被Retota了之后，是否直接使用-20 +20就可以直接表现出震动的感觉了
                
                //发射位移
                //获取当前飞机的转角
                float nCurRotate = (this->getRotation() - 180.0f) * 3.14f / 180.0f;
                
                //float fOffset_x = 0 - sin(nCurRotate) * 20.0f;
                //float fOffset_y = 0 - cos(nCurRotate) * 20.0f;
                float fOffset_x = -20.0f;
                float fOffset_y = 0.0f;
                
                //是否有后坐力
                if(this->getRecoil() && !this->getRecoilAction())
                {
                    this->setRecoilAction(true);
                    
                    CCSprite* spShape = (CCSprite*)nodeContainer->getChildByTag(kTagShape);
                    
                    CCSequence* seq1 = CCSequence::create(CCMoveBy::create(0.1f, ccp(fOffset_x, fOffset_y)),
                                                          CCMoveBy::create(0.1f, ccp(-fOffset_x, -fOffset_y)),
                                                          NULL);
                    spShape->runAction(seq1);
                    
                    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(0.21f),
                                                          CCCallFuncN::create(this, callfuncN_selector(CraftView::callBackResumeRecoil)), 
                                                          NULL);
                    spShape->runAction(seq2);
                }
            }
            else if(!this->getIsCanRotate() && this->getRecoil())
            {
                //发射位移
                //获取当前飞机的转角
                float nCurRotate = (this->getRotation() - 180.0f) * 3.14f / 180.0f;
                
                //float fOffset_x = sin(nCurRotate) * 20.0f;
                //float fOffset_y = cos(nCurRotate) * 20.0f;
                float fOffset_x = -20.0f;
                float fOffset_y = 0.0f;

                
                //是否有后坐力
                if(this->getRecoil() && !this->getRecoilAction())
                {
                    this->setRecoilAction(true);
                    
                    CCSprite* spShape = (CCSprite*)nodeContainer->getChildByTag(kTagShape);
                    
                    CCSequence* seq1 = CCSequence::create(CCMoveBy::create(0.1f, ccp(fOffset_x, fOffset_y)),
                                                          CCMoveBy::create(0.1f, ccp(-fOffset_x, -fOffset_y)),
                                                          NULL);
                    spShape->runAction(seq1);
                    
                    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(0.21f),
                                                          CCCallFuncN::create(this, callfuncN_selector(CraftView::callBackResumeRecoil)), 
                                                          NULL);
                    spShape->runAction(seq2);
                }
            }
            
            delegateBullet->createCraftBullet(this, detail);
        }
        
        //刷新时间
        detail->refreshMidRepeatTimesAndIntervalIndex();
    }
}

//恢复后坐动作状态
void CraftView::callBackResumeRecoil(CCNode* node)
{
    this->setRecoilAction(false);
}

void CraftView::callbackResumeRotate()
{
    this->setIsCreatingBullet(false);
}

void CraftView::callbackClean(CCNode *pSender)
{
    pSender->removeFromParentAndCleanup(true);
    CC_SAFE_RELEASE(pSender);
}

//回复与光波炮的碰撞
void CraftView::resumeAction()
{
    CCSequence* seq = CCSequence::create(CCDelayTime::create(2.0f),
                                         CCCallFuncN::create(this, callfuncN_selector(CraftView::callBackResumeCollisionWithWaveGun)),
                                         NULL);
    
    this->runAction(seq);
}
void CraftView::callBackResumeCollisionWithWaveGun(CCNode* pSender)
{
    this->setCollisionWithWaveGun(false);
}

//回复与聚能炮的碰撞
void CraftView::resumeShapedGunAction()
{
    CCSequence* seq = CCSequence::create(CCDelayTime::create(0.2f),
                                         CCCallFuncN::create(this, callfuncN_selector(CraftView::callBackResumeCollisionWithShapedGun)),
                                         NULL);
    
    this->runAction(seq);
}
void CraftView::callBackResumeCollisionWithShapedGun(CCNode* pSender)
{
    this->setCollisionWithShapedGun(false);
}