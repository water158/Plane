#include "TimeplaneData.h"


TimePlaneData::TimePlaneData()
{
	////CCLog("TimePlaneData Created %d", m_uID);
}

TimePlaneData::~TimePlaneData()
{
	CC_SAFE_RELEASE(m_pArrBatterys);
}

TimePlaneData* TimePlaneData::create(TiXmlElement *elm)
{
	TimePlaneData *data = new TimePlaneData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}
    
	return NULL;
}

bool TimePlaneData::loadData(TiXmlElement *elm)
{
	this->setID(getStringFromInt(m_uID));
	this->setFirstDelay(0.0f);
	this->setRoute("1");
	this->setType("1");
	this->setRepeatDelay(1.0f);
	this->setRepeatTimes(1);
	this->setSpeed(0.0f);
	this->setIsBelongToBoss(false);
	this->setIsReboundCraft(false);
	this->setFirstMove(CCPointZero);
	this->setIsDownBoss(false);
	this->setIsCanRotate(false);
	this->setIsRandRoute(true);


    //属性
	if (elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }
	if(elm->Attribute("route"))
	{
		this->setRoute(elm->Attribute("route"));
	}
	if(elm->Attribute("type"))
	{
		this->setType(elm->Attribute("type"));
	}
	if(elm->Attribute("first_delay"))
	{
		this->setFirstDelay(atof(elm->Attribute("first_delay")));
	}
	if(elm->Attribute("repeat_delay"))
	{
		this->setRepeatDelay(atof(elm->Attribute("repeat_delay")));
	}
	if(elm->Attribute("repeat_times"))
	{
		this->setRepeatTimes(atoi(elm->Attribute("repeat_times")));
	}
	if(elm->Attribute("speed"))
	{
		this->setSpeed(atof(elm->Attribute("speed")));
	}
	if (elm->Attribute("isbelongtoboss"))
	{
		this->setIsBelongToBoss(atoi(elm->Attribute("isbelongtoboss")));
	}
	if (elm->Attribute("rebound_plane"))
	{
		this->setIsReboundCraft(atoi(elm->Attribute("rebound_plane")));
	}
	if(elm->Attribute("first_move_x") && elm->Attribute("first_move_y"))
	{
		this->setFirstMove(ccp(atof(elm->Attribute("first_move_x"))*_s_width/100.0f, atof(elm->Attribute("first_move_y"))*_s_height/100.0f));
	}

	if (elm->Attribute("is_down"))
	{
		this->setIsDownBoss(atoi(elm->Attribute("is_down")));
	}
	if (elm->Attribute("is_rotate"))
	{
		this->setIsCanRotate(atoi(elm->Attribute("is_rotate")));
	}

	if(elm->Attribute("is_missile"))
	{
		this->setIsMissile(atoi(elm->Attribute("is_missile")) == 1 ? true : false);
	}
	else
	{
		this->setIsMissile(false);//默认不是导弹飞机
	}

	if(elm->Attribute("missile_time"))
	{
		this->setMissileTime(atof(elm->Attribute("missile_time")));
	}

	if (elm->Attribute("missile_appear"))
	{
		this->setMissileAppear(atof(elm->Attribute("missile_appear")));
	}
	//是否随机轨迹
	if (elm->Attribute("is_rand_route"))
	{
		this->setIsRandRoute(atoi(elm->Attribute("is_rand_route")));
	}

	float xxx = this->getFirstMove().x;
	float yyy = this->getFirstMove().y;

	//加载炮台数据
	//this->loadBatterys(elm);

    return true;
}

//加载炮台
bool TimePlaneData::loadBatterys(TiXmlElement *elm)
{
	m_pArrBatterys = CCArray::create();
	m_pArrBatterys->retain();

	TiXmlElement *elmBattery = elm->FirstChildElement();

	while(elmBattery)
	{
		BatteryData* data = BatteryData::create(elmBattery);
		//加入到数组中
		m_pArrBatterys->addObject(data);

		//下一个
		elmBattery = elmBattery->NextSiblingElement();
	}

	return true;
}