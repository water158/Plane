#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include "cocos2d.h"

class GameData;
extern GameData *_gameData;

class GameData : public cocos2d::CCObject
{
public:
	GameData();
	~GameData();

	static GameData* getGameData();

    void loadDataOnce();

private:
	bool init();
	
};

#endif