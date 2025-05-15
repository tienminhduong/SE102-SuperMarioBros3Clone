#include "TransformMushroom.h"
#include "debug.h"

CTransformMushroom::CTransformMushroom(float x, float y) : CGameObject(x, y)
{
	OnEnable();
}

void CTransformMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == TRANSFORM_MUSHROOM_STATE_RAISING)
		animations->Get(ID_ANI_TRANSFORM_MUSHROOM)->Render(x, y);
	else if (state == TRANSFORM_MUSHROOM_STATE_MOVING)
		animations->Get(ID_ANI_TRANSFORM_MUSHROOM_COMPLETE)->Render(x, y);
}

void CTransformMushroom::OnEnable()
{
	SetState(TRANSFORM_MUSHROOM_STATE_RAISING);
}

void CTransformMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TRANSFORM_MUSHROOM_STATE_RAISING:
		vx = vy = 0.f;
		break;
	case TRANSFORM_MUSHROOM_STATE_MOVING:
		vx = TRANSFORM_MUSHROOM_VX;
		vy = TRANSFORM_MUSHROOM_VY;
	}
}

void CTransformMushroom::SetDirection(int direction)
{
	this->nx = direction;
}

void CTransformMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"Current state: %d, vx: %f, vy: %f, nx: %d\n", state, vx, vy, nx);
	auto animation = CAnimations::GetInstance()->Get(ID_ANI_TRANSFORM_MUSHROOM);
	if (animation->IsOver())
		SetState(TRANSFORM_MUSHROOM_STATE_MOVING);

	vx = abs(vx) * nx;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	
}

void CTransformMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TRANSFORM_MUSHROOM_BBOX_WIDTH / 2.f;
	t = y - TRANSFORM_MUSHROOM_BBOX_HEIGHT / 2.f;
	r = l + TRANSFORM_MUSHROOM_BBOX_WIDTH;
	b = t + TRANSFORM_MUSHROOM_BBOX_HEIGHT;
}

void CTransformMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CTransformMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking() && e->nx != 0 && e->ny == 0)
		nx = -nx;
	if (e->obj->IsBlocking() && e->ny != 0)
		vy = 0;
}

void CTransformMushroom::OnExitCamera()
{
	SetActive(false);
}
