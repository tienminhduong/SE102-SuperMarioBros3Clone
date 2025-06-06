#include "GoldBrick.h"
#include "GoldBrickButton.h"
#include "TransformMushroom.h"
#include "PlayScene.h"

CGoldBrick::CGoldBrick(float x, float y, int containBtn)
	: CGameObject(x, y)
{
	containButton = containBtn;
	callbackId = -1;
	if (!containBtn)
		callbackId = CGoldBrickButton::Subscribe([this]() { this->ChangeToGold(); });
}

void CGoldBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == GOLD_BRICK_STATE_BUTTON_OUT)
		animations->Get(ID_ANI_GOLD_BRICK_EMPTY)->Render(x, y);
	else if (state == GOLD_BRICK_STATE_COIN)
		animations->Get(ID_ANI_GOLD_BRICK_COIN)->Render(x, y);
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

	if (coinTimeCountDown > 0)
	{
		coinTimeCountDown -= dt;
		if (coinTimeCountDown <= 0)
			SetState(GOLD_BRICK_STATE_NORMAL);
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
	case GOLD_BRICK_STATE_COIN:
		coinTimeCountDown = GOLD_BRICK_COIN_TIME;
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
		if (containButton == 1) {
			CGoldBrickButton* button = new CGoldBrickButton(x, y - GOLD_BRICK_BBOX_HEIGHT);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(button);
		}
		else if (containButton == 2) {
			CMushroom* mushroom = new CMushroom(x, y - GOLD_BRICK_BBOX_HEIGHT);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddNewObject(mushroom);
			mushroom->SetActive(true);
		}

		SetState(GOLD_BRICK_STATE_BUTTON_OUT);
	}
}

void CGoldBrick::ChangeToGold()
{
	SetState(GOLD_BRICK_STATE_COIN);
}

void CGoldBrick::Delete()
{
	CGameObject::Delete();
	CGoldBrickButton::Unsubscribe(callbackId);
}

