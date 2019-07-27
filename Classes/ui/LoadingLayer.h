//
//  LoadingLayer.h   ø…“‘¿©’π
//  
//
//  Created on 14-4-16.
//
//

#ifndef __FERRIS_GAME__LoadingLayer__
#define __FERRIS_GAME__LoadingLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SmartRes.h"
#include "FerrisMaskLayer.h"
#include "GameDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;
using namespace std;

class LoadingLayer : public CCLayer
{
public:
	LoadingLayer();
	
    ~LoadingLayer();
    
    CREATE_FUNC(LoadingLayer);
    
    virtual bool init();
    
    void transformFinish(CCArmature *a, MovementEventType b, std::string c);
    
private:
    TouchGroup* m_pUILayer;
};

#endif
