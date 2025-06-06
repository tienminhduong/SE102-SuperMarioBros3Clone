#pragma once
#include "Koopa.h"

#define KOOPA_PARATROOPA_STATE_FLYING 10
#define KOOPA_PARATROOPA_BASE_FLYING_SPEED 0.05f

#define ID_ANI_KOOPA_PARATROOPA_FLYING_RIGHT 5208
#define ID_ANI_KOOPA_PARATROOPA_FLYING_LEFT 5209

class CKoopaParatroopa :
    public CKoopa
{
public:
	CKoopaParatroopa(float x, float y) : CKoopa(x, y) {}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void SetState(int state) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
};

