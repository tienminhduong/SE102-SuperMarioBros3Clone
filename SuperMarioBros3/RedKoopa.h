#pragma once
#include "Koopa.h"

#define RED_KOOPA_START_ANI_ID 5200

#define ID_ANI_RED_KOOPA_WALK_RIGHT  5200
#define ID_ANI_RED_KOOPA_WALK_LEFT   5201

#define RED_KOOPA_DETECTOR_OFFSET 3

#define RED_KOOPA_DETECTOR_BBOX_X 2	
#define RED_KOOPA_DETECTOR_BBOX_Y 25

class CRedKoopaDetector;

class RedKoopa :
    public Koopa
{
	CRedKoopaDetector* detector;
public:
	RedKoopa(float x, float y);
	void Render() override;
	void Delete() override;
	void OnEnable() override;
	void OnDisable() override;
	void ChangeDirection();
};

class CRedKoopaDetector : public CGameObject
{
	RedKoopa* koopa;
	bool isCollideLastFrame = false;
	int count = 0;

	float ay = ENEMY_GRAVITY;
public:
	CRedKoopaDetector(float x, float y, RedKoopa* koopa) : CGameObject(x, y) { this->koopa = koopa; vy = 0.f; }
	void Render() override { RenderBoundingBox(); }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 0; }
};
