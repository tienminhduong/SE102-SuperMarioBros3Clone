#include "RaccoonTail.h"
#include "QuestionMarkBlock.h"
#include "FirePiranhaPlant.h"
#include "debug.h"
#include "Goomba.h"

void CRaccoonTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	goomba->SetState(GOOMBA_STATE_DIE);
}

void CRaccoonTail::OnCollisionWithQuesBlock(LPCOLLISIONEVENT e)
{
	CQuestionMarkBlock* block = dynamic_cast<CQuestionMarkBlock*>(e->obj);
	block->TriggerOnCollisionWithMario(x);
}

void CRaccoonTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRaccoonTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - RACCOON_TAIL_BBOX_WIDTH / 2;
	top = y - RACCOON_TAIL_BBOX_HEIGHT / 2;
	right = left + RACCOON_TAIL_BBOX_WIDTH;
	bottom = top + RACCOON_TAIL_BBOX_HEIGHT;
}

void CRaccoonTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CQuestionMarkBlock*>(e->obj))
		OnCollisionWithQuesBlock(e);
	else if (dynamic_cast<CFirePiranhaPlant*>(e->obj))
		e->obj->Delete();
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
}

void CRaccoonTail::Render()
{
	RenderBoundingBox(1.f);
}
