#include "Square.h"

int CSquare::GetSpriteId(int index)
{
	return SQUARE_SPRITE_ID + colorID * 1000 + index;
}

void CSquare::Render()
{
	if (this->width * this->height <= 0) return;
	float xx = x;
	CSprites* sprites = CSprites::GetInstance();

	for (int i = 0; i < this->height; ++i) {

		int indexW;
		if (i == 0) indexW = 0;
		else if (i == this->height - 1) indexW = 2;
		else indexW = 1;

		for (int j = 0; j < this->width; ++j)
		{
			int indexH;
			if (j == 0) indexH = 0;
			else if (j == this->width - 1) indexH = 2;
			else indexH = 1;

			int index = indexW * 3 + indexH;

			int spriteId = GetSpriteId(index + 1);
			sprites->Get(spriteId)->Draw(x + j * SQUARE_CELL_SIZE, y + i * SQUARE_CELL_SIZE);
		}
	}

	// Draw shadow
	for (int i = 1; i < this->height + 1; ++i) {
		int index;
		if (i == 1) index = SQUARE_SHADOW_SPRITE_ID;
		else if (i == this->height) index = SQUARE_SHADOW_SPRITE_ID + 2;
		else index = SQUARE_SHADOW_SPRITE_ID + 1;
		sprites->Get(index)->Draw(x + this->width * SQUARE_CELL_SIZE, y + i * SQUARE_CELL_SIZE);
	}

	for (int i = 1; i < this->width; ++i) {
		int index;
		if (i == 1) index = SQUARE_SHADOW_SPRITE_ID + 3;
		else index = SQUARE_SHADOW_SPRITE_ID + 1;
		sprites->Get(index)->Draw(x + i * SQUARE_CELL_SIZE, y + this->height * SQUARE_CELL_SIZE);
	}
}

void CSquare::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SQUARE_CELL_SIZE / 2;
	t = y - SQUARE_CELL_SIZE / 2;
	r = l + SQUARE_CELL_SIZE * this->width;
	b = t + SQUARE_CELL_SIZE * this->height;
}

int CSquare::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	else return 0;
}
