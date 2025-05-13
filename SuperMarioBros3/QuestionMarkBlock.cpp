#include "QuestionMarkBlock.h"
#include "CoinFromQuesBlock.h"
#include "PlayScene.h"
#include "debug.h"
#include "TransformMushroom.h"
#include "TransformLeaf.h"

CQuestionMarkBlock::CQuestionMarkBlock(float x, float y, int containedObjType) : CGameObject(x, y)
{
	state = QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING;
	this->containedObjType = containedObjType;
	LPGAMEOBJECT containedObj = NULL;

	containedObj = new CoinFromQuesBlock(x, y - 20);
	containedObj->SetActive(false);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(containedObj);
	containedObjs.push_back(containedObj);

	int distance = QUESTION_MARK_BLOCK_BBOX_HEIGHT / 2 + TRANSFORM_MUSHROOM_BBOX_WIDTH / 2;
	containedObj = new CTransformMushroom(x, y - distance - 1);
	containedObj->SetActive(false);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(containedObj);
	containedObjs.push_back(containedObj);

	containedObj = new CTransformLeaf(x, y - QUESTION_MARK_BLOCK_BBOX_HEIGHT / 2);
	containedObj->SetActive(false);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(containedObj);
	containedObjs.push_back(containedObj);
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

void CQuestionMarkBlock::TriggerOnCollisionWithMario(float marioX)
{
	if (state == QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING)
		SetState(QUESTION_MARK_BLOCK_STATE_HIT);

	int direction = (x - marioX) >= 0 ? 1 : -1;
	//log x, marioX and direction
	DebugOut(L"[QUES] x = %f, marioX = %f, direction = %d\n", x, marioX, direction);
	((CTransformMushroom*)containedObjs[QUES_BLOCK_TRANSFORM_MUSHROOM_INDEX])->SetDirection(direction);
}

void CQuestionMarkBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - QUESTION_MARK_BLOCK_BBOX_WIDTH / 2;
	top = y - QUESTION_MARK_BLOCK_BBOX_HEIGHT / 2;
	right = left + QUESTION_MARK_BLOCK_BBOX_WIDTH;
	bottom = top + QUESTION_MARK_BLOCK_BBOX_HEIGHT;
}

void CQuestionMarkBlock::ActivateItem()
{
	switch (containedObjType)
	{
	case QUES_BLOCK_TYPE_COIN:
		containedObjs[QUES_BLOCK_COIN_INDEX]->SetPosition(x, y - 20);
		containedObjs[QUES_BLOCK_COIN_INDEX]->SetActive(true);
		break;
	case QUES_BLOCK_TYPE_TRANSFORM:
	{
		CMario* player = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (player->GetLevel() == MARIO_LEVEL_SMALL)
			containedObjs[QUES_BLOCK_TRANSFORM_MUSHROOM_INDEX]->SetActive(true);
		else
			containedObjs[QUES_BLOCK_TRANSFORM_LEAF_INDEX]->SetActive(true);
	}
	break;

	default:
		break;
	}
}

void CQuestionMarkBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_MARK_BLOCK_STATE_HAS_SOMETHING:
		break;
	case QUESTION_MARK_BLOCK_STATE_EMPTY:
	{
		ActivateItem();
		break;
	}
	case QUESTION_MARK_BLOCK_STATE_HIT:
		animHitDuration = CAnimations::GetInstance()->Get(ID_ANI_QUESTION_MARK_BLOCK_HIT)->GetDuration();
		break;
	}
}
