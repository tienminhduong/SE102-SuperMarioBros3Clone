#include "GameManager.h"
#include "Sprites.h"
#include "Game.h"
#include "PlayScene.h"

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

void GameManager::GoToHiddenMap()
{
	CMario* mario = (CMario*)((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer();
	mario->SetLevel(MARIO_LEVEL_RACCOON, true);
}

void GameManager::GoBackFromHiddenMap()
{
	CMario* mario = (CMario*)((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer();
	mario->SetLevel(MARIO_LEVEL_RACCOON, true);

	mario->SetGoToOtherMap(-1);
	mario->SetPosition(PIPE_GO_OUT_X, PIPE_GO_OUT_Y);
}
