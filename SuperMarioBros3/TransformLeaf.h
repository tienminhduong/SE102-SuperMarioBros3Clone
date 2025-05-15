#pragma once
#include "GameObject.h"

#define TRANSFORM_LEAF_STATE_FLY_UP 0
#define TRANSFORM_LEAF_STATE_FLY_DOWN 1

#define ID_ANI_TRANSFORM_LEAF_LEFT 120002
#define ID_ANI_TRANSFORM_LEAF_RIGHT 120003

#define TRANSFORM_LEAF_BBOX_WIDTH 17
#define TRANSFORM_LEAF_BBOX_HEIGHT 15

#define TRANSFORM_LEAF_MAX_FLY_HEIGHT 35
#define TRANSFORM_LEAF_MAX_DISTANCE 32

#define TRANSFORM_LEAF_FLY_SPEED 0.1f

class CTransformLeaf :
    public CGameObject
{
	float startX;
	float startY;

	float t;
	const float amplitude = 20.f;
	const float pi = 3.14159265358979323846f;

public:
	CTransformLeaf(float x, float y) : CGameObject(x, y) { OnEnable(); }
	void OnEnable() override;
	void Render() override;
	void SetState(int state) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	int GetRenderLayer() override { return 2; }
	int IsBlocking() override { return 0; }
	void OnExitCamera() override;
};

