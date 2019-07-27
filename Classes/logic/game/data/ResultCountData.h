//
//  ResultCountData.h
//  ambition
//
//  Created by mini01 on 15-1-6.
//
//

#ifndef __ambition__ResultCountData__
#define __ambition__ResultCountData__

/****************************
 此类用作向结算界面里传入数据
 *****************************/

class ResultCountData
{
public:
    ResultCountData();
    ~ResultCountData();
    
    int m_score; //分数
    int m_wave;  //波数
    int m_gold;  //金币数
    int m_nEquip_white;   //白色装备
    int m_nEquip_green;   //绿色装备
    int m_nEquip_blue;   //蓝色装备
    int m_nEquip_puple;   //紫色装备
    int m_nEquip_orange;   //橙色装备
    int m_nBox;            //箱子数
};

#endif /* defined(__ambition__ResultCountData__) */
