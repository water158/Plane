#include "MissionData.h"

MissionData::MissionData()
{
}

MissionData::~MissionData()
{
    CCLog("MissionData Destroyed.");

    CC_SAFE_RELEASE(timeline);
}

MissionData* MissionData::create(int mission_id)
{
    MissionData* mission = new MissionData();
    
    if (mission)
    {
        mission->init(mission_id);
        
        return mission;
    }
    
    return NULL;
}

void MissionData::init(int mission_id)
{
    CCString* strFileName = CCString::createWithFormat("game/level/level_%03d.xml", mission_id);

    CCLog("Mission Data File %s", strFileName->getCString());

    unsigned long nLength = 0;
    char* pBuff = (char*)CCFileUtils::sharedFileUtils()->getFileData(strFileName->getCString(), "rt", &nLength);

    TiXmlDocument *xmlDoc = new TiXmlDocument();
    xmlDoc->LoadMemory(pBuff, nLength);

    //获得根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();

    if(elmRoot)
    {
        loadData(elmRoot);

        this->mission_id = mission_id;
    }

    delete xmlDoc;
}

void MissionData::loadData(TiXmlElement *elm)
{
    //属性
    this->setID(elm->Attribute("id"));
    this->setEnable(strncmp(elm->Attribute("enable"), "1", 1) == 0 ? true : false);
    this->setStage(elm->Attribute("stage"));
    this->setMode(atoi(elm->Attribute("mode")));
    this->setTimeFire(30.0f);
    this->setPercentageOfHigh(50.0f);

    //start_at
    if(elm->Attribute("start_at"))
    {
        this->setStartAt(atoi(elm->Attribute("start_at")));
    }
    else
    {
        this->setStartAt(0);
    }

    //backgroundID
    if(elm->Attribute("bg_id"))
    {
        this->setBackgroundID(atoi(elm->Attribute("bg_id")));
    }
    else
    {
        this->setBackgroundID(0);
    }

    //tip
    if(elm->Attribute("tip"))
    {
        this->setTip(elm->Attribute("tip"));
    }
    else
    {
        this->setTip("");
    }

    //reward_gold
    if(elm->Attribute("reward_gold"))
    {
        this->setRewardGold(atoi(elm->Attribute("reward_gold")));
    }
    else
    {
		this->setRewardGold(0.0f);
    }

    if(elm->Attribute("reward_crystal"))
    {
        this->setRewardCrystal(atoi(elm->Attribute("reward_crystal")));
    }
    else
    {
        this->setRewardCrystal(0.0f);
    }

    if(elm->Attribute("time_fire"))
    {
        this->setTimeFire(atof(elm->Attribute("time_fire")));
    }

    if(elm->Attribute("percentageof_high"))
    {
        this->setPercentageOfHigh(atof(elm->Attribute("percentageof_high")));
    }

    //子元素(Timeline)
    TiXmlElement *elmTimeline = elm->FirstChildElement();
    this->loadTimeline(elmTimeline);

}


TimeData* MissionData::getTimeData(string key)
{
    return (TimeData*)timeline->objectForKey(key);
}

void MissionData::loadTimeline(TiXmlElement *elm)
{
    timeline = CCDictionary::create();
    timeline->retain();

    TiXmlElement *elmTime = elm->FirstChildElement();

    while(elmTime)
    {
        TimeData* data = TimeData::create(elmTime);
       
        //加入到map中
        timeline->setObject(data, data->getID());

        //下一个
        elmTime = elmTime->NextSiblingElement();
    }
}
