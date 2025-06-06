#pragma once
#include "GameObject.h"

#define ID_ANI_BLACK_PIPE 150000

#define BLACK_PIPE_BBOX_WIDTH 31
#define BLACK_PIPE_BBOX_HEIGHT 39

class CBlackPipe :
    public CGameObject
{
public:
    CBlackPipe(float x, float y) : CGameObject(x, y) {}
    void Render() override;
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    int GetRenderLayer() override { return 4; }
};

