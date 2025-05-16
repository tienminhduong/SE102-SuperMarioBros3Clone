#include "Koopa.h"
#include "debug.h"

int Koopa::GetAniId(int defaultIdAni)
{
	int aniId = -1;
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		aniId = nx > 0 ? ID_ANI_KOOPA_WALK_RIGHT : ID_ANI_KOOPA_WALK_LEFT;
		break;
	default:
		break;
	}

	return defaultIdAni - KOOPA_START_ANI_ID + aniId;
}

Koopa::Koopa(float x, float y)
	: CRespawnableEnemy(x, y)
{
	SetState(KOOPA_STATE_WALKING);
}

void Koopa::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = GetAniId(KOOPA_START_ANI_ID);
	animations->Get(aniId)->Render(x, y);
}

void Koopa::OnEnable()
{
	SetState(KOOPA_STATE_WALKING);
}

void Koopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != ENEMY_STATE_KICKED)
	{
		vx += ax * dt;
		vy += ay * dt;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Koopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Koopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
		vy = 0;
	else if (e->nx != 0)
		vx = -vx, nx = -nx;
}

void Koopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2 + KOOPA_BBOX_OFFSET_Y;
	right = left + KOOPA_BBOX_WIDTH;
	bottom = top + KOOPA_BBOX_HEIGHT;
}

void Koopa::SetState(int state)
{
	CRespawnableEnemy::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		vx = nx * KOOPA_WALKING_SPEED;
		break;
	default:
		break;
	}
}
