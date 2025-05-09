#include "CoinFromQuesBlock.h"

void CoinFromQuesBlock::OnEnable()
{
	y = startY;
	vy = -COIN_QUES_BLOCK_SPEED;
}

void CoinFromQuesBlock::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_COIN_FROM_QUESTION_MARK_BLOCK)->Render(x, y);
}

void CoinFromQuesBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (startY - y >= COIN_QUES_BLOCK_MAX_HEIGHT)
	{
		vy = COIN_QUES_BLOCK_SPEED;
	}
	if (y >= startY && vy > 0) {
		SetActive(false);
	}
}

void CoinFromQuesBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}
