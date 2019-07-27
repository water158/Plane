//
//  LaserBeam.h
//  UseShader
//
//  Created by renyi on 14-9-4.
//
//

#ifndef __UseShader__LaserBeam__
#define __UseShader__LaserBeam__

#include <iostream>
#include "cocos2d.h"
#include "CraftView.h"

using namespace cocos2d;

class LaserBeam : public CCSprite
{
public:
    virtual bool init();
    
    virtual void update(float delta);
    
    static LaserBeam* create(const char *pszFileName, CraftView* node1, CraftView* node2);
    
    float m_fTotalTime;
    
    int m_nTimeUniformLocation;

	//≥÷”–’ﬂ
	CraftView* m_pHero;
	CraftView* m_pEnemy;
};

#endif /* defined(__UseShader__LaserBeam__) */
