//
//  ToolsView.cpp
//  ambition
//
//  Created by mini02 on 14/11/19.
//
//

#include "ToolsView.h"
#include "HeroView.h"
#include "UserData.h"
ToolsView::ToolsView()
{
   
}

ToolsView::~ToolsView()
{
    //CC_SAFE_RELEASE(heroTmp);
}
//创建
ToolsView* ToolsView::create(CraftData* dataCraft, TargetPlaneData* dataTargetPlane,bool isHave)
{
    ToolsView* tools = new ToolsView();
    if (tools && tools->init(dataCraft, dataTargetPlane,isHave))
    {
        tools->autorelease();
        return tools;
    }
    
    return NULL;
}

bool ToolsView::init(CraftData* data, TargetPlaneData* dataTargetPlane,bool isHave)
{
    EnemyView::init(data, dataTargetPlane);
    
    //初始化道具类型
    this->setToolType(-1);
    
    //设置偏移量
    this->setPosOffset(ccp(0,200));
    
    //设置和主机没有碰撞
    this->setIsCollisionWithHero(false);
    
    //屏幕上移动速度
    this->setSpeedVX(CCRANDOM_0_1() > 0.5f ? 3 : -3);
    this->setSpeedVY(CCRANDOM_0_1() > 0.5f ? 3 : -3);
    
    //血量
    this->setCurrentHP(1);
    this->setTotalHP(1);
    this->setIsDisplayHp(false);
    
    isMovoToNewPos = false;
    
    m_posTyps = 0;
    
    //如果道具已经吃到 则在主机前面移动
    if (!isHave)
    {
    
        this->schedule(schedule_selector(ToolsView::updatePosition),0.1f / 60.0f);
    }
    
    //道具自转
    this->runAction(CCRepeatForever::create(CCRotateBy::create(2.0f, 360)));
    
    return true;
}

//道具位置定时器
void ToolsView::updatePosition(float dt)
{
    this->setPositionX(this->getPositionX() + m_nVx);
    this->setPositionY(this->getPositionY() + m_nVy);
    
    // 限定上
    if(this->getPositionY() >= _s_height - this->getContentSize().height / 2)
    {
        m_nVy *= -1;
        this->setPositionY( _s_height - this->getContentSize().height / 2);
    }
    
    // 限定下
    if(this->getPositionY() <= this->getContentSize().height / 2)
    {
        m_nVy *= -1;
        this->setPositionY( this->getContentSize().height / 2);
    }
    
    // 限定左
    if(this->getPositionX() <= this->getContentSize().width / 2)
    {
        m_nVx *= -1;
        this->setPositionX(this->getContentSize().width / 2);
    }
    
    // 限定右
    if(this->getPositionX() >= _s_width - this->getContentSize().width / 2)
    {
        m_nVx *= -1;
        this->setPositionX( _s_width - this->getContentSize().width / 2);
    }
    
    this->setRotation(this->getRotation() + 3);
    
}

//更新速度变化
void ToolsView::updateRandomPosition(float dt)
{
    //飞机最大速度(单位是像素)
    float maxSpeed = 100;
    //飞机现在的位置
    CCPoint posNow = this->getPosition();
    //下一个目标点
    CCPoint posNext = this->getNextAimPos();
    //第二个目标点
    CCPoint posSecond = this->getSecondAimPos();
    
    //获取范围
    float dw = 50.0f * 0.430f;
    float dh = 50.0f * 0.298f;
    
    CCPoint center = this->getPosition();
    
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

//创建粒子
void ToolsView::createToolParticle()
{
    //获取道具类型
    int type = this->getToolType();
    
    //粒子
    string path = "game/particles/prop_";
    path.append(getStringFromInt(type + 1)).append(".plist");
    
    FerrisParticle* particle = FerrisParticle::create(path.c_str());
    particle->setPosition(ccp(0,0));
    particle->setPositionType(kCCPositionTypeGrouped);
    this->addChild(particle);
}

void ToolsView::startUpdateFlyToolPosition()
{
    this->schedule(schedule_selector(ToolsView::updateFlyToolPosition), 0.1f / 60.0f);
}

//主机前飞行道具位置定时器
void ToolsView::updateFlyToolPosition(float dt)
{
    
    if (!heroTmp || heroTmp->getIsDied())
    {
        return;
    }
    
    CCPoint hero_pos = heroTmp->getPosition();
    CCPoint follow_pos = this->getPosition();
    
    int r = 100;
    
    CCPoint tmp_pos = ccpAdd(hero_pos, CCPoint(-25,150));
    
    if (m_posTyps == 1)
    {
        tmp_pos = ccpAdd(hero_pos, CCPoint(25,150));
    }
    
    float distance = tmp_pos.getDistance(follow_pos);
    
    float x = 0;
    float y = 0;
    
    CCPoint offest = ccpSub(tmp_pos, follow_pos);
    float radian = atanf(offest.y / offest.x);
    
    int f = 1;
    if (offest.x > 0)
    {
        f = -1;
    }
    
    if (distance >= r)
    {
        x = (r + 60) *  f * cosf(radian);
        y = (r + 60) *  f * sinf(radian);
        
        x = -x / 200;
        y = -y / 200;

        this->runAction(CCSequence::create(CCMoveBy::create(0.05,CCPoint(x * 3, y * 3)),CCMoveBy::create(0.5,CCPoint(x * 3, y * 3)),CCMoveBy::create(1.2,CCPoint(x, y)),NULL));
    }
    
}

void ToolsView::updateMagnetAction(float dt)
{
    if (!heroTmp || heroTmp->getIsDied())
    {
        return;
    }
    
    //如果存在动作
    MagnetCoinAction* act = (MagnetCoinAction*)this->getActionByTag(TAG_SPEEDUP_ACTION);
    
    if(act)
    {
        CCPoint pos = heroTmp->getPosition();
        act->setEndpos(pos);
    }
}

void ToolsView::startUpdateMagnet()
{
    this->schedule(schedule_selector(ToolsView::updateMagnetAction), 1.0f / 60);
}