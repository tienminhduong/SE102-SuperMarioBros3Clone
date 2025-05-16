#include "Goomba.h"
#include "PlayScene.h"

CGoomba::CGoomba(float x, float y) : CRespawnableEnemy(x, y)
{
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	OnEnable();
}

void CGoomba::OnEnable()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	LPGAMEOBJECT mario = scene->GetPlayer();
	if (mario != NULL)
	{
		float marioX, marioY;
		mario->GetPosition(marioX, marioY);
		nx = marioX > x ? 1 : -1;
		vx = nx * GOOMBA_WALKING_SPEED;
	}
	else {
		DebugOut(L"[GOOMBA] Mario not found!\n");
	}
}

void CGoomba::OnExitCamera()
{
	if (state == ENEMY_STATE_KICKED)
		Delete();
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE || state == ENEMY_STATE_KICKED)
	{
		left = top = right = bottom = 0;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0)
		vy = 0;
	else if (e->nx != 0)
		vx = -vx;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state != ENEMY_STATE_KICKED)
		vy += ay * dt;

	if (state == GOOMBA_STATE_DIE && die_start > 0)
	{
		die_start -= dt;
		if (die_start <= 0)
		{
			Delete();
			return;
		}
	}

	CRespawnableEnemy::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
		aniId = ID_ANI_GOOMBA_DIE;
	else if (state == ENEMY_STATE_KICKED)
		aniId = ID_ANI_GOOMBA_KICKED;

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CRespawnableEnemy::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GOOMBA_DIE_TIMEOUT;
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = GOOMBA_WALKING_SPEED;
			break;
	}
}
