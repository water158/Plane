#include "WingView.h"


WingView::WingView()
{
    m_pEquipItemData = NULL;
    pos_time = 0;
    attack_time = 0;
    m_posState = WING_POSITION_STATE_RIGHT;
    isFire = false;
    
    m_canAttack = false;
}

WingView::~WingView()
{
    
}

WingView* WingView::create(CraftData* data, EquipItem* item_data)
{
    WingView* pNode = new WingView();
    if(pNode && pNode->init(data, item_data))
    {
        pNode->autorelease();
        return pNode;
    }
    
    return NULL;
}

bool WingView::init(CraftData* data, EquipItem* item_data)
{
    if(!CraftView::init(data))
    {
        return false;
    }
    
    //设置炮台类型
    this->setType(CRAFT_TYPE_WING);
    
    m_pEquipItemData = item_data;

    //获取变形动画
    setTransformAnimation();
    
    //初始化dataTargetConnon
    
    string wing_id = item_data->getImgName();
    
    dataTargetConon = DataManager::sharedDataManager()->getTargetConnon(wing_id);
    
    if (!dataTargetConon)
    {
        return false;
    }
    
    return true;
}

void WingView::startWingUpdate()
{
    //更新僚机状态 （是在左边还是右边）
    this->schedule(schedule_selector(WingView::updateState), 1.0f);
    
    //更新僚机自身位置
    this->schedule(schedule_selector(WingView::updatePosition), 1.0f);
    
    //更新僚机攻击
    this->schedule(schedule_selector(WingView::updateAttack), 1.0f);

}

void WingView::setTransformAnimation()
{
    
    wing_anim = (CCArmature*)nodeContainer->getChildByTag(kTagShape);

}

void WingView::callbackStartAttackTransform()
{
    
    wing_anim->getAnimation()->play("attack_open");
    
    //开始攻击
    CCCallFunc* startFire = CCCallFunc::create(this, callfunc_selector(WingView::callbackStartFire));
    //停止攻击
    CCCallFunc* stopFire = CCCallFunc::create(this, callfunc_selector(WingView::callbackStopFire));
    
    //停止变形
    CCCallFunc* transform_end = CCCallFunc::create(this, callfunc_selector(WingView::callbackStopAttackTransform));

    
    wing_anim->runAction(CCSequence::create(CCDelayTime::create(0.25f),startFire,CCDelayTime::create(3.0f),stopFire,CCDelayTime::create(0.25f),transform_end,NULL));
    
}

void WingView::callbackStopAttackTransform()
{
    wing_anim->getAnimation()->play("attack_close");

    //开始随机移动
    CCCallFunc* moveByRandom = CCCallFunc::create(this, callfunc_selector(WingView::callbackRandomMoveStart));

    //开始攻击计时
    CCCallFunc* startAttack = CCCallFunc::create(this, callfunc_selector(WingView::callbackAttackStart));
    
    //开始左右移动计时
    CCCallFunc* notFire = CCCallFunc::create(this, callfunc_selector(WingView::callbackNotFire));
    
    wing_anim->runAction(CCSequence::create(startAttack,notFire,startAttack,moveByRandom,NULL));//startAttack,notFire,startAttack,moveByRandom
}

void WingView::update(float dt)
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
            TargetGunGroupData *dataTargetGG = detail->getTargetGunGroupData();
            
            //设置枪管的伤害值
            dataTargetGG->setHarm(m_pEquipItemData->getAttack());
            //设置腔管的伤害系数
            detail->setHarmRatio(0.03f);
            
            shotBullet(dt, detail);
        }
    }
    
    time += dt;
}

//僚机左右移动
void WingView::updateState(float dt)
{
    //如果是正在攻击 不进行计时
    if (isFire)
    {
        return;
    }
    
    pos_time += dt;
    
    if (pos_time > MOVE_INTERVAL_TIME) {
        
        CCPoint beginPos = CCPointZero;
        CCPoint endPos = CCPointZero;
        
        
        if (m_posState == WING_POSITION_STATE_LIFT) {
            
            beginPos = this->getPosOffsetL();
            endPos = this->getPosOffsetR();
            
        }else if (m_posState == WING_POSITION_STATE_RIGHT){
            
            beginPos = this->getPosOffsetR();
            endPos = this->getPosOffsetL();
        }
        
        CCDelayTime* delay = CCDelayTime::create(0.1f);
        
        //停止随机移动
        CCCallFunc* stopRandomMove = CCCallFunc::create(this, callfunc_selector(WingView::callbackRandomMoveEnd));
        
        //停止左右移动计时
        CCCallFunc* stopMoveUpdate = CCCallFunc::create(this, callfunc_selector(WingView::callbackMoveEnd));
        
        //先移动到起始位置
        CCMoveTo* moveBegin = CCMoveTo::create(0.5f, beginPos);
        //再移动到目标位置
        CCMoveTo* moveEnd = CCMoveTo::create(1.8f, endPos);
        //CCEaseInOut* easeMove = CCEaseInOut::create(moveEnd,1.5f);
        
        
        //开始随机移动
        //CCCallFunc* startRandom = CCCallFunc::create(this, callfunc_selector(WingView::callbackRandomMoveStart));
        
        //开启左右移动计时
        CCCallFunc* startMoveUpdate = CCCallFunc::create(this, callfunc_selector(WingView::callbackMoveStart));
        //设置僚机位置状态
        CCCallFunc* state = CCCallFunc::create(this, callfunc_selector(WingView::callbackSetMoveState));
        
        this->runAction(CCSequence::create(state,stopRandomMove,stopMoveUpdate,moveBegin,delay,moveEnd,delay,startMoveUpdate,NULL));//,moveBegin,delay
        
        pos_time = 0;
    }
    
}

void WingView::callbackSetMoveState()
{
    if (m_posState == WING_POSITION_STATE_LIFT) {
        
        m_posState = WING_POSITION_STATE_RIGHT;
        
    }else if (m_posState == WING_POSITION_STATE_RIGHT){
        
        m_posState = WING_POSITION_STATE_LIFT;
    }
    
}

void WingView::callbackMoveStart()
{
    pos_time = 0;
    this->schedule(schedule_selector(WingView::updateState), 1.0f);
}

void WingView::callbackMoveEnd()
{
    this->unschedule(schedule_selector(WingView::updateState));
    pos_time = 0;
}

//僚机自身随机移动
void WingView::updatePosition(float dt)
{
    //飞机最大速度(单位是像素)
    float maxSpeed = 250;
    //飞机现在的位置
    CCPoint posNow = this->getPosition();
    //下一个目标点
    CCPoint posNext = this->getNextAimPos();
    //第二个目标点
    CCPoint posSecond = this->getSecondAimPos();
    
    //获取范围
    float dw = 50.0f * 0.230f;
    float dh = 50.0f * 0.498f;
    
    CCPoint center = CCPointZero;
    
    if (m_posState == WING_POSITION_STATE_LIFT) {
        
        center = this->getPosOffsetL();
    }else if (m_posState == WING_POSITION_STATE_RIGHT){
        
        center = this->getPosOffsetR();
    }
    
    if(posNext.x == 0.0f && posNext.y == 0.0f)
    {
        //第一次随机一个
        posNext = getRandomPositionByBound(center, 10, 10, dw, dh);
        this->setNextAimPos(posNext);
    }
    else
    {
        //将第二个随机点赋值给第一个
        posNext = posSecond;
    }
    
    //第二个随机点重新赋值
    posSecond = getRandomPositionByBound(center, 10, 10, dw, dh);
    this->setSecondAimPos(posSecond);
    
    CCSequence* seq = NULL;
    
    //计算与目标点的距离
    float fLength = calcTwoPointLength(posNow, posNext);
    //速度从0到最大运行的距离
    float fMaxLen = 40;
    //位移向量
    CCPoint offset = ccpSub(posNext, posNow);
    //第一段加速
    CCPoint vec_first = CCPointZero;
    
    //匀速的距离
    float temp_len = fLength - fMaxLen*2;
    //匀速的时间
    float fTime = temp_len/maxSpeed;
    
    vec_first = ccp(offset.x * ((fMaxLen) / fLength), offset.y * ((fMaxLen) / fLength));
    CCPoint vec_second = ccp(offset.x * ((temp_len) / fLength), offset.y * ((temp_len) / fLength));
    
    seq = CCSequence::create(JumpByAction::create(vec_first, true),
                             CCMoveBy::create(fTime, vec_second),
                             JumpByAction::create(vec_first, false),
                             NULL);
    
    this->runAction(seq);
}

//僚机开始随机移动
void WingView::callbackRandomMoveStart()
{
    
    this->schedule(schedule_selector(WingView::updatePosition), 1.0f);
}

//僚机结束随机移动
void WingView::callbackRandomMoveEnd()
{
    
    this->unschedule(schedule_selector(WingView::updatePosition));
}

void WingView::updateAttack(float dt)
{
    attack_time += dt;
    
    if (attack_time > ATTACK_INTERVAL_TIME) {
        
        //停止随机移动
        CCCallFunc* stopRandomMove = CCCallFunc::create(this, callfunc_selector(WingView::callbackRandomMoveEnd));
        //停止攻击计时
        CCCallFunc* stopAttack = CCCallFunc::create(this, callfunc_selector(WingView::callbackAttackStop));
        //停止左右移动计时
        CCCallFunc* stopMoveByLR = CCCallFunc::create(this, callfunc_selector(WingView::callbackIsFire));
        
        //攻击动画
        CCCallFunc* attackTransform = CCCallFunc::create(this, callfunc_selector(WingView::callbackStartAttackTransform));
        
        
        CCSequence* seq = CCSequence::create(CCSpawn::create(stopMoveByLR,stopAttack,stopRandomMove,NULL),
                                             CCDelayTime::create(1.0f),
                                             attackTransform,
                                             NULL);
        
        this->runAction(seq);
        
        attack_time = 0;
    }
}

void WingView::callbackIsFire()
{
    isFire = true;
}

void WingView::callbackNotFire()
{
    isFire = false;
}

void WingView::callbackAttackStart()
{
    this->schedule(schedule_selector(WingView::updateAttack), 1.0f);
}

void WingView::callbackAttackStop()
{
    
    this->unschedule(schedule_selector(WingView::updateAttack));
    
}

void WingView::callbackStartFire()
{
    this->schedule(schedule_selector(WingView::update), 1.0f / 60.0f);
}

void WingView::callbackStopFire()
{
    this->unschedule(schedule_selector(WingView::update));
}

void WingView::changePhrase(int phrase)
{
    if (dataTargetConon)
    {
        this->setPhrase(phrase);
        
        unscheduleUpdate();
        
        //清理所有枪管
        gun_details->removeAllObjects();
        
        //构建新的枪管
        string phrase_id = getStringFromInt(phrase);
        
        PhaseData* dataPhrase = dataTargetConon->getPhaseData(phrase_id);
        
        if (!dataPhrase)
        {
            return;
        }
        
        CCArray* phrase_targetgungroups = dataPhrase->getTargetGunGroup();
        
        //准备GunDetail是每次update的时候 用到的数据
        prepareGunDetail(phrase_targetgungroups);
        
        initWeapon();
    }
}

void WingView::initWeapon()
{
    //开始UPDATE
    //this->scheduleUpdateWithPriority(0);
}

