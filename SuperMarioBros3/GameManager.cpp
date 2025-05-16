#include "GameManager.h"
#include "Sprites.h"

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::GetInstance()
{
	if (instance == nullptr)
		instance = new GameManager();

	return instance;
}

void GameManager::RenderUI()
{
	CSprites::GetInstance()->Get(100)->DrawOnScreen(128, 200);
}
