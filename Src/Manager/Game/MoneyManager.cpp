#include "../pch.h"
#include "../Application.h"
#include "../Common/Vector2.h"
#include "../Common/FontController.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Resource/FontManager.h"
#include "MoneyManager.h"

void MoneyManager::Load(void)
{
	//フォント
	font_ = std::make_unique<FontController>();

	//フォントの登録
	moneyFont_ = font_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
}

void MoneyManager::Init(void)
{
	//初期化
	nowMoney_ = INITIAL_FUNDS;
}

void MoneyManager::Update(void)
{
}

void MoneyManager::Draw(void)
{
	Vector2 moneyPos = { Application::SCREEN_SIZE_X - 200, 100 };

	DrawBox(moneyPos.x - 150, moneyPos.y - 30, moneyPos.x + 150, moneyPos.y + 30, 0xff0000,true);

	//所持金
	std::wstring str = L"Money : ";
	str += std::to_wstring(nowMoney_ * UNITS);
	str += L"Yen";
	UtilityDraw::DrawStringCenter(moneyPos.x, moneyPos.y, 0xffffff, moneyFont_, str);
}

void MoneyManager::Release(void)
{
}

MoneyManager::MoneyManager(void)
{
	nowMoney_ = 0;
}

MoneyManager::~MoneyManager(void)
{
}
