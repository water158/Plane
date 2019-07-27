#include "lightning.h"
#include "SmartRes.h"
USING_NS_CC;

lightning::lightning()
{

}

lightning::~lightning(){

}

lightning* lightning::create()
{
	lightning* layer = new lightning();
	if (layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}
bool lightning::init(){
	
	
	

	{
		sp_instro = CCSprite::create("game/laser/laser_small_white.png");
		sp_instro->setAnchorPoint(ccp(0.5, 0.0));
		
		this->addChild(sp_instro, 1);
		CCGLProgram *program = new CCGLProgram();

		program->initWithVertexShaderFilename("shaders/lesson.vsh", "shaders/light.fsh");
		sp_instro->setShaderProgram(program);
		program->release();
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->use();
		GLint a = program->getUniformLocationForName("u_opacity");
		program->setUniformLocationWith1f(a, 1);

		program->updateUniforms();
	}
	this->scheduleUpdate();
	setVisible(true);


	//setStartAndEnd(ccp(100, 100), ccp(300, 400));
	return true;
}

void lightning::setStartAndEnd(CCPoint start,CCPoint end)
{
	this->start = start;
	this->end = end;
	
}

void lightning::update(float delta){
	
	CCPoint temp = end - start;
	double dis = sqrtf(temp.x*temp.x + temp.y*temp.y);
	float distance = start.getDistance(end);

	float x = 0;
	float y = 0;

	CCPoint offest = ccpSub(end, start);
	float radian = atanf(offest.y / offest.x);
	float degr = 0;
	//CCLog("radian= %f", CC_RADIANS_TO_DEGREES(radian));
	if (start.x < end.x){
		degr = CC_RADIANS_TO_DEGREES(radian) - 90;
	}
	else{
		degr = CC_RADIANS_TO_DEGREES(radian) + 90;
	}

	sp_instro->setScaleY(dis / (sp_instro->getContentSize().height));


	sp_instro->setRotation(degr);
	//CCLog("radian= %f", sp_instro->getRotation());
	sp_instro->setPosition(start);
}

void lightning::setVisible(bool visible){
	sp_instro->setVisible(visible);
}

void lightning::setBreadth(float size){
	sp_instro->setScaleX(size);
}

void lightning::setBminance(float uminance){
	CCGLProgram *program = sp_instro->getShaderProgram();
	program->use();
	GLint a = program->getUniformLocationForName("u_opacity");
	program->setUniformLocationWith1f(a, uminance);
}