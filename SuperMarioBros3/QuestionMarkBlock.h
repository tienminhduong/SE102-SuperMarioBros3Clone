#pragma once
#include "GameObject.h"

#define QUESTION_MARK_BLOCK_BBOX_WIDTH 15
#define QUESTION_MARK_BLOCK_BBOX_HEIGHT 15

#define QUESTION_MARK_BLOCK_STATE_EMPTY 0
#define QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING 1
#define QUESTION_MARK_BLOCK_STATE_HIT 2

#define ID_ANI_QUESTION_MARK_BLOCK_HAS_SOMETHING 100000
#define ID_ANI_QUESTION_MARK_BLOCK_EMPTY 100001
#define ID_ANI_QUESTION_MARK_BLOCK_HIT 100002

class CQuestionMarkBlock :
    public CGameObject
{
	int animHitDuration = 0;
public:
	CQuestionMarkBlock(float x, float y) : CGameObject(x, y) { state = 1; }
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	int IsCollidable() override { return 1; }
	void SetState(int state) override;
};

