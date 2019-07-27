#ifndef _PHRASETIMELINEDATA_H_
#define _PHRASETIMELINEDATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "FerrisTools.h"

#include "GlobalEnum.h"
#include "TimeData.h"

USING_NS_CC;
using namespace std;

class PhraseTimeLineData : public CCObject
{
public:
	virtual ~PhraseTimeLineData(void);

	static PhraseTimeLineData* create(TiXmlElement *elm);
	//加载自身的数据
	bool loadData(TiXmlElement *elm);

	//加载时间片
	void loadTimeDatas(TiXmlElement *elm);

	//获得key对应的时间片
	TimeData* getTimeData(string key);

private:

	PhraseTimeLineData(void);

	CC_SYNTHESIZE(string, id, ID);
	CC_SYNTHESIZE (CCDictionary*, timeline, Timeline);
};

#endif