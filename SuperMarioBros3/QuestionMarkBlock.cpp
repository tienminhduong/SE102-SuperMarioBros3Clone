#include "QuestionMarkBlock.h"
#include "CoinFromQuesBlock.h"
#include "PlayScene.h"
#include "debug.h"
#include "TransformMushroom.h"

CQuestionMarkBlock::CQuestionMarkBlock(float x, float y, int containedObjId) : CGameObject(x, y)
{
	state = QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING;
	switch (containedObjId)
	{
	case OBJECT_TYPE_COIN_FROM_QUESTION_BLOCK:
		containedObj = new CoinFromQuesBlock(x, y - 20);
		containedObj->SetActive(false);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(containedObj);
		break;
	case OBJECT_TYPE_TRANSFORM_MUSHROOM:
	{
		int distance = QUESTION_MARK_BLOCK_BBOX_HEIGHT / 2 + TRANSFORM_MUSHROOM_BBOX_WIDTH / 2;
		containedObj = new CTransformMushroom(x, y - distance - 1);
		containedObj->SetActive(false);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(containedObj);

		break;
	}
	default:
		break;
	}

	auto animations = CAnimations::GetInstance();
}

void CQuestionMarkBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING)
		animations->Get(ID_ANI_QUESTION_MARK_BLOCK_HAS_SOMETHING)->Render(x, y);
	else if (state == QUESTION_MARK_BLOCK_STATE_EMPTY)
		animations->Get(ID_ANI_QUESTION_MARK_BLOCK_EMPTY)->Render(x, y);
	else
		animations->Get(ID_ANI_QUESTION_MARK_BLOCK_HIT)->Render(x, y);
	RenderBoundingBox();
}

void CQuestionMarkBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == QUESTION_MARK_BLOCK_STATE_HIT)
	{
		animHitDuration -= dt;
		if (animHitDuration <= 0)
		{
			SetState(QUESTION_MARK_BLOCK_STATE_EMPTY);
			CAnimations::GetInstance()->Get(ID_ANI_QUESTION_MARK_BLOCK_HIT)->Reset();
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionMarkBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	auto mario = dynamic_cast<CMario*>(e->obj);
	if (mario != nullptr) {
		if (e->ny > 0 || e->nx != 0)
			return;
	}

	if (state == QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING)
		SetState(QUESTION_MARK_BLOCK_STATE_HIT);
}

void CQuestionMarkBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - QUESTION_MARK_BLOCK_BBOX_WIDTH / 2;
	top = y - QUESTION_MARK_BLOCK_BBOX_HEIGHT / 2;
	right = left + QUESTION_MARK_BLOCK_BBOX_WIDTH;
	bottom = top + QUESTION_MARK_BLOCK_BBOX_HEIGHT;
}

void CQuestionMarkBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING:
		break;
	case QUESTION_MARK_BLOCK_STATE_EMPTY:
		containedObj->SetActive(true);
		break;
	case QUESTION_MARK_BLOCK_STATE_HIT:
		animHitDuration = CAnimations::GetInstance()->Get(ID_ANI_QUESTION_MARK_BLOCK_HIT)->GetDuration();
		break;
	}
}
