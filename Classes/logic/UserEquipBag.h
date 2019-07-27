//
//  UserEquipBag.h
//  ambition
//
//  Created by mini01 on 14-12-10.
//
//

#ifndef __ambition__UserEquipBag__
#define __ambition__UserEquipBag__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "tinyxml.h"
#include "EquipItem.h"
#include "FerrisTools.h"
#include "UserEquip.h"

USING_NS_CC;
USING_NS_STD;

class UserEquipBag
{
    
    //======================
    // 属性
    //======================
    
    //版本
    CC_SYNTHESIZE(int, m_nVersion, Version);
    
public:
    
    UserEquipBag();
    ~UserEquipBag();
    
    static UserEquipBag* shared();
    
    //初始化数据
    void init();
    
    void check();
    
    //保存数据
    void save();
    
    //加载
    void load();
    
    //检测userdata版本
    void checkVersion();
    
    //根据userdata版本加载
    void loadDataFromVer(int version);
    
    //路径
    string m_sUserDataPath;
    
    //根据id删除装备
    void deletEquipByID(string equip_id);
    
    //添加装备
    void addEquip(EquipItem* item);
    
    //根据id获取装备
    EquipItem* getEquipById(string id);
    
    //安装装备
    void useEquip(EquipItem* item);
    
    //卸下装备
    void unUseEquip(EquipItem* item);
    
    //交换装备位置
    void swapEquip(EquipItem* item1,EquipItem* item2);
    
    //装备到指定位置
    void dressEquipToPos(string equip_id,int pos);
    
    //根据装备id交换装备位置
    void swapEquip(string equip1,string equip2);
    
    //重置用户装备信息
    void resetUserEquip();
    
    //获取最小id
    int getMinID();
    
    //获取装备字典
    CCDictionary* getDicEquip();
    
    EquipItem* getWingEquip();
    
    EquipItem* getMainEquip();
    
    EquipItem* getFirSubEquip();
    
    EquipItem* getSecSubEquip();
    
    /****************获取玩家所有装备属性值*****************/
    //获取生命值
    int getTotalHp();
    //获取攻击值
    int getTotalAttack();
    //获取血量掉率
    int getTotalDropBlood();
    //获取道具掉率
    int getTotalDropTool();
    //获取攻击加成
    int getTotalAttackAdd();
    //获取血量加成
    int getTotalHpAdd();
    
    //获取装备的图标路径
    string getIconPath(EquipItem* item);
    
    /**************  排序用 ****************/
    
    void quick_sortArr(CCArray* arr,int low,int high);
    
    void pupple_sortArr(CCArray* arr);
    
    CCArray* getEquipWhiteArr();
    CCArray* getEquipGreenArr();
    CCArray* getEquipBlueArr();
    CCArray* getEquipPupleArr();
    CCArray* getEquipOrangeArr();
    
    //刚刚获得的装备 用于 不排序用
    CC_SYNTHESIZE(CCArray*, m_arrEquipGetRecent, ArrEquipGetRecent);
    
    
    
    void test_quick(int a[],int low,int high);
    
private:
    static UserEquipBag* s_sharedUserEquipBag;
    
    CCDictionary* m_dictEquipBag;
};



#endif /* defined(__ambition__UserEquipBag__) */
