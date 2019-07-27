#include "ShapePartData.h"

ShapePartData::ShapePartData()
{

}

ShapePartData::~ShapePartData()
{

}

ShapePartData* ShapePartData::create(TiXmlElement *elm)
{
	ShapePartData* data = new ShapePartData();
	if(data && data->loadData(elm))
	{
		//data->autorelease();
		return data;
	}

	return NULL;
}

//加载属性数据
bool ShapePartData::loadData(TiXmlElement *elm)
{
	if(!elm)
	{
		CCMessageBox("ShapePartData::loadData  elm is null", "");
	}

	this->setID("1");
	this->setName("1");
    this->setType(2);
	this->setIsAnimation(false);
	this->setDelay(0.1f);
	this->setFrameNum(1);
	this->setPlistName("hero_0_1");
	this->setPlistPath("game/hero/0/hero_0_1.plist");
    this->setIsTransform(false);
    this->setTransformName("plane_wing");
    this->setTransformPath("game/wing/plane_wing.ExportJson");
	this->setX(0.0f);
	this->setY(0.0f);
	this->setZ(0.0f);
	this->setScale_x(1.0f);
	this->setScale_y(1.0f);
	this->setRotation(0.0f);

	//id
	if(elm->Attribute("id"))
	{
		this->setID(elm->Attribute("id"));
	}
    
    if(elm->Attribute("type"))
    {
        this->setType(getIntFromS(elm->Attribute("type")));
    }

	//name
	if(elm->Attribute("name"))
	{
		this->setName(elm->Attribute("name"));
	}

	//是否是动画
	if(elm->Attribute("is_animation"))
	{
		this->setIsAnimation(getIntFromS(elm->Attribute("is_animation")) == 0 ? false : true);
	}

	//动画延迟
	if(elm->Attribute("delay"))
	{
		this->setDelay(getFloatFromS(elm->Attribute("delay")));
	}

	//帧数
	if(elm->Attribute("frame_num"))
	{
		this->setFrameNum(getIntFromS(elm->Attribute("frame_num")));
	}

	//动画名称
	if(elm->Attribute("ani_name"))
	{
		this->setPlistName(elm->Attribute("ani_name"));
	}

	//动画路径
	if(elm->Attribute("plist"))
	{
		this->setPlistPath(elm->Attribute("plist"));
	}
    
    if (elm->Attribute("is_transform")) {
        this->setIsTransform(elm->Attribute("is_transform") == 0 ? false : true);
    }
    
    //transform_name
    if(elm->Attribute("transform_name"))
    {
        this->setTransformName(elm->Attribute("transform_name"));
    }
    
    //transform_path
    if(elm->Attribute("transform_path"))
    {
        this->setTransformPath(elm->Attribute("transform_path"));
    }

	//X
	if(elm->Attribute("x"))
	{
		this->setX(getFloatFromS(elm->Attribute("x")));
	}

	//Y
	if(elm->Attribute("y"))
	{
		this->setY(getFloatFromS(elm->Attribute("y")));
	}

	//Z
	if(elm->Attribute("z"))
	{
		this->setZ(getFloatFromS(elm->Attribute("z")));
	}

	//scale_x
	if(elm->Attribute("scale_x"))
	{
		this->setScale_x(getFloatFromS(elm->Attribute("scale_x")));
	}

	//scale_y
	if(elm->Attribute("scale_y"))
	{
		this->setScale_y(getFloatFromS(elm->Attribute("scale_y")));
	}

	//rotation
	if(elm->Attribute("rotation"))
	{
		this->setRotation(getFloatFromS(elm->Attribute("rotation")));
	}

	return true;
}