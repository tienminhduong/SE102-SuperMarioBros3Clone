#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "RaccoonTail.h"
#include "Koopa.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.00015f
#define MARIO_ACCEL_RUN_X	0.00015f
#define MARIO_ACCEL_JUMP	0.00266666666666666f

#define MARIO_FRICTION	0.0002f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f
#define MARIO_MAX_JUMP_TIME 200

#define MARIO_GRAVITY			0.0015f


#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_NOT_RENDER_MAX_FRAME_COUNT 3

#define MARIO_FLY_TIME_LIMIT 5000 // 5s

// RACCOON TAIL FLAPPING WHILE FALLING
#define MARIO_SLOW_FALLING_TIME 220
#define MARIO_TAIL_FLAP_ANIM_FRAME_COUNT 1

#pragma region ANIMATION_TYPE

#define ANI_MARIO_IDLE_RIGHT 0
#define ANI_MARIO_IDLE_LEFT 1
#define ANI_MARIO_WALKING_RIGHT 2
#define ANI_MARIO_WALKING_LEFT 3
#define ANI_MARIO_RUNNING_RIGHT 4
#define ANI_MARIO_RUNNING_LEFT 5
#define ANI_MARIO_JUMP_WALK_RIGHT 6
#define ANI_MARIO_JUMP_WALK_LEFT 7
#define ANI_MARIO_JUMP_RUN_RIGHT 8
#define ANI_MARIO_JUMP_RUN_LEFT 9
#define ANI_MARIO_BRACE_RIGHT 10
#define ANI_MARIO_BRACE_LEFT 11
#define ANI_MARIO_SIT_RIGHT 12
#define ANI_MARIO_SIT_LEFT 13
#define ANI_MARIO_FALLING_RIGHT 14
#define ANI_MARIO_FALLING_LEFT 15
#define ANI_MARIO_HOLDING_IDLE_RIGHT 16
#define ANI_MARIO_HOLDING_IDLE_LEFT 17
#define ANI_MARIO_HOLDING_RUNNING_RIGHT 18
#define ANI_MARIO_HOLDING_RUNNING_LEFT 19
#define ANI_MARIO_HOLDING_JUMPING_RIGHT 20
#define ANI_MARIO_HOLDING_JUMPING_LEFT 21
#define ANI_MARIO_KICKING_RIGHT 22
#define ANI_MARIO_KICKING_LEFT 23

#pragma endregion

#pragma region ANIMATION_ID

// BIG MARIO
#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_FALLING_RIGHT 1093
#define ID_ANI_MARIO_FALLING_LEFT 1094

#define ID_ANI_MARIO_HOLDING_IDLE_RIGHT 1095
#define ID_ANI_MARIO_HOLDING_IDLE_LEFT 1096

#define ID_ANI_MARIO_HOLDING_RUNNING_RIGHT 1097
#define ID_ANI_MARIO_HOLDING_RUNNING_LEFT 1098

#define ID_ANI_MARIO_HOLDING_JUMPING_RIGHT 1083
#define ID_ANI_MARIO_HOLDING_JUMPING_LEFT 1084

#define ID_ANI_MARIO_KICKING_RIGHT 1085
#define ID_ANI_MARIO_KICKING_LEFT 1086

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_HOLDING_IDLE_RIGHT 1651
#define ID_ANI_MARIO_SMALL_HOLDING_IDLE_LEFT 1652

#define ID_ANI_MARIO_SMALL_HOLDING_RUNNING_RIGHT 1671
#define ID_ANI_MARIO_SMALL_HOLDING_RUNNING_LEFT 1672

#define ID_ANI_MARIO_SMALL_HOLDING_JUMPING_RIGHT 1681
#define ID_ANI_MARIO_SMALL_HOLDING_JUMPING_LEFT 1682

#define ID_ANI_MARIO_SMALL_KICKING_RIGHT 1693
#define ID_ANI_MARIO_SMALL_KICKING_LEFT 1694

// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 1700
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 1701

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 1702
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 1703

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 1704
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 1705

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 1706
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 1707


#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 1710
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 1711

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 1712
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 1713

#define ID_ANI_MARIO_RACCOON_FALLING_RIGHT 1714
#define ID_ANI_MARIO_RACCOON_FALLING_LEFT 1715

#define ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_RIGHT 1716
#define ID_ANI_MARIO_RACCOON_FALL_TAIL_FLAP_LEFT 1717

#define	ID_ANI_MARIO_RACCOON_ROTATING_RIGHT 1718
#define	ID_ANI_MARIO_RACCOON_ROTATING_LEFT 1719

#define ID_ANI_MARIO_RACCOON_TAIL_FLAP_CONTINUOUS_RIGHT 1720
#define ID_ANI_MARIO_RACCOON_TAIL_FLAP_CONTINUOUS_LEFT 1721

#define ID_ANI_MARIO_RACCOON_HOLDING_IDLE_RIGHT 1722
#define ID_ANI_MARIO_RACCOON_HOLDING_IDLE_LEFT 1723

#define ID_ANI_MARIO_RACCOON_HOLDING_RUNNING_RIGHT 1724
#define ID_ANI_MARIO_RACCOON_HOLDING_RUNNING_LEFT 1725

#define ID_ANI_MARIO_RACCOON_HOLDING_JUMPING_RIGHT 1726
#define ID_ANI_MARIO_RACCOON_HOLDING_JUMPING_LEFT 1727

#define ID_ANI_MARIO_RACCOON_KICKING_RIGHT 1728
#define ID_ANI_MARIO_RACCOON_KICKING_LEFT 1729

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 1730
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 1731

#define ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_RIGHT 1732
#define ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_LEFT 1733

#define ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_CONTINUOUS_RIGHT 1734
#define ID_ANI_MARIO_RACCOON_FLY_TAIL_FLAP_CONTINUOUS_LEFT 1735


// TRANSFORM
#define ID_ANI_MARIO_TRANSFORM_TO_BIG_RIGHT 1800
#define ID_ANI_MARIO_TRANSFORM_TO_BIG_LEFT 1801

#define ID_ANI_MARIO_TRANSFORM_RACCOON_SMOKE 1802

#define ID_ANI_MARIO_TRANSFORM_TO_SMALL_RIGHT 1803
#define ID_ANI_MARIO_TRANSFORM_TO_SMALL_LEFT 1804

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON 3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_RACCOON_BBOX_WIDTH  13
#define MARIO_RACCOON_BBOX_HEIGHT 24
#define MARIO_RACCOON_BBOX_OFFSET_X 3
#define MARIO_RACCOON_TAIL_OFFSET_X 7
#define MARIO_RACCOON_TAIL_OFFSET_Y 6


#define MARIO_UNTOUCHABLE_TIME 2500

#define MARIO_KOOPA_HOLDING_OFFSET_X 13
#define MARIO_KOOPA_HOLDING_OFFSET_Y 2

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	BOOLEAN isOnPlatform;
	int coin; 

	int jumpedTime;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithTransformItem(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionMarkBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBrickButton(LPCOLLISIONEVENT e);
	void TakeDamage();

	void GetAniIdAndAniSpeed(int& aniId, float& speed);
	int MapAniTypeToId(int animation_type);

	int raccoonSlowFalling = 0;
	int tailFlapAnimationCurrentDuration = 0;
	int rotatingAnimDuration = 0;
	int rotatingAnimMaxDuration = 0;

	int transformAnimDuration = 0;
	int currentTransformAnim = 0;

	int untouchableDuration = 0;
	int notRenderSpriteFrameCount = 0;

	bool continuousTailFlap = false;

	bool readyToHoldKoopa = false;

	int flyCountDownTime = 0;

	CRaccoonTail* tail;
	bool IsAttacking() { return rotatingAnimDuration > 0; }
	void SetTailPosition(DWORD dt);

	Koopa* holdingKoopa = nullptr;
	void SetHoldKoopa(Koopa* koopa);
	void SetKoopaPosition(DWORD dt);
	bool IsHoldingKoopa() { return holdingKoopa != nullptr; }

	int kickAnimDuration = 0;
	bool isEnergyGeneratable = true;
public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_SMALL;
		isOnPlatform = false;
		coin = 0;
		jumpedTime = 0;

		tail = nullptr;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void SetTail(CRaccoonTail* tail) { this->tail = tail; }
	CRaccoonTail* GetTail() { return tail; }

	bool IsFalling() { return vy > 0 && !isOnPlatform; }
	bool IsFlyable() { return (level == MARIO_LEVEL_RACCOON && GetChargePercent() == 1 && isOnPlatform); }

	int IsCollidable() { return (state != MARIO_STATE_DIE); }
	int IsBlocking() { return 0; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return level; }
	void StartUntouchable() { untouchableDuration = MARIO_UNTOUCHABLE_TIME; notRenderSpriteFrameCount = MARIO_NOT_RENDER_MAX_FRAME_COUNT; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void TriggerRaccoonSlowFalling();
	void SwitchContinuousTailFlap(bool value) { continuousTailFlap = value; }

	void TriggerRaccoonFLy();
	bool IsFlying() { return flyCountDownTime > 0; }

	void TriggerRaccoonAttack();
	void EndRaccoonAttack();

	void TriggerSmallJump();

	void SetReadyHoldKoopa(bool value);
	void ReleaseKoopa();

	void PlayKickKoopaAnim();
	int GetRenderLayer() { return 3; }

	float GetChargePercent();
	int GetChargeInScale(int maxValue);

	void ChangeDirection(float direction);
	bool CanRechargeEnergy() { return isEnergyGeneratable; }
};