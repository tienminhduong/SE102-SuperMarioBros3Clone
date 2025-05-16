#pragma once
#include "Respawnable.h"

#define KOOPA_BBOX_WIDTH 15
#define KOOPA_BBOX_HEIGHT 16
#define KOOPA_BBOX_OFFSET_Y 3

#define KOOPA_STATE_WALKING 0
#define KOOPA_STATE_INSHELL 1
#define KOOPA_STATE_INSHELL_RUNNING 2

#define KOOPA_INSHELL_RUNNING_SPEED 0.23f

#define KOOPA_START_ANI_ID 5100

#define ID_ANI_KOOPA_WALK_RIGHT 5100
#define ID_ANI_KOOPA_WALK_LEFT 5101
#define ID_ANI_KOOPA_INSHELL 5102
#define ID_ANI_KOOPA_INSHELL_RUNNING 5103
#define ID_ANI_KOOPA_INSHELL_FLIPPED 5104
#define ID_ANI_KOOPA_INSHELL_FLIPPED_RUNNING 5105

#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_INSHELL_TIME 10000

class Koopa :
    public CRespawnableEnemy
{
protected:
	LPGAMEOBJECT mario = nullptr;

	int GetAniId(int defaultIdAni);
	void OnCollisionWithQuestionMarkBlock(LPCOLLISIONEVENT e);

	int inShellDuration;
	bool isFlipped = false;
public:
	Koopa(float x, float y);

	virtual void Render() override;
	virtual void OnEnable() override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	int IsBlocking() override;
	int IsCollidable() override { return 1; }

	void OnAttackedByTail(float direction) override;

	void GetKicked(int direction);

	void SetState(int state) override;

	void SetHold(LPGAMEOBJECT mario);
	bool IsHold() { return mario != nullptr; }

	int GetRenderLayer() override { return 0; }
};

