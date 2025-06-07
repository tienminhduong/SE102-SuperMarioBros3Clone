#pragma once
#include "GameObject.h"

#define PARTICAL_GRAVITY 0.001f
#define PARTICAL_BASE_AX 0.00009f
#define PARTICAL_START_VY_TOP 0.3f
#define PARTICAL_START_VY_BOTTOM 0.2f

#define ID_ANI_PARTICAL 130112
#define PARTICAL_TIME_DELETE 1000

class BreakPartical :
    public CGameObject
{
private:
    int index;
    ULONGLONG deleteTime;
    float ax, ay;
public:
    BreakPartical(float x, float y, int index);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
    void OnNoCollision(DWORD dt) override;
    
    bool RenderOnPaused() override { return false; }
};

