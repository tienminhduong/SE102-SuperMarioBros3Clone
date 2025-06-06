#include "BlackPipe.h"

void CBlackPipe::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BLACK_PIPE)->Render(x, y);
}

void CBlackPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BLACK_PIPE_BBOX_WIDTH / 2;
	top = y - BLACK_PIPE_BBOX_HEIGHT / 2;
	right = left + BLACK_PIPE_BBOX_WIDTH;
	bottom = top + BLACK_PIPE_BBOX_HEIGHT;
}
