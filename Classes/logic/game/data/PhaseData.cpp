#include "PhaseData.h"


PhaseData::PhaseData()
{
	conditions = NULL;
}

PhaseData::~PhaseData()
{
    CC_SAFE_RELEASE(target_gungroups);
	CC_SAFE_RELEASE(conditions);
	CC_SAFE_RELEASE(removePlanes);
}

PhaseData* PhaseData::create(TiXmlElement *elm)
{
	PhaseData *data = new PhaseData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}

bool PhaseData::loadData(TiXmlElement *elm)
{
    //属性
	this->setID(elm->Attribute("id"));
	this->setIsPlay(false);
	this->setIsPlayDebrisAni(false);
	this->setIsCanAttack(true);

	if (elm->Attribute("need_remove_part"))
	{
		this->setCurRemovePart(atoi(elm->Attribute("need_remove_part")));
	}
	else
	{
		this->setCurRemovePart(0.0f);
	}
    
	if(elm->Attribute("target_timeline"))
	{
		this->setTargetTimeLineID(elm->Attribute("target_timeline"));
	}
	else
	{
		this->setTargetTimeLineID("1");
	}

	if(elm->Attribute("pathgroup_id"))
	{
		this->setPathGroupID(elm->Attribute("pathgroup_id"));
	}
	else
	{
		this->setPathGroupID("");
	}
	if(elm->Attribute("playanimation"))
	{
		this->setIsPlay(atoi(elm->Attribute("playanimation")) == 1 ? true : false);
	}
	if(elm->Attribute("play_debris_ani"))
	{
		this->setIsPlayDebrisAni(atoi(elm->Attribute("play_debris_ani")) == 1 ? true : false);
	}

	if(elm->Attribute("be_attacked"))
	{
		this->setIsCanAttack(atoi(elm->Attribute("be_attacked")) == 1 ? true : false);
	}

	this->setConditionType(1);
	this->setCurRemovePart(1);
	
	//子元素
	this->loadTargetGunGroups(elm);
    
    return true;
}

void PhaseData::loadTargetGunGroups(TiXmlElement *elm)
{
	target_gungroups = CCArray::create();
	target_gungroups->retain();

	TiXmlElement *elmTargetGunGroup = elm->FirstChildElement();
    TiXmlElement *preelmTargetGunGroup = elmTargetGunGroup;

	if(elmTargetGunGroup)
	{
		const char* pitem = elmTargetGunGroup->Value();
	}
	while (elmTargetGunGroup && compareString(elmTargetGunGroup->Value(),"TargetGunGroup"))
	{
        TargetGunGroupData* data = TargetGunGroupData::create(elmTargetGunGroup);
        //data.loadData(elmTargetGunGroup);
        
        //加入到map中
        //target_gungroups.insert(MapTargetGunGroupData::value_type(data.getID(), data));
        target_gungroups->addObject(data);
        
        //下一个
		preelmTargetGunGroup = elmTargetGunGroup;
        elmTargetGunGroup = elmTargetGunGroup->NextSiblingElement();
    }

	//导入条件数据
	loadConditionDatas(elmTargetGunGroup);
}

void PhaseData::loadConditionDatas(TiXmlElement *elm)
{
	conditions = CCArray::create();
	conditions->retain();

	TiXmlElement *elmConditions = elm;
	TiXmlElement *preelmConditions = elmConditions;
	if(elmConditions)
	{
		const char* pitem = elmConditions->Value();
	}
	while (elmConditions && compareString(elmConditions->Value(),"Condition"))
	{
		ConditionData* data = ConditionData::create(elmConditions);
		conditions->addObject(data);
		
		//下一个
        preelmConditions = elmConditions;
		elmConditions = elmConditions->NextSiblingElement();
	}

	loadRemoveDatas(elmConditions);
}

void PhaseData::loadRemoveDatas(TiXmlElement *elm)
{
	removePlanes = CCArray::create();
	removePlanes->retain();

	TiXmlElement *elmRemoves = elm;
	if(elmRemoves)
	{
		const char* pitem = elmRemoves->Value();
	}

	while (elmRemoves && compareString(elmRemoves->Value(),"Remove"))
	{
		RemoveData* data = RemoveData::create1(elmRemoves);

		removePlanes->addObject(data);

		//下一个
		elmRemoves = elmRemoves->NextSiblingElement();
	}
}
