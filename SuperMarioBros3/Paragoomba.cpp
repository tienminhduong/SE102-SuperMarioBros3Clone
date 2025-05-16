#include "Paragoomba.h"

void Paragoomba::Render()
{
	int aniId = ID_ANI_PARAGOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
		aniId = ID_ANI_PARAGOOMBA_DIE;
	else if (state == ENEMY_STATE_KICKED)
		aniId = ID_ANI_PARAGOOMBA_KICKED;
	else if (state == PARAGOOMBA_STATE_JUMP)
		aniId = ID_ANI_PARAGOOMBA_JUMPING;
	else if (state == PARAGOOMBA_STATE_LOST_WINGS)
		aniId = ID_ANI_PARAGOOMBA_NORMAL;
	else if (state == PARAGOOMBA_STATE_FLY)
		aniId = ID_ANI_PARAGOOMBA_JUMPING;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void Paragoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE || state == ENEMY_STATE_KICKED)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;

		if (state != PARAGOOMBA_STATE_LOST_WINGS && state != GOOMBA_STATE_DIE)
			top += PARAGOOMBA_BBOX_OFFSET_Y, bottom += PARAGOOMBA_BBOX_OFFSET_Y;
	}

}

void Paragoomba::SetState(int state)
{
	CGoomba::SetState(state);
}
