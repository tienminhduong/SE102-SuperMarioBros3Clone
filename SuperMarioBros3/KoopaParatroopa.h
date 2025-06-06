#pragma once
#include "Koopa.h"

#define KOOPA_PARATROOPA_STATE_FLYING 10
#define KOOPA_PARATROOPA_BASE_FLYING_SPEED 0.2f

#define ID_ANI_KOOPA_PARATROOPA_FLYING_RIGHT 5208
#define ID_ANI_KOOPA_PARATROOPA_FLYING_LEFT 5209

class CKoopaParatroopa :
    public CKoopa
{
public:
	CKoopaParatroopa(float x, float y) : CKoopa(x, y) { SetState(KOOPA_PARATROOPA_STATE_FLYING); }
	void OnEnable() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void SetState(int state) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
};

