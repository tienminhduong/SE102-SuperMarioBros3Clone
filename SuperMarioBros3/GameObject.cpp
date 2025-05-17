#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
	isActive = true;
}

void CGameObject::SetActive(bool isActive)
{
	this->isActive = isActive;
	if (isActive)
		OnEnable();
	else
		OnDisable();
}

bool CGameObject::IsEnterCamera()
{
	bool prevCameraStatus = isOnCameraEnterCheck;
	isOnCameraEnterCheck = IsOnCamera();
	return isOnCameraEnterCheck && !prevCameraStatus;
}

bool CGameObject::IsOutCamera()
{
	bool prevCameraStatus = isOutCameraCheck;
	isOutCameraCheck = IsOnCamera();
	return !isOutCameraCheck && prevCameraStatus;
}

bool CGameObject::IsOnCamera()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	return cam_x + INGAME_CAMERA_WIDTH + 15 > x && cam_x - 15 < x &&
		cam_y + INGAME_CAMERA_HEIGHT > y && cam_y < y;
}

void CGameObject::CheckCameraStatus()
{
	if (IsEnterCamera())
		OnEnterCamera();
	if (IsOutCamera())
		OnExitCamera();
}

void CGameObject::RenderBoundingBox(float alpha)
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	float bboxX = (l + r) / 2;
	float bboxY = (t + b) / 2;
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	//CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
	//CGame::GetInstance()->Draw(l + rect.right / 2 - cx, y - cy, bbox, &rect, alpha);
	CGame::GetInstance()->Draw(bboxX - cx, bboxY - cy, bbox, &rect, alpha);
}

CGameObject::~CGameObject()
{

}