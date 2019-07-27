#include "PathGroupData.h"
#include "RoundPathAction.h"
#include "FerrisTools.h"
#include "CraftView.h"


PathGroupData::PathGroupData(void)
{

}

PathGroupData::~PathGroupData(void)
{
    CC_SAFE_RELEASE(pathes);
}

PathGroupData* PathGroupData::create(TiXmlElement *elm)
{
    PathGroupData *data = new PathGroupData();

    if(data && data->loadData(elm))
    {
        data->autorelease();
        return data;
    }

    return NULL;
}

bool PathGroupData::loadData(TiXmlElement *elm)
{
    //属性
    this->setID(elm->Attribute("id"));
    this->setDescription(elm->Attribute("description"));
    this->setSpeed(-1);

    //加载子元素
    this->loadPathes(elm);

    return true;
}

void PathGroupData::loadPathes(TiXmlElement *elm)
{
    pathes = CCDictionary::create();
    pathes->retain();

    TiXmlElement *elmPath = elm->FirstChildElement();

    while(elmPath)
    {
        PathData *data = PathData::create(elmPath);

        this->pathes->setObject(data, data->getID());
        //CCLog("---FirstChildElement---==---%s--", elmPath->Value());
        //下一个
        elmPath = elmPath->NextSiblingElement();
    }
}

PathData* PathGroupData::getPathData(string key)
{
    return (PathData *)pathes->objectForKey(key);
}

CCFiniteTimeAction* PathGroupData::getPathActionByID(string key, CCNode *holder, float speed1)
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

            CraftView *plane = plane = (CraftView *)holder;

            if(plane->getIsAffiliatedPlane())
            {
                //p_end = ccp(p_pathdata->getEndX() - plane->getHolderPosition().x,p_pathdata->getEndY() - plane->getHolderPosition().y);
            }

            //CCLog("Path From (%f, %f) To (%f, %f)", p_begin.x, p_begin.y, p_end.x, p_end.y);
            float f_len = calcTwoPointLength(p_begin, p_end);

            if(speed > 0.0f)
            {
                time = f_len / speed;
            }

            paction = CCMoveTo::create(time, p_end);
        }
        else if(n_action_type == ACTION_MOVEBY)
        {
            CCPoint p_begin = holder->getPosition();
            CCPoint p_offset = ccp(p_pathdata->getMoveByX(), p_pathdata->getMoveByY());
            float f_len = calcTwoPointLength(CCPointZero, p_offset);

            if(speed > 0.0f)
            {
                time = f_len / speed;
            }

            paction = CCMoveBy::create(time, p_offset);
        }
        else if(n_action_type == ACTION_MOVEARC)
        {
            radius = p_pathdata->getRadius();
            //angle_begin = holder->getRotation();
            ////CCLog("DELTA %f BEGIN %f END %f", angle_delta, angle_begin);
            float offsetangle = (angle_delta > 0) ? angle_delta : -angle_delta;
            //float f_len = offsetangle / 360.0f * 2.0f * radius * 3.14f;
            float f_len = CC_DEGREES_TO_RADIANS(offsetangle) * radius;

            if(speed > 0.0f)
            {
                time = f_len / speed;
            }

            paction = RoundPathAction::create(time, radius, holder->getRotation(), angle_delta, getPathAutoRotation(getIntFromS(key.c_str())));
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

CCPoint PathGroupData::getFirstPoint()
{
    PathData *p_pathdata = this->getPathData("1");
    CCPoint p_begin = CCPointZero;

    if(p_pathdata)
    {
        p_begin = ccp(p_pathdata->getBeginX(), p_pathdata->getBeginY());
    }

    return p_begin;
}

CCPoint PathGroupData::getBXPoint()
{
    PathData* p_pathdata = this->getPathData("1");
    CCPoint p_xy = CCPointZero;

    if(p_pathdata)
    {
        p_xy = ccp(p_pathdata->getBeginX(), _s_height);
    }

    return p_xy;

}

bool PathGroupData::getPathAutoRotation(int n_pathid)
{
    PathData *p_pathdata = this->getPathData(getStringFromInt(n_pathid));

    return p_pathdata->getIsAutoRotation();
}

bool PathGroupData::getCurPathRotation(int n_pathid, CCPoint curpos, float& rotation)
{
    PathData *p_pathdata = this->getPathData(getStringFromInt(n_pathid));

    if(p_pathdata)
    {
        float angle = p_pathdata->getAngle();

        if(angle != 0)
        {
            return false;
        }

        CCPoint p_begin = curpos;
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
