#pragma once
#include "Respawnable.h"

#define KOOPA_BBOX_WIDTH 15
#define KOOPA_BBOX_HEIGHT 16
#define KOOPA_BBOX_OFFSET_Y 3

#define KOOPA_STATE_WALKING 0

#define KOOPA_START_ANI_ID 5100

#define ID_ANI_KOOPA_WALK_RIGHT 5100
#define ID_ANI_KOOPA_WALK_LEFT 5101

#define KOOPA_WALKING_SPEED 0.05f

class Koopa :
    public CRespawnableEnemy
{
protected:
	int GetAniId(int defaultIdAni);
public:
	Koopa(float x, float y);

	virtual void Render() override;
	virtual void OnEnable() override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	int IsBlocking() override { return 1; }
	int IsCollidable() override { return 1; }

	void SetState(int state) override;
};

