#pragma once
#include "GameObject.h"

class RespawnPoint;

class Respawnable :
    public CGameObject
{
	RespawnPoint* respawnPoint = nullptr;
public:
	Respawnable(float x, float y);
	void OnExitCamera() override;
	void Delete() override;
};

class RespawnPoint : public CGameObject {
private:
	Respawnable* obj = nullptr;
public:
	RespawnPoint(float x, float y, Respawnable* obj) : CGameObject(x, y) { this->obj = obj; }
	void Render() override {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) override {}
	void OnEnterCamera() override;
	void Delete() override;
};