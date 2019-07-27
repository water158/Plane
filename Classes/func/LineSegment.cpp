//
//  LineSegment.cpp
//  HelloCpp
//
//  Created by renyi on 14/12/12.
//
//

#include "LineSegment.h"

void LineSegment::init(CCPoint start_point, CCPoint end_point)
{
    m_oStartPoint = start_point;
    m_oEndPoint = end_point;
}

bool LineSegment::addChildRef(LineSegment* child)
{
    //if already have this child, do not add, return false
    int nChild = (int)children.size();
    
    for(int i = 0; i < nChild; i++)
    {
        if(children[i]==child)
        {
            return false;
        }
    }
    
    children.push_back(child);
    
    return true;
}

bool LineSegment::removeChildRef(LineSegment*child)
{
    //only remove the first matched child
    int nChild = (int)children.size();
    
    for(int i = 0; i < nChild; i++)
    {
        if(children[i] == child)
        {
            children.erase(children.begin() + i);
            nChild--;
            i--;
            return true;
        }
    }
    
    return false;
}

CCPoint LineSegment::getMidPoint() const
{
    return ccpMidpoint(m_oStartPoint, m_oEndPoint);
}

float LineSegment::getLen() const
{
    return ccpDistance(m_oStartPoint, m_oEndPoint);
}

CCPoint LineSegment::getDirection() const
{
    return ccpNormalize(m_oEndPoint - m_oStartPoint);
}

CCPoint LineSegment::getVector() const
{
    return m_oEndPoint - m_oStartPoint;
}

CCPoint LineSegment::getRightNormal() const
{
    //right side normal or outer normal
    CCPoint dir = getDirection();
    CCPoint normal(dir.y, 0 - dir.x);
    return normal;
}

