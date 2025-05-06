#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"

#define STAT_LOG_CONDITION 0

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	// If Mario is in idle state, and velocity is not in the same direction as the Mario,
	// stop Mario since the friction make its velocity down to below 0
	if (state == MARIO_STATE_IDLE || state == MARIO_STATE_SIT)
	{
		if (vx * nx < 0) vx = ax = 0;
		if (vy <= -MARIO_JUMP_SPEED_Y)
		{
			if (ay != 0) ay = 0;
		}
	}
	
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// Jumping logic
	if (vy < 0) {
		if (jumpedTime < MARIO_MAX_JUMP_TIME) {
			jumpedTime += dt;
			if (jumpedTime >= MARIO_MAX_JUMP_TIME / 2)
				ay = 0;
			if (jumpedTime >= MARIO_MAX_JUMP_TIME)
				ay = MARIO_GRAVITY;
		}
	}

	// Tail flapping animation
	if (raccoonSlowFalling > 0)
	{
		vy = MARIO_JUMP_SPEED_Y / 10;
		raccoonSlowFalling -= dt;
		tailFlapAnimationCurrentDuration -= dt;
	}

	if (rotatingAnimDuration > 0) rotatingAnimDuration -= dt;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (STAT_LOG_CONDITION) {
		DebugOut(L"==========[CONDITION LOG]==========\n");
		if (state != 0)
			DebugOut(L"Mario state: %d\n", state);
		DebugOut(L"[JUMP STAT] vy: %f, ay: %f, jumpedTime: %d\n", vy, ay, jumpedTime);
		DebugOut(L"[WALKING STAT]: vx: %f, ax: %f\n", vx, ax);
		DebugOut(L"==========[END CONDITION LOG]==========\n");
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			isOnPlatform = true;
			raccoonSlowFalling = 0;
		}
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

int mapAniId[][16] = {
	{
		ID_ANI_MARIO_SMALL_IDLE_RIGHT, ID_ANI_MARIO_SMALL_IDLE_LEFT,
		ID_ANI_MARIO_SMALL_WALKING_RIGHT, ID_ANI_MARIO_SMALL_WALKING_LEFT,
		ID_ANI_MARIO_SMALL_RUNNING_RIGHT, ID_ANI_MARIO_SMALL_RUNNING_LEFT,
		ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT,
		ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT, ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT,
		ID_ANI_MARIO_SMALL_BRACE_RIGHT, ID_ANI_MARIO_SMALL_BRACE_LEFT,
		ID_ANI_MARIO_SMALL_IDLE_RIGHT, ID_ANI_MARIO_SMALL_IDLE_LEFT, // small Mario has no sit animation
		ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT // small Mario has no falling animation
	},
	{
		ID_ANI_MARIO_IDLE_RIGHT, ID_ANI_MARIO_IDLE_LEFT,
		ID_ANI_MARIO_WALKING_RIGHT, ID_ANI_MARIO_WALKING_LEFT,
		ID_ANI_MARIO_RUNNING_RIGHT, ID_ANI_MARIO_RUNNING_LEFT,
		ID_ANI_MARIO_JUMP_WALK_RIGHT, ID_ANI_MARIO_JUMP_WALK_LEFT,
		ID_ANI_MARIO_JUMP_RUN_RIGHT, ID_ANI_MARIO_JUMP_RUN_LEFT,
		ID_ANI_MARIO_BRACE_RIGHT, ID_ANI_MARIO_BRACE_LEFT,
		ID_ANI_MARIO_SIT_RIGHT, ID_ANI_MARIO_SIT_LEFT,
		ID_ANI_MARIO_FALLING_RIGHT, ID_ANI_MARIO_FALLING_LEFT
	},
	{
		ID_ANI_MARIO_RACCOON_IDLE_RIGHT, ID_ANI_MARIO_RACCOON_IDLE_LEFT,
		ID_ANI_MARIO_RACCOON_WALKING_RIGHT, ID_ANI_MARIO_RACCOON_WALKING_LEFT,
		ID_ANI_MARIO_RACCOON_RUNNING_RIGHT, ID_ANI_MARIO_RACCOON_RUNNING_LEFT,
		ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT, ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT,
		ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT, ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT,
		ID_ANI_MARIO_RACCOON_BRACE_RIGHT, ID_ANI_MARIO_RACCOON_BRACE_LEFT,
		ID_ANI_MARIO_RACCOON_SIT_RIGHT, ID_ANI_MARIO_RACCOON_SIT_LEFT,
		ID_ANI_MARIO_RACCOON_FALLING_RIGHT, ID_ANI_MARIO_RACCOON_FALLING_LEFT
	}
};

void CMario::GetAniIdAndSpeed(int &aniId, float& speed)
{
	aniId = -1, speed = 1.f;
	if (!isOnPlatform)
	{
		if (isSitting)
		{
			if (nx > 0)
				aniId = MapAniTypeToId(ANI_MARIO_SIT_RIGHT);
			else
				aniId = MapAniTypeToId(ANI_MARIO_SIT_LEFT);
		}
		else if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MapAniTypeToId(ANI_MARIO_JUMP_RUN_RIGHT);
			else
				aniId = MapAniTypeToId(ANI_MARIO_JUMP_RUN_LEFT);
		}
		else {
			if (vy > 0) {
				if (nx >= 0)
					aniId = MapAniTypeToId(ANI_MARIO_FALLING_RIGHT);
				else
					aniId = MapAniTypeToId(ANI_MARIO_FALLING_LEFT);
				if (tailFlapAnimationCurrentDuration > 0)
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_RIGHT;
					else
						aniId = ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_LEFT;
				}
			}
			else
			{
				if (nx >= 0)
					aniId = MapAniTypeToId(ANI_MARIO_JUMP_WALK_RIGHT);
				else
					aniId = MapAniTypeToId(ANI_MARIO_JUMP_WALK_LEFT);
			}
		}
	}
	else {
		if (isSitting)
		{
			if (nx > 0)
				aniId = MapAniTypeToId(ANI_MARIO_SIT_RIGHT);
			else
				aniId = MapAniTypeToId(ANI_MARIO_SIT_LEFT);
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = MapAniTypeToId(ANI_MARIO_IDLE_RIGHT);
				else aniId = MapAniTypeToId(ANI_MARIO_IDLE_LEFT);
			}
			else if (vx > 0)
			{
				if (ax < 0 && abs(ax) != MARIO_FRICTION)
					aniId = MapAniTypeToId(ANI_MARIO_BRACE_RIGHT);
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = MapAniTypeToId(ANI_MARIO_RUNNING_RIGHT), speed = 2.f;
				else if (ax == MARIO_ACCEL_WALK_X || abs(ax) == MARIO_FRICTION)
					aniId = MapAniTypeToId(ANI_MARIO_WALKING_RIGHT), speed = 1.5f;
			}
			else // vx < 0
			{
				if (ax > 0 && abs(ax) != MARIO_FRICTION)
					aniId = MapAniTypeToId(ANI_MARIO_BRACE_LEFT);
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = MapAniTypeToId(ANI_MARIO_RUNNING_LEFT), speed = 2.f;
				else if (ax == -MARIO_ACCEL_WALK_X || abs(ax) == MARIO_FRICTION)
					aniId = MapAniTypeToId(ANI_MARIO_WALKING_LEFT), speed = 1.5f;
			}
	}
	if (rotatingAnimDuration > 0) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACCOON_ROTATING_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACCOON_ROTATING_LEFT;
		speed = 1.f;
	}

	if (aniId == -1) aniId = nx == 1 ? MapAniTypeToId(ANI_MARIO_IDLE_RIGHT) : MapAniTypeToId(ANI_MARIO_IDLE_LEFT);
}

int CMario::MapAniTypeToId(int animation_type)
{
	return mapAniId[level - 1][animation_type];
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	float speed = 1.f;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else
		GetAniIdAndSpeed(aniId, speed);

	//DebugOut(L"[ANIMATION]\nCurrent Mario level: %d\nCurrent state: %d\nCurrent aniId: %d\nCurrent frame rate: %f\n",
		//level, state, aniId, DEFAULT_FRAME_TIME * modifier);
	animations->Get(aniId)->SetSpeed(speed);
	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting)
			SetState(MARIO_STATE_SIT_RELEASE);
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting)
			SetState(MARIO_STATE_SIT_RELEASE);
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting)
			SetState(MARIO_STATE_SIT_RELEASE);
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting)
			SetState(MARIO_STATE_SIT_RELEASE);
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (!isOnPlatform) break;

		if (abs(this->vx) == MARIO_RUNNING_SPEED)
			vy = -MARIO_JUMP_RUN_SPEED_Y;
		else
			ay = -MARIO_ACCEL_JUMP, jumpedTime = 0;

		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (jumpedTime > 0)
			jumpedTime = min(jumpedTime + MARIO_MAX_JUMP_TIME / 2, MARIO_MAX_JUMP_TIME - 1);
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL && !isSitting)
		{
			isSitting = true;
			vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
			SetState(MARIO_STATE_IDLE);
		}

		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = (-1) * nx * MARIO_FRICTION;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		if (level == MARIO_LEVEL_RACCOON) {
			if (nx == 1) {
				left += MARIO_RACCOON_BBOX_OFFSET_X;
				right += MARIO_RACCOON_BBOX_OFFSET_X;
			}
			else {
				left -= MARIO_RACCOON_BBOX_OFFSET_X;
				right -= MARIO_RACCOON_BBOX_OFFSET_X;
			}
		}
	}
}

void CMario::TriggerRaccoonSlowFalling()
{
	if (level != MARIO_LEVEL_RACCOON)
		return;

	raccoonSlowFalling = MARIO_SLOW_FALLING_TIME;
	tailFlapAnimationCurrentDuration = CAnimations::GetInstance()->Get(ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_RIGHT)->GetDuration();
}

void CMario::TriggerRotate()
{
	if (level != MARIO_LEVEL_RACCOON || rotatingAnimDuration > 0)
		return;
	rotatingAnimDuration = CAnimations::GetInstance()->Get(ID_ANI_MARIO_RACCOON_ROTATING_RIGHT)->GetDuration();
	DebugOut(L"Triggered!, duration: %d\n", rotatingAnimDuration);
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

