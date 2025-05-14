#pragma once
#include "GameObject.h"

#define FIRE_PIRANHA_PLANT_BBOX_WIDTH 17
#define FIRE_PIRANHA_PLANT_BBOX_HEIGHT 33

#define FIRE_PIRANHA_PLANT_STATE_MOVING_UP 0
#define FIRE_PIRANHA_PLANT_STATE_IDLE 1
#define FIRE_PIRANHA_PLANT_STATE_MOVING_DOWN 2
#define FIRE_PIRANHA_PLANT_STATE_UNACTIVE 3

#define ID_ANI_FIRE_PIRANHA_DEFAULT 130000
#define ID_ANI_FIRE_PIRANHA_LEFT_BOTTOM_MOVING 130000
#define ID_ANI_FIRE_PIRANHA_LEFT_TOP_MOVING 130001
#define ID_ANI_FIRE_PIRANHA_RIGHT_TOP_MOVING 130002
#define ID_ANI_FIRE_PIRANHA_RIGHT_BOTTOM_MOVING 130003

#define ID_ANI_FIRE_PIRANHA_LEFT_BOTTOM_IDLE 130004
#define ID_ANI_FIRE_PIRANHA_LEFT_TOP_IDLE 130005
#define ID_ANI_FIRE_PIRANHA_RIGHT_TOP_IDLE 130006
#define ID_ANI_FIRE_PIRANHA_RIGHT_BOTTOM_IDLE 130007

#define FIRE_PIRANHA_ANI_DIR_LEFT_BOTTOM 0
#define FIRE_PIRANHA_ANI_DIR_LEFT_TOP 1
#define FIRE_PIRANHA_ANI_DIR_RIGHT_TOP 2
#define FIRE_PIRANHA_ANI_DIR_RIGHT_BOTTOM 3

#define FIRE_PIRANHA_NOT_MOVE_UP_RANGE 27
#define FIRE_PIRANHA_MOVE_UP_TIME_COUNT 2000 // 2000ms
#define FIRE_PIRANHA_MOVE_DOWN_TIME_COUNT 3000 // 3000ms
#define FIRE_PIRANHA_SHOOT_TIME_COUNT 2000 //2000ms

#define FIRE_PIRANHA_MOVE_SPEED 0.05f

#define FIRE_PIRANHA_SHOOTING_OFFSET 9


class CFirePiranhaPlant :
    public CGameObject
{
	int animationDirection = 0;
	int GetAniID();
	LPGAMEOBJECT mario;
	LPGAMEOBJECT pipe;
	int movingCountdown = 0;
	int shootCountdown = 0;
	bool fired = false;

	float SnapAngle(float angle, int& index);
	float FindAngleOfMario(int& index);
	void FireBullet();
	vector<float> shootAngles = { 20, 45, 135, 160, 200, 225, 315, 340 };
public:
	CFirePiranhaPlant(float x, float y, LPGAMEOBJECT mario, LPGAMEOBJECT pipe);
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void SetState(int state) override;
	int IsBlocking() override { return 0; }
	//int GetRenderLayer() override { return 2; }
};

