#ifndef _TRAILGROUPDATA_H_
#define _TRAILGROUPDATA_H_

#include "cocos2d.h"
#include "FerrisTools.h"

#include "GlobalEnum.h"
#include "PathData.h"
#include "BulletRoundPathAction.h"

USING_NS_CC;
using namespace std;
//typedef map<string, PathData> MapPathData;

class TrailGroupData : public CCObject
{
public:
	virtual ~TrailGroupData(void);

	static TrailGroupData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

	//加载当前枪管组内的所有枪管
	void loadPathes(TiXmlElement *elm);

	//根据key获得Path
	PathData* getPathData(string key);

	//根据key获得Path的动作
	CCFiniteTimeAction* getTrailActionByID(string key,CCNode *holder, float trilgroup_rot, float speed=0.0f);

	//获取当前路径是否支持自动转向
	bool getPathAutoRotation(int n_pathid);

	//获取当前路径的转角
	bool getCurPathRotation(int n_pathid, CCPoint pos, float& rotation);

	//获取第一条路径的转角
	bool getFirstPathRotation(int n_pathid, float& rotation, float trilgroup_rot);


private:
	TrailGroupData(void);

	CC_SYNTHESIZE (float, speed, Speed);
	CC_SYNTHESIZE (string, id, ID);
	CC_SYNTHESIZE (string, description, Description);
	CC_SYNTHESIZE (CCDictionary*, pathes, Pathes);

	//路径的一个整体角度
	CC_SYNTHESIZE (float, m_fAngle, Angle);

};

#endif