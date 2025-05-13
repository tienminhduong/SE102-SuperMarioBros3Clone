#include "Pipe.h"

void CPipe::Render()
{
	if (this->height <= 0)
		return;

	float yy = y;
	CSprites* s = CSprites::GetInstance();
	s->Get(PIPE_SPRITE_BEGIN_ID)->Draw(x, yy);
	yy += PIPE_CELL_HEIGHT;

	for (int i = 1; i < this->height; i++)
	{
		s->Get(PIPE_SPRITE_MID_ID)->Draw(x, yy);
		yy += PIPE_CELL_HEIGHT;
	}
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPE_CELL_WIDTH / 2;
	t = y - PIPE_CELL_HEIGHT / 2;
	r = l + PIPE_CELL_WIDTH;
	b = t + this->height * PIPE_CELL_HEIGHT;
}
