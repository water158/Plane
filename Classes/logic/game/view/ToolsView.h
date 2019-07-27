//
//  ToolsView.h
//  ambition
//
//  Created by mini02 on 14/11/19.
//
//

#ifndef __ambition__ToolsView__
#define __ambition__ToolsView__

#include <stdio.h>

#include "EnemyView.h"
#include "JumpByAction.h"
#include "MagnetCoinAction.h"
#include "GlobalTagZOrder.h"

USING_NS_CC;

class HeroView;
class ToolsView : public EnemyView
{
public:
    ToolsView();
    ~ToolsView();
    
    //创建 isHave表示创建拥有的道具还是还没吃到的道具
    static ToolsView* create(CraftData* dataCraft, TargetPlaneData* dataTargetPlane,bool isHave);
    
    //道具位置定时器
    void updatePosition(float dt);
    
    //随机位置定时器
    void updateRandomPosition(float dt);
    
    //创建粒子
    void createToolParticle();
    
    void startUpdateFlyToolPosition();
    void updateFlyToolPosition(float dt);
    
    void updateMagnetAction(float dt);
    void startUpdateMagnet();
    
private:
    bool init(CraftData* data, TargetPlaneData* dataTargetPlane,bool isHave);

    //道具类型
    CC_SYNTHESIZE(int, toolType, ToolType);
    
    //主机的位置
    CC_SYNTHESIZE(CCPoint, m_posOffset, PosOffset);
    
    //x轴速度
    CC_SYNTHESIZE(int, m_nVx, SpeedVX);
    
    //y轴速度
    CC_SYNTHESIZE(int, m_nVy, SpeedVY);
    
    //是否与与主机检测框碰撞
    CC_SYNTHESIZE(bool, m_bIsCollisionWithHero, IsCollisionWithHero);
    
    //主机对象
    CC_SYNTHESIZE(HeroView*, heroTmp, HeroTmp);
    
    //是否开始移动到新位置
    CC_SYNTHESIZE(bool, isMovoToNewPos, IsMovoToNewPos);
    
    //位置type
    CC_SYNTHESIZE(int, m_posTyps, PosType);

};

#endif /* defined(__ambition__ToolsView__) */
