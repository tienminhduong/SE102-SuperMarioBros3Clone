#pragma once
#include "GameObject.h"

#define ENEMY_STATE_KICKED -1
#define ENEMY_KICKED_FLY_SPEED 0.3f
#define ENEMY_KICKED_X 0.05f
#define ENEMY_GRAVITY 0.0008f

class CRespawnPoint;

class CRespawnableEnemy :
    public CGameObject
{
	CRespawnPoint* respawnPoint = nullptr;
	int flyDirection = 1;
protected:
	float ax;
	float ay;
	bool IsOtherEnemyOrMario(LPGAMEOBJECT obj);
public:
	CRespawnableEnemy(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void OnExitCamera() override;
	void Delete() override;
	virtual void OnAttackedByTail(float direction);
	virtual void SetState(int state) override;
	bool RenderOnPaused() override { return false; }
};

class CRespawnPoint : public CGameObject {
private:
	CRespawnableEnemy* obj = nullptr;
public:
	CRespawnPoint(float x, float y, CRespawnableEnemy* obj);
	void Render() override {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) override {}
	void OnEnterCamera() override;
	void Delete() override;
};