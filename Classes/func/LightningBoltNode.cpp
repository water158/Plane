//
//  LightningBoltNode.cpp
//  HelloCpp
//
//  Created by renyi on 14/12/12.
//
//

#include "LightningBoltNode.h"


LightningBoltNode::LightningBoltNode()
{
    m_lbSprite = NULL;
    m_lbSprite2 = NULL;
    m_isDrawDebug = false;
}

bool LightningBoltNode::init(const CCPoint& start,const CCPoint& end)
{
    m_oStartPoint = start;
    m_oEndPoint = end;
    
    //lightingSprit
    {
        m_lbSprite=new LightningBoltSprite();
        m_lbSprite->autorelease();
        m_lbSprite->init();
        m_lbSprite->setStartAndEnd(start, end);
        m_lbSprite->genLighting();
        this->addChild(m_lbSprite);
    }
    
    
    //lightingSprit2
    {
        m_lbSprite2=new LightningBoltSprite();
        m_lbSprite2->autorelease();
        m_lbSprite2->init();
        m_lbSprite2->setStartAndEnd(m_oStartPoint, m_oEndPoint);
        m_lbSprite2->genLighting();
       // this->addChild(m_lbSprite2);
        
    }
    
    lightningbullet= CCSprite::create("game/animations/lightning_shoot_effect.png");
    lightningbullet->setPosition(start);
    this->addChild(lightningbullet);
    
    block=CCSprite::create("game/animations/lightning_hit_effect.png");
    block->setPosition(end);
    block->setScaleY(2.0f);
    block->setOpacity(60);
    this->addChild(block);
    CCActionInterval * rotateto = CCRotateTo::create(2, rand()*180);
    block->runAction(CCRepeatForever::create(rotateto));
    
    
    return true;
}

void LightningBoltNode::setStartAndEnd(const CCPoint &start, const CCPoint &end)
{
    m_oStartPoint = start;
    m_oEndPoint = end;
    
    m_lbSprite->setStartAndEnd(m_oStartPoint, m_oEndPoint);
    m_lbSprite2->setStartAndEnd(m_oStartPoint, m_oEndPoint);
    
    lightningbullet->setPosition(start);
}

void LightningBoltNode::updatePosition(const CCPoint &start, const CCPoint &end)
{
    m_lbSprite->setStartAndEnd(start, end);
    m_lbSprite->genLighting();
    
    m_lbSprite2->setStartAndEnd(start, end);
    m_lbSprite2->genLighting();
    
    //击中效果
    lightningbullet->setPosition(start);
    block->setPosition(end);
}