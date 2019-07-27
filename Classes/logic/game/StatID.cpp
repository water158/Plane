//
//  StatID.cpp
//  plane
//
//  Created by mini01 on 14-4-16.
//
//

#include "StatID.h"

StatID::StatID()
{

}

StatID::~StatID()
{

}

const char* StatID::getMissionID(int missionID)
{
    return CCString::createWithFormat("level_%d",missionID)->getCString();
}

const char* StatID::getSMissionID(int missionID)
{
    return CCString::createWithFormat("Slevel_%d",missionID)->getCString();
}

const char* StatID::getHeroUpgradeUseCoin(int type, int level)
{
    return CCString::createWithFormat("C_hero_%d_%d",type,level)->getCString();
}

const char* StatID::getHeroUpgradeUseCrystal(int type, int level)
{
    return CCString::createWithFormat("D_hero_%d_%d",type,level)->getCString();
}

const char* StatID::getWingUpgtadeUseCoin(int type, int level)
{
    return CCString::createWithFormat("C_wing_%d_%d",type,level)->getCString();
}

const char* StatID::getWingUpgradeUseCryStal(int type, int level)
{
    return CCString::createWithFormat("D_wing_%d_%d",type,level)->getCString();
}

const char* StatID::getUnlockHero(int type)
{
    return CCString::createWithFormat("unlock_hero_%d",type)->getCString();
}

const char* StatID::getUnlockWing(int type)
{
    return CCString::createWithFormat("unlock_wing_%d",type)->getCString();
}

const char* StatID::getToolsID(int toolType)
{
    return CCString::createWithFormat("tools_%d",toolType)->getCString();
}


const char* StatID::getReBornID()
{
    return CCString::createWithFormat("%s","reborn")->getCString();
}

const char* StatID::getAttributes50()
{
    return CCString::createWithFormat("%s","Attributes50")->getCString();
}

const char* StatID::getCoinID(int coinNum)
{
    return CCString::createWithFormat("coin_%d",coinNum)->getCString();
}

const char* StatID::getCrystalID(int crystalNum)
{
    return CCString::createWithFormat("crystal_%d",crystalNum)->getCString();
}

const char* StatID::getPerDoubleCoin()
{
    return CCString::createWithFormat("%s","doubleCoin")->getCString();
}

const char* StatID::getMagnet()
{
    return CCString::createWithFormat("%s","magnet")->getCString();
}

const char* StatID::getHeroSmallGift()
{
    return CCString::createWithFormat("%s","hero_small_gift")->getCString();
}

const char* StatID::getTripleCoin()
{
    return CCString::createWithFormat("%s","tripleCoin")->getCString();
}

const char* StatID::getHeroBigGift()
{
    return CCString::createWithFormat("%s","hero_big_gift")->getCString();
}

const char* StatID::getWingGift()
{
    return CCString::createWithFormat("%s","wing_big_gift")->getCString();
}

const char* StatID::getToolTopLimit()
{
    return CCString::createWithFormat("%s","toolTopLimit")->getCString();
}