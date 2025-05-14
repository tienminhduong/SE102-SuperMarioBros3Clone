#include "Pipe.h"
#include "FirePiranhaPlant.h"
#include "Game.h"
#include "PlayScene.h"

CPipe::CPipe(float x, float y, int height, int type) : CGameObject(x, y)
{
	this->height = height;
	this->type = type;
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	if (type == PIPE_TYPE_CONTAIN_FIRE_PIRANHA_PLANT)
	{
		containedObj = new CFirePiranhaPlant(x, y + FIRE_PIRANHA_PLANT_BBOX_HEIGHT / 2,
			currentScene->GetPlayer(), this);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(containedObj);
	}
}

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

	if (IsEnterCamera())
	{
		DebugOut(L"Pipe has come!\n");
	}
	if (IsOutCamera())
	{
		DebugOut(L"Pipe has gone!\n");
	}
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPE_CELL_WIDTH / 2;
	t = y - PIPE_CELL_HEIGHT / 2;
	r = l + PIPE_CELL_WIDTH;
	b = t + this->height * PIPE_CELL_HEIGHT;
}
