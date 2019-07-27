//
//  AnalyticsStatID.cpp
//  plane
//
//  Created by mini01 on 14-4-16.
//
//

#include "AnalyticsStatID.h"

AnalyticsStatID::AnalyticsStatID()
{

}

AnalyticsStatID::~AnalyticsStatID()
{

}

const char* AnalyticsStatID::getMissionID(int missionID)
{
    return CCString::createWithFormat("level_%d",missionID)->getCString();
}

const char* AnalyticsStatID::getSMissionID(int missionID)
{
    return CCString::createWithFormat("Slevel_%d",missionID)->getCString();
}

const char* AnalyticsStatID::getHeroUpgradeUseCoin(int type, int level)
{
    return CCString::createWithFormat("C_hero_%d_%d",type,level)->getCString();
}

const char* AnalyticsStatID::getHeroUpgradeUseGem(int type, int level)
{
    return CCString::createWithFormat("D_hero_%d_%d",type,level)->getCString();
}

const char* AnalyticsStatID::getWingUpgtadeUseCoin(int type, int level)
{
    return CCString::createWithFormat("C_wing_%d_%d",type,level)->getCString();
}

const char* AnalyticsStatID::getWingUpgradeUseGem(int type, int level)
{
    return CCString::createWithFormat("D_wing_%d_%d",type,level)->getCString();
}

const char* AnalyticsStatID::getUnlockHero(int type)
{
    return CCString::createWithFormat("unlock_hero_%d",type)->getCString();
}

const char* AnalyticsStatID::getUnlockWing(int type)
{
    return CCString::createWithFormat("unlock_wing_%d",type)->getCString();
}

const char* AnalyticsStatID::getToolsID(int toolType)
{
    return CCString::createWithFormat("tools_%d",toolType)->getCString();
}


const char* AnalyticsStatID::getReBornID()
{
    return CCString::createWithFormat("%s","reborn")->getCString();
}

const char* AnalyticsStatID::getAttributes50()
{
    return CCString::createWithFormat("%s","Attributes50")->getCString();
}

const char* AnalyticsStatID::getCoinID(int coinNum)
{
    return CCString::createWithFormat("coin_%d",coinNum)->getCString();
}

const char* AnalyticsStatID::getGemID(int gemNum)
{
    return CCString::createWithFormat("gem_%d",gemNum)->getCString();
}

const char* AnalyticsStatID::getPerDoubleCoin()
{
    return CCString::createWithFormat("%s","doubleCoin")->getCString();
}

const char* AnalyticsStatID::getMagnet()
{
    return CCString::createWithFormat("%s","magnet")->getCString();
}

const char* AnalyticsStatID::getHeroSmallGift()
{
    return CCString::createWithFormat("%s","hero_small_gift")->getCString();
}

const char* AnalyticsStatID::getTripleCoin()
{
    return CCString::createWithFormat("%s","tripleCoin")->getCString();
}

const char* AnalyticsStatID::getHeroBigGift()
{
    return CCString::createWithFormat("%s","hero_big_gift")->getCString();
}

const char* AnalyticsStatID::getWingGift()
{
    return CCString::createWithFormat("%s","wing_big_gift")->getCString();
}

const char* AnalyticsStatID::getToolTopLimit()
{
    return CCString::createWithFormat("%s","toolTopLimit")->getCString();
}