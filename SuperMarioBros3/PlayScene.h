#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "RaccoonTail.h"

#define CAM_MIN_X 0
#define CAM_MIN_Y -236
#define CAM_MAX_X 2596
#define CAM_MAX_Y 0
#define CAM_MARIO_Y_BREAKPOINT 46

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	float cam_prev_y = -1;
	bool cam_is_moving = false;
	float mario_prev_x = 0;
	float mario_prev_y = 0;
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	void AddNewObject(LPGAMEOBJECT obj) { objects.push_back(obj); }

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

