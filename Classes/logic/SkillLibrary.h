#ifndef _SKILL_LIBRARY_H_
#define _SKILL_LIBRARY_H_

#include "cocos2d.h"
#include "tinyxml.h"

#include "FerrisTools.h"
#include "GlobalEnum.h"
#include "SkillLevelData.h"
#include "SkillData.h"

USING_NS_CC;
using namespace std;

class SkillLibrary
{
public:
	SkillLibrary(void);
	virtual ~SkillLibrary(void);

	static SkillLibrary* sharedSkillLibrary(void);

	//初始化数据
	void init();

	//预加载
	void preload();

	//
	CCObject* getSkillDataByID(int pg_id);
	CCObject* getSkillDataByID(string pg_id);

private:
	static SkillLibrary* s_sharedSkillLibrary;

	//===========技能据组===========
	CC_SYNTHESIZE(CCDictionary*, m_pDictSkillsData, DictSkillsData);
};

#endif