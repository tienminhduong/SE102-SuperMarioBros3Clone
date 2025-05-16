#include "FirePiranhaPlant.h"
#include "FirePiranhaPlantBullet.h"
#include "Pipe.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "debug.h"

int CFirePiranhaPlant::GetAniID()
{
	int id = ID_ANI_FIRE_PIRANHA_DEFAULT + animationDirection + 4 * (state % 2);
	if (id < ID_ANI_FIRE_PIRANHA_DEFAULT || id > ID_ANI_FIRE_PIRANHA_RIGHT_BOTTOM_IDLE)
	{
		DebugOut(L"[ERROR] Invalid animation ID: %d, direction: %d, state: %d\n", id, animationDirection, state);
		return ID_ANI_FIRE_PIRANHA_DEFAULT;
	}
}

float CFirePiranhaPlant::SnapAngle(float angle, int& index)
{
	const int angleNum = 8;

	float result = shootAngles[0];
	index = 0;
	for (int i = 0; i < angleNum; i++)
	{
		if (abs(angle - shootAngles[i]) < abs(result - angle))
			result = shootAngles[i], index = i;
	}

	return result;
}

float CFirePiranhaPlant::FindAngleOfMario(int &index)
{
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);


	float dx = marioX - x;
	float dy = y - marioY;
	float angle = atan2(dy, dx) * 180 / PI;
	angle += 180.f;

	float result = SnapAngle(angle, index);
	return result;
}

void CFirePiranhaPlant::FireBullet()
{
	float shootingX = x, shootingY = y - FIRE_PIRANHA_SHOOTING_OFFSET;
	float dirX, dirY;

	int index;
	float angle = (FindAngleOfMario(index) - 180.f) * PI / 180.f;
	dirX = cos(angle);
	dirY = -sin(angle);

	LPGAMEOBJECT bullet = new CFirePiranhaPlantBullet(shootingX, shootingY, dirX, dirY);
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currentScene->AddNewObject(bullet);
	fired = true;
}

CFirePiranhaPlant::CFirePiranhaPlant(float x, float y, LPGAMEOBJECT mario, LPGAMEOBJECT pipe) : CGameObject(x, y)
{
	SetState(FIRE_PIRANHA_PLANT_STATE_UNACTIVE);
	this->mario = mario;
	this->pipe = pipe;
}

void CFirePiranhaPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = GetAniID();
	animations->Get(aniId)->Render(x, y);
}

void CFirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
	float distance = abs(marioX - x);

	switch (state)
	{
	case FIRE_PIRANHA_PLANT_STATE_UNACTIVE:
	{
		if (movingCountdown > 0)
			movingCountdown -= dt;
		if (movingCountdown <= 0 && distance > FIRE_PIRANHA_NOT_MOVE_UP_RANGE && IsOnCamera())
			SetState(FIRE_PIRANHA_PLANT_STATE_MOVING_UP);
	}
	break;

	case FIRE_PIRANHA_PLANT_STATE_MOVING_UP:
	{
		y += vy * dt;
		if (y + FIRE_PIRANHA_PLANT_BBOX_HEIGHT / 2 <= ((CPipe*)pipe)->GetTop())
			SetState(FIRE_PIRANHA_PLANT_STATE_IDLE);
	}
	break;

	case FIRE_PIRANHA_PLANT_STATE_IDLE:
	{
		movingCountdown -= dt;
		if (movingCountdown <= 0)
			SetState(FIRE_PIRANHA_PLANT_STATE_MOVING_DOWN);

		shootCountdown -= dt;
		if (shootCountdown <= 0 && !fired)
			FireBullet();
	}
	break;

	case FIRE_PIRANHA_PLANT_STATE_MOVING_DOWN:
		y += vy * dt;
		float pipeX, pipeY;
		pipe->GetPosition(pipeX, pipeY);
		if (y - FIRE_PIRANHA_PLANT_BBOX_HEIGHT / 2 >= pipeY)
			SetState(FIRE_PIRANHA_PLANT_STATE_UNACTIVE);
	}

	if (state % 2 != 0) // not moving state
	{
		int snapAngleIndex;
		float angle = FindAngleOfMario(snapAngleIndex);
		animationDirection = snapAngleIndex / 2;
	}
}

void CFirePiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == FIRE_PIRANHA_PLANT_STATE_UNACTIVE)
	{
		l = t = r = b = 0;
		return;
	}

	l = x - FIRE_PIRANHA_PLANT_BBOX_WIDTH / 2;
	t = y - FIRE_PIRANHA_PLANT_BBOX_HEIGHT / 2;
	r = l + FIRE_PIRANHA_PLANT_BBOX_WIDTH;
	b = t + FIRE_PIRANHA_PLANT_BBOX_HEIGHT;
}

void CFirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_PIRANHA_PLANT_STATE_MOVING_UP:
		vy = -FIRE_PIRANHA_MOVE_SPEED;
		break;
	case FIRE_PIRANHA_PLANT_STATE_IDLE:
		movingCountdown = FIRE_PIRANHA_MOVE_DOWN_TIME_COUNT;
		shootCountdown = FIRE_PIRANHA_SHOOT_TIME_COUNT;
		fired = false;
		break;
	case FIRE_PIRANHA_PLANT_STATE_MOVING_DOWN:
		vy = FIRE_PIRANHA_MOVE_SPEED;
		break;
	case FIRE_PIRANHA_PLANT_STATE_UNACTIVE:
		movingCountdown = FIRE_PIRANHA_MOVE_UP_TIME_COUNT;
		break;
	default:
		break;
	}
}
