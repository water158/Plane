#ifndef __FERRIS_GAME__SplashLayer__
#define __FERRIS_GAME__SplashLayer__

#include "cocos2d.h"
#include "SmartRes.h"
#include "GameDelegate.h"

USING_NS_CC;

class SplashLayer : public CCLayer
{
public:
	~SplashLayer();

	static SplashLayer* create();

	void callBackEnter();

private:
	SplashLayer();

	bool init();
};


#endif