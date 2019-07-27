#ifndef __FERRIS_GAME__FerrisMaskLayer__
#define __FERRIS_GAME__FerrisMaskLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FerrisMaskLayer : public CCLayer
{
public:
	FerrisMaskLayer();
	bool init();
	static FerrisMaskLayer *create(float width, float height,CCPoint bpos);
	virtual bool ccTouchBegan(CCTouch *pTouches, CCEvent *pEvent);
	void destroy();
	void setColorLayer(ccColor3B color,unsigned char opacity);

	float layerWidth;
	float layerHeight;
	CCPoint begainPos;
};


#endif