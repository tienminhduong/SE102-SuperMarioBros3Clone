#include "Animation.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	if (sprite == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::SetAllFrameTime(DWORD time)
{
	for (int i = 0; i < frames.size(); i++)
	{
		frames[i]->SetTime(time);
	}
}

void CAnimation::LogFrameTime()
{
	for (int i = 0; i < frames.size(); i++)
	{
		DebugOut(L"Frame %d: %d\n", i, frames[i]->GetTime());
	}
}

DWORD CAnimation::GetDuration()
{
	DWORD duration = 0;
	for (int i = 0; i < frames.size(); i++)
		duration += frames[i]->GetTime();
	return duration;
}


void CAnimation::Render(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}

	frames[currentFrame]->GetSprite()->Draw(x, y);
}

