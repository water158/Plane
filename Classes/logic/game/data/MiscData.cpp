//
//  MiscData.cpp
//  ambition
//
//  Created by mini02 on 14/11/28.
//
//

#include "MiscData.h"

MiscData::MiscData()
{

}

MiscData::~MiscData()
{

}

MiscData* MiscData::create(TiXmlElement *elm)
{
    MiscData* data = new MiscData();
    if (data && data->loadData(elm)) {
        
        return data;
    }

    return NULL;
}

bool MiscData::loadData(TiXmlElement *elm)
{
    this->setHealdth(0.0f);
    this->setStreakType(0);
    this->setStreakParticleType(0);
    this->setBulletHitType(0);
    this->setBulletShootType(0);
    this->setIsBulletSelfRotate(false);
    this->setBulletSelfRotateTime(0.0f);
    
    if (elm->Attribute("healdth"))
    {
        this->setHealdth(getFloatFromS(elm->Attribute("healdth")));
    }
    
    if (elm->Attribute("streak_type"))
    {
        this->setStreakType(getFloatFromS(elm->Attribute("streak_type")));
    }
    
    if (elm->Attribute("particle_type"))
    {
        this->setStreakParticleType(getFloatFromS(elm->Attribute("particle_type")));
    }
    
    if (elm->Attribute("hit_type"))
    {
        this->setBulletHitType(getIntFromS(elm->Attribute("hit_type")));
    }
    
    if (elm->Attribute("shoot_type"))
    {
        this->setBulletShootType(getIntFromS(elm->Attribute("shoot_type")));
    }
    
    if (elm->Attribute("is_self_rotate"))
    {
        this->setIsBulletSelfRotate(atoi(elm->Attribute("is_self_rotate")) == 1 ? true : false);
    }
    
    if (elm->Attribute("self_rotate"))
    {
        this->setBulletSelfRotateTime(getFloatFromS(elm->Attribute("self_rotate")));
    }

    return true;
}