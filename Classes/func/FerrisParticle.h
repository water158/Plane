#ifndef __FERRIS_GAME__FerrisParticle__
#define __FERRIS_GAME__FerrisParticle__
#include "cocos2d.h"
USING_NS_CC;
/*
			2013 11 13
			对于粒子的试用就是保证播放完之后的释放问题
			非循环播放的粒子会自动释放掉，通过获取粒子数量来释放粒子
			对于循环播放的粒子直接手动释放掉
*/

class FerrisParticle : public CCParticleSystemQuad
{
public:
	FerrisParticle();
	virtual ~FerrisParticle();
    
    static FerrisParticle * create(const char *plistFile);

	virtual bool initWithFile(const char *plistFile);

    virtual bool initWithDictionary(CCDictionary *dictionary, const char *dirname);

};

#endif