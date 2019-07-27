//
//  LaserLayer.h
//  ambition
//
//  Created by mini01 on 15-2-12.
//
//

#ifndef __ambition__LaserLayer__
#define __ambition__LaserLayer__

#include "cocos2d.h"
#include "GlobalDelegate.h"
#include "GameCombatLayer.h"
#include "lightning.h"
#include "DynamicLaser.h"

USING_NS_CC;

#define TAG_SMALL_LASER 2000
#define TAG_LAYSER 500

#define TAG_SHOULDER_LEFT 400   //左肩炮激光
#define TAG_SHOULDER_RIGHT 450  //右肩炮激光

#define TAG_PINION_LEFT 600  //左翼炮激光
#define TAG_PINION_RIGHT 650 //右翼炮激光

#define TAG_WING_LASER  800  //僚机激光

#define TAG_MAIN_LEFTLASER 900 //主炮左激光

#define TAG_MAIN_RIGHTLASER 1000  //主炮右激光

#define TAG_MAINLASER 1100  //主炮激光

#define LINE_LASER_WIDTH_SCALE 0.15  //激光宽度缩放


#define SHOOT_SHOULDER_LEFT  1200   //左肩炮激光发射效果

#define SHOOT_SHOULDER_RIGHT 1250   //右肩炮激光发射效果

#define SHOOT_PINION_LEFT 1300    //左翼炮激光发射效果

#define SHOOT_PINION_RIGHT 1350   //右翼炮激光发射效果

#define SHOOT_MAIN_LEFT  1400   //左主炮激光发射效果

#define SHOOT_MAIN_RIGHT  1450   //右主炮激光发射效果

#define SHOOT_MAIN_LASER  1500   //主炮激光发射效果

#define HIT_LINE_LASER 1550   //线型激光击中效果

#define HIT_SHADER_LASER 1600  //shader激光击中效果

#define HIT_IMAGE_LASER 1650  //图片型激光击中效果


class LayserLayer : public CCLayer,public LaserDelegate
{
public:
    bool init();
    
    LayserLayer(GameCombatLayer* parent);
    
    ~LayserLayer();
    
    virtual void updateLaser(float dt);
    
    
    //根据激光的类型 和tag值来更新 激光
    void updateLaserBy(LaserType type,int tag,CraftView* craft);
    
    //创建图片型激光
    void createImageLaser(int tag);
    
    //设置僚机激光是否可见
    void setWingLaserVisible(bool visible);
    
    //设置肩炮激光是否可见
    void setShoulderLaserVisible(bool visible);
    
    //设置翼炮激光是否可见
    void setPinionLaserVisible(bool visible);
    
    //设置主机激光是否可见
    void setMainLaserVisible(bool visible);
    
private:
    GameCombatLayer *m_pParent;
    
    bool m_bIsCross;   //激光是否与敌机相交
    
    LaserType m_mainLaserType;   //主炮激光类型
    
    LaserType m_shoulderLaserType;   //肩炮激光类型
    
    LaserType m_pinionLaserType;    //翼炮激光类型
    
    LaserType m_wingLaserType;    //僚机激光类型
    
    bool _bMainLaserIsVisible;    //主机激光是否可见
    
    bool _bShoulderLaserIsVisible;  //肩炮激光是否可见
    
    bool _bPinionLaserIsVisible;   //翼炮激光是否可见
    
    bool _bWingLaserIsVisible;    //僚机激光是否可见
    
};

#endif /* defined(__ambition__LaserLayer__) */
