#include "RaccoonTail.h"

void CRaccoonTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - RACCOON_TAIL_BBOX_WIDTH / 2;
	top = y - RACCOON_TAIL_BBOX_HEIGHT / 2;
	right = left + RACCOON_TAIL_BBOX_WIDTH;
	bottom = top + RACCOON_TAIL_BBOX_HEIGHT;
}

void CRaccoonTail::Render()
{
	RenderBoundingBox(1.f);
}
