#pragma once
#include "GameObject.h"

#define PIPE_TYPE_EMPTY 0
#define PIPE_TYPE_CONTAIN_FIRE_PIRANHA_PLANT 1
#define PIPE_TYPE_CONTAIN_PIRANHA_PLANT 2
#define PIPE_TYPE_EMPTY_UPSIDE_DOWN 3

#define PIPE_SPRITE_BEGIN_ID 120020
#define PIPE_SPRITE_MID_ID 120021

#define PIPE_CELL_WIDTH 31
#define PIPE_CELL_HEIGHT 15

class CPipe :
    public CGameObject
{
protected:
	int height;
	int type;
	LPGAMEOBJECT containedObj = NULL;
	int enterMapKey;
public:
	CPipe(float x, float y, int height, int type, int enterMapKey);

	float GetTop() { return y - PIPE_CELL_HEIGHT / 2; }

	void Render();
	void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() override { return 1; }
	void OnExitCamera() override;
	int GetEnterMapKey() { return enterMapKey; }
};

