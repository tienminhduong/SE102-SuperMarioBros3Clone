#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Square.h"
#include "QuestionMarkBlock.h"
#include "StaticObject.h"
#include "Pipe.h"
#include "RedKoopa.h"
#include "Paragoomba.h"
#include "GameManager.h"
#include "Wall.h"
#include "HardBrick.h"
#include "GoldBrick.h"
#include "KoopaParatroopa.h"
#include "BlackPipe.h"
#include "CardEndGame.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");

		objects.push_back(obj);
		obj = new CRaccoonTail(x, y);
		obj->SetActive(false);
		((CMario*)player)->SetTail((CRaccoonTail*)obj);

		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_PARAGOOMBA: obj = new CParagoomba(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopa(x, y); break;
	case OBJECT_TYPE_RED_KOOPA: obj = new CRedKoopa(x, y); break;
	case OBJECT_TYPE_KOOPA_PARATROOPA: obj = new CKoopaParatroopa(x, y); break;
	case OBJECT_TYPE_HARD_BRICK: obj = new CHardBrick(x, y); break;
	case OBJECT_TYPE_BLACK_PIPE: obj = new CBlackPipe(x, y); break;
	case OBJECT_TYPE_WIN_CARD: obj = new CardEndGame(x, y); break;

	case OBJECT_TYPE_GOLD_BRICK:
	{
		DebugOut(L"[INFO] Gold Brick object has been created!\n");
		int containBtn = atoi(tokens[3].c_str());
		obj = new CGoldBrick(x, y, containBtn);
		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_SQUARE:
	{
		int width = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());
		int colorID = atoi(tokens[5].c_str());
		obj = new CSquare(x, y, width, height, colorID);
	}
	break;

	case OBJECT_TYPE_PIPE:
	{
		int height = atoi(tokens[3].c_str());
		int type = atoi(tokens[4].c_str());
		int enterMapKey = 0;
		if (tokens.size() == 6)
			enterMapKey = atoi(tokens[5].c_str());
		obj = new CPipe(x, y, height, type, enterMapKey);
	}
	break;

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;

	case OBJECT_TYPE_QUESTION_BLOCK:
	{
		int containedObjId = atoi(tokens[3].c_str());
		obj = new CQuestionMarkBlock(x, y, containedObjId);
	}
		break;

	case OBJECT_TYPE_STATIC:
	{
		int ani_id = atoi(tokens[3].c_str());
		obj = new StaticObject(x, y, ani_id);
	}
	break;
	case OBJECT_TYPE_WALL:
	{
		int width = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());

		obj = new Wall(x, y, width, height);
	}
	break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->GetActive())
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetActive())
			objects[i]->Update(dt, &coObjects);

		objects[i]->CheckCameraStatus();
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;



	CMario* mario = (CMario*)player;
	if (!mario->CanRechargeEnergy() && mario->GetY() < CAM_MARIO_Y_BREAKPOINT)
		cam_is_moving = true;
	if (cam_is_moving)
	{
		cam_prev_y += mario->GetY() - mario_prev_y;
		cy = cam_prev_y;
	}
	else {
		cy = 0;
	}


	if (cx < CAM_MIN_X) cx = CAM_MIN_X;
	if (cx > CAM_MAX_X) cx = CAM_MAX_X;
	if (cy >= CAM_MAX_Y) cy = CAM_MAX_Y, cam_is_moving = false;

	if (CGame::GetInstance()->GetCurrentSceneIndex() == 1)
		cy = 0;

	CGame::GetInstance()->SetCamPos(cx, cy);

	mario_prev_x = mario->GetX();
	mario_prev_y = mario->GetY();
	cam_prev_y = cy;

	GameManager::GetInstance()->Update(dt);
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int layer = 0; layer < MAX_RENDER_LAYER; ++layer) {
		for (int i = 0; i < objects.size(); i++)
			if (objects[i]->GetActive() && layer == objects[i]->GetRenderLayer()
				&& !(GameManager::GetInstance()->isGamePaused && !objects[i]->RenderOnPaused()))
				objects[i]->Render();
	}

	GameManager::GetInstance()->RenderUI();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}