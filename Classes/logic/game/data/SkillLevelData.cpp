#include "SkillLevelData.h"

SkillLevelData::SkillLevelData()
{

}

SkillLevelData::~SkillLevelData()
{

}

SkillLevelData* SkillLevelData::create(TiXmlElement *elm)
{
	SkillLevelData* data = new SkillLevelData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载属性数据
bool SkillLevelData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("SkillLevelData::loadData  elm is null", "");
	}

	this->setID("1");
	this->setPrepareTime(0);
	this->setLastTime(0.0f);
	this->setCD(0.0f);
	this->setAttrackMultiple(0.0f);
	this->setStrengthenInterval(0.0f);
	this->setMaxTime(0.0f);
	this->setRemoveNum(0);
	this->setCarryNum(10);
	this->setPercentage(1);
	
	//id
	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}

	//蓄力时间
	if(elm->Attribute("prepare_time"))
	{
		this->setPrepareTime(atof(elm->Attribute("prepare_time")));
	}

	//持续时间
	if(elm->Attribute("last_time"))
	{
		this->setLastTime(atof(elm->Attribute("last_time")));
	}

	//技能CD
	if(elm->Attribute("cd"))
	{
		this->setCD(atof(elm->Attribute("cd")));
	}

	//攻击次数
	if(elm->Attribute("attrack_multiple"))
	{
		this->setAttrackMultiple(atof(elm->Attribute("attrack_multiple")));
	}

	//强化间隔
	if(elm->Attribute("strengthen_interval"))
	{
		this->setStrengthenInterval(atof(elm->Attribute("strengthen_interval")));
	}

	//最大时间
	if(elm->Attribute("max_time"))
	{
		this->setMaxTime(atof(elm->Attribute("max_time")));
	}

	//消除数量
	if(elm->Attribute("remove_num"))
	{
		this->setRemoveNum(atoi(elm->Attribute("remove_num")));
	}

	//携带数量
	if(elm->Attribute("carry_num"))
	{
		this->setCarryNum(atoi(elm->Attribute("carry_num")));
	}

	//百分比
	if(elm->Attribute("percentage"))
	{
		this->setPercentage(atof(elm->Attribute("percentage")));
	}
    
    //技能类型
    if (elm->Attribute("style"))
    {
        this->setSkillType(elm->Attribute("style"));
    }
    
    //des
    if (elm->Attribute("description"))
    {
        this->setDes(elm->Attribute("description"));
    }

	return true;
}