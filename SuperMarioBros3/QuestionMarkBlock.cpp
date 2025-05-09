#include "QuestionMarkBlock.h"
#include "debug.h"

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
			animHitDuration = 0;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionMarkBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
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
		// shoot something out
		break;
	case QUESTION_MARK_BLOCK_STATE_HIT:
		animHitDuration = CAnimations::GetInstance()->Get(ID_ANI_QUESTION_MARK_BLOCK_HIT)->GetDuration();
		break;
	}
}
