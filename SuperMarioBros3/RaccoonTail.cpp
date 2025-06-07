#include "RaccoonTail.h"
#include "QuestionMarkBlock.h"
#include "FirePiranhaPlant.h"
#include "debug.h"
#include "Goomba.h"
#include "GoldBrick.h"
#include "GameManager.h"

void CRaccoonTail::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	//float eX, eY;
	//e->obj->GetPosition(eX, eY);
	CRespawnableEnemy* enemy = dynamic_cast<CRespawnableEnemy*>(e->obj);
	enemy->OnAttackedByTail(e->obj->GetX() - x);
	float avX = (x + e->obj->GetX()) / 2.f;
	float avY = (y + e->obj->GetY()) / 2.f;
	CAnimations::GetInstance()->PlayEffect(ID_ANI_TAIL_ATTACK, avX, avY);
	if (dynamic_cast<CKoopa*>(enemy) == nullptr) {
		CAnimations::GetInstance()->PlayEffect(ID_ANI_100_UP, avX, avY - 20);
		GameManager::GetInstance()->IncreasePoint();
	}
}

void CRaccoonTail::OnCollisionWithQuesBlock(LPCOLLISIONEVENT e)
{
	CQuestionMarkBlock* block = dynamic_cast<CQuestionMarkBlock*>(e->obj);
	block->TriggerOnCollisionWithMario(x);
}

void CRaccoonTail::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
	CGoldBrick* gb = dynamic_cast<CGoldBrick*>(e->obj);
	gb->TriggerOnCollision();
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
	else if (dynamic_cast<CGoldBrick*>(e->obj))
		OnCollisionWithGoldBrick(e);
	else if (dynamic_cast<CFirePiranhaPlant*>(e->obj))
	{
		e->obj->Delete();
		CAnimations::GetInstance()->PlayEffect(ID_ANI_TAIL_ATTACK, e->obj->GetX(), e->obj->GetY());
		CAnimations::GetInstance()->PlayEffect(ID_ANI_100_UP, x, y - 20);
	}
	else if (dynamic_cast<CRespawnableEnemy*>(e->obj))
		OnCollisionWithEnemy(e);
}

void CRaccoonTail::Render()
{
	RenderBoundingBox(1.f);
}
