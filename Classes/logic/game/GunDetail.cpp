#include "GunDetail.h"

GunDetail::GunDetail()
:m_bGunGroupTransfromDirty(true)
,m_fHarmRatio(1.0f)
{
    this->uuid = NEWUUID;
}

GunDetail::~GunDetail()
{
    CC_SAFE_RELEASE(dataTargetGunGroup);
    CC_SAFE_RELEASE(dataGun);
}

GunDetail* GunDetail::create(TargetGunGroupData *data1, GunData *data2)
{
    GunDetail *data = new GunDetail();
	
    if(data && data->init(data1, data2))
	{
		data->autorelease();
		return data;
	}
    
	return NULL;
}

bool GunDetail::init(TargetGunGroupData *data1, GunData *data2)
{
    this->dataTargetGunGroup = data1;
    this->dataTargetGunGroup->retain();
    
    this->dataGun = data2;
    this->dataGun->retain();
    
    this->setCurMidRepeatTimes(0);
    this->setCurrentRepeatTimes(0);
    
    this->setIsBeginFire(false);
    
    this->setDelayTime(0.0f);
    
    this->setIsFixedGunDetail(dataTargetGunGroup->getIsFixedGunDetail());
    
    return true;
}

/*********************************************
 * Time Judgement
 *********************************************/

//增加重复次数的计数
void GunDetail::increaseRepeatTimes()
{
	this->current_repeat_times++;
}

bool GunDetail::isRepeatTimeCurrentLargeThenTotal()
{
    if (dataTargetGunGroup->getRepeatTimes() == 0)
    {
        return false;
    }
    
    return current_repeat_times >= dataTargetGunGroup->getRepeatTimes();
}

/*
//增加Interval次数的计数
//因为删除了Interval 所以就不要了
void GunDetail::increaseIntervalIndex()
{
    if (getIntervalCount() > 0)
    {
        this->interval_index++;
    }
}

//获得当前的Interval总值
//因为删除了Interval 所以就不要了
float GunDetail::getCurrentTotalInterval()
{
    float ret = 0.0f;
    for (int i = 0; i < getIntervalIndex(); i++)
    {
        ret += dataGun->getIntervals()[i];
    }
    
    return ret;
}
*/

//获得Interval的总个数
int GunDetail::getIntervalCount()
{
    return dataGun->getIntervals().size();
}

//获得初次开枪时间
float GunDetail::getTimeBeginFire()
{
    return dataTargetGunGroup->getFirstDelay() + dataGun->getFirstDelay() + this->getDelayTime();
}

//获得目标时间
float GunDetail::getTimeTarget()
{
    return dataTargetGunGroup->getFirstDelay()  //1
    + dataGun->getFirstDelay()                  //2
    + this->getDelayTime()
    + dataTargetGunGroup->getRepeatDelay() * getCurrentRepeatTimes()  //3
    + dataTargetGunGroup->getMidDelay() * getCurMidRepeatTimes();
    //+ getCurrentTotalInterval() * dataTargetGunGroup->getMidRepeat();
}

//获得子弹发射所用总时间
float GunDetail::getTimeTotal()
{
    return dataTargetGunGroup->getFirstDelay()  
    + dataGun->getFirstDelay()
    + this->getDelayTime()
    + dataTargetGunGroup->getRepeatDelay() * dataTargetGunGroup->getRepeatTimes();
    //+ getCurrentTotalInterval() * dataTargetGunGroup->getRepeatTimes();
}

/*
// 因为删除了Interval 所以就不要了
void GunDetail::refreshRepeatTimesAndIntervalIndex()
{
    //RepeatTimes IntarvalIndex的增加处理
    int interval_count_total = this->getIntervalCount();
    if (interval_count_total == 0)
    {
        //因为没有interval，所以只需要增加RepeatTimes
        this->increaseRepeatTimes();
    }
    else
    {
        //Interval自动加1
        this->increaseIntervalIndex();
        
        if (this->getIntervalIndex() > interval_count_total)
        {
            //每次interval循环完成之后，就会自动在currentRepeatTimes这里自动加1，也就是完成了一次大的循环
            this->setIntervalIndex(0);
            this->increaseRepeatTimes();
        }
    }
}*/

bool GunDetail::needRefreshPosition()
{
    if (cur_mid_repeat_times == 0)
    {
        return true;
    }
    
    return false;
}

void GunDetail::refreshMidRepeatTimesAndIntervalIndex()
{
    //中循环++
    cur_mid_repeat_times++;
    
    //判断一下中循环次数
    if (cur_mid_repeat_times >= dataTargetGunGroup->getMidRepeat())
    {
        cur_mid_repeat_times = 0;
        this->increaseRepeatTimes();
    }
}

void GunDetail::refreshRepeatTimes()
{
    this->increaseRepeatTimes();
}

/*********************************************
 * Transform
 *********************************************/

//设定XY位移
void GunDetail::setGunGroupPosition(const CCPoint& newPosition)
{
    if (m_obGunGroupPosition.x != newPosition.x || m_obGunGroupPosition.y != newPosition.y)
    {
        m_obGunGroupPosition = newPosition;
        
        m_bGunGroupTransfromDirty = true;
    }
}

const CCPoint& GunDetail::getGunGroupPosition()
{
    return m_obGunGroupPosition;
}

//获得仿射矩阵
CCAffineTransform GunDetail::getGunGroupTransform()
{
	if (m_bGunGroupTransfromDirty)
	{
		float cos_angle = 1, sin_angle = 0;

		if (m_fGunGroupRotation)
		{
			float radian = CC_DEGREES_TO_RADIANS(m_fGunGroupRotation);

			cos_angle = cosf(radian);
			sin_angle = sinf(radian);
		}

		m_sGunGroupTransform = CCAffineTransformMake(cos_angle,
			sin_angle,
			-sin_angle,
			cos_angle,
			m_obGunGroupPosition.x,
			m_obGunGroupPosition.y);

		m_bGunGroupTransfromDirty = false;
	}

	return m_sGunGroupTransform;
}

//设定转角
void GunDetail::setGunGroupRotation(float newRotation)
{
    if (m_fGunGroupRotation != newRotation)
    {
        m_fGunGroupRotation = newRotation;
        
        m_bGunGroupTransfromDirty = true;
    }
}

//获得转角
float GunDetail::getGunGroupRotation()
{
    return m_fGunGroupRotation;
}

/*********************************************
 * PointExt
 *********************************************/

PointExt::PointExt()
{
    
}

PointExt::~PointExt()
{
    
}

PointExt* PointExt::create(float x, float y)
{
    PointExt *data = new PointExt();
	
    if(data && data->init(x, y))
	{
		data->autorelease();
		return data;
	}
    
	return NULL;
}

bool PointExt::init(float x, float y)
{
    this->setX(x);
    this->setY(y);
    
    return true;
}