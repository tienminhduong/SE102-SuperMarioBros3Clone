#pragma once
#include "Koopa.h"

#define RED_KOOPA_START_ANI_ID 5200

#define ID_ANI_RED_KOOPA_WALK_RIGHT  5200
#define ID_ANI_RED_KOOPA_WALK_LEFT   5201

#define RED_KOOPA_DETECTOR_OFFSET 2

#define RED_KOOPA_DETECTOR_BBOX_X 1
#define RED_KOOPA_DETECTOR_BBOX_Y 25

class CRedKoopaDetector;

class RedKoopa :
    public CKoopa
{
	CRedKoopaDetector* detector;
	bool isOnPlatform = false;
public:
	RedKoopa(float x, float y);
	void Render() override;
	void Delete() override;
	void SetState(int state) override;

	void OnEnable() override;
	void OnDisable() override;

	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	bool IsOnPlatform() { return isOnPlatform; }
};

class CRedKoopaDetector : public CGameObject
{
	RedKoopa* koopa;
	bool isCollideLastFrame = false;
	int count = 0;

	float ay = ENEMY_GRAVITY;
public:
	CRedKoopaDetector(float x, float y, RedKoopa* koopa) : CGameObject(x, y) { this->koopa = koopa; vy = 0.f; }
	void OnEnable() override;

	void Render() override { RenderBoundingBox(); }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 0; }
};
