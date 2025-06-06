#include "KoopaParatroopa.h"
#include "debug.h"

void CKoopaParatroopa::OnEnable()
{
	CKoopa::OnEnable();
	SetState(KOOPA_STATE_WALKING);
	SetState(KOOPA_PARATROOPA_STATE_FLYING);
}

void CKoopaParatroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CKoopa::Update(dt, coObjects);

	DebugOut(L"KoopaParatroopa::Update: vx = %f, vy = %f, state = %d\n", vx, vy, state);
}

void CKoopaParatroopa::Render()
{
	if (state == KOOPA_PARATROOPA_STATE_FLYING) {
		auto animations = CAnimations::GetInstance();
		animations->Get(nx > 0 ?
			ID_ANI_KOOPA_PARATROOPA_FLYING_RIGHT : ID_ANI_KOOPA_PARATROOPA_FLYING_LEFT)->Render(x, y);
	}
	else
		CKoopa::Render();

	RenderBoundingBox();
}

void CKoopaParatroopa::SetState(int state)
{
	CKoopa::SetState(state);
	if (state == KOOPA_PARATROOPA_STATE_FLYING) {
		DebugOut(L"KoopaParatroopa::SetState: state = KOOPA_PARATROOPA_STATE_FLYING\n");
		vy = -KOOPA_PARATROOPA_BASE_FLYING_SPEED;
		ay /= 1.5;
	}
}

void CKoopaParatroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_PARATROOPA_STATE_FLYING) {
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2 + KOOPA_BBOX_OFFSET_Y;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else {
		CKoopa::GetBoundingBox(left, top, right, bottom);
	}
}

void CKoopaParatroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CKoopa::OnCollisionWith(e);
	if (e->ny < 0 && state == KOOPA_PARATROOPA_STATE_FLYING) {
		DebugOut(L"Collision\n");
		vy = -KOOPA_PARATROOPA_BASE_FLYING_SPEED;
		DebugOut(L"vy = %f\n", vy);
	}
}
