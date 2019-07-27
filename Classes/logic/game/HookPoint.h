#ifndef _HOOK_POINT_H_
#define _HOOK_POINT_H_

#include "cocos2d.h"
#include "FerrisTools.h"

USING_NS_CC;
using namespace std;

class HookPoint
{

public:
    HookPoint();
    HookPoint(CCPoint hook, CCPoint origin);
    HookPoint(float hook_x, float hook_y, float origin_x, float origin_y);
    HookPoint(const HookPoint& other);
    HookPoint& operator= (const HookPoint& other);
    
    CCPoint getAbsolutePosition(CCPoint point_origin, float angle_origin);
    
    CCPoint getAbsolutePosition(CCNode *holder);
    
public:
    //Hook的位置
    CCPoint pointHook;
    
    //Origin的位置
    CCPoint pointOrigin;
    
    //从Hook到Origin得距离
    float radiusFromHookToOrigin;
    
    //连接Hook和Origin两点所组成的直线，在笛卡尔坐标系中得夹角
    float angleFromHookToOrigin;
    
private:
    void init(CCPoint hook, CCPoint origin);
};

#endif  // _GUN_DETAIL_H_