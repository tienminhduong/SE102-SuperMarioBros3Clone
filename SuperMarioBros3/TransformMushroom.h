#pragma once
#include "GameObject.h"
#include "Mushroom.h"

#define ID_ANI_TRANSFORM_MUSHROOM 120000
#define ID_ANI_TRANSFORM_MUSHROOM_COMPLETE 120001



class CTransformMushroom : public CMushroom
{
protected:
	int GetAniId(int state) override;
public:
	CTransformMushroom(float x, float y) : CMushroom(x, y) {}

	void OnExitCamera() override { SetActive(false); }
};