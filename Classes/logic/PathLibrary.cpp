#include "PathLibrary.h"


PathLibrary::PathLibrary()
{
    ////CCLog("PathLibrary Constructed");
}

PathLibrary::~PathLibrary()
{
    ////CCLog("PathLibrary Destroyed");
    
    CC_SAFE_RELEASE(path_groups);
}

PathLibrary* PathLibrary::s_sharedPathLibrary = NULL;

PathLibrary* PathLibrary::sharedPathLibrary(void)
{
    if (!s_sharedPathLibrary)
    {
        s_sharedPathLibrary = new PathLibrary();
        s_sharedPathLibrary->init();
    }
    
    return s_sharedPathLibrary;
}

void PathLibrary::init()
{
    string path_PathLibrary = "game/pathgroup/pathdata.xml";
    
	unsigned long nLength = 0;
	char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(path_PathLibrary.c_str(),"rt", &nLength);
    
	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory(pBuff, nLength);
    
    path_groups = CCDictionary::create();
    path_groups->retain();
    
	//获得根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //获得第一个PathGroup元素
	TiXmlElement *elmPathGroup = elmRoot->FirstChildElement();
    
	while (elmPathGroup)
	{
        PathGroupData* dataPathGroup = PathGroupData::create(elmPathGroup);
        
		//添加到groups中
        path_groups->setObject(dataPathGroup, dataPathGroup->getID());
        //CCLog("dataPathGroup  dataPathGroup->getID == %s ", dataPathGroup->getID().c_str());
		//下一个
		elmPathGroup = elmPathGroup->NextSiblingElement();
	}
    
    delete xmlDoc;
}

void PathLibrary::preload()
{

}

CCObject* PathLibrary::getPathGroupDataByID(int pg_id)
{
    return getPathGroupDataByID(getStringFromInt(pg_id));
}

CCObject* PathLibrary::getPathGroupDataByID(string pg_id)
{
    return path_groups->objectForKey(pg_id);
}