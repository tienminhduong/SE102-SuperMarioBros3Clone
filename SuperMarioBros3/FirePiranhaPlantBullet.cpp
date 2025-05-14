#include "FirePiranhaPlantBullet.h"

void CFirePiranhaPlantBullet::NormalizeDirection()
{
	float length = sqrt(dirX * dirX + dirY * dirY);
	if (length == 0)
		return;

	dirX /= length;
	dirY /= length;
}

CFirePiranhaPlantBullet::CFirePiranhaPlantBullet(float x, float y, float dirX, float dirY)
	: CGameObject(x, y)
{
	this->dirX = dirX;
	this->dirY = dirY;

	NormalizeDirection();
}

void CFirePiranhaPlantBullet::Render()
{
	if (dirX > 0)
		CAnimations::GetInstance()->Get(ID_ANI_FIRE_PIRANHA_BULLET_RIGHT)->Render(x, y);
	else
		CAnimations::GetInstance()->Get(ID_ANI_FIRE_PIRANHA_BULLET_LEFT)->Render(x, y);
}

void CFirePiranhaPlantBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += dirX * dt * FIRE_PIRANHA_BULLET_SPEED;
	y += dirY * dt * FIRE_PIRANHA_BULLET_SPEED;
}

void CFirePiranhaPlantBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIRE_PIRANHA_BBOX_SIZE / 2;
	t = y - FIRE_PIRANHA_BBOX_SIZE / 2;
	r = l + FIRE_PIRANHA_BBOX_SIZE;
	b = t + FIRE_PIRANHA_BBOX_SIZE;
}
