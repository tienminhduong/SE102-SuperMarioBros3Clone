#pragma once
#include "debug.h"
#include "Mario.h"

#define PIPE_GO_OUT_X 2373
#define PIPE_GO_OUT_Y 150

class GameManager
{
	static GameManager* instance;
	GameManager(){}

	int coinNumber = 0;
	int score = 0;
public:
	bool isGamePaused = false;

public:
	static GameManager* GetInstance();
	void RenderUI();
	void CollectCoin() { ++coinNumber; DebugOut(L"Number of coin collected: %d\n", coinNumber); }

	void GoToHiddenMap();
	void GoBackFromHiddenMap();
	int GetPipeGoOutX() { return PIPE_GO_OUT_X; }
	int GetPipeGoOutY() { return PIPE_GO_OUT_Y; }
};

