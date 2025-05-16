#include "Respawnable.h"
#include "PlayScene.h"
#include "Mario.h"
#include <typeinfo>

bool CRespawnableEnemy::IsOtherEnemyOrMario(LPGAMEOBJECT obj)
{
	if (dynamic_cast<CMario*>(obj) != nullptr)
		return true;
	if (dynamic_cast<CRespawnableEnemy*>(obj) != nullptr) {
		return typeid(*obj) != typeid(*this);
	}
}

CRespawnableEnemy::CRespawnableEnemy(float x, float y) : CGameObject(x, y)
{
	respawnPoint = new CRespawnPoint(x, y, this);
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	scene->AddNewObject(respawnPoint);

	ax = 0.f;
	ay = ENEMY_GRAVITY;
}

void CRespawnableEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == ENEMY_STATE_KICKED)
	{
		vy += ay * dt;
		y += vy * dt;

		DebugOut(L"[ENEMY] Kicked, vy: %f, position y: %f\n", vy, y);
	}
}

void CRespawnableEnemy::OnExitCamera()
{
	float posX, posY;
	respawnPoint->GetPosition(posX, posY);
	SetPosition(posX, posY);
	SetActive(false);
}

void CRespawnableEnemy::Delete()
{
	if (respawnPoint != nullptr)
	{
		respawnPoint->Delete();
		respawnPoint = nullptr;
	}
	isDeleted = true;
}

void CRespawnableEnemy::OnAttackedByTail(float direction)
{
	flyDirection = (int)(direction / abs(direction));
	SetState(ENEMY_STATE_KICKED);
}

void CRespawnableEnemy::SetState(int state)
{
	this->state = state;
	if (state == ENEMY_STATE_KICKED)
	{
		nx = flyDirection;
		vx = flyDirection * ENEMY_KICKED_X;
		vy = -ENEMY_KICKED_FLY_SPEED;
	}
}

CRespawnPoint::CRespawnPoint(float x, float y, CRespawnableEnemy* obj)
	: CGameObject(x, y)
{
	this->obj = obj;
	if (!IsOnCamera())
		obj->SetActive(false);
}

void CRespawnPoint::OnEnterCamera()
{
	if (obj != nullptr)
		obj->SetActive(true);
}

void CRespawnPoint::Delete()
{
	if (obj != nullptr)
		obj = nullptr;
	isDeleted = true;
}
