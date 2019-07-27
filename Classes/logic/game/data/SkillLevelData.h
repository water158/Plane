#ifndef __SKILL_LEVEL_DATA_H__
#define __SKILL_LEVEL_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "FerrisTools.h"


USING_NS_CC;
using namespace std;

class SkillLevelData : public CCObject
{
public:
	virtual ~SkillLevelData();

	static SkillLevelData* create(TiXmlElement *elm);

	//加载属性数据
	bool loadData(TiXmlElement *elm);

private:
	SkillLevelData();

/*************数据属性***********/
	//id
	CC_SYNTHESIZE (string , m_strID, ID);
	//蓄力时间
	CC_SYNTHESIZE (float , m_fPrepareTime, PrepareTime);
	//持续时间
	CC_SYNTHESIZE (float , m_fLastTime, LastTime);
	//技能CD
	CC_SYNTHESIZE (float , m_fCD, CD);
	//攻击伤害倍数
	CC_SYNTHESIZE (float , m_nAttrackMultiple, AttrackMultiple);
	//强化间隔
	CC_SYNTHESIZE (float , m_fStrengthenInterval, StrengthenInterval);
	//最大时间
	CC_SYNTHESIZE (float , m_fMaxTime, MaxTime);
	//消除数量
	CC_SYNTHESIZE (int , m_nRemoveNum, RemoveNum);
	//携带数量
	CC_SYNTHESIZE (int , m_nCarryNum, CarryNum);
	//百分比
	CC_SYNTHESIZE (float , m_fPercentage, Percentage);
    //技能类型
    CC_SYNTHESIZE(string, m_skillType, SkillType);
    //des
    CC_SYNTHESIZE(string, m_des, Des);
    
    
};

#endif