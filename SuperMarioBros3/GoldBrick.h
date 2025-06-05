#pragma once
#include "GameObject.h"
#include "debug.h"

#define ID_ANI_GOLD_BRICK 100003

#define GOLD_BRICK_BBOX_WIDTH 15
#define GOLD_BRICK_BBOX_HEIGHT 15

#define GOLD_BRICK_STATE_NORMAL 0
#define GOLD_BRICK_STATE_BUTTON_OUT 1
#define GOLD_BRICK_STATE_COIN 2
#define GOLD_BRICK_STATE_HIT_NOTBROKEN 3

#define GOLD_BRICK_MOVE_SPEED 0.075f
#define GOLD_BRICK_MAX_MOVE_DY GOLD_BRICK_BBOX_HEIGHT / 2

class GoldBrick :
    public CGameObject
{
	bool containButton;
	float dy = 0;
	int moveDirection = -1;
public:
	GoldBrick(float x, float y, int containBtn) : CGameObject(x, y) { containButton = containBtn; }
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void SetState(int state) override;

	void TriggerOnCollision();
};

