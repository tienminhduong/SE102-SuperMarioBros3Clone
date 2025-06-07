#pragma once
#include "GameObject.h"
#include "BreakPartical.h"
#include "PlayScene.h"

class BreakAnimation
{
public:
	BreakAnimation(float x, float y) {
		LPPLAYSCENE curScene = dynamic_cast<LPPLAYSCENE>(CGame::GetInstance()->GetCurrentScene());
		BreakPartical* particalBreak1 = new BreakPartical(x - 4, y - 5, 0);
		BreakPartical* particalBreak2 = new BreakPartical(x + 4, y - 5, 1);
		BreakPartical* particalBreak3 = new BreakPartical(x - 4, y + 5, 2);
		BreakPartical* particalBreak4 = new BreakPartical(x + 4, y + 5, 3);
		curScene->AddNewObject(particalBreak1);
		curScene->AddNewObject(particalBreak2);
		curScene->AddNewObject(particalBreak3);
		curScene->AddNewObject(particalBreak4);
		delete this;
	}
};

