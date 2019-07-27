//
//  ScoreView.cpp
//  ambition
//
//  Created by mini02 on 15/1/13.
//
//

#include "ScoreView.h"

ScoreView::ScoreView()
{

}
ScoreView::~ScoreView()
{
}

ScoreView* ScoreView::create(DropData* data)
{
    ScoreView* score = new ScoreView();
    if (score && score->init(data))
    {
        score->autorelease();
        return score;
    }

    return NULL;
}


bool ScoreView::init(DropData* data)
{
    DropView::init(data);
    
    //this->scheduleUpdate();
    
    this->runAction(CCSequence::create(CCDelayTime::create(3.0f),CCFadeOut::create(1.0f),CCCallFuncN::create(this, callfuncN_selector(ScoreView::callbackRemoveFormParent)),NULL));
    
    return true;
}

void ScoreView::update(float dt)
{
    
}

void ScoreView::callbackRemoveFormParent()
{
    this->setIsDied(true);
    this->removeAllChildrenWithCleanup(true);
}
