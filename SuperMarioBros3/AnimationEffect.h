#pragma once
#include "GameObject.h"

class AnimationEffect : public CGameObject
{
    int id;
public:
    AnimationEffect(int id, float x, float y) : CGameObject(x, y)
    {
        this->id = id;
        CAnimations::GetInstance()->Get(id)->Reset();
    }
    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    int IsBlocking() override { return 0; }
};

