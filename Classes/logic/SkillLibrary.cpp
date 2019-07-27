#include "SkillLibrary.h"


SkillLibrary::SkillLibrary()
{
	////CCLog("SkillLibrary Constructed");
}

SkillLibrary::~SkillLibrary()
{
	////CCLog("SkillLibrary Destroyed");

	CC_SAFE_RELEASE(m_pDictSkillsData);
}

SkillLibrary* SkillLibrary::s_sharedSkillLibrary = NULL;

SkillLibrary* SkillLibrary::sharedSkillLibrary(void)
{
	if (!s_sharedSkillLibrary)
	{
		s_sharedSkillLibrary = new SkillLibrary();
		s_sharedSkillLibrary->init();
	}

	return s_sharedSkillLibrary;
}

void SkillLibrary::init()
{
	string path_SkillLibrary = "common/skilldata.xml";

	unsigned long nLength = 0;
	char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(path_SkillLibrary.c_str(),"rt", &nLength);

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory(pBuff, nLength);

	m_pDictSkillsData = CCDictionary::create();
	m_pDictSkillsData->retain();

	//获得根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();

	//获得第一个PathGroup元素
	TiXmlElement *elmSkillData = elmRoot->FirstChildElement();

	while (elmSkillData)
	{
		SkillData* dataSkillData = SkillData::create(elmSkillData);

		//添加到groups中
		m_pDictSkillsData->setObject(dataSkillData, dataSkillData->getID());
		//下一个
		elmSkillData = elmSkillData->NextSiblingElement();
	}

	delete xmlDoc;
}

void SkillLibrary::preload()
{

}

CCObject* SkillLibrary::getSkillDataByID(int pg_id)
{
	return getSkillDataByID(getStringFromInt(pg_id));
}

CCObject* SkillLibrary::getSkillDataByID(string pg_id)
{
	return m_pDictSkillsData->objectForKey(pg_id);
}