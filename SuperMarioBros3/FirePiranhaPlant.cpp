#include "FirePiranhaPlant.h"
#include "Pipe.h"
#include "debug.h"

int FirePiranhaPlant::GetAniID()
{
	int id = ID_ANI_FIRE_PIRANHA_DEFAULT + animationDirection + 4 * (state % 2);
	if (id < ID_ANI_FIRE_PIRANHA_DEFAULT || id > ID_ANI_FIRE_PIRANHA_RIGHT_BOTTOM_IDLE)
	{
		DebugOut(L"[ERROR] Invalid animation ID: %d\n", id);
		return ID_ANI_FIRE_PIRANHA_DEFAULT;
	}
}

float FirePiranhaPlant::SnapAngle(float angle, int& index)
{
	const int angleNum = 8;

	float result = shootAngles[0];
	for (int i = 0; i < angleNum; i++)
	{
		//DebugOut(L"angle: %f, a: %f, res1: %f, res2: %f\n", angle, availableAngles[i], res1, res2);
		if (abs(angle - shootAngles[i]) < abs(result - angle))
			result = shootAngles[i], index = i;
	}

	return result;
}

float FirePiranhaPlant::FindAngleOfMario(int &index)
{
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);

	const float pi = 3.1415926535898f;

	float dx = marioX - x;
	float dy = marioY - y;
	float angle = dx != 0 ? ((atan(dy / dx) + pi / 2) * 180 / pi) : (dy < 0 ? 90 : 270);

	float result = SnapAngle(marioX < x ? angle : angle + 180, index);
	DebugOut(L"angle: %f, result: %f\n", angle, result);
	return result;
}

FirePiranhaPlant::FirePiranhaPlant(float x, float y, LPGAMEOBJECT mario, LPGAMEOBJECT pipe) : CGameObject(x, y)
{
	SetState(FIRE_PIRANHA_PLANT_STATE_UNACTIVE);
	this->mario = mario;
	this->pipe = pipe;
}

void FirePiranhaPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = GetAniID();
	// log animation id
	DebugOut(L"aniId: %d\n", aniId);
	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
	float distance = abs(marioX - x);

	//DebugOut(L"movingCountdown: %d, distance: %f, STATE: %d\n", movingCountdown, distance, state);
	switch (state)
	{
	case FIRE_PIRANHA_PLANT_STATE_UNACTIVE:
	{
		if (movingCountdown > 0)
			movingCountdown -= dt;
		if (movingCountdown <= 0 && distance > FIRE_PIRANHA_NOT_MOVE_UP_RANGE)
			SetState(FIRE_PIRANHA_PLANT_STATE_MOVING_UP);
	}
	break;

	case FIRE_PIRANHA_PLANT_STATE_MOVING_UP:
	{
		y += vy * dt;
		if (y + FIRE_PIRANHA_PLANT_BBOX_HEIGHT / 2 <= ((CPipe*)pipe)->GetTop())
		{
			movingCountdown = FIRE_PIRANHA_MOVE_TIME_COUNT;
			SetState(FIRE_PIRANHA_PLANT_STATE_IDLE);
		}
	}
	break;

	case FIRE_PIRANHA_PLANT_STATE_IDLE:
	{
		//movingCountdown -= dt;
		if (movingCountdown <= 0)
			SetState(FIRE_PIRANHA_PLANT_STATE_MOVING_DOWN);
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
		DebugOut(L"index: %d\n", animationDirection);
	}
}

void FirePiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIRE_PIRANHA_PLANT_BBOX_WIDTH / 2;
	t = y - FIRE_PIRANHA_PLANT_BBOX_HEIGHT / 2;
	r = l + FIRE_PIRANHA_PLANT_BBOX_WIDTH;
	b = t + FIRE_PIRANHA_PLANT_BBOX_HEIGHT;
}

void FirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_PIRANHA_PLANT_STATE_MOVING_UP:
		vy = -FIRE_PIRANHA_MOVE_SPEED;
		break;
	case FIRE_PIRANHA_PLANT_STATE_IDLE:

		break;
	case FIRE_PIRANHA_PLANT_STATE_MOVING_DOWN:
		vy = FIRE_PIRANHA_MOVE_SPEED;
		break;
	case FIRE_PIRANHA_PLANT_STATE_UNACTIVE:
		movingCountdown = FIRE_PIRANHA_MOVE_TIME_COUNT;
		break;
	default:
		break;
	}
}
