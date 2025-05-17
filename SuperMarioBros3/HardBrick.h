#pragma once
#include "GameObject.h"
#define ID_ANI_STATIC_BRICK 140000

#define STATIC_BRICK_BBOX_WIDTH 15
#define STATIC_BRICK_BBOX_HEIGHT 15

class HardBrick :
	public CGameObject
{
public:
	HardBrick(float x, float y) : CGameObject(x, y) {}
	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};
