#include "RedKoopa.h"
#include "PlayScene.h"

RedKoopa::RedKoopa(float x, float y)
	: Koopa(x, y)
{
	float detectorX = x + nx * (RED_KOOPA_DETECTOR_OFFSET + RED_KOOPA_DETECTOR_BBOX_X / 2);
	detector = new CRedKoopaDetector(detectorX, y + RED_KOOPA_DETECTOR_OFFSET, this);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	scene->AddNewObject(detector);
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
	detector->SetPosition(x + nx * (RED_KOOPA_DETECTOR_OFFSET + KOOPA_BBOX_WIDTH / 2), y + RED_KOOPA_DETECTOR_OFFSET);
	detector->SetActive(true);
}

void RedKoopa::OnDisable()
{
	Koopa::OnDisable();
	detector->SetActive(false);
}

void RedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
		vy = 0;
	else if (e->nx != 0)
		ChangeDirection();
}

void RedKoopa::ChangeDirection()
{
	vx = -vx;
	nx = -nx;

	float detectorX = x + nx * (RED_KOOPA_DETECTOR_OFFSET + KOOPA_BBOX_WIDTH / 2);
	detector->SetPosition(detectorX, y);
	

	float deX, deY;
	detector->GetPosition(deX, deY);
	//DebugOut(L"[Red koopa] x = %f, y = %f, deX = %f, deY = %f\n", x, y, deX, deY);
}

void CRedKoopaDetector::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//koopa->GetSpeed(vx, vy);
	//x += vx * dt;
	//y += vy * dt;

	DebugOut(L"[detector] vx = %f, vy = %f\n", vx, vy);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedKoopaDetector::OnNoCollision(DWORD dt)
{
	DebugOut(L"Arise!%d\n", count++);
	//if (isCollideLastFrame)
		koopa->ChangeDirection();
	isCollideLastFrame = false;
}

void CRedKoopaDetector::OnCollisionWith(LPCOLLISIONEVENT e)
{
	isCollideLastFrame = true;
}

void CRedKoopaDetector::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - RED_KOOPA_DETECTOR_BBOX_X / 2;
	top = y - RED_KOOPA_DETECTOR_BBOX_Y / 2;
	right = left + RED_KOOPA_DETECTOR_BBOX_X;
	bottom = top + RED_KOOPA_DETECTOR_BBOX_Y;
}
