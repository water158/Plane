#include "HookPoint.h"

HookPoint::HookPoint()
{
}

HookPoint::HookPoint(CCPoint hook, CCPoint origin)
{
    init(hook, origin);
}

HookPoint::HookPoint(float hook_x, float hook_y, float origin_x, float origin_y)
{
    init(ccp(hook_x, hook_y), ccp(origin_x, origin_y));
}

HookPoint::HookPoint(const HookPoint& other)
{
    init(other.pointHook, other.pointOrigin);
}

HookPoint& HookPoint::operator= (const HookPoint& other)
{
    init(other.pointHook, other.pointOrigin);
    return *this;
}

void HookPoint::init(CCPoint hook, CCPoint origin)
{
    pointHook = hook;
    pointOrigin = origin;
    
    radiusFromHookToOrigin = calcTwoPointLength(pointHook, pointOrigin);
    angleFromHookToOrigin = calcAngleFromOriginToTarget(pointOrigin, pointHook);
}

CCPoint HookPoint::getAbsolutePosition(CCPoint point_origin, float angle_origin)
{
    //base_angle是当前Gun所附着的物体的转角，就是飞机的转角
    float cur_x = cos(CC_DEGREES_TO_RADIANS(angle_origin + angleFromHookToOrigin)) * radiusFromHookToOrigin;
    float cur_y = sin(CC_DEGREES_TO_RADIANS(angle_origin + angleFromHookToOrigin)) * radiusFromHookToOrigin;
    
    return point_origin + ccp(cur_x, cur_y);
}

CCPoint HookPoint::getAbsolutePosition(CCNode *holder)
{
    return getAbsolutePosition(holder->getPosition(), holder->getRotation());
}