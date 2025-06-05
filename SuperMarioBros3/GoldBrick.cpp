#include "GoldBrick.h"
#include "GoldBrickButton.h"
#include "PlayScene.h"

void CGoldBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == GOLD_BRICK_STATE_BUTTON_OUT)
		animations->Get(ID_ANI_GOLD_BRICK_EMPTY)->Render(x, y);
	else
		animations->Get(ID_ANI_GOLD_BRICK)->Render(x, y);
	RenderBoundingBox();
}

void CGoldBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GOLD_BRICK_STATE_HIT_NOTBROKEN) {
		y -= dy;
		dy += GOLD_BRICK_MOVE_SPEED * moveDirection * dt;
		y += dy;

		if (abs(dy) >= GOLD_BRICK_MAX_MOVE_DY)
			if (moveDirection == -1)
				moveDirection = 1;
			else {
				SetState(GOLD_BRICK_STATE_NORMAL);
			}
	}
}

void CGoldBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GOLD_BRICK_BBOX_WIDTH / 2;
	top = y - GOLD_BRICK_BBOX_HEIGHT / 2;
	right = left + GOLD_BRICK_BBOX_WIDTH;
	bottom = top + GOLD_BRICK_BBOX_HEIGHT;
}

void CGoldBrick::SetState(int state)
{
	if (this->state == GOLD_BRICK_STATE_BUTTON_OUT)
		return;

	CGameObject::SetState(state);
	switch (state)
	{
	case GOLD_BRICK_STATE_HIT_NOTBROKEN:
		moveDirection = -1; // Move up
		dy = 0;
		break;
	default:
		y -= dy;
		break;
	}
}

void CGoldBrick::TriggerOnCollision()
{
	if (!containButton) {
		Delete();
	}
	else if (state != GOLD_BRICK_STATE_BUTTON_OUT) {
		CGoldBrickButton* button = new CGoldBrickButton(x, y - GOLD_BRICK_BBOX_HEIGHT);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(button);

		SetState(GOLD_BRICK_STATE_BUTTON_OUT);
	}
}

