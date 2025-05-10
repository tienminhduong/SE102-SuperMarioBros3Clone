#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

using namespace std;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	float speed;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; speed = 1.f; }
	void Add(int spriteId, DWORD time = 0);
	void SetSpeed(float speed);
	void LogFrameTime();
	DWORD GetDuration();
	void GetCurrentFrameSize(int& width, int& height);
	void Render(float x, float y);
	void Reset();
	int GetCurrentFrameIndex() { return currentFrame; }
	int GetNumberFrames() { return (int)frames.size(); }
	bool IsOver() { return currentFrame == frames.size() - 1; }
};

typedef CAnimation* LPANIMATION;