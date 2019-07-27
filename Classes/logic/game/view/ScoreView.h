//
//  ScoreView.h
//  ambition
//
//  Created by mini02 on 15/1/13.
//
//

#ifndef __ambition__ScoreView__
#define __ambition__ScoreView__

#include "cocos2d.h"

#include "DropView.h"

USING_NS_CC;

class ScoreView : public DropView
{
public:
    ScoreView();
    ~ScoreView();
    
    static ScoreView* create(DropData* data);
    
    virtual void update(float dt);
    
    void callbackRemoveFormParent();

private:
    virtual bool init(DropData* data);

};

#endif /* defined(__ambition__ScoreView__) */
