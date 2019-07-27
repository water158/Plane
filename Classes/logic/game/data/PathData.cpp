#include "PathData.h"


PathData::PathData()
{

}

PathData::~PathData()
{

}

PathData* PathData::create(TiXmlElement *elm)
{
    PathData *data = new PathData();
	
    if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}
    
	return NULL;
}

float PathData::getBeginX()
{
    return (bx / 100.0f) * _s_width;
}

float PathData::getBeginY()
{
    return (by / 100.0f) * _s_height;
}

float PathData::getEndX()
{
    return (bx1 / 100.0f) * _s_width;
}

float PathData::getEndY()
{
    return (by1 / 100.0f) * _s_height;
}

float PathData::getMoveByX()
{
	return (moveBy_x / 100.0f) * _s_width;
}

float PathData::getMoveByY()
{
	return (moveBy_y / 100.0f) * _s_height;
}

bool PathData::loadData(TiXmlElement *elm)
{
	//属性
	this->setID(getStringFromInt(m_uID));
	this->setBX(0.0f);
	this->setBY(0.0f);
	this->setBX1(0.0f);
	this->setBY1(0.0f);
	this->setTime(0.0f);
	this->setRadius(0.0f);
	this->setAngle(0.0f);
	this->setMoveBy_X(0.0f);
	this->setMoveBy_Y(0.0f);
	this->setIsPlayEffect(false);
	this->setBID(1);
	this->setEID(1);
	this->setTotalRepeatTimes(0);
    this->setIsAutoRotation(true);

    if (elm->Attribute("id"))
    {
        this->setID(elm->Attribute("id"));
    }

	if(elm->Attribute("bx"))
	{
        this->setBX(atof(elm->Attribute("bx")));
    }
    
	if(elm->Attribute("by"))
	{
        this->setBY(atof(elm->Attribute("by")));
    }
    
	if(elm->Attribute("bx1"))
	{
        this->setBX1(atof(elm->Attribute("bx1")));
    }
	
    if(elm->Attribute("by1"))
    {
        this->setBY1(atof(elm->Attribute("by1")));
    }
	
    if(elm->Attribute("time"))
    {
        this->setTime(atof(elm->Attribute("time")));
    }
	
    if(elm->Attribute("angle"))
	{
        this->setAngle(atof(elm->Attribute("angle")));
    }
	
    if(elm->Attribute("radius"))
	{
        this->setRadius(atof(elm->Attribute("radius")));
    }

	if(elm->Attribute("moveby_x"))
	{
		this->setMoveBy_X(atof(elm->Attribute("moveby_x")));
	}

	if(elm->Attribute("moveby_y"))
	{
		this->setMoveBy_Y(atof(elm->Attribute("moveby_y")));
	}

	if(elm->Attribute("isplayeffect"))
	{
		this->setIsPlayEffect(atoi(elm->Attribute("isplayeffect"))==0 ? false : true);
	}

	if (elm->Attribute("b_id"))
	{
		this->setBID(atoi(elm->Attribute("b_id")));
	}

	if (elm->Attribute("e_id"))
	{
		this->setEID(atoi(elm->Attribute("e_id")));
	}

	if(elm->Attribute("repeat_times"))
	{
		this->setTotalRepeatTimes(atoi(elm->Attribute("repeat_times")));
	}
    
	if(elm->Attribute("auto_rot"))
	{
		this->setIsAutoRotation(strncmp(elm->Attribute("auto_rot"), "1", 1) == 0 ? true : false);
	}

	//设置类型
    this->setType(getDataType(elm));

    return true;
}

int PathData::getDataType(TiXmlElement *elm)
{
	int n_type = -1;
	if(elm->Value())
	{
		const char* pstr_type = elm->Value();
		

		if(compareString(elm->Value(), "MoveTo") || (compareString(pstr_type, "Path") && this->getAngle()==0.0f))
		{
			//CCLog("---getDataType---==---%s----true type === ---%d--", elm->Value(),n_type);
			n_type = ACTION_MOVETO;
		}
		else if(compareString(elm->Value(), "MoveBy"))
		{
			n_type = ACTION_MOVEBY;
		}
		else if(compareString(elm->Value(), "MoveArc") || (compareString(pstr_type, "Path") && this->getAngle()!=0.0f))
		{
			n_type = ACTION_MOVEARC;
		}
		else if(compareString(elm->Value(), "StayAt"))
		{
			n_type = ACTION_STAYAT;
		}
		else if(compareString(elm->Value(), "AddHp"))
		{
			n_type = ACTION_ADDHP;
		}
		else if(compareString(elm->Value(), "Repeat"))
		{
			n_type = ACTION_REPEAT;
		}
		else if(compareString(elm->Value(), "Died"))
		{
			n_type = ACTION_DIED;
		}
	}

	return n_type;
}
