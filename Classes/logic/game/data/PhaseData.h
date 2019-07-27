#ifndef _PHASEDATA_H_
#define _PHASEDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "TargetGunGroupData.h"
#include "ConditionData.h"
#include "RemoveData.h"
#include "FerrisTools.h"

USING_NS_CC;
using namespace std;

//typedef map<string, TargetGunGroupData> MapTargetGunGroupData;

class PhaseData : public CCObject
{
public:
	virtual ~PhaseData(void);
    
    static PhaseData* create(TiXmlElement *elm);
    
    //加载自身的数据
	bool loadData(TiXmlElement *elm);
    
    //加载TargetGunGroup数据
    void loadTargetGunGroups(TiXmlElement *elm);

	//加载ConditionDatas数据
	void loadConditionDatas(TiXmlElement *elm);

	//加载ConditionDatas数据
	void loadRemoveDatas(TiXmlElement *elm);

    
private:
	PhaseData(void);
    
	CC_SYNTHESIZE (string, id, ID);
	//需要移除的partid
	CC_SYNTHESIZE (int, curRemovePart, CurRemovePart);
	//boss每个阶段一个时间轴
	CC_SYNTHESIZE (string, targetTimeLineID, TargetTimeLineID);
	//是否播放动画（boss用）
	CC_SYNTHESIZE (bool, isPlay, IsPlay);
	//是否播放碎片动画（boss用）
	CC_SYNTHESIZE (bool, isPlayDebrisAni, IsPlayDebrisAni);
	//boss每个阶段一个路径
	CC_SYNTHESIZE (string, pathGroupID, PathGroupID);
	CC_SYNTHESIZE (CCArray*, target_gungroups, TargetGunGroup);
	CC_SYNTHESIZE (CCArray*, conditions, Conditions);
	CC_SYNTHESIZE (CCArray*, removePlanes, RemovePlanes);

	CC_SYNTHESIZE (int, conditionType, ConditionType);

	//飞机当前阶段能否被攻击到
	CC_SYNTHESIZE (bool, m_bIsCanAttack, IsCanAttack);
};

#endif