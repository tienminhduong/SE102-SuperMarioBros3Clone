#pragma once
#include "GameObject.h"
#include "AssetIDs.h"

#define QUESTION_MARK_BLOCK_BBOX_WIDTH 16
#define QUESTION_MARK_BLOCK_BBOX_HEIGHT 16

#define QUESTION_MARK_BLOCK_STATE_EMPTY 0
#define QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING 1
#define QUESTION_MARK_BLOCK_STATE_HIT 2

#define ID_ANI_QUESTION_MARK_BLOCK_HAS_SOMETHING 100000
#define ID_ANI_QUESTION_MARK_BLOCK_EMPTY 100001
#define ID_ANI_QUESTION_MARK_BLOCK_HIT 100002

#define QUESTION_MARK_ITEM_COIN 52
#define QUESTION_MARK_ITEM_TRANSFORM 0

#define QUES_BLOCK_TYPE_COIN 0
#define QUES_BLOCK_TYPE_TRANSFORM 1
#define QUES_BLOCK_TYPE_GREEN_MUSHROOM 2

#define QUES_BLOCK_COIN_INDEX 0
#define QUES_BLOCK_TRANSFORM_MUSHROOM_INDEX 1
#define QUES_BLOCK_TRANSFORM_LEAF_INDEX 2

class CQuestionMarkBlock :
    public CGameObject
{
	int animHitDuration = 0;
	vector<LPGAMEOBJECT> containedObjs;
	int containedObjType;
	void ActivateItem();
public:
	CQuestionMarkBlock(float x, float y, int containedObjType);
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void TriggerOnCollisionWithMario(float marioX);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	int IsCollidable() override { return 1; }
	void SetState(int state) override;
};

