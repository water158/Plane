#ifndef _LIGHTNING_H_
#define _LIGHTNING_H_

#include "cocos2d.h"

USING_NS_CC;

class lightning : public CCLayer
{
public :
	lightning();
	~lightning();
	static lightning* create();
	bool init();
	void setStartAndEnd(CCPoint start,CCPoint end);
	void setVisible(bool visible);
	void setBreadth(float size);
	void setBminance(float uminance);
	CCSprite *sp_instro;
	virtual void update(float delta);

	CCPoint start;
	CCPoint end;
	
};


#endif