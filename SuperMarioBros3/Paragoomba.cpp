#include "Paragoomba.h"
#include "PlayScene.h"

void CParagoomba::Render()
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

void CParagoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CParagoomba::FindMario()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	LPGAMEOBJECT mario = scene->GetPlayer();
	if (mario != NULL)
	{
		float marioX, marioY;
		mario->GetPosition(marioX, marioY);
		nx = marioX > x ? 1 : -1;
		vx = nx * GOOMBA_WALKING_SPEED;
	}
	else {
		DebugOut(L"[PARAGOOMBA] Mario not found!\n");
	}
}

CParagoomba::CParagoomba(float x, float y) : CGoomba(x, y)
{
	OnEnable();
}

void CParagoomba::OnEnable()
{
	CGoomba::OnEnable();
	SetState(PARAGOOMBA_STATE_WALK);
}

void CParagoomba::SetState(int state)
{
	CGoomba::SetState(state);

	//get mario

	switch (state)
	{
	case PARAGOOMBA_STATE_WALK:
		changeStateDuration = CAnimations::GetInstance()->Get(ID_ANI_PARAGOOMBA_WALKING)->GetDuration();
		vy = 0;
		FindMario();
		break;
	case PARAGOOMBA_STATE_JUMP:
		jumpCount = 0;
		vy = -PARAGOOMBA_JUMP_SPEED;
		break;
	case PARAGOOMBA_STATE_FLY:
		vy = -PARAGOOMBA_FLY_SPEED;
		break;
	case PARAGOOMBA_STATE_LOST_WINGS:
		changeStateDuration = 0;
		break;
	default:
		break;
	}
}

void CParagoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGoomba::Update(dt, coObjects);
	if (changeStateDuration > 0 && state == PARAGOOMBA_STATE_WALK)
	{
		changeStateDuration -= dt;
		if (changeStateDuration <= 0)
			SetState(PARAGOOMBA_STATE_JUMP);
	}
}

void CParagoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CGoomba::OnCollisionWith(e);
	if (e->ny < 0)
	{
		if (state == PARAGOOMBA_STATE_FLY)
			SetState(PARAGOOMBA_STATE_WALK);
		else if (state == PARAGOOMBA_STATE_JUMP)
		{
			jumpCount++;
			if (jumpCount == PARAGOOMBA_JUMP_COUNT)
				SetState(PARAGOOMBA_STATE_FLY);
			else
				vy = -PARAGOOMBA_JUMP_SPEED;
		}
	}
}
