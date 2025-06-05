#pragma once
#include "debug.h"

class GameManager
{
	static GameManager* instance;
	GameManager(){}

	int coinNumber = 0;
	int score = 0;
public:
	static GameManager* GetInstance();
	void RenderUI();
	void CollectCoin() { ++coinNumber; DebugOut(L"Number of coin collected: %d\n", coinNumber); }
};

