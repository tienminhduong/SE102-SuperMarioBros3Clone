#include "Respawnable.h"
#include "PlayScene.h"

Respawnable::Respawnable(float x, float y) : CGameObject(x, y)
{
	respawnPoint = new RespawnPoint(x, y, this);
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	scene->AddNewObject(respawnPoint);
}

void Respawnable::OnExitCamera()
{
	float posX, posY;
	respawnPoint->GetPosition(posX, posY);
	SetPosition(posX, posY);
	SetActive(false);
}

void Respawnable::Delete()
{
	if (respawnPoint != nullptr)
	{
		respawnPoint->Delete();
		respawnPoint = nullptr;
	}
	isDeleted = true;
}

void RespawnPoint::OnEnterCamera()
{
	if (obj != nullptr)
		obj->SetActive(true);
}

void RespawnPoint::Delete()
{
	if (obj != nullptr)
		obj = nullptr;
	isDeleted = true;
}
