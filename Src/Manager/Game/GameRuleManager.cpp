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
}

void GameRuleManager::Update(void)
{
}

void GameRuleManager::Draw(void)
{
	//所持金
	Vector2 moneyPos = { Application::SCREEN_SIZE_X - 200, 100 };

	DrawBox(moneyPos.x - 150, moneyPos.y - 30, moneyPos.x + 150, moneyPos.y + 30, 0xff0000,true);

	std::wstring str = L"Money : ";
	str += std::to_wstring(nowMoney_ * UNITS);
	str += L"Yen";
	UtilityDraw::DrawStringCenterToFontHandle(moneyPos.x, moneyPos.y, 0xffffff, moneyFont_, str);

	//不満度
	Vector2 disPos = { 60, 300 };

	DrawBox(disPos.x - 40, disPos.y - 200, disPos.x + 40, disPos.y + 200,0x666666, true);
	if(dissatisfaction_ > 0)
		DrawBox(disPos.x - 40, disPos.y + 200, disPos.x + 40, disPos.y + 200 - 400 * (static_cast<float>(dissatisfaction_) / static_cast<float>(DISSATISFACTION_MAX)),0xff00ff, true);
	DrawBox(disPos.x - 40, disPos.y - 200, disPos.x + 40, disPos.y + 200,0x0, false);
}

void GameRuleManager::Release(void)
{
}

const bool GameRuleManager::IsGameOver(void)
{
	//不満度
	if (dissatisfaction_ >= DISSATISFACTION_MAX)
	{
		return true;
	}

	//所持金
	if (CharacterManager::GetInstance().IsMoneyMax())
	{
		return true;
	}

	return false;
}

GameRuleManager::GameRuleManager(void)
{
	nowMoney_ = 0;
}

GameRuleManager::~GameRuleManager(void)
{
}
