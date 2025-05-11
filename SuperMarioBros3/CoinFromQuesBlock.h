#pragma once
#include "GameObject.h"

#define ID_ANI_COIN_FROM_QUESTION_MARK_BLOCK 110000

#define COIN_QUES_BLOCK_SPEED 0.18f
#define COIN_QUES_BLOCK_MAX_HEIGHT 40

class CoinFromQuesBlock :
	public CGameObject
{
private:
	float startY;
public:
	CoinFromQuesBlock(float x, float y) : CGameObject(x, y) { state = 1; startY = y; }
	void OnEnable() override;
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	int IsCollidable() override { return 0; }
};

