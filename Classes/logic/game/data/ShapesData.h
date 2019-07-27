#ifndef __SHAPES_DATA_H__
#define __SHAPES_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "ShapePartData.h"

USING_NS_CC;
using namespace std;

class ShapesData : public CCObject
{
public:
	virtual ~ShapesData();

	static ShapesData* create(TiXmlElement *elm);

	//加载ShapePartData
	bool loadData(TiXmlElement *elm);

	//通过ID获取数据ShapePartData
	ShapePartData* getShapePartDataByKeys(string key);

private:
	ShapesData();

	
	CC_SYNTHESIZE (CCDictionary*, m_pDictShapesData, DictShapesData);

};

#endif