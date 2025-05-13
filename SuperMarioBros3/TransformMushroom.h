#pragma once
#include "GameObject.h"

#define ID_ANI_TRANSFORM_MUSHROOM 120000
#define ID_ANI_TRANSFORM_MUSHROOM_COMPLETE 120001

#define TRANSFORM_MUSHROOM_STATE_RAISING 1
#define TRANSFORM_MUSHROOM_STATE_MOVING 2

#define TRANSFORM_MUSHROOM_BBOX_WIDTH 15
#define TRANSFORM_MUSHROOM_BBOX_HEIGHT 15

#define TRANSFORM_MUSHROOM_VX 0.05f
#define TRANSFORM_MUSHROOM_VY 0.2f

class CTransformMushroom :
    public CGameObject
{
	bool isOnGround = false;
	bool check = false;
public:
	CTransformMushroom(float x, float y);
	void Render();
	void OnEnable() override;
	void SetState(int state) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
};

