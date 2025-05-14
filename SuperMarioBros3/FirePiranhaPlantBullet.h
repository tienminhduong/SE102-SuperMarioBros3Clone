#pragma once
#include "GameObject.h"

#define ID_ANI_FIRE_PIRANHA_BULLET_LEFT 130008
#define ID_ANI_FIRE_PIRANHA_BULLET_RIGHT 130009

#define FIRE_PIRANHA_BULLET_SPEED 0.1f

#define FIRE_PIRANHA_BBOX_SIZE 9

class CFirePiranhaPlantBullet :
    public CGameObject
{
private:
	float dirX;
	float dirY;

	void NormalizeDirection();
public:
	CFirePiranhaPlantBullet(float x, float y, float dirX, float dirY);
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	int IsBlocking() override { return 0; }
	int GetRenderLayer() override { return 2; }
};

