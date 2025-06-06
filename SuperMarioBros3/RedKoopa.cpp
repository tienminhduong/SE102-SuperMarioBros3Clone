#include "RedKoopa.h"
#include "PlayScene.h"

CRedKoopa::CRedKoopa(float x, float y)
	: CKoopa(x, y)
{
	detector = new CRedKoopaDetector(x, y, this);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	scene->AddNewObject(detector);

	if (IsOnCamera())
		SetActive(true);
	else
		SetActive(false);
}

void CRedKoopa::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = GetAniId(RED_KOOPA_START_ANI_ID);
	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CRedKoopa::Delete()
{
	CKoopa::Delete();
}

void CRedKoopa::OnEnable()
{
	CKoopa::OnEnable();
	detector->SetActive(true);
}

void CRedKoopa::OnDisable()
{
	CKoopa::OnDisable();
	detector->SetActive(false);
}

void CRedKoopa::OnNoCollision(DWORD dt)
{
	CKoopa::OnNoCollision(dt);
	isOnPlatform = false;
}

void CRedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CKoopa::OnCollisionWith(e);
	if (e->ny != 0)
	{
		if (e->obj->IsBlocking())
			isOnPlatform = true;
	}
}

void CRedKoopa::SetState(int state)
{
	CKoopa::SetState(state);
	detector->SetActive(state == KOOPA_STATE_WALKING);
}

void CRedKoopaDetector::OnEnable()
{
	SetPosition(koopa->GetX(), koopa->GetY() - RED_KOOPA_DETECTOR_OFFSET);
	//SetPosition(koopa->GetX(), koopa->GetY());
}

void CRedKoopaDetector::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	x = koopa->GetX() + koopa->GetDirection();
	
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedKoopaDetector::OnNoCollision(DWORD dt)
{
	y += vy * dt;
	if (koopa->IsOnPlatform())
	{
		koopa->ChangeDirection();
		SetActive(true);
	}
}

void CRedKoopaDetector::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
		vy = 0.f;
}

void CRedKoopaDetector::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - RED_KOOPA_DETECTOR_BBOX_X / 2;
	top = y - RED_KOOPA_DETECTOR_BBOX_Y / 2;
	right = left + RED_KOOPA_DETECTOR_BBOX_X;
	bottom = top + RED_KOOPA_DETECTOR_BBOX_Y;
}
