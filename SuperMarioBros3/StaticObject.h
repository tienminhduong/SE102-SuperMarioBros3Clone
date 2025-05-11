#pragma once
#include "GameObject.h"
class StaticObject :
    public CGameObject
{
	int spriteID;
public:
	StaticObject(float x, float y, int spriteID) : CGameObject(x, y) { this->spriteID = spriteID; }
	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override
	{ left = top = right = bottom = 0; }
	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 0; }
	int GetRenderLayer() override { return 0; }
};

