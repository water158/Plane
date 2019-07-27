#include "TrailLibrary.h"


TrailLibrary::TrailLibrary()
{
	
}

TrailLibrary::~TrailLibrary()
{
	CC_SAFE_RELEASE(trail_groups);
}

TrailLibrary* TrailLibrary::s_sharedTrailLibrary = NULL;

TrailLibrary* TrailLibrary::sharedTrailLibrary(void)
{
	if (!s_sharedTrailLibrary)
	{
		s_sharedTrailLibrary = new TrailLibrary();
		s_sharedTrailLibrary->init();
	}

	return s_sharedTrailLibrary;
}

void TrailLibrary::init()
{
	string path_TrailLibrary = "game/trailgroup/traildata.xml";

	unsigned long nLength = 0;
	char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(path_TrailLibrary.c_str(),"rt", &nLength);

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory(pBuff, nLength);

	trail_groups = CCDictionary::create();
	trail_groups->retain();

	//获得根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();

	//获得第一个PathGroup元素
	TiXmlElement *elmTrailGroup = elmRoot->FirstChildElement();

	while (elmTrailGroup)
	{
		TrailGroupData* dataPathGroup = TrailGroupData::create(elmTrailGroup);

		//添加到groups中
		trail_groups->setObject(dataPathGroup, dataPathGroup->getID());

		//下一个
		elmTrailGroup = elmTrailGroup->NextSiblingElement();
	}

	delete xmlDoc;
}

void TrailLibrary::preload()
{

}

CCObject* TrailLibrary::getTrailGroupDataByID(int pg_id)
{
	return getTrailGroupDataByID(getStringFromInt(pg_id));
}

CCObject* TrailLibrary::getTrailGroupDataByID(string pg_id)
{
	return trail_groups->objectForKey(pg_id);
}