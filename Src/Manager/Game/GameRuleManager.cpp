#include "../pch.h"
#include "../Application.h"
#include "../Common/Vector2.h"
#include "../Common/FontController.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Object/Character/UIData/DissatisfactionBar.h"
#include "GameRuleManager.h"

void GameRuleManager::Load(void)
{
	//フォント
	font_ = std::make_unique<FontController>();

	//フォントの登録
	moneyFont_ = font_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);

	//不満度ゲージ
	dissatifactionBar_ = std::make_unique<DissatisfactionBar>();
	dissatifactionBar_->Load();
	dissatifactionBar_->Init();
}

void GameRuleManager::Init(void)
{
	//初期化
	nowMoney_ = INITIAL_FUNDS;
	elapsedYear_ = 0; // 経過年数の初期化
}

void GameRuleManager::Update(void)
{
	dissatifactionBar_->Update();
	dissatifactionBar_->Sweep();
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

	dissatifactionBar_->Draw();
}

void GameRuleManager::Release(void)
{
	dissatifactionBar_->Release();
}

void GameRuleManager::AddDissatisfaction(const int _value)
{
	dissatifactionBar_->AddDissatisfaction(_value);
}

const int GameRuleManager::GetDissatisfaction(void) const
{
	return dissatifactionBar_->GetDissatisfaction();
}

const bool GameRuleManager::IsGameOver(void)const
{
	//不満度が溜まった　又は　大名のお金が溜まったか
	return dissatifactionBar_->IsDissatisfactionMax() && CharacterManager::GetInstance().IsMoneyMax();
}
GameRuleManager::GameRuleManager(void)
{
}

GameRuleManager::~GameRuleManager(void)
{ 
}



