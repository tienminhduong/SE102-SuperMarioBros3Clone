#include "BreakPartical.h"
#include "debug.h"

BreakPartical::BreakPartical(float x, float y, int index) : CGameObject(x, y)
{
	this->index = index;
	deleteTime = GetTickCount64();
	ay = PARTICAL_GRAVITY;
	ax = (index % 2 == 0) ? -PARTICAL_BASE_AX : PARTICAL_BASE_AX;
	vy = (index < 2) ? -PARTICAL_START_VY_TOP : -PARTICAL_START_VY_BOTTOM;
	vx = 0;
}

void BreakPartical::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}

void BreakPartical::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_PARTICAL)->Render(x, y);
}

void BreakPartical::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - deleteTime > PARTICAL_TIME_DELETE)
	{
		this->Delete();
		return;
	}

	vx += ax * dt;
	vy += ay * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void BreakPartical::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
