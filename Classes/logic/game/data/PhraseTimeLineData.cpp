#include "PhraseTimeLineData.h"



PhraseTimeLineData::PhraseTimeLineData(void)
{

}

PhraseTimeLineData::~PhraseTimeLineData(void)
{
	CC_SAFE_RELEASE(timeline);
}

PhraseTimeLineData* PhraseTimeLineData::create(TiXmlElement *elm)
{
	PhraseTimeLineData *data = new PhraseTimeLineData();
	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}

bool PhraseTimeLineData::loadData(TiXmlElement *elm)
{
	//属性
	this->setID(elm->Attribute("id"));

	//子元素
	this->loadTimeDatas(elm);

	return true;
}

void PhraseTimeLineData::loadTimeDatas(TiXmlElement *elm)
{
	timeline = CCDictionary::create();
	timeline->retain();

	TiXmlElement *elmTime = elm->FirstChildElement();

	while (elmTime)
	{
		TimeData* data = TimeData::create(elmTime);

		timeline->setObject(data, data->getID());

		//下一个
		elmTime = elmTime->NextSiblingElement();
	}
}

TimeData* PhraseTimeLineData::getTimeData(string key)
{
    return (TimeData*)timeline->objectForKey(key);
}

