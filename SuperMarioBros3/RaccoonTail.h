#pragma once
#include "GameObject.h"

#define RACCOON_TAIL_BBOX_WIDTH 11
#define RACCOON_TAIL_BBOX_HEIGHT 3

class CRaccoonTail :
    public CGameObject
{
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithQuesBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBrick(LPCOLLISIONEVENT e);
public:
	CRaccoonTail(float x, float y) : CGameObject(x, y) { }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 0; }
	void Render() override;
	int GetRenderLayer() override { return 4; }
};
