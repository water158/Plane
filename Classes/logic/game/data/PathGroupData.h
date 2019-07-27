#ifndef _PATHGROUPDATA_H_
#define _PATHGROUPDATA_H_

#include "cocos2d.h"
#include "FerrisTools.h"

#include "GlobalEnum.h"
#include "PathData.h"

USING_NS_CC;
using namespace std;
//typedef map<string, PathData> MapPathData;

class PathGroupData : public CCObject
{
public:
	virtual ~PathGroupData(void);
    
    static PathGroupData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

	//加载当前枪管组内的所有枪管
	void loadPathes(TiXmlElement *elm);

	//根据key获得Path
	PathData* getPathData(string key);

	//根据key获得Path的动作
	CCFiniteTimeAction* getPathActionByID(string key,CCNode *holder,float speed=0.0f);

	CCPoint getFirstPoint();

	CCPoint getBXPoint();
    
    //获取当前路径是否支持自动转向
    bool getPathAutoRotation(int n_pathid);
    
	//获取当前路径的转角
	bool getCurPathRotation(int n_pathid, CCPoint curpos, float& rotation);


private:
    PathGroupData(void);


    
	CC_SYNTHESIZE (float, speed, Speed);
	CC_SYNTHESIZE (string, id, ID);
	CC_SYNTHESIZE (string, description, Description);
	CC_SYNTHESIZE (CCDictionary*, pathes, Pathes);

};

#endif