#include "KoopaParatroopa.h"
#include "debug.h"

void CKoopaParatroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CKoopa::Update(dt, coObjects);
}

void CKoopaParatroopa::SetState(int state)
{
	CKoopa::SetState(state);
}

void CKoopaParatroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny > 0 && state == KOOPA_PARATROOPA_STATE_FLYING) {
		DebugOut(L"Collision\n");
	}
	CKoopa::OnCollisionWith(e);
}
