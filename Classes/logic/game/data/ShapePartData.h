#ifndef __SHAPE_PART_DATA_H__
#define __SHAPE_PART_DATA_H__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalParams.h"
#include "FerrisTools.h"


USING_NS_CC;
using namespace std;

class ShapePartData : public CCObject
{
public:
	virtual ~ShapePartData();

	static ShapePartData* create(TiXmlElement *elm);

	//加载属性数据
	bool loadData(TiXmlElement *elm);

private:
	ShapePartData();

//////数据属性
	//id
	CC_SYNTHESIZE (string , m_strID, ID);
    //type
    CC_SYNTHESIZE (int , m_nType, Type);
	//NAME
	CC_SYNTHESIZE (string , m_strName, Name);
	//是否是动画
	CC_SYNTHESIZE (bool , m_bIsAnimation, IsAnimation);
	//动画延迟
	CC_SYNTHESIZE (float , m_fDelay, Delay);
	//动画帧数
	CC_SYNTHESIZE (int , m_nFrameNum, FrameNum);
    //是否有变形动画
    CC_SYNTHESIZE (bool , m_strIsTransform, IsTransform);
    //变形动画name
    CC_SYNTHESIZE (string , m_strTransformName, TransformName);
    //变形动画path
    CC_SYNTHESIZE (string , m_strTransformPath, TransformPath);
	//PlistName
	CC_SYNTHESIZE (string , m_strPlistName, PlistName);
	//path
	CC_SYNTHESIZE (string , m_strPlistPath, PlistPath);
	//x
	CC_SYNTHESIZE (float , m_fX, X);
	//y
	CC_SYNTHESIZE (float , m_fY, Y);
	//z
	CC_SYNTHESIZE (float , m_fZ, Z);
	//scale_x
	CC_SYNTHESIZE (float , m_fScale_x, Scale_x);
	//scale_y
	CC_SYNTHESIZE (float , m_fScale_y, Scale_y);
	//rotation
	CC_SYNTHESIZE (float , m_fRotation, Rotation);
};

#endif