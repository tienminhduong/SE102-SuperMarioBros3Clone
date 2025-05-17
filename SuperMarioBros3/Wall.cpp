#include "Wall.h"

Wall::Wall(float x, float y, int width, int height)
	: CGameObject(x, y)
{
	this->width = width;
	this->height = height;
}

void Wall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - width / 2;
	top = y - height / 2;
	right = left + width;
	bottom = top + height;
}
