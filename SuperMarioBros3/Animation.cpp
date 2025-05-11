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

void CAnimation::SetSpeed(float speed)
{
	this->speed = speed;
}

void CAnimation::LogFrameTime()
{
	for (int i = 0; i < frames.size(); i++)
	{
		DebugOut(L"Frame %d: %d\n", i, (DWORD)(frames[i]->GetTime() / speed));
	}
}

DWORD CAnimation::GetDuration()
{
	DWORD duration = 0;
	for (int i = 0; i < frames.size(); i++)
		duration += (DWORD)(frames[i]->GetTime() / speed);
	return duration;
}

void CAnimation::GetCurrentFrameSize(int& width, int& height)
{
	frames[currentFrame]->GetSprite()->GetSize(width, height);
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
		DWORD t = (DWORD)(frames[currentFrame]->GetTime() / speed);
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}

	frames[currentFrame]->GetSprite()->Draw(x, y);
}

void CAnimation::Reset()
{
	lastFrameTime = -1; currentFrame = -1;
}

