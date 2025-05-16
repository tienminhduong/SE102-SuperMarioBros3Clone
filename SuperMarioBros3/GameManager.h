#pragma once
class GameManager
{
	static GameManager* instance;
	GameManager(){}

	int coinNumber;
	int score;
public:
	static GameManager* GetInstance();
	void RenderUI();
};

