#pragma once

#include "Animation.h"
#include "Sprite.h"

#define ID_ANI_TAIL_ATTACK 130108
#define ID_ANI_100_UP 130109
#define ID_ANI_1_UP 130110
#define ID_ANI_1000_UP 130111

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();
	void PlayEffect(int id, float x, float y);

	static CAnimations* GetInstance();
};