#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Paragoomba.h"
#include "Koopa.h"
#include "KoopaParatroopa.h"
#include "Coin.h"
#include "Portal.h"
#include "QuestionMarkBlock.h"
#include "TransformMushroom.h"
#include "TransformLeaf.h"
#include "FirePiranhaPlant.h"
#include "FirePiranhaPlantBullet.h"
#include "GoldBrick.h"
#include "GoldBrickButton.h"
#include "GameManager.h"
#include "Collision.h"
#include "Pipe.h"
#include "BlackPipe.h"

#define STAT_LOG_CONDITION 0

void CMario::SetTailPosition(DWORD dt)
{
	if (tail != nullptr && tail->GetActive() && rotatingAnimDuration < rotatingAnimMaxDuration)
	{
		float tail_offset_x = MARIO_RACCOON_TAIL_OFFSET_X;

		int t = rotatingAnimDuration, t0 = rotatingAnimMaxDuration;
		float c = 2.f * RACCOON_TAIL_BBOX_WIDTH / (MARIO_RACCOON_BBOX_WIDTH + RACCOON_TAIL_BBOX_WIDTH);
		tail_offset_x *= abs((4 + 2 * c) * (t - t0 / 2) / t0) - (1 + c);

		float tailX, tailY;
		tail->GetPosition(tailX, tailY);

		float newTailX = x - tail_offset_x * nx, newTailY = y + MARIO_RACCOON_TAIL_OFFSET_Y;

		tail->SetPosition(newTailX, newTailY, dt);
		if (rotatingAnimDuration + dt == rotatingAnimMaxDuration)
			tail->SetSpeed(0, 0);
	}
}

void CMario::SetHoldKoopa(CKoopa* koopa)
{
	if (holdingKoopa)
		return;

	holdingKoopa = koopa;
	holdingKoopa->SetHold(this);
}

void CMario::SetKoopaPosition(DWORD dt)
{
	if (holdingKoopa != nullptr)
	{
		float kx, ky;
		holdingKoopa->GetPosition(kx, ky);

		float t, l, r, b;
		this->GetBoundingBox(l, t, r, b);

		float kx2, ky2;
		kx2 = x + MARIO_KOOPA_HOLDING_OFFSET_X * nx;
		ky2 = y - MARIO_KOOPA_HOLDING_OFFSET_Y;

		float dx = kx2 - kx, dy = ky2 - ky;
		holdingKoopa->SetSpeed(dx / dt, dy / dt);

		holdingKoopa->SetPosition(kx2, ky2);
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (specialState_Uninterruptable)
	{
		vy = MARIO_FALL_SPEED_LIMIT * otherMapDirection / 10;
		y += vy * dt;
		CCollision::GetInstance()->Process(this, dt, coObjects);
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		if (x == GameManager::GetInstance()->GetPipeGoOutX() && b <= GameManager::GetInstance()->GetPipeGoOutY() - 8)
			specialState_Uninterruptable = false;
		return;
	}

	if (untouchableDuration > 0) {
		untouchableDuration -= dt;
		if (untouchableDuration <= 0)
		{
			notRenderSpriteFrameCount = 0;
		}
		else {
			notRenderSpriteFrameCount--;
			if (notRenderSpriteFrameCount == -MARIO_NOT_RENDER_MAX_FRAME_COUNT) {
				notRenderSpriteFrameCount = MARIO_NOT_RENDER_MAX_FRAME_COUNT;
			}
		}
	}

	if (transformAnimDuration > 0)
	{
		transformAnimDuration -= dt;
		return;
	}

	vy += ay * dt;
	if (vy > MARIO_FALL_SPEED_LIMIT)
		vy = MARIO_FALL_SPEED_LIMIT;
	if (isEnergyGeneratable)
		vx += ax * dt;

	if (!isOnPlatform && abs(vx) > MARIO_WALKING_SPEED && !IsFlying())
		vx = vx / abs(vx) * MARIO_WALKING_SPEED;

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
		if (flyCountDownTime > 0)
			vy = -vy;
		raccoonSlowFalling -= dt;
		tailFlapAnimationCurrentDuration -= dt;
	}

	if (flyCountDownTime > 0)
	{
		flyCountDownTime -= dt;
		if (flyCountDownTime <= 0)
			vx = (vx / abs(vx)) * MARIO_WALKING_SPEED;
	}

	if (kickAnimDuration > 0)
		kickAnimDuration -= dt;

	if (IsAttacking())
	{
		rotatingAnimDuration -= dt;
		if (rotatingAnimDuration <= 0)
			EndRaccoonAttack();
	}



	CCollision::GetInstance()->Process(this, dt, coObjects);

	SetTailPosition(dt);
	SetKoopaPosition(dt);

	if (x < 15)
		vx = 0, x = 15;

	// log charge
	//DebugOut(L"[CHARGE] ");
	//for (int i = 0; i < GetChargeInScale(7); ++i)
	//	DebugOut(L"=");
	//DebugOut(L"\n");

}

void CMario::OnNoCollision(DWORD dt)
{
	if (!isBlocked)
		x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
	isBlocked = false;
	enterHiddenMapKey = 0;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		if (e->ny < 0)
		{
			vy = 0;
			isOnPlatform = true;
			isEnergyGeneratable = true;
			raccoonSlowFalling = 0;
			flyCountDownTime = 0;
		}
		else {
			jumpedTime = MARIO_MAX_JUMP_TIME - 1;
			vy = -0.001f;
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		if (!isEnergyGeneratable)
			isBlocked = true;
		else
			vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CTransformMushroom*>(e->obj))
		OnCollisionWithTransformItem(e);
	else if (dynamic_cast<CTransformLeaf*>(e->obj))
		OnCollisionWithTransformItem(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithLifeUpMushroom(e);
	else if (dynamic_cast<CQuestionMarkBlock*>(e->obj))
		OnCollisionWithQuestionMarkBlock(e);
	else if (dynamic_cast<CGoldBrick*>(e->obj))
		OnCollisionWithGoldBrick(e);
	else if (dynamic_cast<CGoldBrickButton*>(e->obj))
		OnCollisionWithGoldBrickButton(e);
	else if (dynamic_cast<CFirePiranhaPlant*>(e->obj))
		TakeDamage();
	else if (dynamic_cast<CFirePiranhaPlantBullet*>(e->obj))
		TakeDamage();
	else if (dynamic_cast<CPipe*>(e->obj))
		OnCollisionWithPipe(e);
	else if (dynamic_cast<CBlackPipe*>(e->obj))
		OnCollisionWithBlackPipe(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() > GOOMBA_CHECKISPARAGOOMBA)
		{
			goomba->SetState(PARAGOOMBA_STATE_LOST_WINGS);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		GameManager::GetInstance()->IncreasePoint();
		CAnimations::GetInstance()->PlayEffect(ID_ANI_100_UP, x, y);
	}
	else // hit by Goomba
	{
		if (untouchableDuration <= 0 && goomba->GetState() != GOOMBA_STATE_DIE)
			TakeDamage();
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (koopa->IsHold())
		return;

	if (koopa->GetState() == KOOPA_STATE_INSHELL && readyToHoldKoopa && !koopa->IsHold() && e->nx != 0) {
		SetHoldKoopa(koopa);
		return;
	}

	if (koopa->GetState() == KOOPA_STATE_INSHELL)
	{
		float kx, ky;
		koopa->GetPosition(kx, ky);
		int direction;
		if (kx != x)
			direction = (int)((kx - x) / abs(kx - x));
		else direction = 1;
		koopa->SetPosition(kx + KOOPA_BBOX_WIDTH / 2 * direction, ky);
		koopa->GetKicked(direction);
		PlayKickKoopaAnim();
		return;
	}

	if (e->ny < 0)
	{
		if (koopa->GetState() != KOOPA_STATE_INSHELL && koopa->GetState() != ENEMY_STATE_KICKED) {
			if (koopa->GetState() == KOOPA_PARATROOPA_STATE_FLYING)
				koopa->SetState(KOOPA_STATE_WALKING);
			else
				koopa->SetState(KOOPA_STATE_INSHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else {
		if (koopa->GetState() != KOOPA_STATE_INSHELL && koopa->GetState() != ENEMY_STATE_KICKED
			&& untouchableDuration <= 0)
			TakeDamage();
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	GameManager::GetInstance()->CollectCoin();
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithTransformItem(LPCOLLISIONEVENT e)
{
	e->obj->SetActive(false);
	if (GetLevel() < MARIO_LEVEL_RACCOON)
		SetLevel(level + 1);

	CAnimations::GetInstance()->PlayEffect(ID_ANI_1000_UP, e->obj->GetX(), e->obj->GetY());
	GameManager::GetInstance()->IncreasePoint(1000);
}

void CMario::OnCollisionWithQuestionMarkBlock(LPCOLLISIONEVENT e)
{
	if (e->ny > 0) {
		CQuestionMarkBlock* q = (CQuestionMarkBlock*)e->obj;
		q->TriggerOnCollisionWithMario(x);
	}
}

void CMario::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
	CGoldBrick* gb = (CGoldBrick*)e->obj;

	if (gb->GetState() == GOLD_BRICK_STATE_COIN)
	{
		gb->Delete();
		GameManager::GetInstance()->CollectCoin();
		return;
	}

	if (e->ny > 0) {
		if (level > MARIO_LEVEL_SMALL)
			gb->TriggerOnCollision();
		else {
			gb->SetState(GOLD_BRICK_STATE_HIT_NOTBROKEN);
		}
	}
}

void CMario::OnCollisionWithGoldBrickButton(LPCOLLISIONEVENT e)
{
	CGoldBrickButton* gbbtn = (CGoldBrickButton*)e->obj;
	gbbtn->TriggerOnCollision();
}

void CMario::OnCollisionWithLifeUpMushroom(LPCOLLISIONEVENT e)
{
	e->obj->SetActive(false);
	GameManager::GetInstance()->IncreaseLife();
	CAnimations::GetInstance()->PlayEffect(ID_ANI_1_UP, e->obj->GetX(), e->obj->GetY());
}

void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
		enterHiddenMapKey = ((CPipe*)e->obj)->GetEnterMapKey(), snapXOtherMap = e->obj->GetX();
}

void CMario::OnCollisionWithBlackPipe(LPCOLLISIONEVENT e)
{
	if (e->ny > 0)
		enterHiddenMapKey = -1, snapXOtherMap = e->obj->GetX();
}

void CMario::TakeDamage()
{
	if (untouchableDuration > 0)
		return;
	if (level > MARIO_LEVEL_SMALL)
	{
		SetLevel(GetLevel() - 1);
		StartUntouchable();
	}
	else
	{
		DebugOut(L">>> Mario DIE >>> \n");
		SetState(MARIO_STATE_DIE);
	}
}

int mapAniId[][24] = {
	{
		ID_ANI_MARIO_SMALL_IDLE_RIGHT, ID_ANI_MARIO_SMALL_IDLE_LEFT,
		ID_ANI_MARIO_SMALL_WALKING_RIGHT, ID_ANI_MARIO_SMALL_WALKING_LEFT,
		ID_ANI_MARIO_SMALL_RUNNING_RIGHT, ID_ANI_MARIO_SMALL_RUNNING_LEFT,
		ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT,
		ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT, ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT,
		ID_ANI_MARIO_SMALL_BRACE_RIGHT, ID_ANI_MARIO_SMALL_BRACE_LEFT,
		ID_ANI_MARIO_SMALL_IDLE_RIGHT, ID_ANI_MARIO_SMALL_IDLE_LEFT, // small Mario has no sit animation
		ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT, // small Mario has no falling animation
		ID_ANI_MARIO_SMALL_HOLDING_IDLE_RIGHT, ID_ANI_MARIO_SMALL_HOLDING_IDLE_LEFT,
		ID_ANI_MARIO_SMALL_HOLDING_RUNNING_RIGHT, ID_ANI_MARIO_SMALL_HOLDING_RUNNING_LEFT,
		ID_ANI_MARIO_SMALL_HOLDING_JUMPING_RIGHT, ID_ANI_MARIO_SMALL_HOLDING_JUMPING_LEFT,
		ID_ANI_MARIO_SMALL_KICKING_RIGHT, ID_ANI_MARIO_SMALL_KICKING_LEFT,
	},
	{
		ID_ANI_MARIO_IDLE_RIGHT, ID_ANI_MARIO_IDLE_LEFT,
		ID_ANI_MARIO_WALKING_RIGHT, ID_ANI_MARIO_WALKING_LEFT,
		ID_ANI_MARIO_RUNNING_RIGHT, ID_ANI_MARIO_RUNNING_LEFT,
		ID_ANI_MARIO_JUMP_WALK_RIGHT, ID_ANI_MARIO_JUMP_WALK_LEFT,
		ID_ANI_MARIO_JUMP_RUN_RIGHT, ID_ANI_MARIO_JUMP_RUN_LEFT,
		ID_ANI_MARIO_BRACE_RIGHT, ID_ANI_MARIO_BRACE_LEFT,
		ID_ANI_MARIO_SIT_RIGHT, ID_ANI_MARIO_SIT_LEFT,
		ID_ANI_MARIO_FALLING_RIGHT, ID_ANI_MARIO_FALLING_LEFT,
		ID_ANI_MARIO_HOLDING_IDLE_RIGHT, ID_ANI_MARIO_HOLDING_IDLE_LEFT,
		ID_ANI_MARIO_HOLDING_RUNNING_RIGHT, ID_ANI_MARIO_HOLDING_RUNNING_LEFT,
		ID_ANI_MARIO_HOLDING_JUMPING_RIGHT, ID_ANI_MARIO_HOLDING_JUMPING_LEFT,
		ID_ANI_MARIO_KICKING_RIGHT, ID_ANI_MARIO_KICKING_LEFT,
	},
	{
		ID_ANI_MARIO_RACCOON_IDLE_RIGHT, ID_ANI_MARIO_RACCOON_IDLE_LEFT,
		ID_ANI_MARIO_RACCOON_WALKING_RIGHT, ID_ANI_MARIO_RACCOON_WALKING_LEFT,
		ID_ANI_MARIO_RACCOON_RUNNING_RIGHT, ID_ANI_MARIO_RACCOON_RUNNING_LEFT,
		ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT, ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT,
		ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT, ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT,
		ID_ANI_MARIO_RACCOON_BRACE_RIGHT, ID_ANI_MARIO_RACCOON_BRACE_LEFT,
		ID_ANI_MARIO_RACCOON_SIT_RIGHT, ID_ANI_MARIO_RACCOON_SIT_LEFT,
		ID_ANI_MARIO_RACCOON_FALLING_RIGHT, ID_ANI_MARIO_RACCOON_FALLING_LEFT,
		ID_ANI_MARIO_RACCOON_HOLDING_IDLE_RIGHT, ID_ANI_MARIO_RACCOON_HOLDING_IDLE_LEFT,
		ID_ANI_MARIO_RACCOON_HOLDING_RUNNING_RIGHT, ID_ANI_MARIO_RACCOON_HOLDING_RUNNING_LEFT,
		ID_ANI_MARIO_RACCOON_HOLDING_JUMPING_RIGHT, ID_ANI_MARIO_RACCOON_HOLDING_JUMPING_LEFT,
		ID_ANI_MARIO_RACCOON_KICKING_RIGHT, ID_ANI_MARIO_RACCOON_KICKING_LEFT,
	}
};

void CMario::GetAniIdAndAniSpeed(int &aniId, float& speed)
{
	aniId = -1, speed = 1.f;

	if (specialState_Uninterruptable)
	{
		aniId = ID_ANI_MARIO_INTO_HIDDEN_MAP;
		return;
	}

	if (transformAnimDuration > 0)
	{
		aniId = currentTransformAnim;
		return;
	}
	if (kickAnimDuration > 0)
	{
		if (nx > 0)
			aniId = MapAniTypeToId(ANI_MARIO_KICKING_RIGHT);
		else
			aniId = MapAniTypeToId(ANI_MARIO_KICKING_LEFT);
		return;
	}
	if (!isOnPlatform)
	{
		if (level == MARIO_LEVEL_RACCOON && IsFlying())
		{
			DebugOut(L"[ANIMATION] Mario is flying\n");
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;

			if (tailFlapAnimationCurrentDuration > 0)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_LEFT;
			}
			if (continuousTailFlap) {
				if (nx >= 0)
					aniId = ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_CONTINUOUS_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_CONTINUOUS_LEFT;
			}
		}
		else if (isSitting)
		{
			if (nx > 0)
				aniId = MapAniTypeToId(ANI_MARIO_SIT_RIGHT);
			else
				aniId = MapAniTypeToId(ANI_MARIO_SIT_LEFT);
		}
		else if (abs(vx) == MARIO_RUNNING_SPEED && tailFlapAnimationCurrentDuration <= 0)
		{
			if (nx >= 0)
				aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_JUMPING_RIGHT : ANI_MARIO_JUMP_RUN_RIGHT);
			else
				aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_JUMPING_LEFT : ANI_MARIO_JUMP_RUN_LEFT);
		}
		else {
			if (vy > 0) {
				if (nx >= 0)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_JUMPING_RIGHT : ANI_MARIO_FALLING_RIGHT);
				else
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_JUMPING_LEFT : ANI_MARIO_FALLING_LEFT);
				if (level == MARIO_LEVEL_RACCOON) {
					if (tailFlapAnimationCurrentDuration > 0)
					{
						if (nx >= 0)
							aniId = ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_RIGHT;
						else
							aniId = ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_LEFT;
					}
					if (continuousTailFlap) {
						if (nx >= 0)
							aniId = ID_ANI_MARIO_RACCOON_TAIL_FLAP_CONTINUOUS_RIGHT;
						else
							aniId = ID_ANI_MARIO_RACCOON_TAIL_FLAP_CONTINUOUS_LEFT;
					}
				}
			}
			else
			{
				if (nx >= 0)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_JUMPING_RIGHT : ANI_MARIO_JUMP_WALK_RIGHT);
				else
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_JUMPING_LEFT : ANI_MARIO_JUMP_WALK_LEFT);
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
				if (nx > 0) aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_IDLE_RIGHT : ANI_MARIO_IDLE_RIGHT);
				else aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_IDLE_LEFT : ANI_MARIO_IDLE_LEFT);
			}
			else if (vx > 0)
			{
				if (ax < 0 && abs(ax) != MARIO_FRICTION)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_RUNNING_LEFT : ANI_MARIO_BRACE_RIGHT);
				else if (vx == MARIO_RUNNING_SPEED)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_RUNNING_RIGHT : ANI_MARIO_RUNNING_RIGHT), speed = 2.f;
				else if (ax == MARIO_ACCEL_WALK_X || abs(ax) == MARIO_FRICTION)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_RUNNING_RIGHT : ANI_MARIO_WALKING_RIGHT), speed = 1.5f;
			}
			else // vx < 0
			{
				if (ax > 0 && abs(ax) != MARIO_FRICTION)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_RUNNING_RIGHT : ANI_MARIO_BRACE_LEFT);
				else if (vx == -MARIO_RUNNING_SPEED)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_RUNNING_LEFT : ANI_MARIO_RUNNING_LEFT), speed = 2.f;
				else if (ax == -MARIO_ACCEL_WALK_X || abs(ax) == MARIO_FRICTION)
					aniId = MapAniTypeToId(IsHoldingKoopa() ? ANI_MARIO_HOLDING_RUNNING_LEFT : ANI_MARIO_WALKING_LEFT), speed = 1.5f;
			}
	}
	if (IsAttacking()) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACCOON_ROTATING_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACCOON_ROTATING_LEFT;
		speed = 1.f;
	}
	if (aniId == -1)
		if (IsHoldingKoopa())
			aniId = nx == 1 ? MapAniTypeToId(ANI_MARIO_HOLDING_IDLE_RIGHT) : MapAniTypeToId(ANI_MARIO_HOLDING_IDLE_LEFT);
		else
			aniId = nx == 1 ? MapAniTypeToId(ANI_MARIO_IDLE_RIGHT) : MapAniTypeToId(ANI_MARIO_IDLE_LEFT);

	//log aniId
	//DebugOut(L"[ANIMATION] Mario aniId: %d, speed: %f\n", aniId, speed);
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
		GetAniIdAndAniSpeed(aniId, speed);

	//DebugOut(L"[ANIMATION]\nCurrent Mario level: %d\nCurrent state: %d\nCurrent aniId: %d\nCurrent frame rate: %f\n",
		//level, state, aniId, DEFAULT_FRAME_TIME * modifier);
	animations->Get(aniId)->SetSpeed(speed);
	if (notRenderSpriteFrameCount <= 0)
		animations->Get(aniId)->Render(x, y);

	if (!isEnergyGeneratable)
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

		if (abs(vx) > MARIO_WALKING_SPEED && abs(vx) < MARIO_RUNNING_SPEED)
			vx = MARIO_WALKING_SPEED * nx;

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
	auto animation = CAnimations::GetInstance()->Get(nx > 0 ? ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_RIGHT
														: ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_LEFT);
	tailFlapAnimationCurrentDuration = animation->GetDuration();
	//animation->Reset();
}

void CMario::TriggerRaccoonFLy()
{
	if (level != MARIO_LEVEL_RACCOON || !IsFlyable() || flyCountDownTime > 0)
		return;

	flyCountDownTime = MARIO_FLY_TIME_LIMIT;
	isEnergyGeneratable = false;
}

void CMario::TriggerRaccoonAttack()
{
	if (level != MARIO_LEVEL_RACCOON || rotatingAnimDuration > 0 || state == MARIO_STATE_SIT || !isEnergyGeneratable)
		return;

	auto animation = CAnimations::GetInstance()
		->Get(nx > 0 ? ID_ANI_MARIO_RACCOON_ROTATING_RIGHT : ID_ANI_MARIO_RACCOON_ROTATING_LEFT);
	rotatingAnimMaxDuration = animation->GetDuration();
	animation->Reset();
	rotatingAnimDuration = rotatingAnimMaxDuration;
	tail->SetActive(true);
}

void CMario::EndRaccoonAttack()
{
	tail->SetActive(false);
}

void CMario::TriggerSmallJump()
{
	if (!isOnPlatform)
		return;
	SetState(MARIO_STATE_JUMP);
	jumpedTime = MARIO_MAX_JUMP_TIME / 4;
}

void CMario::SetReadyHoldKoopa(bool value)
{
	this->readyToHoldKoopa = value;

	// Release koopa if holding
	if (!value && holdingKoopa != nullptr)
		ReleaseKoopa();
}

void CMario::ReleaseKoopa()
{
	if (holdingKoopa != nullptr)
	{
		holdingKoopa->SetHold(nullptr);
		if (holdingKoopa->GetState() == KOOPA_STATE_INSHELL)
		{
			holdingKoopa->GetKicked(nx);
			PlayKickKoopaAnim();
		}
		else if (holdingKoopa->GetState() != ENEMY_STATE_KICKED) {
			TakeDamage();
		}
		holdingKoopa = nullptr;
	}
}

void CMario::PlayKickKoopaAnim()
{
	CAnimations* animations = CAnimations::GetInstance();
	kickAnimDuration = animations->Get(mapAniId[level - 1][ANI_MARIO_KICKING_RIGHT])->GetDuration();
}

float CMario::GetChargePercent()
{
	float avx = abs(vx);
	if (avx < MARIO_WALKING_SPEED)
		return 0;

	avx -= MARIO_WALKING_SPEED;
	float maxCharge = MARIO_RUNNING_SPEED - MARIO_WALKING_SPEED;
	return avx / maxCharge;
}

int CMario::GetChargeInScale(int maxValue)
{
	return (int)(GetChargePercent() * maxValue);
}

void CMario::ChangeDirection(float direction)
{
	nx = (int)direction;
	vx = abs(vx) * nx;
	ax = abs(ax) * nx;
}

void CMario::SetGoToOtherMap(int direction)
{
	specialState_Uninterruptable = true;
	otherMapDirection = direction;
	x = snapXOtherMap + 2;
}


void CMario::SetLevel(int l, bool noAnim)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2.f;

	if (level == MARIO_LEVEL_RACCOON && l != level && tail != nullptr)
		tail->SetActive(false);
	if (!noAnim) {
		if (l == MARIO_LEVEL_SMALL)
		{
			currentTransformAnim = nx > 0 ? ID_ANI_MARIO_TRANSFORM_TO_SMALL_RIGHT : ID_ANI_MARIO_TRANSFORM_TO_SMALL_LEFT;
		}
		else if (l == MARIO_LEVEL_BIG)
		{
			if (level == MARIO_LEVEL_SMALL)
				currentTransformAnim = nx > 0 ? ID_ANI_MARIO_TRANSFORM_TO_BIG_RIGHT : ID_ANI_MARIO_TRANSFORM_TO_BIG_LEFT;
			else
				currentTransformAnim = ID_ANI_MARIO_TRANSFORM_RACCOON_SMOKE;
		}
		else if (l == MARIO_LEVEL_RACCOON)
		{
			currentTransformAnim = ID_ANI_MARIO_TRANSFORM_RACCOON_SMOKE;
		}
		transformAnimDuration = CAnimations::GetInstance()->Get(currentTransformAnim)->GetDuration();
		CAnimations::GetInstance()->Get(currentTransformAnim)->Reset();
	}
	level = l;
}

