#include "FerrisTools.h"


//两个点的位置，指定速度，就能计算出平均速度需要的时间
float calcAverageSpeed(CCPoint posBegin, CCPoint posEnd, float fSpeed)
{
	float temp_time = 0.0f;
	float tempx = posEnd.x - posBegin.x;
	float tempy = posEnd.y - posBegin.y;
	float tempz = (float)(sqrt(tempx*tempx + tempy*tempy));
	temp_time = tempz / fSpeed;
	return temp_time;
}

/*

            |0度
			|
			|
负90度-------------90度
或270度
            |
			|
			|180度

可以把起始点和终点同时移动，然后让起始点落在原点上，之后连接起点和终点，看直线与Y轴的夹角
*/
float calcTwoPointVerticalAngle(CCPoint begin_point, CCPoint end_point, bool isUp)
{
	float delta_x = end_point.x - begin_point.x;
	float delta_y = end_point.y - begin_point.y;
	float angle = (float)CC_RADIANS_TO_DEGREES(atanf(delta_x / delta_y));
	if (delta_y < 0)
	{
		if (delta_x < 0)
		{
			angle = 180 + fabs(angle);
		}
		else
		{
			angle = 180 - fabs(angle);
		}
	}

	if (isUp)
	{
		return angle;
	}
	else
	{
		return angle + 180.0f;
	}
}

//计算两个点之间的距离
float calcTwoPointLength(CCPoint p_begin, CCPoint p_end)
{
	float f_temp_x = p_end.x - p_begin.x;
	float f_temp_y = p_end.y - p_begin.y;
    
	return sqrt(f_temp_x * f_temp_x + f_temp_y * f_temp_y);
}

//计算两个点形成的向量与右边形成的夹角
float calcTwoPointAngle(CCPoint begin_point, CCPoint end_point)
{
	float x = end_point.x - begin_point.x;
	float y = end_point.y - begin_point.y;
    
	// 开方
	float hypotenuse = sqrtf(x * x + y * y);
	// 获得弧度
	float cos = x / hypotenuse;
	// 求出弧度
	float radian = acosf(cos);
	// calculated angle in radians
	float angle = CC_RADIANS_TO_DEGREES(radian);
    
	// 根据coco2dx的坐标系进行修改
	if (y > 0.0f)
    {
		angle = 0.0f - angle;
	}
    
    if (angle > 0.0f)
    {
		angle -= 180.0f;
	}
    else if(angle < 0.0f)
    {
		angle += 180.0f;
	}
	
    return angle;
}

//计算两个点形成的向量与右边形成的夹角
float calcTwoPointAngle2(CCPoint begin_point, CCPoint end_point)
{
    float x = end_point.x - begin_point.x;
    float y = end_point.y - begin_point.y;
    
    //角度
    float angle = CC_RADIANS_TO_DEGREES(atan2f(y, x));
    
    // 根据coco2dx的坐标系进行修改
    if (y > 0.0f)
    {
        angle = angle - 180.0f;
        
        if (angle > 0.0f)
        {
            angle -= 180.0f;
        }
        else if(angle < 0.0f)
        {
            angle += 180.0f;
        }
        
    }
    
    return angle;
}

//计算原始点与目标位置形成的角度
float calcAngleFromOriginToTarget(CCPoint origin, CCPoint target)
{
    float delta_x = target.x - origin.x;
	float delta_y = target.y - origin.y;
    
    return CC_RADIANS_TO_DEGREES(ccp(delta_x, delta_y).getAngle());
}

CCSprite* ccspmaskedSpriteWithSprite(CCSprite* textureSprite, CCSprite* maskSprite)
{
	// 1
	int w = maskSprite->getContentSize().width * maskSprite->getScaleX();
	int h = maskSprite->getContentSize().height * maskSprite->getScaleY();
	CCRenderTexture* rt = CCRenderTexture::create(w, h);
	// 2
	maskSprite->setPosition(ccp(maskSprite->getContentSize().width *  maskSprite->getScaleX() / 2,
		maskSprite->getContentSize().height * maskSprite->getScaleY() / 2));

	textureSprite->setPosition(ccp(textureSprite->getContentSize().width *  textureSprite->getScaleX() / 2,
		textureSprite->getContentSize().height * textureSprite->getScaleY() / 2));

	// 3
	ccBlendFunc blendFunc;
	/************************************************************************/
	/* 可有可无                                                                     */
	/************************************************************************/
	//blendFunc.src = GL_ONE;
	//blendFunc.dst = GL_ZERO;
	//maskSprite->setBlendFunc(blendFunc);

	blendFunc.src = GL_DST_ALPHA;			// mask图片的当前alpha值是多少，如果是0（完全透明），那么就显示mask的。如果是1（完全不透明）
	blendFunc.dst = GL_ZERO;				// maskSprite不可见
	textureSprite->setBlendFunc(blendFunc);

	// 4
	rt->begin();
	maskSprite->visit();
	textureSprite->visit();
	rt->end();

	// 5
	CCSprite* retval = CCSprite::create();
	retval->initWithTexture(rt->getSprite()->getTexture());
	retval->setFlipY(true);
	return retval;
}

//圆形与点的碰撞（将目标点看成圆）
bool isRoundContainsPoint(CCNode *aimnode, CCPoint aimpoint)
{
	if (!aimnode)
	{
		return false;
	}
	CCPoint myanchorpoint = aimnode->getAnchorPoint();

	float sca_x = aimnode->getScaleX();
	float sca_y = aimnode->getScaleY();
	//计算圆形的中心点位置坐标
	float mymidpoint_x = aimnode->getPositionX() - aimnode->getContentSize().width*myanchorpoint.x*sca_x + aimnode->getContentSize().width / 2 * sca_x;
	float mymidpoint_y = aimnode->getPositionY() - aimnode->getContentSize().height*myanchorpoint.y*sca_y + aimnode->getContentSize().height / 2 * sca_y;

	//计算半径
	float rlength = aimnode->getContentSize().width / 2 * sca_x;
	//计算两点之间的距离
	float tempx = aimpoint.x - mymidpoint_x;
	float tempy = aimpoint.y - mymidpoint_y;
	float tempz = (float)(sqrt(tempx*tempx + tempy*tempy));

	if (tempz > rlength)
		return false;
	else
		return true;
}

//圆形与圆形的碰撞（两个node都看成圆形）
bool isRoundContainsNode(CCNode *aimroundnode, CCNode *aimnode)
{
	if (!aimnode || !aimroundnode)
	{
		return false;
	}
	CCPoint myanchorpoint = aimroundnode->getAnchorPoint();
	CCPoint myanchorpoint1 = aimnode->getAnchorPoint();

	float sca_x = aimroundnode->getScaleX();
	float sca_y = aimroundnode->getScaleY();
	float sca_x1 = aimnode->getScaleX();
	float sca_y1 = aimnode->getScaleY();
	//计算圆形的中心点位置坐标
	float mymidpoint_x = aimroundnode->getPositionX() - aimroundnode->getContentSize().width*myanchorpoint.x*sca_x + aimroundnode->getContentSize().width / 2 * sca_x;
	float mymidpoint_y = aimroundnode->getPositionY() - aimroundnode->getContentSize().height*myanchorpoint.y*sca_y + aimroundnode->getContentSize().height / 2 * sca_y;
	float mymidpoint_x1 = aimnode->getPositionX() - aimnode->getContentSize().width*myanchorpoint1.x*sca_x1 + aimnode->getContentSize().width / 2 * sca_x1;
	float mymidpoint_y1 = aimnode->getPositionY() - aimnode->getContentSize().height*myanchorpoint1.y*sca_y1 + aimnode->getContentSize().height / 2 * sca_y1;

	//计算半径
	float rlength = aimroundnode->getContentSize().width / 2 * sca_x;
	float rlength1 = aimnode->getContentSize().width / 2 * sca_x1;


	//计算两点之间的距离
	float tempx = mymidpoint_x1 - mymidpoint_x;
	float tempy = mymidpoint_y1 - mymidpoint_y;
	float tempz = (float)(sqrt(tempx*tempx + tempy*tempy));

	if (tempz > rlength + rlength1)
		return false;
	else
		return true;
}

bool collision_RectWithCircle(CCRect rect, CCPoint p, float r)
{
    //获取矩形信息
    //左下角坐标：( lx , ly )
    //右上角坐标：( rx , ry )
    float lx = rect.getMinX();
    float ly = rect.getMinY();
    float rx = rect.getMaxX();
    float ry = rect.getMaxY();
    
    //计算圆心到四个顶点的距离
    float d1 = p.getDistance( ccp(lx, ly) );
    float d2 = p.getDistance( ccp(lx, ry) );
    float d3 = p.getDistance( ccp(rx, ly) );
    float d4 = p.getDistance( ccp(rx, ry) );
    
    //判断是否碰撞
    //判断距离是否小于半径
    if( d1<r || d2<r || d3<r || d4<r ) return true;
    //是否在圆角矩形的，横向矩形内
    if( p.x > (lx-r) && p.x < (rx+r) && p.y > ly && p.y < ry ) return true;
    //是否在圆角矩形的，纵向矩形内
    if( p.x > lx && p.x < rx && p.y > (ly-r) && p.y < (ry+r) ) return true;
    
    //不发生碰撞
    return false;
}

//用于创建无法修改的包内文件副本（创建用户存储文件）
void copyData(const char* pFileURL, const char* pFileName)
{
	std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileURL);
	unsigned long len = 0;
	unsigned char *data = NULL;

	data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "r", &len);
	CCAssert(data, "copyData is null");
	std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	destPath += pFileName;
    
    /*
	FILE *fp = fopen(destPath.c_str(), "w+");

	if (fp)
	{
		fwrite(data, sizeof(char), len, fp);
		fclose(fp);
		delete[]data;
		data = NULL;
	}
    */
    
    std::string strContent((char *)data);
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, destPath);
}

//用于判断本地文件是否存在
bool isFileExist(const char* pFileName)
{
	if (!pFileName) return false;
	std::string filePath = pFileName;

	FILE *fp = fopen(filePath.c_str(), "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}

	return false;
}

//随机范围值（包括两端部分）
int getRandom(int begain_num, int end_num)
{
    int temp = rand() % (end_num - begain_num +1) + begain_num;

	return temp;
}

//char *转为 int  float
int getIntFromS(const char *temp_string)
{
	if (temp_string)
	{
		return atoi(temp_string);
	}
	else
	{
		return -1;
	}
}
float getFloatFromS(const char *temp_string)
{
	if (temp_string)
	{
		return atof(temp_string);
	}
	else
	{
		return -1;
	}
}

//int转为string
std::string getStringFromInt(int temp_int)
{
    char temp_str[200];
    std::string format = "%d";
    sprintf(temp_str, format.c_str(), temp_int);
    
    return temp_str;
}
std::string convertIntToString(int temp_int)
{
	char temp_str[200];
	std::string format = "%d";
	sprintf(temp_str, format.c_str(), temp_int);

	return temp_str;
}
//float转为string
std::string getStringFromFloat(float temp_float)
{
	char temp_str[200];
	std::string format = "%f";
	sprintf(temp_str, format.c_str(), temp_float);

	return temp_str;
}

//根据向量的x、y计算出半径
float calcRadius(float x, float y)
{
    return sqrtf(x * x + y * y);
}

//根据圆上的一点求出圆心角度
float calcAngleDegrees(float x, float y)
{
    return CC_RADIANS_TO_DEGREES(atan2f(y, x));
}

//以一点为中心，根据x和y的偏移量随机生成一个点
CCPoint getRandomPositionByOffset(CCPoint anchor_point, float offset_len_x, float offset_len_y)
{
	if(offset_len_x < 0)
	{
		offset_len_x = -offset_len_x;
	}
	if(offset_len_y < 0)
	{
		offset_len_y = -offset_len_y;
	}

	//范围内随机x、y值
	float temp_x = CCRANDOM_0_1()*offset_len_x;
	float temp_y = CCRANDOM_0_1()*offset_len_y;

	//随机正负
	temp_x *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;
	temp_y *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;

	return ccp(anchor_point.x + temp_x, anchor_point.y + temp_y);
}

//以一点为中心，根据x和y的偏移量随机生成一个点（生成点的范围是一个方形空心形状）
CCPoint getRandomPositionByBound(CCPoint anchor_point, float min_x, float min_y, float max_x, float max_y)
{
	float dela_x = fabs(max_x) - fabs(min_x);
	float dela_y = fabs(max_y) - fabs(min_y);              

	//范围内随机x、y值
	float temp_x = CCRANDOM_0_1()*dela_x + fabs(min_x);
	float temp_y = CCRANDOM_0_1()*dela_y + fabs(min_y);

	//随机正负
	temp_x *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;
	temp_y *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;

	//CCLog("-----rend_x----===  (%f)-----rend_y----===  (%f)", temp_x, temp_y);

	return ccpAdd(anchor_point, ccp(temp_x, temp_y));
}

CCPoint getRandomPositionByMinLen(CCPoint anchor,CCPoint posNow,float w,float h,float len)
{
    //随机点
    float temp_x = CCRANDOM_0_1() * w;
    float temp_y = CCRANDOM_0_1() * h;
    
    CCPoint posNext = ccp(temp_x,temp_y);
    
    //两点距离
    float temp_len = ccpDistance(posNow,posNext);
    
    len *= 2;
    
    //如果两点小于最小长度*2
    if(temp_len < len)
    {
        //判断当前点在中心点位置
        if(posNow.x <= anchor.x)
        {
            //在左边
            temp_x += len * 2;
        }
        else
        {
            //在右边
            temp_x -= len * 2;
        }
    }
    
    //随机正负
    temp_x *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;
    temp_y *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;
    
    posNext = ccp(temp_x,temp_y);
    
    return ccpAdd(anchor,posNext);
}

//比较字符串是否一样
bool compareString(const char* src, const char* dst)
{
	int n_len1 = strlen(src);
	int n_len2 = strlen(dst);

	if (n_len1 != n_len2)
	{
		return false;
	}
    return strncmp(src, dst, n_len1) == 0 ? true : false;
}

//获得一个点相对于另一个点在某个角度的对称点
CCPoint getMappingPoint(CCPoint pCenter, CCPoint pOne, float angle)
{
	//先算出运来的与水平角度的夹角
	float fAngle = calcAngleFromOriginToTarget(pCenter, pOne);

	//两点之间的距离就是半径
	float fRadius = calcTwoPointLength(pCenter, pOne);

	float temp_x = fRadius * cos(CC_DEGREES_TO_RADIANS(fAngle + angle));
	float temp_y = fRadius * sin(CC_DEGREES_TO_RADIANS(fAngle + angle));

	return ccp(pCenter.x + temp_x, pCenter.y + temp_y);
}

//获取系统当前时间（单位：毫秒数）
long getCurrentSystemTime() 
{ 
	struct tm *tm; 
	time_t timep; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct cc_timeval now;  
	CCTime::gettimeofdayCocos2d(&now, NULL);  
	timep = now.tv_sec; 
#endif  


	tm = localtime(&timep); 
	int year = tm->tm_year + 1900; 
	int month = tm->tm_mon + 1; 
	int day = tm->tm_mday; 
	int hour=tm->tm_hour; 
	int min=tm->tm_min; 
	int second=tm->tm_sec; 

	//换算成秒来存储
	long temp = second+min*60+hour*3600+day*3600*24+month*3600*24*31+year*3600*24*31*365;
	return temp;
}

int getCurrentTime(GetTimeType type)
{
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour=tm->tm_hour;
    int min=tm->tm_min;
    int second=tm->tm_sec;
    
    switch (type)
    {
        case kGetTimeYear:
            return year;
            break;
        case kGetTimeMonth:
            return month;
            break;
        case kGetTimeDay:
            return day;
            break;
        case kGetTimeHour:
            return hour;
            break;
        case kGetTimeMin:
            return min;
            break;
        case kGetTimeSec:
            return second;
            break;
            
        default:
            break;
    }
    
    return 0;
}
