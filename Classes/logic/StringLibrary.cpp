#include "StringLibrary.h"


StringLibrary::StringLibrary()
{
    ////CCLog("StringLibrary Constructed");
}

StringLibrary::~StringLibrary()
{
    ////CCLog("StringLibrary Destroyed");
    
    CC_SAFE_RELEASE(dict_strings);
}

StringLibrary* StringLibrary::s_sharedStringLibrary = NULL;

StringLibrary* StringLibrary::shared()
{
    if (!s_sharedStringLibrary)
    {
        s_sharedStringLibrary = new StringLibrary();
        s_sharedStringLibrary->init();
    }
    return s_sharedStringLibrary;
}

void StringLibrary::init()
{
	string path_userdata = "common/strings.xml";
    
    dict_strings = CCDictionary::create();
    dict_strings->retain();
    
    TiXmlDocument *xmlDoc = new TiXmlDocument();
	unsigned long nLength = 0;
    
	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(path_userdata.c_str(), "rt", &nLength);
    xmlDoc->LoadMemory(pBuff, nLength);
    
    //获得数据的根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //String
    TiXmlElement *elmCursor = elmRoot->FirstChildElement();
    
    while (elmCursor)
	{
        if (elmCursor->Attribute("name"))
        {
            string key = elmCursor->Attribute("name");
            string value = elmCursor->FirstChild()->Value();
            
            //CCLog("StringLibrary::init %s = {%s}", key.c_str(), value.c_str());
            
            dict_strings->setObject(CCString::create(value.c_str()), key);
        }
		
        //下一个
        elmCursor = elmCursor->NextSiblingElement();
	}
    
    delete xmlDoc;
}

void StringLibrary::preload()
{

}

CCString* StringLibrary::getStringByKey(string key)
{
    CCObject *obj = dict_strings->objectForKey(key);
    
    if (obj)
    {
        //CCLog("getStringByKey %s", ((CCString *)dict_strings->objectForKey(key))->getCString());
        return (CCString *)dict_strings->objectForKey(key);
    }
    
    return CCString::create("");
}