#include "HardBrick.h"

void CHardBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_STATIC_BRICK)->Render(x, y);
	RenderBoundingBox();
}

void CHardBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - STATIC_BRICK_BBOX_WIDTH / 2;
	top = y - STATIC_BRICK_BBOX_HEIGHT / 2;
	right = left + STATIC_BRICK_BBOX_WIDTH;
	bottom = top + STATIC_BRICK_BBOX_HEIGHT;
}
