#include "GameManager.h"
#include "Sprites.h"
#include "Game.h"
#include "PlayScene.h"


#define ID_UI_PANEL 100
#define ID_UI_PAUSE_TEXT 101
#define ID_UI_ENERGY 1002
#define ID_UI_ENERGY_P 1003
#define ID_UI_NUMBER_0 1010
#define ID_UI_CLEAR 1004
#define ID_UI_YOU_GOT 1005

GameManager* GameManager::instance = nullptr;

CMario* GetMario() {
	return (CMario*)((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer();
}

void DrawUI(int id, float x, float y) {
	CSprites::GetInstance()->Get(id)->DrawOnScreen(x, y);
}

std::string IntToString(int n, int width = 8)
{
	auto s = std::to_string(n);
	if (int(s.size()) < width) s.insert(0, width - s.size(), '0');
	return s;
}

void DrawNumber(int number, float x, float y, int width) {
	std::string s = IntToString(number, width);
	for (int i = 0; i < s.size(); ++i) {
		int id = s[i] - '0' + ID_UI_NUMBER_0;
		DrawUI(id, x + i * 8, y);
	}
}

void DrawNumberFromRight(int number, float x, float y) {
	std::string s = IntToString(number, 0);
	for (int i = 0; i < s.size(); ++i) {
		int id = s[s.size() - i - 1] - '0' + ID_UI_NUMBER_0;
		DrawUI(id, x - i * 8, y);
	}
}

GameManager* GameManager::GetInstance()
{
	if (instance == nullptr)
		instance = new GameManager();

	return instance;
}

void GameManager::RenderUI()
{
	CMario* mario = GetMario();
	DrawUI(ID_UI_PANEL, 128, 200);
	if (isGamePaused)
		DrawUI(ID_UI_PAUSE_TEXT, 128, 100);

	for (int i = 0; i < min(mario->GetChargeInScale(7), 6); ++i) {
		DrawUI(ID_UI_ENERGY, 63 + i * 8, 197);
	}

	if (mario->GetChargeInScale(7) == 7)
		if (frameCount < 8)
			DrawUI(ID_UI_ENERGY_P, 116, 197);

	DrawNumber(score, 62, 205, 7);
	DrawNumber(timer/1000, 135, 205, 3);

	DrawNumberFromRight(coinNumber, 151, 197);

	DrawNumber(life, 49, 205, 1);

	if (displayWinningText) {
		DrawUI(ID_UI_CLEAR, 130, 25);
		DrawUI(ID_UI_YOU_GOT, 130, 50);
	}

	frameCount++;
	frameCount %= 16;
}

void GameManager::GoToHiddenMap()
{
	CMario* mario = GetMario();
	mario->SetLevel(MARIO_LEVEL_RACCOON, true);
}

void GameManager::GoBackFromHiddenMap()
{
	CMario* mario = GetMario();
	mario->SetLevel(MARIO_LEVEL_RACCOON, true);

	mario->SetGoToOtherMap(-1);
	mario->SetPosition(PIPE_GO_OUT_X, PIPE_GO_OUT_Y);
}

void GameManager::EndGame()
{
	isGameOver = true;
	isStopped = true;
}

void GameManager::WinGame()
{
	isWinning = true;
}


void GameManager::Update(DWORD dt) {
	timer -= dt;
	if (timer <= 0)
		GetMario()->SetState(MARIO_STATE_DIE);

	if (isWinning) {
		auto mario = GetMario();
		mario->SetState(MARIO_STATE_WALKING_RIGHT);

		if (mario->GetX() >= 2860) {
			displayWinningText = true;
			mario->SetPosition(2860, mario->GetY());
			isStopped = true;
		}
	}
}