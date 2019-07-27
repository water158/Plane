#include "ShapesData.h"


ShapesData::ShapesData()
{

}

ShapesData::~ShapesData()
{

}

ShapesData* ShapesData::create(TiXmlElement *elm)
{
	ShapesData* data = new ShapesData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载ShapePartData
bool ShapesData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("ShapesData::loadData  elm is null", "");
	}

	m_pDictShapesData = CCDictionary::create();
	m_pDictShapesData->retain();

	TiXmlElement *elmData = elm->FirstChildElement();
	while(elmData)
	{
		ShapePartData* data = ShapePartData::create(elmData);

		m_pDictShapesData->setObject(data, data->getID());

		elmData = elmData->NextSiblingElement();
	}

	return true;
}

//通过ID获取数据ShapePartData
ShapePartData* ShapesData::getShapePartDataByKeys(string key)
{
	return (ShapePartData*)m_pDictShapesData->objectForKey(key);
}