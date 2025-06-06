#pragma once
#include "GameObject.h"

#define ID_ANI_MUSHROOM 120100
#define ID_ANI_MUSHROOM_COMPLETE 120101

#define MUSHROOM_STATE_RAISING 1
#define MUSHROOM_STATE_MOVING 2

#define MUSHROOM_BBOX_WIDTH 15
#define MUSHROOM_BBOX_HEIGHT 15

#define MUSHROOM_VX 0.05f
#define MUSHROOM_VY 0.2f

class CMushroom :
	public CGameObject
{
	bool isOnGround = false;
protected:
	virtual int GetAniId(int state);
public:
	CMushroom(float x, float y);
	void Render();
	void OnEnable() override;
	void SetState(int state) override;
	void SetDirection(int direction);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }

};


