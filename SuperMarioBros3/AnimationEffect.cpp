#include "AnimationEffect.h"

void AnimationEffect::Render()
{
	CAnimations::GetInstance()->Get(id)->Render(x, y);
}

void AnimationEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CAnimations::GetInstance()->Get(id)->IsOver())
		Delete();
}

void AnimationEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0.0f;
}