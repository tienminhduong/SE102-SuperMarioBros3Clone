#include "GoldBrickButton.h"
#include <algorithm>

std::vector<pair<int, function<void()>>> CGoldBrickButton::callbacks;

int CGoldBrickButton::Subscribe(function<void()> func)
{
	int id;
	if (callbacks.empty())
		id = 0;
	else
		id = callbacks[callbacks.size() - 1].first + 1;
	callbacks.push_back({ id, func });
	return id;
}

void CGoldBrickButton::Unsubscribe(int id)
{
	callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
		[id](const pair<int, function<void()>>& item) {return item.first == id; }), callbacks.end());
}

void CGoldBrickButton::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == GOLD_BRICK_BTN_STATE_ACTIVE)
		animations->Get(ID_ANI_GOLD_BRICK_BTN_ACTIVE)->Render(x, y);
	else if (state == GOLD_BRICK_BTN_STATE_INACTIVE)
		animations->Get(ID_ANI_GOLD_BRICK_BTN_INACTIVE)->Render(x, y);
}

void CGoldBrickButton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOLD_BRICK_BTN_STATE_INACTIVE)
	{
		left = top = right = bottom = 0;
		return;
	}

	left = x - GOLD_BRICK_BTN_BBOX_WIDTH / 2;
	top = y - GOLD_BRICK_BTN_BBOX_HEIGHT / 2;
	right = left + GOLD_BRICK_BTN_BBOX_WIDTH;
	bottom = top + GOLD_BRICK_BTN_BBOX_HEIGHT;
}

void CGoldBrickButton::TriggerOnCollision()
{
	if (state == GOLD_BRICK_BTN_STATE_ACTIVE)
	{
		for (auto& f : callbacks)
			f.second();
	}
	SetState(GOLD_BRICK_BTN_STATE_INACTIVE);
}