#include "SplashLayer.h"


SplashLayer::SplashLayer()
{

}

SplashLayer::~SplashLayer()
{

}

SplashLayer* SplashLayer::create()
{
	SplashLayer* pLayer = new SplashLayer();
	if(pLayer && pLayer->init())
	{
		pLayer->autorelease();
		return pLayer;
	}

	return NULL;
}

bool SplashLayer::init()
{
	CCLayer::init();


	CCSprite* spLogo = CCSprite::create("ui/icon/icon_logo.png");
	spLogo->setPosition(ccp(_s_width/2, _s_height/5*3));
    spLogo->setScale(3.0f);
	this->addChild(spLogo);

	spLogo->runAction(CCSequence::create(
		CCScaleTo::create(0.4f, 1.3f),
		CCScaleTo::create(0.1f, 1.7f),
		CCDelayTime::create(1),
		CCCallFunc::create(this, callfunc_selector(SplashLayer::callBackEnter)),
		CCDelayTime::create(1),
		NULL));

	return true;
}

void SplashLayer::callBackEnter()
{
    
    //加载封面动画
	GameDelegate::shared()->openSceneTools(true);
}