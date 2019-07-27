#ifndef __FERRIS_GAME__DataHelper__
#define __FERRIS_GAME__DataHelper__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"
#include "GlobalCache.h"
#include "particle_nodes/CCParticleSystemQuad.h"

USING_NS_CC;
USING_NS_STD;

class ParticleHelper
{
public:
	static void addParticleForButton(const char* filepath, CCNode* pAimButton, CCPoint init_pos, float width, float height, bool symmetry, float offset_x, float offset_y);
};

#endif