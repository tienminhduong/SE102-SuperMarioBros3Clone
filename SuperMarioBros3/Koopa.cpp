#include "Koopa.h"
#include "Mario.h"
#include "QuestionMarkBlock.h"
#include "Goomba.h"
#include "FirePiranhaPlant.h"
#include "debug.h"
#include "GameManager.h"

int CKoopa::GetAniId(int defaultIdAni)
{
	int aniId = -1;
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		aniId = vx > 0 ? ID_ANI_KOOPA_WALK_RIGHT : ID_ANI_KOOPA_WALK_LEFT;
		break;
	case KOOPA_STATE_INSHELL:
		aniId = isFlipped ? ID_ANI_KOOPA_INSHELL_FLIPPED : ID_ANI_KOOPA_INSHELL;
		if (inShellDuration > 0 && inShellDuration < 2000)
			aniId = isFlipped ? ID_ANI_KOOPA_INSHELL_FLIPPED_TIMEOUT : ID_ANI_KOOPA_TIMEOUT_INSHELL;
		break;
	case KOOPA_STATE_INSHELL_RUNNING:
		aniId = isFlipped ? ID_ANI_KOOPA_INSHELL_FLIPPED_RUNNING : ID_ANI_KOOPA_INSHELL_RUNNING;
		break;
	case ENEMY_STATE_KICKED:
		aniId = isFlipped ? ID_ANI_KOOPA_INSHELL_FLIPPED : ID_ANI_KOOPA_INSHELL;
		break;
	default:
		break;
	}

	return defaultIdAni - KOOPA_START_ANI_ID + aniId;
}

void CKoopa::OnCollisionWithQuestionMarkBlock(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_INSHELL_RUNNING && e->nx != 0)
	{
		CQuestionMarkBlock* q = dynamic_cast<CQuestionMarkBlock*>(e->obj);
		q->TriggerOnCollisionWithMario(x);
	}
}

void CKoopa::OnCollisionWithFirePiranhaPlant(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_INSHELL_RUNNING)
	{
		e->obj->Delete();
		CAnimations::GetInstance()->PlayEffect(ID_ANI_100_UP, x, y);
		GameManager::GetInstance()->IncreasePoint();
	}
}

CKoopa::CKoopa(float x, float y)
	: CRespawnableEnemy(x, y)
{
	SetState(KOOPA_STATE_WALKING);
	nx = -1;
}

void CKoopa::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = GetAniId(KOOPA_START_ANI_ID);
	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
}

void CKoopa::OnEnable()
{
	SetState(KOOPA_STATE_WALKING);
	nx = -1;
	markedAsDead = false;
}

void CKoopa::OnDisable()
{
	CRespawnableEnemy::OnDisable();
	if (markedAsDead)
		Delete();
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CRespawnableEnemy::Update(dt, coObjects);
	if (state != ENEMY_STATE_KICKED && !IsHold())
	{
		vx += ax * dt;
		vy += ay * dt;
	}

	if (state == KOOPA_STATE_INSHELL && inShellDuration > 0)
	{
		inShellDuration -= dt;
		if (inShellDuration <= 0)
		{
			SetState(KOOPA_STATE_WALKING);
			if (IsHold()) {
				((CMario*)mario)->ReleaseKoopa();
				mario = nullptr;
			}
			y -= KOOPA_BBOX_OFFSET_Y;
		}
	}

	//DebugOut(L"Current koopa state: %d\n", state);

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::OnNoCollision(DWORD dt)
{
	if (IsHold())
		return;

	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!IsHold())
	{
		if (e->ny < 0)
		{
			vy = 0;
			if (state == ENEMY_STATE_KICKED)
				SetState(KOOPA_STATE_INSHELL);
		}
		if (e->nx != 0) {
			if (e->obj->IsBlocking())
				vx = -vx, nx = -nx;
			if (dynamic_cast<CQuestionMarkBlock*>(e->obj))
				OnCollisionWithQuestionMarkBlock(e);
			if (dynamic_cast<CFirePiranhaPlant*>(e->obj))
				OnCollisionWithFirePiranhaPlant(e);
		}

		if (dynamic_cast<CKoopa*>(e->obj))
		{
			CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
			if (koopa->IsHold() || koopa->GetState() == KOOPA_STATE_INSHELL_RUNNING) {
				OnAttackedByTail((float)koopa->GetDirection());
				SetDead();

				koopa->ReleaseFromMario();
				koopa->SetDead();
				if (koopa->GetState() == KOOPA_STATE_INSHELL_RUNNING)
					koopa->OnAttackedByTail(-(float)koopa->GetDirection());
				else
					koopa->OnAttackedByTail((float)koopa->GetDirection());
			}
			else {
				koopa->ChangeDirection();
				this->ChangeDirection();
			}
		}
	}
	else {
		// check collision with fire piranha, destory it if it collide, and release the koopa
		if (dynamic_cast<CFirePiranhaPlant*>(e->obj))
		{
			e->obj->Delete();
			ReleaseFromMario();
			SetDead();
			OnAttackedByTail((float)nx);
			CAnimations::GetInstance()->PlayEffect(ID_ANI_100_UP, x, y - 20);
			GameManager::GetInstance()->IncreasePoint();
		}

	}
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2 + KOOPA_BBOX_OFFSET_Y * (state == KOOPA_STATE_WALKING);
	right = left + KOOPA_BBOX_WIDTH;
	bottom = top + KOOPA_BBOX_HEIGHT;
}

int CKoopa::IsBlocking()
{
	return 0;
}

void CKoopa::OnAttackedByTail(float direction)
{
	CRespawnableEnemy::OnAttackedByTail(direction);
	isFlipped = true;
}

void CKoopa::GetKicked(int direction)
{
	SetState(KOOPA_STATE_INSHELL_RUNNING);
	vx = direction * KOOPA_INSHELL_RUNNING_SPEED;
}

void CKoopa::SetState(int state)
{
	ay = ENEMY_GRAVITY;
	CRespawnableEnemy::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		vx = nx * KOOPA_WALKING_SPEED;
		isFlipped = false;
		break;
	case KOOPA_STATE_INSHELL:
		vx = 0;
		inShellDuration = KOOPA_INSHELL_TIME;
		break;
	case KOOPA_STATE_INSHELL_RUNNING:
		ay *= 2;
		break;
	default:
		break;
	}
}

void CKoopa::SetHold(LPGAMEOBJECT mario)
{
	this->mario = mario;
}

void CKoopa::ReleaseFromMario()
{
	if (IsHold())
	{
		((CMario*)mario)->ReleaseKoopa();
		mario = nullptr;
	}
}

void CKoopa::ChangeDirection()
{
	vx = -vx;
	nx = -nx;
}
