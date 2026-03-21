#include "../pch.h"
#include "../Manager/Generic/MenuController.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/DataBank.h"
#include "SettingScene.h"

SettingScene::SettingScene(void):
	state_(SETTING_STATE::MAX),
	soundMng_(SoundManager::GetInstance()),
	dataBank_(DataBank::GetInstance()),
	bgmVol_(VOL_MAX),
	seVol_(VOL_MAX),
	textSpd_(VOL_MAX)
{
	//更新関数のセット
	updateFunc_ = [this]() {NormalUpdate(); };
	//描画関数のセット
	drawFunc_ = [this]() {NormalDraw(); };

	buttonStrTable_ = {
		{SETTING_BTN::BGM,L"BGM(%d)"},
		{SETTING_BTN::SE,L"SE(%d)"},
		{SETTING_BTN::TEXT_SPD,L"テキスト速度(%d)"},
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
		Vector2 pos = { BUTTON_POS.x,BUTTON_POS.y + BUTTON_OFFSET * i};
		menuController_->AddMenu(static_cast<int>(button.first), button.second, pos);
		i++;
	}
}

SettingScene::~SettingScene(void)
{
}

void SettingScene::Load(void)
{
	menuController_->LoadFont(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE);
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
	menuController_->DrawFormat(std::vector<int>{ bgmVol_, seVol_, textSpd_ });
	auto& menuList = menuController_->GetMenuList();
	for (auto& menu : menuList)
	{
		if (!menuController_->IsHasFormat(menu.second.btnStr))continue;

		Vector2 menuPos = menu.second.curPos;
		Vector2 centerPos = menuController_->GetMenuCenterPos(menu.second.btnStr);
		DrawSliderUI({ static_cast<float>(menuPos.x) + 300.0f,static_cast<float>(menuPos.y+(centerPos.y/2.0f))}, { 200.0f,20.0f }, GetVolumeFromString(menu.second.btnStr)/VOL_MAX);
	}


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
	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK)&&menuController_->GetSelectMenuNum()==static_cast<int>(SETTING_STATE::EXIT_SETTING))
	{
		const int selectNum = menuController_->GetSelectMenuNum();
		ChangeSetting(static_cast<SETTING_STATE>(selectNum));
	}
	else if (inputMngS_.IsTrgDown(INPUT_EVENT::LEFT) || inputMngS_.IsTrgDown(INPUT_EVENT::RIGHT))
	{

	}

}

void SettingScene::UpdateBGM(void)
{
	SetNumber(bgmVol_);
	dataBank_.SetBGMVolume(bgmVol_ / VOL_MAX);
	ChangeSetting(SETTING_STATE::NORMAL);
}

void SettingScene::UpdateSE(void)
{
	SetNumber(seVol_);
	dataBank_.SetBGMVolume(bgmVol_ / VOL_MAX);
	ChangeSetting(SETTING_STATE::NORMAL);
}

void SettingScene::UpdateTextSpeed(void)
{

}

void SettingScene::UpdateExitSetting(void)
{
	scnMng_.PopScene();
}

void SettingScene::SetNumber(int& _num)
{
	if (inputMngS_.IsTrgDown(INPUT_EVENT::RIGHT))
	{
		_num++;
	}
	else if (inputMngS_.IsTrgDown(INPUT_EVENT::LEFT))
	{
		_num--;
	}
}

void SettingScene::DrawSliderUI(Vector2F _leftTopPos, Vector2F _length, float _value)
{

	//ゲージの描画
	Vector2F rightDownPos = _leftTopPos + _length;
	DrawBoxAA(_leftTopPos.x, _leftTopPos.y, rightDownPos.x, rightDownPos.y, UtilityCommon::GRAY, true);
	DrawBoxAA(_leftTopPos.x, _leftTopPos.y, rightDownPos.x*_value, rightDownPos.y, UtilityCommon::YELLOW, true);

	const float CIRCLE_RADIUS = _length.y;
	constexpr int POSNUM = 8;
	//円の描画
	Vector2F circlePos = { rightDownPos.x * _value , _leftTopPos.y + rightDownPos.y / 2.0f };
	DrawCircleAA(rightDownPos.x * _value, _leftTopPos.y+_length.y/2.0f , CIRCLE_RADIUS, POSNUM, UtilityCommon::RED);

}

const float SettingScene::GetVolumeFromString(const std::wstring _str) const
{
	if (_str.find(L"BGM") != std::wstring::npos)
	{
		return bgmVol_;
	}
	else if (_str.find(L"SE") != std::wstring::npos)
	{
		return seVol_;
	}
	else if (_str.find(L"テキスト速度") != std::wstring::npos)
	{
		return textSpd_;
	}
	return 0.0f;
}
