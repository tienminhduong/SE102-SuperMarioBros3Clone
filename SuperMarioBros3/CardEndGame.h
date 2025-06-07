#pragma once
#include "GameObject.h"

#define ID_ANI_CARD_END_GAME 130113
#define ID_ANI_CARD_END_GAME_CLAIMED 130114

#define CARD_END_GAME_BBOX_WIDTH 23
#define CARD_END_GAME_BBOX_HEIGHT 27

#define CARD_END_GAME_STATE_NORMAL 0
#define CARD_END_GAME_STATE_CLAIMED 1

class CardEndGame :
    public CGameObject
{
public:
    CardEndGame(float x, float y) : CGameObject(x, y) { SetState(CARD_END_GAME_STATE_NORMAL); }
    void Render() override;
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    void TriggerOnCollision();
    int IsBlocking() override { return 0; }
    int GetRenderLayer() override { return 4; }
};

