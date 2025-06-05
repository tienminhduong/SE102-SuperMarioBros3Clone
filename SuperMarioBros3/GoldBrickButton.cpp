#include "GoldBrickButton.h"

void CGoldBrickButton::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == GOLD_BRICK_BTN_STATE_ACTIVE)
		animations->Get(ID_ANI_GOLD_BRICK_BTN_ACTIVE)->Render(x, y);
	else if (state == GOLD_BRICK_BTN_STATE_INACTIVE)
		animations->Get(ID_ANI_GOLD_BRICK_BTN_INACTIVE)->Render(x, y);
}

void CGoldBrickButton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOLD_BRICK_BTN_STATE_INACTIVE)
	{
		left = top = right = bottom = 0;
		return;
	}

	left = x - GOLD_BRICK_BTN_BBOX_WIDTH / 2;
	top = y - GOLD_BRICK_BTN_BBOX_HEIGHT / 2;
	right = left + GOLD_BRICK_BTN_BBOX_WIDTH;
	bottom = top + GOLD_BRICK_BTN_BBOX_HEIGHT;
}

void CGoldBrickButton::TriggerOnCollision()
{
	SetState(GOLD_BRICK_BTN_STATE_INACTIVE);
}