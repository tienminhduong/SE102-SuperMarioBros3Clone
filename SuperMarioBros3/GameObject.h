#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

//#define INGAME_CAMERA_WIDTH 251
//#define INGAME_CAMERA_WIDTH 256
//#define INGAME_CAMERA_HEIGHT 187
#define INGAME_CAMERA_WIDTH 266
#define INGAME_CAMERA_HEIGHT 200

class CGameObject
{
private:
	bool isActive;
	bool isOnCameraEnterCheck = false;
	bool isOutCameraCheck = false;

	bool IsEnterCamera();
	bool IsOutCamera();
protected:

	float x; 
	float y;

	float vx;
	float vy;

	int nx;	 

	int state;

	bool isDeleted;
public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetPosition(float x, float y, DWORD dt);
	void SetPosition(float x, float y, DWORD dt, bool removeY);
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	int GetDirection() { return (int)(vx / abs(vx)); }
	float GetX() { return x; }
	float GetY() { return y; }

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox(float alpha = BBOX_ALPHA);

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }


	void SetActive(bool isActive);
	bool GetActive() { return isActive; }
	virtual void OnEnable() {};
	virtual void OnDisable() {};

	bool IsOnCamera();
	void CheckCameraStatus();
	virtual void OnEnterCamera() {};
	virtual void OnExitCamera() {};

	virtual int GetRenderLayer() { return 1; }
	virtual bool RenderOnPaused() { return true; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// Does this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	virtual ~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};
