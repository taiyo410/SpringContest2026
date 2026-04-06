#include "../pch.h"
#include "../Application.h"
#include "../Common/Vector2.h"
#include "../Common/FontController.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Object/Character/UIData/DissatisfactionBar.h"
#include "GameRuleManager.h"

void GameRuleManager::Load(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();

	//フォント
	font_ = std::make_unique<FontController>();

	//フォントの登録
	moneyFont_ = font_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);


	soundMng_.LoadResource(SoundManager::SRC::DISSATISFACTION_DOWN);
	soundMng_.LoadResource(SoundManager::SRC::EVENT_SE);
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
	Vector2 moneyPos = { Application::SCREEN_SIZE_X - 220.0f, 50.0f };
	Vector2F moneyBox = { 200.0f,40.0f };
	DrawExtendGraph(moneyPos.x - moneyBox.x, moneyPos.y - moneyBox.y, moneyPos.x + moneyBox.x, moneyPos.y + moneyBox.y, imageId_, true);

	std::wstring str = L"所持金 : ";
	std::wstringstream ss;
	ss.imbue(std::locale(""));
	ss << nowMoney_ * UNITS;
	str += ss.str();
	str += L"円";
	UtilityDraw::DrawStringCenterToFontHandle(moneyPos.x, moneyPos.y, 0, moneyFont_, str);

	//年数
	Vector2F yearPos = { 150.0f,50.0f };
	Vector2F yearBox = { 130.0f,40.0f };
	DrawExtendGraph(yearPos.x - yearBox.x, yearPos.y - yearBox.y, yearPos.x + yearBox.x, yearPos.y + yearBox.y, imageId_, true);
	
	std::wstring yearStr = L"経過年数：";
	yearStr += std::to_wstring(elapsedYear_);
	yearStr += L"年";
	UtilityDraw::DrawStringCenterToFontHandle(yearPos.x, yearPos.y, 0, moneyFont_, yearStr);
}

void GameRuleManager::DrawSelect(void)
{
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

void GameRuleManager::AddYear(const int _year)
{
	int oldYear = elapsedYear_;
	elapsedYear_ += _year;

	// 20年に到達、あるいはまたいだ瞬間にSEを鳴らす
	if (oldYear < 20 && elapsedYear_ >= 20)
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::EVENT_SE, SoundManager::PLAYTYPE::BACK);
	}

	// 40年に到達、あるいはまたいだ瞬間にSEを鳴らす
	if (oldYear < 40 && elapsedYear_ >= 40)
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::EVENT_SE, SoundManager::PLAYTYPE::BACK);
	}

}

const bool GameRuleManager::IsGameOver(void)const
{
	//不満度が溜まった　又は　大名のお金が溜まったか
	return dissatifactionBar_->IsDissatisfactionMax() || CharacterManager::GetInstance().IsMoneyMax();
}
GameRuleManager::GameRuleManager(void):
	soundMng_(SoundManager::GetInstance())
{
}

GameRuleManager::~GameRuleManager(void)
{ 
}



