#include "GunGroupData.h"
#include "FerrisTools.h"
#include "tinyxml.h"

GunGroupData::GunGroupData(void)
{

}

GunGroupData::~GunGroupData(void)
{
    CC_SAFE_RELEASE(guns);
}

GunGroupData* GunGroupData::create(TiXmlElement *elm)
{
    GunGroupData *data = new GunGroupData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}
    
	return NULL;
}

bool GunGroupData::loadData(TiXmlElement *elmParent)
{
	//属性
	this->setID(elmParent->Attribute("id"));
	
	if (elmParent->Attribute("description"))
	{
		this->setDescription(elmParent->Attribute("description"));
	}
	else
	{
		this->setDescription("");
	}
    
    if (elmParent->Attribute("num"))
    {
        this->setNum(atoi(elmParent->Attribute("num")));
    }
    else
    {
        this->setNum(1);
    }

	//加载子元素
	this->loadGuns(elmParent);
    
    return true;
}

void GunGroupData::loadGuns(TiXmlElement *elm)
{
    guns = CCArray::create();
    guns->retain();
    
    TiXmlElement *elmGun = elm->FirstChildElement();
    
	while (elmGun)
	{
        GunData* data = GunData::create(elmGun);
        
        //添加到guns中
        //guns->setObject(gunData, gunData->getID());
        guns->addObject(data);
		
        //下一个
        elmGun = elmGun->NextSiblingElement();
	}
}