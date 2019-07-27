#ifndef _SpriteAnimationNew_H_
#define _SpriteAnimationNew_H_

#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class SpriteAnimationNew : public CCSprite
{
public:
	virtual ~SpriteAnimationNew();
	static SpriteAnimationNew *create(const char *animation_name, int loop = -1, float delay = 0.1f,CCActionInterval* action = NULL);

    //不用父类的函数，使用本类的函数，其实是设置精灵的属性
	//virtual void setPosition(const CCPoint &position);
	//virtual void setAnchorPoint(const CCPoint& anchorPoint);
	//virtual void SetOpacity(GLubyte opacity);
	virtual void SetColor(const ccColor3B& color);
    
    //播放动画
	void playAnimation(int loop ,float delaytime = 1.0f,CCActionInterval* action = NULL);
    
    //设置到动画某帧
	void setFrameByIndex(const char *ani_name,unsigned int index);
    
    //停止动画
	void stopAnimation();
	
    //恢复动画
    void resumeAnimation();
    
    void callbackCleanUP();

protected:
	SpriteAnimationNew();
	bool init(const char *animation_name, int loop, float delay,CCActionInterval* action);

private:
	//CCSprite * mysprite;
    string pre;
};

#endif