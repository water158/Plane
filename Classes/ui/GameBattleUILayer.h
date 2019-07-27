//
//  GameBattleUILayer.h
//  ambition
//
//  Created by mini01 on 14-11-26.
//
//

#ifndef __ambition__GameBattleUILayer__
#define __ambition__GameBattleUILayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalEnum.h"


USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

class GameBattleUILayer : public CCLayer
{
public:
    GameBattleUILayer();
    ~GameBattleUILayer();
    
    virtual bool init();
    
    CREATE_FUNC(GameBattleUILayer);
    
    void setHpPercent(int percent);
    
    void setWave(int nWave);
    
    void setScore(int nScore);
    
    void setSkillPercent(int percent);
    
    int getSkillPercent();
    
    void setBoxNum(int num);
    
private:
    TouchGroup* m_pUILayer;
    
    void btnPauseCallBack(CCObject* pSender,TouchEventType type);
    
};

#endif /* defined(__ambition__GameBattleUILayer__) */
