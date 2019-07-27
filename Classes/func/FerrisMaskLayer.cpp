#include "FerrisMaskLayer.h"


FerrisMaskLayer::FerrisMaskLayer()
{
	
}

bool FerrisMaskLayer::init()
{
	CCLayer::init();

	setColorLayer(ccc3(255,255,255),128);
	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	setTouchPriority(-128);

	return true;
}

bool FerrisMaskLayer::ccTouchBegan(CCTouch *pTouches, CCEvent *pEvent)
{
	CCPoint location = pTouches->getLocation(); 
	CCRect clickrect = CCRectMake(begainPos.x,begainPos.y,layerWidth,layerHeight);

	if(clickrect.containsPoint(location))
		return true;
	return false;
}

void FerrisMaskLayer::setColorLayer(ccColor3B color, unsigned char opacity)
{
	CCLayerColor *backlayer = (CCLayerColor *)getChildByTag(0);
	if(!backlayer)
	{
		backlayer = CCLayerColor::create(ccc4(color.r,color.g,color.b,opacity),layerWidth,layerHeight);
		backlayer->setAnchorPoint(ccp(0.0f,0.0f));
		backlayer->setPosition(begainPos);
		addChild(backlayer,-1,0);
	}else
	{
		backlayer->setColor(ccc3(color.r,color.g,color.b));
		backlayer->setOpacity(opacity);
	}
}

void FerrisMaskLayer::destroy()
{
	if(this->getParent())
		this->removeFromParent();
	if(this->getChildrenCount()>0)
		this->removeAllChildren();

	this->release();
}

FerrisMaskLayer *FerrisMaskLayer::create(float width, float height,CCPoint bpos)
{
	FerrisMaskLayer *templayer = new FerrisMaskLayer();
	templayer->layerWidth = width;
	templayer->layerHeight = height;
	templayer->begainPos = bpos;

	if(templayer && templayer->init())
		return templayer;

	return NULL;
}