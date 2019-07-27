#include "TargetGunGroupData.h"


TargetGunGroupData::TargetGunGroupData()
{
    //CCLog("TargetGunGroupData Created %d", m_uID);
}

TargetGunGroupData::~TargetGunGroupData()
{
    //CCLog("TargetGunGroupData Destroyed #%s", this->getID().c_str());
}

TargetGunGroupData* TargetGunGroupData::create(TiXmlElement *elm)
{
    TargetGunGroupData *data = new TargetGunGroupData();

    if(data && data->loadData(elm))
    {
        data->autorelease();
        return data;
    }

    return NULL;
}

bool TargetGunGroupData::loadData(TiXmlElement *elm)
{
    //属性
    if(elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }
    else
    {
        this->setID(getStringFromInt(m_uID));
    }

    if(elm->Attribute("gungroup_id"))
    {
        this->setGunGroupID(elm->Attribute("gungroup_id"));
    }
    else
    {
        this->setGunGroupID("1");
    }

    //CCLog("gungroup_id = %s", gungroup_id.c_str());

    if(elm->Attribute("x"))
    {
        this->setX(atof(elm->Attribute("x")));
    }
    else
    {
        this->setX(0.0f);
    }


    if(elm->Attribute("y"))
    {
        this->setY(atof(elm->Attribute("y")));
    }
    else
    {
        this->setY(0.0f);
    }

    if(elm->Attribute("first_delay"))
    {
        this->setFirstDelay(atof(elm->Attribute("first_delay")));
    }
    else
    {
        this->setFirstDelay(0.0f);
    }


    if(elm->Attribute("repeat_delay"))
    {
        this->setRepeatDelay(atof(elm->Attribute("repeat_delay")));
    }
    else
    {
        this->setRepeatDelay(0.0f);
    }


    if(elm->Attribute("repeat_times"))
    {
        this->setRepeatTimes(atoi(elm->Attribute("repeat_times")));
    }
    else
    {
        this->setRepeatTimes(0);
    }


    if(elm->Attribute("harm"))
    {
        this->setHarm(atof(elm->Attribute("harm")));
    }
    else
    {
        this->setHarm(0.0f);
    }

    if(elm->Attribute("is_track"))
    {
        this->setIsTrack(strncmp(elm->Attribute("is_track"), "1", 1) == 0 ? true : false);
    }
    else
    {
        this->setIsTrack(false);
    }

    if(elm->Attribute("rotation_init"))
    {
        this->setRotationInit(atof(elm->Attribute("rotation_init")));
    }
    else
    {
        this->setRotationInit(0.0f);
    }

    if(elm->Attribute("rotation_offset"))
    {
        this->setRotationOffset(atof(elm->Attribute("rotation_offset")));
    }
    else
    {
        this->setRotationOffset(0.0f);
    }
    
    if(elm->Attribute("mid_delay"))
    {
        this->setMidDelay(atof(elm->Attribute("mid_delay")));
    }
    else
    {
        this->setMidDelay(0.0f);
    }
    
    if(elm->Attribute("mid_repeat"))
    {
        this->setMidRepeat(atoi(elm->Attribute("mid_repeat")));
    }
    else
    {
        this->setMidRepeat(1);
    }
    
    this->setDelayTime(0.0f);
    
    if (elm->Attribute("is_fixed"))
    {
        this->setIsFixedGunDetail(strncmp(elm->Attribute("is_fixed"), "1", 1) == 0 ? true : false);
    }
    else
    {
        this->setIsFixedGunDetail(false);
    }

    return true;
}

void TargetGunGroupData::print()
{
    //CCLog("TargetGunGroupData first_delay=%f", first_delay);
}