#include "SkillData.h"


SkillData::SkillData()
{

}

SkillData::~SkillData()
{

}

SkillData* SkillData::create(TiXmlElement *elm)
{
	SkillData* data = new SkillData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载SkillLevelData
bool SkillData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("SkillData::loadData  elm is null", "");
	}
	//id
	this->setID("1");
	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}
    
    if (elm->Attribute("auto_release"))
    {
        this->setIsAutoRelease(atoi(elm->Attribute("auto_release")) == 1 ? true : false);
    }
    else
    {
        this->setIsAutoRelease(false);
    }

	m_pDictSkillLevelData = CCDictionary::create();
	m_pDictSkillLevelData->retain();

	TiXmlElement *elmData = elm->FirstChildElement();
	while(elmData)
	{
		SkillLevelData* data = SkillLevelData::create(elmData);

		m_pDictSkillLevelData->setObject(data, data->getID());

		elmData = elmData->NextSiblingElement();
	}

	return true;
}

//通过ID获取数据SkillLevelData
SkillLevelData* SkillData::getSkillLevelDataByKeys(string key)
{
	return (SkillLevelData*)m_pDictSkillLevelData->objectForKey(key);
}