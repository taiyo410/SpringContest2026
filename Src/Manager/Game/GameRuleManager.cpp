#include "../pch.h"
#include "../Application.h"
#include "../Common/Vector2.h"
#include "../Common/FontController.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Object/Character/Daimyo/Daimyo.h"
#include "GameRuleManager.h"

void GameRuleManager::Load(void)
{
	//フォント
	font_ = std::make_unique<FontController>();

	//フォントの登録
	moneyFont_ = font_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
}

void GameRuleManager::Init(void)
{
	//初期化
	nowMoney_ = INITIAL_FUNDS;
	dissatisfaction_ = 0;
	elapsedYear_ = 0; // 経過年数の初期化
}

void GameRuleManager::Update(void)
{
}

void GameRuleManager::Draw(void)
{
	//所持金
	Vector2 moneyPos = { Application::SCREEN_SIZE_X - 200, 100 };

	DrawBox(moneyPos.x - 150, moneyPos.y - 30, moneyPos.x + 150, moneyPos.y + 30, 0xff0000, true);

	std::wstring str = L"Money : ";
	str += std::to_wstring(nowMoney_ * UNITS);
	str += L"Yen";
	UtilityDraw::DrawStringCenter(moneyPos.x, moneyPos.y, 0xffffff, moneyFont_, str);

	//不満度
	Vector2 disPos = { 60, 300 };

	DrawBox(disPos.x - 40, disPos.y - 200, disPos.x + 40, disPos.y + 200, 0x666666, true);
	if (dissatisfaction_ > 0)
		DrawBox(disPos.x - 40, disPos.y + 200 - (dissatisfaction_ * 4), disPos.x + 40, disPos.y + 200, 0x0000ff, true);
}

void GameRuleManager::Release(void)
{
}

GameRuleManager::GameRuleManager(void)
{
}

GameRuleManager::~GameRuleManager(void)
{
}
