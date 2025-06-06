#pragma once
#include "GameObject.h"
#include "debug.h"

#define ID_ANI_GOLD_BRICK 100003
#define ID_ANI_GOLD_BRICK_EMPTY 100001
#define ID_ANI_GOLD_BRICK_COIN 100006

#define GOLD_BRICK_BBOX_WIDTH 15
#define GOLD_BRICK_BBOX_HEIGHT 15

#define GOLD_BRICK_STATE_NORMAL 0
#define GOLD_BRICK_STATE_BUTTON_OUT 1
#define GOLD_BRICK_STATE_COIN 2
#define GOLD_BRICK_STATE_HIT_NOTBROKEN 3

#define GOLD_BRICK_MOVE_SPEED 0.1f
#define GOLD_BRICK_MAX_MOVE_DY GOLD_BRICK_BBOX_HEIGHT / 2

#define GOLD_BRICK_COIN_TIME 10000 //10s

class CGoldBrick :
    public CGameObject
{
	int containButton;
	float dy = 0;
	int moveDirection = -1;
	int coinTimeCountDown = 0;

	int callbackId;
public:
	CGoldBrick(float x, float y, int containBtn);
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void SetState(int state) override;
	int IsBlocking() override { return state != GOLD_BRICK_STATE_COIN; }

	void TriggerOnCollision();
	void ChangeToGold();
	void Delete() override;
	~CGoldBrick();
};

