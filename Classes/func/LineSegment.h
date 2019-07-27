//
//  LineSegment.h
//  HelloCpp
//
//  Created by renyi on 14/12/12.
//
//

#ifndef __HelloCpp__LineSegment__
#define __HelloCpp__LineSegment__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class LineSegment
{
    
public:
    
    //start point
    CC_SYNTHESIZE(CCPoint, m_oStartPoint, StartPoint);
    
    //end point
    CC_SYNTHESIZE(CCPoint, m_oEndPoint, EndPoint);
    
    //whether the branch use this seg as root seg need to regenerate
    CC_SYNTHESIZE(bool, m_bNeedRegeneration, NeedRegeneration);
    
    //only seed seg store this
    CC_SYNTHESIZE(CCPoint, m_oBranchEndPoint, BranchEndPoint);
    
    //which branch the seg belongs to. branchID==0 means belongs to mainBranch
    int branchID;
    
    //in which generation the branch this seg belongs to start generated.
    int branchStartGenerationID;
    
    //this seg created in which generation, the original seg's generationID is 0
    int generationID;
    
    //father
    LineSegment* father;
    
    //children
    vector<LineSegment*> children;
    
    
    
public:
    
    /***************
     * Basic
     ***************/
    LineSegment()
    {
        initParameter();
    }
    
    LineSegment(CCPoint start,CCPoint end)
    {
        initParameter();//do not forget this
        init(start, end);
    }
    
    virtual~LineSegment()
    {
    }
    
    void init(CCPoint start_point, CCPoint end_point);
    
    /***************
     * Children
     ***************/
    bool addChildRef(LineSegment*child);
    bool removeChildRef(LineSegment*child);
    
    /***************
     * Method
     ***************/
    float getLen()const;
    CCPoint getDirection() const;
    CCPoint getVector() const;
    CCPoint getRightNormal() const;
    CCPoint getMidPoint() const;
    
protected:
    void initParameter()
    {
        branchID = -1;
        branchStartGenerationID = -1;
        generationID = -1;
        
        father = NULL;
        m_bNeedRegeneration = false;
    }
};


#endif /* defined(__HelloCpp__LineSegment__) */
