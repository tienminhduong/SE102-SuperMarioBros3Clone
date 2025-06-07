#include "GetCardItemEffect.h"

void GetCardItemEffect::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_GET_CARD_ITEM_EFFECT)->Render(x, y);
}

void GetCardItemEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}

void GetCardItemEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y -= CARD_ITEM_EFFECT_SPEED * dt;
}
