#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_S:
		if (mario->IsFalling() && mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->TriggerRaccoonSlowFalling();
		else
			mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A: 
		mario->TriggerRaccoonAttack();
		mario->SetReadyHoldKoopa(true);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	case DIK_NUMPADENTER:
	{
		float marioX, marioY;
		mario->GetPosition(marioX, marioY);
		DebugOut(L"[ENTER KEY PRESSED] Mario current position is: (%f, %f)\n", marioX, marioY);
	}
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_A:
		mario->SetReadyHoldKoopa(false);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_STATE_SIT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);

	if (game->IsKeyDown(DIK_Z)) {
		mario->TriggerRaccoonAttack();
	}

	if (game->IsKeyDown(DIK_X)) {
		mario->TriggerSmallJump();

		if (mario->IsFalling())
			mario->TriggerRaccoonSlowFalling();

		mario->SwitchContinuousTailFlap(true);
	}
	else {
		mario->SwitchContinuousTailFlap(false);
	}
}