#ifndef __EQUIPMENTS_DATA_H__
#define __EQUIPMENTS_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "EquipItemData.h"

USING_NS_CC;
using namespace std;

class EquipmentsData : public CCObject
{
public:
	virtual ~EquipmentsData();

	static EquipmentsData* create(TiXmlElement *elm);

	//加载EquipItemData
	bool loadData(TiXmlElement *elm);

	//通过ID获取数据EquipItemData
	EquipItemData* getEquipItemDataByKeys(string key);

private:
	EquipmentsData();


	CC_SYNTHESIZE (CCDictionary*, m_pDictEquipmentsData, DictEquipmentsData);

};

#endif