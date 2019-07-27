#include "TrailGroupData.h"
#include "RoundPathAction.h"
#include "FerrisTools.h"
#include "CraftView.h"


TrailGroupData::TrailGroupData(void)
{

}

TrailGroupData::~TrailGroupData(void)
{
	//CC_SAFE_RELEASE(pathes);
}

TrailGroupData* TrailGroupData::create(TiXmlElement *elm)
{
	TrailGroupData *data = new TrailGroupData();

	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}

bool TrailGroupData::loadData(TiXmlElement *elm)
{
	//属性
	this->setID(elm->Attribute("id"));
	this->setDescription(elm->Attribute("description"));
	this->setSpeed(-1);
	this->setAngle(0);

	//加载子元素
	this->loadPathes(elm);

	return true;
}

void TrailGroupData::loadPathes(TiXmlElement *elm)
{
	pathes = CCDictionary::create();
	pathes->retain();

	TiXmlElement *elmPath = elm->FirstChildElement();

	while (elmPath)
	{
		PathData *data = PathData::create(elmPath);

		this->pathes->setObject(data, data->getID());
		//CCLog("---FirstChildElement---==---%s--", elmPath->Value());
		//下一个
		elmPath = elmPath->NextSiblingElement();
	}
}

PathData* TrailGroupData::getPathData(string key)
{
	//CCLog("TrailGroupData::getPathData   key == (%s)", key.c_str());
	return (PathData *)pathes->objectForKey(key);
}

CCFiniteTimeAction* TrailGroupData::getTrailActionByID(string key, CCNode *holder, float trilgroup_rot, float speed1)
{
	if(!holder)
	{
		return NULL;
	}

	if(speed1 > 0.0f)
	{
		speed = speed1;
	}

	PathData *p_pathdata = this->getPathData(key);
	if(p_pathdata)
	{
		float time = p_pathdata->getTime();
		float angle_delta = p_pathdata->getAngle();
		float radius = 0;
		float angle_begin = 0;
		float angle_end = 0;

		int n_action_type = p_pathdata->getType();

		CCFiniteTimeAction *paction = NULL;
		if(n_action_type == ACTION_MOVETO)
		{
			CCPoint p_begin = holder->getPosition();
			CCPoint p_end = ccp(p_pathdata->getEndX(), p_pathdata->getEndY());

			p_end = getMappingPoint(p_begin, p_end, m_fAngle);

			float f_len = calcTwoPointLength(p_begin, p_end);

			if(speed > 0.0f)
			{
				time = f_len/speed;
			}

			paction = CCMoveTo::create(time, p_end);
		}
		else if(n_action_type == ACTION_MOVEBY)
		{
			CCPoint p_begin = holder->getPosition();
			CCPoint p_offset = ccp(p_pathdata->getMoveByX()/_s_width*_s_height, p_pathdata->getMoveByY());

			p_offset = getMappingPoint(CCPointZero, p_offset, trilgroup_rot);
			//子弹路径的moveby以高为百分比的总数

			float f_len = calcTwoPointLength(CCPointZero, p_offset);
			//先算出运来的与水平角度的夹角
			float fAngle = calcAngleFromOriginToTarget(p_begin, ccpAdd(p_begin, p_offset));
			float temp_x = f_len * cos(CC_DEGREES_TO_RADIANS(fAngle+1 + m_fAngle));
			float temp_y = f_len * sin(CC_DEGREES_TO_RADIANS(fAngle + m_fAngle));

			if(speed > 0.0f)
			{
				time = f_len/speed;
			}
			paction = CCMoveBy::create(time, /*p_offset*/ccp(temp_x, temp_y));
		}
		else if(n_action_type == ACTION_MOVEARC)
		{
			radius = p_pathdata->getRadius();
            CCLog("获取的旋转角度 === %f",radius);
			//angle_begin = holder->getRotation();
			////CCLog("DELTA %f BEGIN %f END %f", angle_delta, angle_begin);
			float offsetangle = (angle_delta > 0) ? angle_delta : -angle_delta;
			//float f_len = offsetangle / 360.0f * 2.0f * radius * 3.14f;
			float f_len = CC_DEGREES_TO_RADIANS(offsetangle) * radius;

			if(speed > 0.0f)
			{
				time = f_len / speed;
			}
			float fRot = holder->getRotation();
			CCLog("子弹角度 == fRot === (%f)", fRot);
			paction = BulletRoundPathAction::create(time, radius, fRot, angle_delta, getPathAutoRotation(getIntFromS(key.c_str())));
		}
		else if(n_action_type == ACTION_ADDHP)
		{
			paction = CCMoveBy::create(time, CCPointZero);
		}
		else if(n_action_type == ACTION_STAYAT)
		{
			paction = CCMoveBy::create(time, CCPointZero);
		}
		else if(n_action_type == ACTION_DIED)
		{
			return NULL;
		}
		else if(n_action_type == ACTION_REPEAT)
		{
			return NULL;
		}
		return paction;
	}
	return NULL;
}

bool TrailGroupData::getPathAutoRotation(int n_pathid)
{
	PathData *p_pathdata = this->getPathData(getStringFromInt(n_pathid));

	return p_pathdata->getIsAutoRotation();
}

bool TrailGroupData::getCurPathRotation(int n_pathid, CCPoint pos, float& rotation)
{
	PathData* p_pathdata = this->getPathData(getStringFromInt(n_pathid));

	if(p_pathdata)
	{
		float angle = p_pathdata->getAngle();

		if(angle != 0)
		{
			return false;
		}

		CCPoint p_begin = pos;
		CCPoint p_end = ccp(p_pathdata->getEndX(), p_pathdata->getEndY());

		//未配置目标点则不转向
		if(p_end.x == 0 && p_end.y == 0)
		{
			return false;
		}

		rotation = calcAngleFromOriginToTarget(p_begin, p_end);

		return true;
	}

	return false;
}

bool TrailGroupData::getFirstPathRotation(int n_pathid, float& rotation, float trilgroup_rot)
{
	PathData* p_pathdata = this->getPathData(getStringFromInt(n_pathid));

	if(p_pathdata)
	{
		float angle = p_pathdata->getAngle();

		if(angle != 0)
		{
			return false;
		}

		CCPoint p_begin = CCPointZero;
		CCPoint p_end = ccp(p_pathdata->getEndX(), p_pathdata->getEndY());

		p_end = getMappingPoint(CCPointZero, p_end, trilgroup_rot);
		//未配置目标点则不转向
		if(p_end.x == 0 && p_end.y == 0)
		{
			return false;
		}

		rotation = calcAngleFromOriginToTarget(p_begin, p_end);
		return true;
	}

	return false;
}
