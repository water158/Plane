#include "CollisionGroupData.h"

CollisionGroupData::CollisionGroupData()
{

}

CollisionGroupData::~CollisionGroupData()
{

}

CollisionGroupData* CollisionGroupData::create(TiXmlElement *elm)
{
	CollisionGroupData* data = new CollisionGroupData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载属性数据
bool CollisionGroupData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("CollisionGroupData::loadData  elm is null", "");
	}

	this->setID("1");
	
	//id
	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}


	//所有矩形的导入
	m_pDictCollisionGroupRects = CCDictionary::create();
	m_pDictCollisionGroupRects->retain();

	TiXmlElement *elmRect = elm->FirstChildElement();
	while(elmRect)
	{
		CollisionRectData* pRectData = CollisionRectData::create(elmRect);

		m_pDictCollisionGroupRects->setObject(pRectData, pRectData->getID());

		elmRect = elmRect->NextSiblingElement();
	}


	return true;
}

//获取次碰撞组下的某个碰撞矩形数据
CollisionRectData* CollisionGroupData::getCollisionRectDataByKey(string key)
{
	return (CollisionRectData*)m_pDictCollisionGroupRects->objectForKey(key);
}