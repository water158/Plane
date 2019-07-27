#include "TargetPlaneData.h"
#include "FerrisTools.h"


TargetPlaneData::TargetPlaneData()
{
    ////CCLog("TargetPlaneData Constructed. %d", m_uID);
}

TargetPlaneData::~TargetPlaneData()
{
    ////CCLog("TargetPlaneData Destroyed.");
    
    CC_SAFE_RELEASE(phases);
}

TargetPlaneData* TargetPlaneData::create(TiXmlElement *elm)
{
	TargetPlaneData *data = new TargetPlaneData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}
    
	return NULL;
}

bool TargetPlaneData::loadData(TiXmlElement *elm)
{
	//属性
	this->setID(elm->Attribute("id"));
	this->setPlaneID(elm->Attribute("plane_id"));
	this->setPlaneSizeLv(CRAFT_SIZE_MID);
	this->setDropCoinNum(-1);
	this->setDropRate(-1);
	this->setDropStoneNum(-1);
	this->setDropStoneRate(-1);
    this->setDataType(CRAFT_DATA_TYPE_ENEMY);
    this->setLevel(1);
    this->setDelayTime(0.0f);
    
    //blood
    if(elm->Attribute("blood"))
	{
        this->setPlaneBlood(atof(elm->Attribute("blood")));
    }
    else
    {
        this->setPlaneBlood(0.0f);
    }
    
    //formgroup_id
	if(elm->Attribute("formgroup_id"))
	{
		this->setFormGroupID(atoi(elm->Attribute("formgroup_id")));
	}
	else
	{
		this->setFormGroupID(0);
	}

	//飞机是否是boss
	if (elm->Attribute("plane_type"))
	{
		this->setPlaneType(atoi(elm->Attribute("plane_type")));
	}
	else
	{
		this->setPlaneType(CRAFT_TYPE_ENEMY);
	}

	if(elm->Attribute("size_lv"))
	{
		this->setPlaneSizeLv(atoi(elm->Attribute("size_lv")));
	}

	if(elm->Attribute("drop_coin_num"))
	{
		this->setDropCoinNum(atoi(elm->Attribute("drop_coin_num")));
	}

	if(elm->Attribute("drop_rate"))
	{
		this->setDropRate(atof(elm->Attribute("drop_rate")));
	}

	if(elm->Attribute("drop_stone_num"))
	{
		this->setDropStoneNum(atoi(elm->Attribute("drop_stone_num")));
	}

	if(elm->Attribute("drop_stone_rate"))
	{
		this->setDropStoneRate(atof(elm->Attribute("drop_stone_rate")));
	}
    
    //
    if (elm->Attribute("data_type"))
    {
        this->setDataType(atof(elm->Attribute("data_type")));
    }
    else
    {
        this->setDataType(CRAFT_DATA_TYPE_ENEMY);
    }
    
    if (elm->Attribute("level"))
    {
        this->setLevel(atoi(elm->Attribute("level")));
    }
    else
    {
        this->setLevel(1);
    }
    
    //自动转向
    if(elm->Attribute("is_rotate"))
    {
        this->setAutomaticRotate(atoi(elm->Attribute("is_rotate")) == 0 ? false : true);
    }
    else
    {
        this->setAutomaticRotate(false);
    }
    
    //是否有后坐力
    if(elm->Attribute("recoil"))
    {
        this->setRecoil(atoi(elm->Attribute("recoil")) == 0 ? false : true);
    }else
    {
        this->setRecoil(false);
    }
    
    //炮台角速度
    if(elm->Attribute("speed"))
    {
        this->setSpeed(atof(elm->Attribute("speed")));
    }else
    {
        this->setSpeed(90.0f);
    }


    TiXmlElement* elmBattery = elm->FirstChildElement();
    
    this->loadConnon(elmBattery);
    
	//子元素
	this->loadPhases(elmBattery);
    
    return true;
}

void TargetPlaneData::loadConnon(TiXmlElement *elm)
{
    m_pArrBatterys = CCArray::create();
    
    m_pArrBatterys->retain();
    
    TiXmlElement *elmBattery = elm->FirstChildElement();
    
    while (elmBattery)
    {
        BatteryData* data = BatteryData::create(elmBattery);
        
        //加入到数组中
        m_pArrBatterys->addObject(data);
        
        elmBattery = elmBattery->NextSiblingElement();
    }
}

PhaseData* TargetPlaneData::getPhaseData(string key)
{
    /*
	MapPhaseData::iterator it = phases.find(key);
	
	if(it == phases.end())
	{
		return NULL;
	}
	else
	{
		return &it->second;
	}*/
    
    return (PhaseData *)phases->objectForKey(key);
}

void TargetPlaneData::loadPhases(TiXmlElement *elm)
{
    phases = CCDictionary::create();
    phases->retain();
    
    TiXmlElement *elmPhase = elm->NextSiblingElement();
    
	while (elmPhase)
	{
        PhaseData* data = PhaseData::create(elmPhase);
        //data.loadData(elmPhase);
        
        //加入到map中
        //phases.insert(MapPhaseData::value_type(data.getID(), data));
        phases->setObject(data, data->getID());
        
        //下一个
        elmPhase = elmPhase->NextSiblingElement();
    }

}