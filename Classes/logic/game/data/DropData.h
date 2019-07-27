//
//  DropData.h
//  ambition
//
//  Created by mini02 on 15/1/14.
//
//

#ifndef __ambition__DropData__
#define __ambition__DropData__

#include "cocos2d.h"
#include "tinyxml.h"

#include "GlobalEnum.h"

#include "ObjectData.h"
#include "ShapesData.h"
#include "CollisionsData.h"


USING_NS_CC;
using namespace std;

class DropData : public ObjectData
{
public:
    virtual ~DropData();
    
    static DropData* create(TiXmlElement *elm);
    
    virtual bool loadData(TiXmlElement *elm);
    
    //加载子弹形态数据
    void loadShapesData(TiXmlElement *elm);
    
    //加载子弹碰撞数据
    void loadCollisionsData(TiXmlElement *elm);
    
private:
    DropData();
    
    //id
    CC_SYNTHESIZE(string, m_strID, ID);
    
    //形态数据
    CC_SYNTHESIZE(ShapesData*, m_pShapesData, ShapesData);
    
    //碰撞数据
    CC_SYNTHESIZE(CollisionsData*, m_pCollisionsData, CollisionsData);

};

#endif /* defined(__ambition__DropData__) */
