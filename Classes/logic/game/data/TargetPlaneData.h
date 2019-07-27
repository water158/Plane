#ifndef _TARGETPLANEATA_H_
#define _TARGETPLANEATA_H_

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "tinyxml.h"
#include "PhaseData.h"
#include "BatteryData.h"

USING_NS_CC;
using namespace std;

//typedef map<string, PhaseData> MapPhaseData;

class TargetPlaneData : public CCObject
{
public:
	virtual ~TargetPlaneData(void);
    
    static TargetPlaneData* create(TiXmlElement *elm);
    
    //加载自身的数据
	bool loadData(TiXmlElement *elm);
    
    //加载Phase数据
    void loadPhases(TiXmlElement *elm);
    
    //加载炮管数据
    void loadConnon(TiXmlElement *elm);
    
    //获得key对应的Phase
    PhaseData* getPhaseData(string key);
    
    CCArray* m_pArrBatterys;
    
private:
    
    TargetPlaneData(void);
    
    CC_SYNTHESIZE (string, id, ID);
	CC_SYNTHESIZE (string, plane_id, PlaneID);
	CC_SYNTHESIZE (int, formgroup_id, FormGroupID);
	CC_SYNTHESIZE (float, plane_blood, PlaneBlood);
	//CC_SYNTHESIZE (float, plane_speed, PlaneSpeed);
	//飞机类型是否是boss
	CC_SYNTHESIZE(int, planeType, PlaneType);
	CC_SYNTHESIZE(int, m_nDropCoinNum, DropCoinNum);
	CC_SYNTHESIZE(float, m_fDropRate, DropRate);
	CC_SYNTHESIZE(int, planeSizeLv, PlaneSizeLv);
    CC_SYNTHESIZE (CCDictionary* , phases, Phases);

	CC_SYNTHESIZE(int, m_nDropStoneNum, DropStoneNum);
	CC_SYNTHESIZE(float, m_fDropStoneRate, DropStoneRate);
    
    CC_SYNTHESIZE(int, m_level, Level);
    
    //数据加载类型
    CC_SYNTHESIZE(int, dataType, DataType);
    
    //是否自动旋转
    CC_SYNTHESIZE(bool, m_bAutomaticRotate, AutomaticRotate);
    
    //是否有后坐力
    CC_SYNTHESIZE(bool, m_bRecoil, Recoil);
    
    //炮台角速度
    CC_SYNTHESIZE(float, m_fSpeed, Speed);
    
    //初始延迟
    CC_SYNTHESIZE(float, m_fDelayTime, DelayTime);
};

#endif