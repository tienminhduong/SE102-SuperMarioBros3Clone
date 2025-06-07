#include "CardEndGame.h"
#include "GameManager.h"
#include "GetCardItemEffect.h"
#include "PlayScene.h"

void CardEndGame::Render()
{
	if (state == CARD_END_GAME_STATE_NORMAL)
		CAnimations::GetInstance()->Get(ID_ANI_CARD_END_GAME)->Render(x, y);
	else
		CAnimations::GetInstance()->Get(ID_ANI_CARD_END_GAME_CLAIMED)->Render(x, y);
}

void CardEndGame::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CARD_END_GAME_BBOX_WIDTH / 2;
	top = y - CARD_END_GAME_BBOX_HEIGHT / 2;
	right = left + CARD_END_GAME_BBOX_WIDTH;
	bottom = top + CARD_END_GAME_BBOX_HEIGHT;
}

void CardEndGame::TriggerOnCollision() {
	GameManager::GetInstance()->WinGame();
	SetState(CARD_END_GAME_STATE_CLAIMED);

	GetCardItemEffect* obj = new GetCardItemEffect(x, y);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(obj);
}