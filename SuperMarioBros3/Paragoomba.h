#pragma once
#include "Goomba.h"

#define GOOMBA_CHECKISPARAGOOMBA 500

#define PARAGOOMBA_STATE_WALK 501
#define PARAGOOMBA_STATE_JUMP 502
#define PARAGOOMBA_STATE_FLY 503
#define PARAGOOMBA_STATE_LOST_WINGS 300

#define ID_ANI_PARAGOOMBA_WALKING 5003
#define ID_ANI_PARAGOOMBA_JUMPING 5004
#define ID_ANI_PARAGOOMBA_NORMAL 5005
#define ID_ANI_PARAGOOMBA_DIE 5006
#define ID_ANI_PARAGOOMBA_KICKED 5007

#define PARAGOOMBA_BBOX_OFFSET_Y 4

#define PARAGOOMBA_FLY_SPEED 0.16f
#define PARAGOOMBA_JUMP_SPEED 0.05f
#define PARAGOOMBA_JUMP_COUNT 3

class Paragoomba :
    public CGoomba
{
	int changeStateDuration = 0;
	int jumpCount = 0;
	void FindMario();
public:
	Paragoomba(float x, float y);
	void OnEnable() override;
	void SetState(int state) override;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

