#include "TransformLeaf.h"
#include "debug.h"

void CTransformLeaf::OnEnable()
{
	SetState(TRANSFORM_LEAF_STATE_FLY_UP);
	nx = -1;
}

void CTransformLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (nx > 0)
		animations->Get(ID_ANI_TRANSFORM_LEAF_RIGHT)->Render(x, y);
	else
		animations->Get(ID_ANI_TRANSFORM_LEAF_LEFT)->Render(x, y);
	//RenderBoundingBox();
}

void CTransformLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case TRANSFORM_LEAF_STATE_FLY_UP:
		startY = y;
		startX = x;
		vy = -TRANSFORM_LEAF_FLY_SPEED;
		vx = 0;
		break;
	case TRANSFORM_LEAF_STATE_FLY_DOWN:
		vy = TRANSFORM_LEAF_FLY_SPEED / 10;
		vx = TRANSFORM_LEAF_FLY_SPEED;
		t = -500 / 1.5f;
		startX = x;
		startY = y;
		break;
	}
}

void CTransformLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == TRANSFORM_LEAF_STATE_FLY_UP)
	{
		y += vy * dt;
		if (y < startY - TRANSFORM_LEAF_MAX_FLY_HEIGHT)
			SetState(TRANSFORM_LEAF_STATE_FLY_DOWN);
	}
	else {

		float px = x;
		x = amplitude * sin(1.5f*pi * t / 1000) + startX + amplitude;
		float dx = x - px;
		if (dx * nx < 0) {
			nx = dx / abs(dx);
		}

		y = vy * t + startY;
		t += dt;
	}
}

void CTransformLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TRANSFORM_LEAF_BBOX_WIDTH / 2;
	top = y - TRANSFORM_LEAF_BBOX_HEIGHT / 2;
	right = left + TRANSFORM_LEAF_BBOX_WIDTH;
	bottom = top + TRANSFORM_LEAF_BBOX_HEIGHT;
}
