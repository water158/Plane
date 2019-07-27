#ifndef __FERRIS_GAME__FerrisTools__
#define __FERRIS_GAME__FerrisTools__

#include "cocos2d.h"
#include "GlobalEnum.h"
USING_NS_CC;

class OPoint : public CCObject
{
public:
    OPoint(float x, float y)
    {
        m_fX = x;
        m_fY = y;
    }
    ~OPoint()
    {
        
    }
    
    float m_fX;
    float m_fY;
};

//两个点的位置，指定速度，就能计算出平均速度需要的时间
float calcAverageSpeed(CCPoint posBegin, CCPoint posEnd, float fSpeed);

//两个点形成的向量与竖直方向的夹角
float calcTwoPointVerticalAngle(CCPoint begin_point, CCPoint end_point, bool isUp=true);

//计算两个点之间的距离
float calcTwoPointLength(CCPoint p_begin, CCPoint p_end);

//计算两个点形成的向量与右边形成的夹角
float calcTwoPointAngle(CCPoint begin_point, CCPoint end_point);

//计算两个点形成的向量与右边形成的夹角2  第一第二象限 角度范围(0 到 180) 否则 第三第四（-180 到 0）
float calcTwoPointAngle2(CCPoint begin_point, CCPoint end_point);

//计算原始点与目标位置形成的角度
float calcAngleFromOriginToTarget(CCPoint origin, CCPoint target);

//精灵遮罩
CCSprite* ccspmaskedSpriteWithSprite(cocos2d::CCSprite* textureSprite, cocos2d::CCSprite* maskSprite);

//圆形与点的碰撞（将目标点看成圆）
bool isRoundContainsPoint(CCNode *aimnode,CCPoint aimpoint);

//圆形与圆形的碰撞（两个node都看成圆形）
bool isRoundContainsNode(CCNode *aimroundnode,CCNode *aimnode);

//圆形与矩形的碰撞
bool collision_RectWithCircle(CCRect rect, CCPoint p, float r);

//用于创建无法修改的包内文件副本（创建用户存储文件）
void copyData(const char* pFileURL,const char* pFileName);

//用于判断本地文件是否存在
bool isFileExist(const char* pFileName);

//随机范围值（包括两端部分）
int getRandom(int begain_num,int end_num);

//char *转为 int  float
int getIntFromS(const char *temp_string);
float getFloatFromS(const char *temp_string);

//int float转为string
std::string convertIntToString(int temp_int);
std::string getStringFromInt(int temp_int);
std::string getStringFromFloat(float temp_float);

//根据向量的x、y计算出半径
float calcRadius(float x, float y);

//根据圆上的一点求出圆心角度
float calcAngleDegrees(float x, float y);

//以一点为中心，根据x和y的偏移量随机生成一个点
CCPoint getRandomPositionByOffset(CCPoint anchor_point, float offset_len_x, float offset_len_y);

//以一点为中心，根据x和y的偏移量随机生成一个点（生成点的范围是一个方形空心形状）
CCPoint getRandomPositionByBound(CCPoint anchor_point, float min_x, float min_y, float max_x, float max_y);

CCPoint getRandomPositionByMinLen(CCPoint anchor,CCPoint posNow,float w,float h,float len);

//比较字符串是否一样
bool compareString(const char* src, const char* dst);

//获得一个点相对于另一个点在某个角度的对称点
CCPoint getMappingPoint(CCPoint pCenter, CCPoint pOne, float angle);

//获取系统当前时间（返回的数转换成毫秒）
long getCurrentSystemTime();

//获取系统当前时间
int getCurrentTime(GetTimeType type);


#endif