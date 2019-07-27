#include "WeaponLibrary.h"


WeaponLibrary::WeaponLibrary()
{
    ////CCLog("WeaponLibrary Constructed");
}

WeaponLibrary::~WeaponLibrary()
{
    ////CCLog("WeaponLibrary Destroyed");
    
    CC_SAFE_RELEASE(groups);
}

WeaponLibrary* WeaponLibrary::s_sharedWeaponLibrary = NULL;

WeaponLibrary* WeaponLibrary::sharedWeaponLibrary(void)
{
    if (!s_sharedWeaponLibrary)
    {
        s_sharedWeaponLibrary = new WeaponLibrary();
        s_sharedWeaponLibrary->init();
    }
    
    return s_sharedWeaponLibrary;
}

void WeaponLibrary::init()
{
    string path_WeaponLibrary = "game/gungroup/gundata.xml";
    
	unsigned long nLength = 0;
	char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(path_WeaponLibrary.c_str(), "rt", &nLength);
    
	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory(pBuff, nLength);
    
    groups = CCDictionary::create();
    groups->retain();
    
	//获得根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();
    
    //获得第一个GunGroup元素
	TiXmlElement *elmGunGroup = elmRoot->FirstChildElement();
    
	while (elmGunGroup)
	{
		GunGroupData* dataGunGroup = GunGroupData::create(elmGunGroup);
        

        CCLog("WeaponLibrary init GunGroupData ID = %s", dataGunGroup->getID().c_str());
            
		//添加到groups中
        groups->setObject(dataGunGroup, dataGunGroup->getID());
        
		//下一个
		elmGunGroup = elmGunGroup->NextSiblingElement();
	}
    
    delete xmlDoc;
}

void WeaponLibrary::preload()
{

}

CCObject* WeaponLibrary::getGunGroupDataByID(int gg_id)
{
	return getGunGroupDataByID(getStringFromInt(gg_id));
}

CCObject* WeaponLibrary::getGunGroupDataByID(string gg_id)
{
	CCObject* obj = groups->objectForKey(gg_id);
	if(!obj)
	{
		//CCLOG("NO SUCH GUNGROUP ID --assert at --WeaponLibrary::getGunGroupDataByID---%s----",gg_id);
	}
	return obj;
}
