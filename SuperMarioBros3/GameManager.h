#pragma once
#include "debug.h"
#include "Mario.h"

#define PIPE_GO_OUT_X 2373
#define PIPE_GO_OUT_Y 150

#define MAX_TIME 300000

class GameManager
{
	static GameManager* instance;
	GameManager(){}

	int coinNumber = 0;
	int score = 0;
	int frameCount = 0;
	int timer = MAX_TIME;
	int life = 3;
public:
	bool isGamePaused = false;
	bool isGameOver = false;
	bool isWinning = false;
	bool displayWinningText = false;
	bool isStopped = false;
public:
	static GameManager* GetInstance();
	void RenderUI();
	void CollectCoin() { ++coinNumber; }
	void IncreasePoint(int score = 100) { this->score += score; }

	void GoToHiddenMap();
	void GoBackFromHiddenMap();
	int GetPipeGoOutX() { return PIPE_GO_OUT_X; }
	int GetPipeGoOutY() { return PIPE_GO_OUT_Y; }
	void Update(DWORD dt);
	void IncreaseLife() { ++life; }
	void EndGame();
	void WinGame();
};

