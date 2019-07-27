#include "GunData.h"


GunData::GunData()
{
}

GunData::~GunData()
{
}

GunData* GunData::create(TiXmlElement *elm)
{
    GunData *data = new GunData();

    if(data && data->loadData(elm))
    {
        data->autorelease();
        return data;
    }

    return NULL;
}

bool GunData::loadData(TiXmlElement *elm)
{
    //必要属性
    this->setBulletID(elm->Attribute("bullet_id"));
	this->setTrailGroupID(-1);
	this->setTrailGroupAngle(0.0f);
	this->setOffsetAngle(-1.0f);
	this->setPercentageHeight(5.0f);
	this->setChildBulletID(1);
    
	//可选属性
    if (elm->Attribute("id"))
    this->setIsTrace(false);
	this->setIsPass(false);
    this->setTrailGroupID(-1);
    this->setTrailGroupAngle(0.0f);
    this->setIsMissile(false);
    this->setRadiateType(1);
    this->setDelayTime(0.0f);

    //可选属性
    if(elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }
    else
    {
        this->setID(getStringFromInt(m_uID));
    }

    /********************************
     * 通用
     ********************************/
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

    if(elm->Attribute("delay"))
    {
        this->setFirstDelay(atof(elm->Attribute("delay")));
    }
    else
    {
        this->setFirstDelay(0.0f);
    }

    if(elm->Attribute("rotation"))
    {
        this->setDirectionAngle(atof(elm->Attribute("rotation")));
    }
    else
    {
        this->setDirectionAngle(0.0f);
    }

    /********************************
     * 子弹
     ********************************/

    if(elm->Attribute("speed"))
    {
        this->setSpeed(atof(elm->Attribute("speed")));
    }
    else
    {
        this->setSpeed(0.0f);
    }

    if(elm->Attribute("is_trace"))
    {
        this->setIsTrace(atoi(elm->Attribute("is_trace")) == 0 ? false : true);
    }

    if(elm->Attribute("is_pass"))
    {
        this->setIsPass(atoi(elm->Attribute("is_pass")) == 0 ? false : true);
    }
    
    if(elm->Attribute("is_missile"))
    {
        this->setIsMissile(atoi(elm->Attribute("is_missile")) == 0 ? false : true);
    }

    //初始化并不是XML中得属性
    this->setTotalInterval(0.0f);

    //子元素
    this->loadIntervals(elm);

    if(elm->Attribute("trail_group_id"))
    {
        this->setTrailGroupID(atoi(elm->Attribute("trail_group_id")));
    }

	if(elm->Attribute("trail_group_angle"))
	{
		this->setTrailGroupAngle(atof(elm->Attribute("trail_group_angle")));
	}

	if (elm->Attribute("offset_angle"))
	{
		this->setOffsetAngle(atof(elm->Attribute("offset_angle")));
	}

	if (elm->Attribute("percentage_height"))
	{
		this->setPercentageHeight(atof(elm->Attribute("percentage_height")));
	}

	if (elm->Attribute("child_bullet_id"))
	{
		this->setChildBulletID(atoi(elm->Attribute("child_bullet_id")));
	}
    
    if(elm->Attribute("trail_group_angle"))
    {
        this->setTrailGroupAngle(atof(elm->Attribute("trail_group_angle")));
    }
    
    if(elm->Attribute("radiate_type"))
    {
        this->setRadiateType(atoi(elm->Attribute("radiate_type")));
    }
    
    if (elm->Attribute("delaytime"))
    {
        this->setDelayTime(atof(elm->Attribute("delaytime")));
    }
    
    return true;
}

void GunData::loadIntervals(TiXmlElement *elm)
{
    TiXmlElement *elmInterval = elm->FirstChildElement();

    while(elmInterval)
    {
//        //添加到vector中
//        float duration = atof(elmInterval->Attribute("duration"));
//
//        //增加到Total中
//        this->total_interval += duration;
//
//        this->intervals.push_back(duration);

        //下一个
        elmInterval = elmInterval->NextSiblingElement();
    }
}