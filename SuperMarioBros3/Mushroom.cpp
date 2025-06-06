#include "Mushroom.h"
#include "debug.h"


int CMushroom::GetAniId(int state)
{
	if (state == MUSHROOM_STATE_RAISING)
		return ID_ANI_MUSHROOM;
	return ID_ANI_MUSHROOM_COMPLETE;
}

CMushroom::CMushroom(float x, float y) : CGameObject(x, y)
{
	OnEnable();
}

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == MUSHROOM_STATE_RAISING)
		animations->Get(GetAniId(this->state))->Render(x, y);
	else if (state == MUSHROOM_STATE_MOVING)
		animations->Get(GetAniId(this->state))->Render(x, y);
}

void CMushroom::OnEnable()
{
	SetState(MUSHROOM_STATE_RAISING);
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_RAISING:
		vx = vy = 0.f;
		break;
	case MUSHROOM_STATE_MOVING:
		vx = MUSHROOM_VX;
		vy = MUSHROOM_VY;
	}
}

void CMushroom::SetDirection(int direction)
{
	this->nx = direction;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"Current state: %d, vx: %f, vy: %f, nx: %d\n", state, vx, vy, nx);
	auto animation = CAnimations::GetInstance()->Get(GetAniId(MUSHROOM_STATE_RAISING));
	if (animation->IsOver())
		SetState(MUSHROOM_STATE_MOVING);

	vx = abs(vx) * nx;
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2.f;
	t = y - MUSHROOM_BBOX_HEIGHT / 2.f;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking() && e->nx != 0 && e->ny == 0)
		nx = -nx;
	if (e->obj->IsBlocking() && e->ny != 0)
		vy = 0;
}
