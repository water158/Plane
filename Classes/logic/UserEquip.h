//
//  UserEquip.h
//  ambition
//
//  Created by mini01 on 14-12-10.
//
//

#ifndef __ambition__UserEquip__
#define __ambition__UserEquip__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "tinyxml.h"
#include "FerrisTools.h"

USING_NS_CC;
USING_NS_STD;

class UserEquip
{

    //======================
    // 属性
    //======================
    
    //版本
    CC_SYNTHESIZE(int, m_nVersion, Version);
    
    UserEquip();
    
    //初始化数据
    void init();
    
    void check();
    
    //加载数据
    void load();
    
    //检测版本
    void checkVersion();
    
    //根据版本加载
    void loadDataFromVer(int version);
    
    //路径
    string m_sUserEquipPath;
    
    static UserEquip* m_sharedInstance;
    
public:
    
    static UserEquip* shared();
    
    //主炮id
    CC_SYNTHESIZE(string, m_mainGunID, MainGunID);
    
    //副炮1id
    CC_SYNTHESIZE(string, m_firSubGunID, FirSubGunID);
    
    //副炮2id
    CC_SYNTHESIZE(string, m_secSubGunID, SecSubGunID);
    
    //僚机id
    CC_SYNTHESIZE(string, m_wingID, WingID);
    
    void save();
    
    ~UserEquip();
};


#endif /* defined(__ambition__UserEquip__) */
