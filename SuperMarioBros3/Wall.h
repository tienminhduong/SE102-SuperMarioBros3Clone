#pragma once
#include "GameObject.h"
class Wall :
    public CGameObject
{
	int width;
	int height;
public:
	Wall(float x, float y, int width, int height);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void Render() override { RenderBoundingBox(); };
	int IsBlocking() override { return 1; };
};

