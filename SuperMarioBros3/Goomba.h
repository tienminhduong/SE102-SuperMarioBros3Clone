#pragma once
#include "GameObject.h"
#include "Respawnable.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_KICKED 5002

class CGoomba : public CRespawnableEnemy
{
protected:
	int die_start;
	bool CheckKoopaCollision(LPCOLLISIONEVENT e);
public:
	CGoomba(float x, float y);
	void OnEnable() override;
	void OnExitCamera() override;
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	int GetRenderLayer() { return 4; }

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};