#pragma once
#include "GameObject.h"

#define SQUARE_COLOR_ID_ORANGE 0
#define SQUARE_COLOR_ID_BLUE 1
#define SQUARE_COLOR_ID_GREEN 2
#define SQUARE_COLOR_ID_WHITE 3

#define SQUARE_SPRITE_ID 51000
#define SQUARE_SHADOW_SPRITE_ID 55000

#define SQUARE_CELL_SIZE 8

class CSquare :
    public CGameObject
{
protected:
	int width, height;				// Unit: cell
	int colorID;

	int GetSpriteId(int index);
public:
	CSquare(float x, float y,
		int width, int height, int colorID) :CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
		this->colorID = colorID;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetRenderLayer() override { return 0; }

	int IsDirectionColliable(float nx, float ny);
};

