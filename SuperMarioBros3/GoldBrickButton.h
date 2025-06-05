#pragma once
#include "GameObject.h"

#define ID_ANI_GOLD_BRICK_BTN_ACTIVE 100004
#define ID_ANI_GOLD_BRICK_BTN_INACTIVE 100005

#define GOLD_BRICK_BTN_BBOX_WIDTH 13
#define GOLD_BRICK_BTN_BBOX_HEIGHT 13

#define GOLD_BRICK_BTN_STATE_ACTIVE 0
#define GOLD_BRICK_BTN_STATE_INACTIVE 1


class CGoldBrickButton :
    public CGameObject
{
public:
	CGoldBrickButton(float x, float y) : CGameObject(x, y) { state = GOLD_BRICK_BTN_STATE_ACTIVE; }
	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	int IsBlocking() override { return state == GOLD_BRICK_BTN_STATE_ACTIVE; }

	void TriggerOnCollision();
};

