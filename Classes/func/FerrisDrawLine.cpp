//
//  FerrisDrawLine.cpp
//  ambition
//
//  Created by mini02 on 15/2/4.
//
//

#include "FerrisDrawLine.h"

FerrisDrawLine* FerrisDrawLine::create(CCPoint start,CCPoint end)
{
    FerrisDrawLine* line = new FerrisDrawLine();
    if (line && line->init(start, end))
    {
        line->autorelease();
        return line;
    }

    return NULL;
}

bool FerrisDrawLine::init(CCPoint start,CCPoint end)
{
    if (!CCLayer::init()) return false;
    
    this->setStartLinePoint(start);
    this->setEndLinePoint(end);
    
    return true;
}

void FerrisDrawLine::draw()
{
    glLineWidth( 5.0f );
    ccDrawColor4F(255,255,255,255);
    ccDrawLine(m_startLinePoint,m_endLinePoint);
}

