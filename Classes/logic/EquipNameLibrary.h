//
//  EquipNameLibrary.h
//  ambition
//
//  Created by mini01 on 15-2-5.
//
//

#ifndef __ambition__EquipNameLibrary__
#define __ambition__EquipNameLibrary__

#include "cocos2d.h"

#include "tinyxml.h"

USING_NS_CC;
using namespace std;

class EquipNameLibrary
{
public:
    EquipNameLibrary();
    ~EquipNameLibrary();
    
    static EquipNameLibrary* shared();
    
    //初始化数据
    void init();
    
    CCString* getNameByKey(string key);
    
private:
    static EquipNameLibrary* s_sharedInstance;
    
    CC_SYNTHESIZE(CCDictionary*, dict_names, DictNames);
};

#endif /* defined(__ambition__EquipNameLibrary__) */
