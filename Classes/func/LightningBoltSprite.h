

#ifndef __HelloCpp__LightningBoltSprite__
#define __HelloCpp__LightningBoltSprite__

#include <iostream>

#include "cocos2d.h"
#include "ensCommon.h"
#include "LineSegment.h"

using namespace std;
using namespace ens;
using namespace cocos2d;

class LightningBoltSprite : public CCSprite
{
    
public:
    LightningBoltSprite()
    {
        m_fSegmentLength = 14.0;
        
        m_k_offset=0.13;
        
        m_headSeg=NULL;
        m_tailSeg=NULL;
        m_program=NULL;
        m_indexVBO=NULL;
        m_isDrawDebug=false;
        m_percentageOfJitterBranch=0.5;
        m_opacityZoom=1.2;//1.2;
        m_mesh=NULL;
        
        updateGenerationTimes();
    }
    
    virtual ~LightningBoltSprite()
    {
        if(m_program)m_program->release();
        if(m_indexVBO)m_indexVBO->release();
        if(m_mesh)m_mesh->release();
    }
    
    bool init();
    
    
    void updateGenerationTimes();
    void draw();
    
    void setStartAndEnd(CCPoint start,CCPoint end);
    //    void setEndPoint(CCPoint endPointWorld);
    //    void setStartPoint(CCPoint startPointWorld);
    void genLighting();
    void doJitter();
    bool getIsDrawDebug()const{return m_isDrawDebug;}
    void setIsDrawDebug(bool value){m_isDrawDebug=value;}
    
protected:
    //----init program
    void initProgram();
    //----generate lighting
    void genSeges();
    void genMesh();
    vector<LineSegment*> generateSegmentsFromBranch(LineSegment*branchSeg, int generation);
    vector<LineSegment*> splitSeg(LineSegment*seg,int generationID);
    //----jitter
    void releaseNeedReGenBranch(vector<LineSegment*>&branchList,vector<LineSegment*>&segList);
    void releaseNeedReGenBranch_inn(LineSegment*seg,vector<LineSegment*>&segList);
    //----debug draw
    void debugDraw_segWireFrame(float lineWidth);
    void debugDraw_segLineAndDot(float lineWidth,float pointSize);
protected:
    //shader program
    CGLProgramWithUnifos *m_program;
    //core data of lightning
    CCPoint m_startPoint_parentSpace;//parent space start point
    CCPoint m_endPoint_parentSpace;//parent space end point
    CCPoint m_startPoint;//local space start point
    CCPoint m_endPoint;//local space end point
    vector<LineSegment*> m_segList;//hold seges compose the lighting
    int m_nGenerationTimes;//split how many generations
    LineSegment*m_headSeg;//the lighting tree's main branch's head seg
    LineSegment*m_tailSeg;//the lighting tree's main branch's tail seg
    //model
    Cmesh* m_mesh;
    CindexVBO* m_indexVBO;
    //settings
    float m_k_offset;
    float m_fSegmentLength;
    float m_percentageOfJitterBranch;
    float m_opacityZoom;
    bool m_isDrawDebug;
};


#endif /* defined(__HelloCpp__ensLightingSprite__) */
