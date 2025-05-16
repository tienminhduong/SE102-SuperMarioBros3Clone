#include "RedKoopa.h"
#include "PlayScene.h"

RedKoopa::RedKoopa(float x, float y)
	: Koopa(x, y)
{
	detector = new CRedKoopaDetector(x, y, this);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	scene->AddNewObject(detector);

	if (IsOnCamera())
		SetActive(true);
	else
		SetActive(false);
}

void RedKoopa::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = GetAniId(RED_KOOPA_START_ANI_ID);
	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void RedKoopa::Delete()
{
	Koopa::Delete();
}

void RedKoopa::OnEnable()
{
	Koopa::OnEnable();
	detector->SetPosition(x, y);
	detector->SetActive(true);
}

void RedKoopa::OnDisable()
{
	Koopa::OnDisable();
	detector->SetActive(false);
}

void RedKoopa::ChangeDirection()
{
	vx = -vx;
	nx = -nx;
}

void CRedKoopaDetector::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	float temp;
	koopa->GetPosition(x, temp);
	x += koopa->GetDirection();
	
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedKoopaDetector::OnNoCollision(DWORD dt)
{
	koopa->ChangeDirection();
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
