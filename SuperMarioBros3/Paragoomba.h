#pragma once
#include "Goomba.h"

#define PARAGOOMBA_STATE_WALK 100
#define PARAGOOMBA_STATE_JUMP 101
#define PARAGOOMBA_STATE_FLY 102
#define PARAGOOMBA_STATE_LOST_WINGS 103

#define ID_ANI_PARAGOOMBA_WALKING 5003
#define ID_ANI_PARAGOOMBA_JUMPING 5004
#define ID_ANI_PARAGOOMBA_NORMAL 5005
#define ID_ANI_PARAGOOMBA_DIE 5006
#define ID_ANI_PARAGOOMBA_KICKED 5007

#define PARAGOOMBA_BBOX_OFFSET_Y 4

class Paragoomba :
    public CGoomba
{
public:
	Paragoomba(float x, float y) : CGoomba(x, y) { SetState(PARAGOOMBA_STATE_FLY); }
	void SetState(int state) override;
	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

