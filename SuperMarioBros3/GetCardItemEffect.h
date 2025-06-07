#pragma once
#include "GameObject.h"

#define ID_ANI_GET_CARD_ITEM_EFFECT 130115
#define CARD_ITEM_EFFECT_SPEED 0.25

class GetCardItemEffect : public CGameObject
{
public:
	GetCardItemEffect(float x, float y) : CGameObject(x, y) {}
	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
};

