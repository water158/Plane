//
//  LaserLayer.cpp
//  ambition
//
//  Created by mini01 on 15-2-12.
//
//

#include "LaserLayer.h"

LayserLayer::LayserLayer(GameCombatLayer* parent)
{
    m_pParent = parent;
    
    _bMainLaserIsVisible = false;
    
    _bShoulderLaserIsVisible = false;
    
    _bPinionLaserIsVisible = false;
    
    _bWingLaserIsVisible = false;
    
    this->init();
    
    m_pParent->addChild(this,GAME_ZORDER_HERO_BULLET);
}

LayserLayer::~LayserLayer()
{

}


bool LayserLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    //数据初始化
    
    m_bIsCross = false;
    
    /************初始化激光**************/
    
    //初始化主机激光
    EquipItem* equip_main = UserEquipBag::shared()->getMainEquip();
    
    int nMain_laserID = atoi(equip_main->getImgName().c_str());
    
    CCPoint pos_hero = m_pParent->m_pHeroView->getPosition();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/animations/laser_image.plist");
    
    if (nMain_laserID == 4)
    {
        m_mainLaserType = LASER_LINE;
        
        CCSprite* pSprite_laser = CCSprite::create("game/laser/laser.png");
        
        pSprite_laser->setAnchorPoint(ccp(0.5f, 0.0f));
        
        pSprite_laser->setScaleX(0.3f);
        
        pSprite_laser->setScaleY(_s_height / 40);
        
        pSprite_laser->setRotation(0);
        
        pSprite_laser->setPosition(ccp(0, 0));
        
        pSprite_laser->setVisible(false);
        
        addChild(pSprite_laser,1,TAG_LAYSER);
        
        //主炮左激光
        lightning* laser_left = lightning::create();
        
        laser_left->setVisible(false);
        
        laser_left->setBreadth(LINE_LASER_WIDTH_SCALE);
        
        addChild(laser_left,0,TAG_MAIN_LEFTLASER);
        
        //主炮右激光
        lightning* laser_right = lightning::create();
        
        laser_right->setVisible(false);
        
        laser_right->setBreadth(LINE_LASER_WIDTH_SCALE);
        
        addChild(laser_right,0,TAG_MAIN_RIGHTLASER);
        
    }
    else if (nMain_laserID == 5)
    {
        //竖直型激光 图片
        m_mainLaserType = LASER_IMAGE;
        
        createImageLaser(TAG_MAINLASER);
        
    }
    else if (nMain_laserID == 6)
    {
        //shader激光
        m_mainLaserType = LASER_SHADER;
        
        ClaserLayer * laser = new ClaserLayer();
        
        laser->init();
        
        laser->setVisible(false);
        
        addChild(laser,0,TAG_MAINLASER);
    }
    
    //初始化肩炮激光
    EquipItem* equip_shoulder = UserEquipBag::shared()->getFirSubEquip();
    
    int nShoulder_laserID = atoi(equip_shoulder->getImgName().c_str());
    
    if (nShoulder_laserID == 4)
    {
        //线型激光
        m_shoulderLaserType = LASER_LINE;
        //主炮左激光
        lightning* laser_left = lightning::create();
        
        laser_left->setVisible(false);
        
        laser_left->setBreadth(LINE_LASER_WIDTH_SCALE);
        
        addChild(laser_left,0,TAG_SHOULDER_LEFT);
        
        //主炮右激光
        lightning* laser_right = lightning::create();
        
        laser_right->setVisible(false);
        
        laser_right->setBreadth(LINE_LASER_WIDTH_SCALE);
        
        addChild(laser_right,0,TAG_SHOULDER_RIGHT);
        
    }
    else if (nShoulder_laserID == 5)
    {
        //竖直型激光  图片
        m_shoulderLaserType = LASER_IMAGE;
        
        createImageLaser(TAG_SHOULDER_LEFT);
        
        createImageLaser(TAG_SHOULDER_RIGHT);
        
    }
    else if (nShoulder_laserID == 6)
    {
        //shader激光
        m_shoulderLaserType = LASER_SHADER;
        
        ClaserLayer * laser = new ClaserLayer();
        
        laser->init();
        
        laser->setVisible(false);
        
        addChild(laser,0,TAG_SHOULDER_LEFT);

        
        ClaserLayer * laser_right = new ClaserLayer();
        
        
        laser_right->init();
        
        laser_right->setVisible(false);
        
        addChild(laser_right,0,TAG_SHOULDER_RIGHT);
    }
    
    //初始化翼炮激光
    EquipItem* equip_pinion = UserEquipBag::shared()->getSecSubEquip();
    
    int nPinion_laserID = atoi(equip_pinion->getImgName().c_str());
    
    if (nPinion_laserID == 4)
    {
        //线型激光
        m_pinionLaserType = LASER_LINE;
        
        //主炮左激光
        lightning* laser_left = lightning::create();
        
        laser_left->setBreadth(LINE_LASER_WIDTH_SCALE);
        
        laser_left->setVisible(false);
        
        addChild(laser_left,0,TAG_PINION_LEFT);
        
        //主炮右激光
        lightning* laser_right = lightning::create();
        
        laser_right->setBreadth(LINE_LASER_WIDTH_SCALE);
        
        laser_right->setVisible(false);
        
        addChild(laser_right,0,TAG_PINION_RIGHT);
    }
    else if (nPinion_laserID == 5)
    {
        //竖直激光 图片
        m_pinionLaserType = LASER_IMAGE;
        
        createImageLaser(TAG_PINION_LEFT);
        
        createImageLaser(TAG_PINION_RIGHT);
        
    }
    else if (nPinion_laserID == 6)
    {
        //shader激光
        m_pinionLaserType = LASER_SHADER;
        
        ClaserLayer * laser = new ClaserLayer();
        
        laser->init();
        
        laser->setVisible(false);
        
        addChild(laser,0,TAG_PINION_LEFT);
        
        ClaserLayer * laser_right = new ClaserLayer();
        
        laser_right->init();
        
        laser_right->setVisible(false);
        
        addChild(laser_right,0,TAG_PINION_RIGHT);
    }
    
    EquipItem* equip_wing = UserEquipBag::shared()->getWingEquip();
    
    int nWing_laser = atoi(equip_wing->getImgName().c_str());
    
    if (nWing_laser == 4)
    {
        //线型激光
        m_wingLaserType = LASER_LINE;
        
        lightning* laser_left = lightning::create();
        
        laser_left->setBreadth(LINE_LASER_WIDTH_SCALE);
        
        laser_left->setVisible(false);
        
        addChild(laser_left,0,TAG_WING_LASER);
    }
    else if(nWing_laser == 5)
    {
        //竖直激光 图片
        m_wingLaserType = LASER_IMAGE;
        
        createImageLaser(TAG_WING_LASER);
    }
    else if (nWing_laser == 6)
    {
        
        //shader 激光
        m_wingLaserType = LASER_SHADER;
        
        ClaserLayer * laser = new ClaserLayer();
        
        laser->init();
        
        laser->setVisible(false);
        
        addChild(laser,0,TAG_WING_LASER);
    }
    
    return true;
}

void LayserLayer::createImageLaser(int tag)
{
    //创建激光帧动画
    
    CCArray* arrFrame = CCArray::createWithCapacity(4);
    
    char str[100] = {0};
    
    for (int i = 1; i < 5; i++)
    {
        sprintf(str, "laser_imag_%d.png",i);
        
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        
        arrFrame->addObject(frame);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(arrFrame , 0.1f);
    
    animation->setLoops(-1);
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    CCSprite* pSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("laser_imag_1.png"));
    
    pSprite->setAnchorPoint(ccp(0.5, 0));
    
    pSprite->setScaleY(_s_height / pSprite->getContentSize().height);
    
    pSprite->setPosition(ccp(-pSprite->getContentSize().width, 0));
    
    pSprite->runAction(animate);
    
    pSprite->setVisible(false);
    
    addChild(pSprite,0,tag);
}



//根据激光的类型 和tag值来更新 激光
void LayserLayer::updateLaserBy(LaserType type,int tag,CraftView* craft)
{
    
    //定义激光的起点、终点
    CCPoint laser_a = CCPointZero;
    
    CCPoint laser_b = CCPointZero;
    
    //获取主机位置信息
    CraftData *heroCraft = DataManager::sharedDataManager()->getCraftData(CRAFT_DATA_TYPE_HERO, 0);
    
    CCPoint pos_hero = m_pParent->m_pHeroView->getPosition();
    
    EquipItem* item = NULL;
    
    float multi = 0.0f;  //倍数  不同炮管有不同的伤害倍数
    
    float addMulti = 0.0f;  //属性加成倍数
    
    //是否可见
    bool bIsVisible = true;
    
    //根据tag值判断是什么位置的炮  确定炮的起点坐标
    switch (tag)
    {
        case TAG_MAINLASER:  //主炮位置
        {
            PositionData* posData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("1");
            laser_a = ccp(pos_hero.x + posData->getX(), pos_hero.y + posData->getY());
            
            item = UserEquipBag::shared()->getMainEquip();
            
            multi = 1.0f;   //主炮伤害为100%
            
            addMulti = 1.0f;
            
            bIsVisible = _bMainLaserIsVisible;
            break;
        }
            
        case TAG_MAIN_LEFTLASER:  //主炮左激光位置  线型
        {
            laser_a = ccp(pos_hero.x - 30,pos_hero.y);
            
            item = UserEquipBag::shared()->getMainEquip();
            
            multi = 0.25f;  //暂时设定线性激光 主炮位置为主炮攻击的25%
            
            addMulti = 1.0f;
            
            bIsVisible = _bMainLaserIsVisible;
            break;
        }
            
        case TAG_MAIN_RIGHTLASER:  //主炮右激光位置  线型
        {
            laser_a = ccp(pos_hero.x + 30, pos_hero.y);
            
            item = UserEquipBag::shared()->getMainEquip();
            
            multi = 0.25f;  //暂时设定线性激光 主炮位置为主炮攻击的25%
            
            addMulti = 1.0f;
            
            bIsVisible = _bMainLaserIsVisible;
            break;
        }
            
        case TAG_SHOULDER_LEFT:  //左肩炮位置
        {
            PositionData* posData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("2");
            laser_a = ccp(pos_hero.x + posData->getX(), pos_hero.y + posData->getY());
            
            item = UserEquipBag::shared()->getFirSubEquip();
            
            multi = 0.5f;
            
            addMulti = 0.6f;
            
            bIsVisible = _bShoulderLaserIsVisible;
            break;
        }
            
        case TAG_SHOULDER_RIGHT:  //右肩炮位置
        {
            PositionData* posData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("3");
            laser_a = ccp(pos_hero.x + posData->getX(), pos_hero.y + posData->getY());
            
            item = UserEquipBag::shared()->getFirSubEquip();
            
            multi = 0.5f;
            
            addMulti = 0.6f;
            
            bIsVisible = _bShoulderLaserIsVisible;
            break;
        }
            
        case TAG_PINION_LEFT:    //左翼炮位置
        {
            PositionData* posData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("4");
            laser_a = ccp(pos_hero.x + posData->getX(), pos_hero.y + posData->getY());
            
            item = UserEquipBag::shared()->getSecSubEquip();
            
            multi = 0.5f;
            
            addMulti = 0.5f;
            
            bIsVisible = _bPinionLaserIsVisible;
            break;
        }
            
        case TAG_PINION_RIGHT:   //右翼炮位置
        {
            PositionData* posData = (PositionData*)heroCraft->getBatteryPositionData()->getDictPositionsData()->objectForKey("5");
            laser_a = ccp(pos_hero.x + posData->getX(), pos_hero.y + posData->getY());
            
            item = UserEquipBag::shared()->getSecSubEquip();
            
            multi = 0.5f;
            
            addMulti = 0.5f;
            
            bIsVisible = _bPinionLaserIsVisible;
            break;
        }
            
        case TAG_WING_LASER:   //僚机位置
        {
            laser_a = m_pParent->getWingPosition();
            
            item = UserEquipBag::shared()->getWingEquip();
            
            multi = 1.0f;
            
            addMulti = 0.33f;
            
            bIsVisible = _bWingLaserIsVisible;
            break;
        }
            
        default:
            break;
    }
    
    float base_harm = item->getAttack() * item->getLevel();
    
    int attack = base_harm * (100 + item->getAttackAdd() * addMulti) / 100.0  * multi;
    
    //shader型激光处理 和 图片型激光处理  逻辑一致  故写在一起
    if (type == LASER_SHADER || type == LASER_IMAGE)
    {
        laser_b = ccp(laser_a.x, laser_a.y + _s_height);
        
        //shader激光
        ClaserLayer* laser = NULL;
        if (type == LASER_SHADER)
        {
            laser = (ClaserLayer*)this->getChildByTag(tag);
        }
        
        
        //图片型激光
        CCSprite* spr_laser = NULL;
        if (type == LASER_IMAGE)
        {
            spr_laser = (CCSprite*)this->getChildByTag(tag);
        }
        
        
        //遍历敌机字典  判断是否有碰撞产生
        CCDictionary* dic = m_pParent->m_pDictEnemy;
        
        bool isCross = false;   //是否碰撞
        
        CCDictElement *_ele = NULL;
        CCDICT_FOREACH(dic, _ele)
        {
            CraftView *craft = (CraftView *)_ele->getObject();
            
            //所有碰撞数据
            CollisionsData* pCollisionData1 = craft->dataCraft->getCollisionsData();
            
            //碰撞线  确定激光的长短
            CollisionLineData* line = pCollisionData1->getCollisionLineData();
            
            CCPoint tar_a = ccp(craft->getPositionX() + line->getLeftX(),craft->getPositionY() + line->getLeftY());
            
            CCPoint tar_b = ccp(craft->getPositionX() + line->getRightX(),craft->getPositionY() + line->getRightY());
            
            CCPoint cross = CCPointZero;
            bool bCross = ccpSegmentIntersectPoint(tar_a,tar_b,laser_a,laser_b,cross);
            
            CCLog("cross x:%0.1f, y:%0.1f",cross.x,cross.y);
    
            if (craft)
            {
                if (bCross)
                {
                    EnemyView* enemy = (EnemyView*)_ele->getObject();
                    
                    if (bIsVisible)
                    {
                        enemy->theHurted(attack / 60.0f);
                    }
                    
                    if (laser)
                    {
                        laser->setStartAndEnd(laser_a, cross);
                        
                        laser->setVisible(bIsVisible);
                    }
                    
                    if (spr_laser)
                    {
                        float dis = ccpDistance(laser_a, cross);
                        
                        spr_laser->setScaleY(dis / spr_laser->getContentSize().height);
                        
                        spr_laser->setVisible(bIsVisible);
                        
                        spr_laser->setPosition(laser_a);
                    }
                    
                    isCross = true;
                }
            }
            
        }
        
        if (!isCross)
        {
            if (laser)
            {
                laser->setStartAndEnd(laser_a, laser_b);
            }
            
            if (spr_laser)
            {
                spr_laser->setScaleY(_s_height / spr_laser->getContentSize().height);
                
                spr_laser->setPosition(laser_a);
            }
        }
        
    }
    else   //线型激光
    {
        lightning* laser = (lightning*)this->getChildByTag(tag);
        
        
        
        if(craft)
        {
            CCPoint pos_enemy = craft->getPosition();
            
            if (laser)
            {
                laser->setVisible(bIsVisible);
                
                laser->setStartAndEnd(laser_a, pos_enemy);
                
                EnemyView* enemy = (EnemyView*)craft;
                
                
                if (bIsVisible)
                {
                    enemy->theHurted(attack / 60.0f);
                }
            }
        }
        else
        {
            if(laser)
            {
                laser->setVisible(false);
            }
        }

    }
    
}

void LayserLayer::setShoulderLaserVisible(bool visible)
{
    _bShoulderLaserIsVisible = visible;
}

void LayserLayer::setPinionLaserVisible(bool visible)
{
    _bPinionLaserIsVisible = visible;
}

void LayserLayer::setMainLaserVisible(bool visible)
{
    _bMainLaserIsVisible = visible;
}

void LayserLayer::setWingLaserVisible(bool visible)
{
    _bWingLaserIsVisible = visible;
}


void LayserLayer::updateLaser(float dt)
{
    if (m_pParent && m_pParent->m_pHeroView)
    {
        CCPoint pos_hero = m_pParent->m_pHeroView->getPosition();
        
        CraftView* craft = m_pParent->getAimedCraft();
        
        /*************** 激光处理 ********************/
        
        //更新主机激光
        if (m_mainLaserType == LASER_LINE)
        {
            updateLaserBy(m_mainLaserType,TAG_MAIN_LEFTLASER,craft);
            
            updateLaserBy(m_mainLaserType,TAG_MAIN_RIGHTLASER,craft);
            
        }
        else
        {
            updateLaserBy(m_mainLaserType,TAG_MAINLASER,craft);
        }
        
        //更新肩炮激光
        updateLaserBy(m_shoulderLaserType,TAG_SHOULDER_LEFT,craft);
        
        updateLaserBy(m_shoulderLaserType,TAG_SHOULDER_RIGHT,craft);
        
        //更新翼炮激光
        updateLaserBy(m_pinionLaserType,TAG_PINION_LEFT,craft);
        
        updateLaserBy(m_pinionLaserType,TAG_PINION_RIGHT,craft);
        
        //更新僚机激光
        updateLaserBy(m_wingLaserType,TAG_WING_LASER,craft);
        
        
        
        if (!craft)
        {
            setMainLaserVisible(false);
        }
        else
        {
            setMainLaserVisible(true);
        }
        
        /*************** 大激光处理 ********************/
        
        CCSprite* pLaser = (CCSprite*)this->getChildByTag(TAG_LAYSER);
        
        if (pLaser && craft)
        {
            pLaser->setVisible(_bMainLaserIsVisible);
            
            pLaser->setPosition(pos_hero);
            
            //if (!m_bIsCross)
            {
                CCPoint pos_enemy = craft->getPosition();
                
                CCPoint v = ccp(pos_enemy.x - pos_hero.x, pos_enemy.y - pos_hero.y);
                
                float angle_heroAndEnemy = CC_RADIANS_TO_DEGREES(ccpToAngle(v));
                
                float angle_laser = pLaser->getRotation();
                
                if ((angle_heroAndEnemy - 90 - angle_laser) >= 2)
                {
                    pLaser->setRotation(pLaser->getRotation() + 1.3f);
                }
                else if ( (angle_heroAndEnemy - 90 - angle_laser) <= -2 )
                {
                    pLaser->setRotation(pLaser->getRotation() - 1.3f);
                }
            }
            
            CCPoint pos_enemy = craft->getPosition();
            
            CCPoint laser_A = pos_hero;
            
            float laser_rotation = pLaser->getRotation();
            
            CCPoint laser_B = ccpAdd(laser_A, ccpForAngle(CC_DEGREES_TO_RADIANS(laser_rotation - 270)) * CCDirector::sharedDirector()->getVisibleSize().height);
            
            CollisionLineData* lineData = craft->dataCraft->getCollisionsData()->getCollisionLineData();
            
            CCPoint tar_A = ccp(pos_enemy.x + lineData->getLeftX(),pos_enemy.y + lineData->getLeftY());
            
            CCPoint tar_B = ccp(pos_enemy.x + lineData->getRightX(),pos_enemy.y + lineData->getRightY());

            CCPoint cross = CCPointZero;
            bool isCross =  ccpSegmentIntersectPoint(tar_A,tar_B,laser_A,laser_B,cross);
            
            if (isCross)
            {
                float dis = ccpDistance(pos_hero, cross);
                
                EquipItem* item = UserEquipBag::shared()->getMainEquip();
                
                float base_harm = item->getAttack() * item->getLevel();
                
                int attack = base_harm * (100 + item->getAttackAdd()) / 100.0 / 2;
                
                EnemyView* enemy = (EnemyView*)craft;
                
                enemy->theHurted(attack * dt);
                
                pLaser->setScaleY((dis) / pLaser->getContentSize().height);
                
//                CCLog("height is : %f",pLaser->getContentSize().height);
                
                m_bIsCross = true;
                
            }
            else
            {
                pLaser->setScaleY(500);
                
                m_bIsCross = false;
            }
        }
        
        if (pLaser && !craft)
        {
            pLaser->setRotation(0);
            
            pLaser->setVisible(false);
            
            m_bIsCross = false;
        }
    
    }
}

