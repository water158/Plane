#ifndef _ANIMATIONMANAGER_H_
#define _ANIMATIONMANAGER_H_

/*
			2013 11 12
			现在主要是统一加载动画和动画图片资源
			相关的管理动画的函数正在进一步完善
			这个类主要用于管理所有动画，现在只是通过配置相关xml文件，导入所有动画，添加动画一定要记着修改
			xml文件并附上相关图片和plist文件
*/

#include "MySingleton.h"
#include "tinyxml.h"
#include "cocos2d.h"
using namespace cocos2d;


class AnimationManager : public MySingleton<AnimationManager>
{
public:
	AnimationManager();
	~AnimationManager();
	/*通过配置xml文件，导入游戏中的所有动画到动画缓存*/
	void getAnimationByXML(const char* xmlfile);

	//通过节点以及ID加载相应的资源
	void preLoadResByID(TiXmlElement *xml_ele);
	/*设置动画到某一帧停止*/
	CCSpriteFrame* getAniFrameByIndex(const char* anitag,unsigned int index); //从0开始

	void freeAnimation(const char* ani_name);
	void freeAllAnimations();

private:
	TiXmlDocument *aniList;    //动画列表
};
#define sAnimationManager AnimationManager::instance()

#endif