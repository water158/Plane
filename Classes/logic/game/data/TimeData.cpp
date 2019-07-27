#include "TimeData.h"


TimeData::TimeData()
{
    ////CCLog("TimeData Created %d", m_uID);
}

TimeData::~TimeData()
{
    CC_SAFE_RELEASE(time_planes);
}

TimeData* TimeData::create(TiXmlElement *elm)
{
    TimeData *data = new TimeData();

    if(data && data->loadData(elm))
    {
        data->autorelease();
        return data;
    }

    return NULL;
}

bool TimeData::loadData(TiXmlElement *elm)
{
    //属性
    this->setID(elm->Attribute("id"));
    this->setWarning(false);
    this->setWarningTime(0);

    //游戏是否结束
    if(elm->Attribute("is_end"))
    {
        this->setIsEnd(strncmp(elm->Attribute("is_end"), "1", 1) == 0 ? true : false);
    }
    else
    {
        this->setIsEnd(false);
    }

    //速度变化量
    if(elm->Attribute("speedoffset"))
    {
        this->setMapSpeedOffset(atof(elm->Attribute("speedoffset")));
    }
    else
    {
        this->setMapSpeedOffset(0);
    }

    //速度变化所用总时间
    if(elm->Attribute("time"))
    {
        this->setTime(atof(elm->Attribute("time")));
    }
    else
    {
        this->setTime(0);
    }

    //时间片是否暂停
    if(elm->Attribute("is_pause"))
    {
        this->setIsPause(atoi(elm->Attribute("is_pause")) == 1 ? true : false);
    }
    else
    {
        this->setIsPause(false);
    }

    if(elm->Attribute("warning"))
    {
        this->setWarning(atoi(elm->Attribute("warning")) == 1 ? true : false);
    }

    if(elm->Attribute("warning_time"))
    {
        this->setWarningTime(atof(elm->Attribute("warning_time")));
    }

    //子元素
    this->loadTimePlaneData(elm);

    return true;
}

void TimeData::loadTimePlaneData(TiXmlElement *elm)
{
    time_planes = CCArray::create();
    time_planes->retain();

    TiXmlElement *elmTimePlane = elm->FirstChildElement();

    while(elmTimePlane)
    {
        TimePlaneData* data = TimePlaneData::create(elmTimePlane);
        //data.loadData(elmTimePlane);

        //加入到map中
        //time_planes.insert(MapTimePlaneData::value_type(data.getID(), data));
        time_planes->addObject(data);

        //下一个
        elmTimePlane = elmTimePlane->NextSiblingElement();
    }

}