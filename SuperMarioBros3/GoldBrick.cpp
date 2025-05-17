#include "GoldBrick.h"

void GoldBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_GOLD_BRICK)->Render(x, y);
	RenderBoundingBox();
}

void GoldBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GOLD_BRICK_BBOX_WIDTH / 2;
	top = y - GOLD_BRICK_BBOX_HEIGHT / 2;
	right = left + GOLD_BRICK_BBOX_WIDTH;
	bottom = top + GOLD_BRICK_BBOX_HEIGHT;
}

