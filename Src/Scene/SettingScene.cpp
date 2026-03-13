#include "../pch.h"
#include "../Manager/Generic/MenuController.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "SettingScene.h"

SettingScene::SettingScene(void):
	state_(SETTING_STATE::MAX),
	soundMng_(SoundManager::GetInstance())
{
	//更新関数のセット
	updateFunc_ = [this]() {NormalUpdate(); };
	//描画関数のセット
	drawFunc_ = [this]() {NormalDraw(); };

	buttonStrTable_ = {
		{SETTING_BTN::BGM,L"BGM"},
		{SETTING_BTN::SE,L"SE"},
		{SETTING_BTN::TEXT_SPD,L"テキスト速度"},
		{SETTING_BTN::EXIT_SETTING,L"タイトルメニューへ"}
	};
	
	changeSetting_ = {
		{SETTING_STATE::NORMAL,[this]() {ChangeSettingNormal(); }},
		{SETTING_STATE::BGM,[this]() {ChangeBGM(); }},
		{SETTING_STATE::SE,[this]() {ChangeSE(); }},
		{SETTING_STATE::TEXT_SPD,[this]() {ChangeTextSpeed(); }},
		{SETTING_STATE::EXIT_SETTING,[this]() {ChangeExitSetting(); }}
	};

	menuController_ = std::make_unique<MenuController>();
	
	int i = 0;
	for (auto& button : buttonStrTable_)
	{
		//イージング演出をするために初期位置は画面外にする
		Vector2 pos = { Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y + BUTTON_OFFSAET * i};
		menuController_->AddMenu(static_cast<int>(button.first), button.second, pos);
		i++;
	}
}

SettingScene::~SettingScene(void)
{
}

void SettingScene::Load(void)
{
	menuController_->LoadFont(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE);
}

void SettingScene::Init(void)
{
	ChangeSetting(SETTING_STATE::NORMAL);
}

void SettingScene::NormalUpdate(void)
{
	updateSetting_();
}

void SettingScene::NormalDraw(void)
{
	menuController_->DrawCenter();
}

void SettingScene::ChangeSetting(const SETTING_STATE _state)
{
	if (state_ == _state)return;
	state_ = _state;
	changeSetting_[state_]();
}

void SettingScene::ChangeSettingNormal(void)
{
	updateSetting_ = [this]() {UpdateSettingNormal(); };
}

void SettingScene::ChangeBGM(void)
{
	updateSetting_ = [this]() {UpdateBGM(); };
}

void SettingScene::ChangeSE(void)
{
	updateSetting_ = [this]() {UpdateSE(); };
}

void SettingScene::ChangeTextSpeed(void)
{
	updateSetting_ = [this]() {UpdateTextSpeed(); };
}

void SettingScene::ChangeExitSetting(void)
{
	updateSetting_ = [this]() {UpdateExitSetting(); };
}

void SettingScene::UpdateSettingNormal(void)
{
	menuController_->SelectMenu();
	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		const int selectNum = menuController_->GetSelectMenuNum();
		ChangeSetting(static_cast<SETTING_STATE>(selectNum));
	}

}

void SettingScene::UpdateBGM(void)
{

}

void SettingScene::UpdateSE(void)
{

}

void SettingScene::UpdateTextSpeed(void)
{

}

void SettingScene::UpdateExitSetting(void)
{
	scnMng_.PopScene();
}
