#ifndef __SKILL_DATA_H__
#define __SKILL_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "SkillLevelData.h"

USING_NS_CC;
using namespace std;

class SkillData : public CCObject
{
public:
	virtual ~SkillData();

	static SkillData* create(TiXmlElement *elm);

	//加载SkillLevelData
	bool loadData(TiXmlElement *elm);

	//通过ID获取数据SkillLevelData
	SkillLevelData* getSkillLevelDataByKeys(string key);

private:
	SkillData();

	//id
	CC_SYNTHESIZE (string , m_strID, ID);
    //is auto_release
    CC_SYNTHESIZE(bool, m_isAutoRelease, IsAutoRelease);
	//SkillLevelData
	CC_SYNTHESIZE (CCDictionary*, m_pDictSkillLevelData, DictSkillLevelData);
};

#endif