#include "GameData.h"
#include "MyAnimationManager.h"

GameData *_gameData = NULL;

GameData::GameData()
{
}

GameData::~GameData()
{
}

GameData* GameData::getGameData()
{
	GameData *gamedata = new GameData();
	if(gamedata && gamedata->init())
	{
		return gamedata;
	}
	return NULL;
}

bool GameData::init()
{
	return true;
}

void GameData::loadDataOnce()
{
   
}