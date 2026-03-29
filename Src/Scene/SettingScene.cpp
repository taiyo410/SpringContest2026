#include "../pch.h"
#include "../Manager/Game/CollisionManager2D.h"
#include "../Manager/Generic/MenuManager.h"
#include "../Manager/Generic/MenuController.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/DataBank.h"
#include "../Utility/UtilityTemplates.h"
#include "../Object/Character/Cursor/Cursor.h"
#include "../Manager/Generic/SliderUIManager.h"
#include "SettingScene.h"

SettingScene::SettingScene(void):
	state_(SETTING_STATE::MAX),
	soundMng_(SoundManager::GetInstance()),
	dataBank_(DataBank::GetInstance()),
	volume_(VOL_MAX, VOL_MAX, VOL_MAX),
	volumePer_(1.0f,1.0f,1.0f)
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

	menuMng_ = std::make_unique<MenuManager>();
	cursor_ = std::make_unique<Cursor>();
	sliderUIMng_ = std::make_unique<SliderUIManager>();
	int i = 0;
	for (auto& button : buttonStrTable_)
	{
		//イージング演出をするために初期位置は画面外にする
		bool isMakeCollider = !UtilityCommon::IsHasFormat(button.second);	//タイトルメニューへはコライダを作らない
		Vector2 pos = { BUTTON_POS.x,BUTTON_POS.y + BUTTON_OFFSET * i};
		menuMng_->AddMenu(static_cast<int>(button.first), button.second, pos, isMakeCollider);
		i++;
	}
}

SettingScene::~SettingScene(void)
{
}

void SettingScene::Load(void)
{
	cursor_->Load();
	menuMng_->LoadFont(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE);

	auto& menuList = menuMng_->GetMenuList();
	for (auto& menu : menuList)
	{
		if (!menu->IsHasFormat())continue;

		Vector2 menuPos = menu->GetCurrentPos();
		std::wstring menuStr = menu->GetMenuButtonString();
		Vector2 menuCenterPos = menuMng_->GetMenuCenterPos(menuStr);
		Vector2F sliderPos = { static_cast<float>(menuPos.x) + 300.0f
				,static_cast<float>(menuPos.y + (menuCenterPos.y / 2.0f)) };
		Vector2F length = { 200.0f,20.0f };
		VOLUME_TYPE type = GetVolumeFromString(menuStr);

		sliderUIMng_->AddSliderUI(*cursor_, volumePer_[static_cast<int>(type)], sliderPos, length);
	}
	sliderUIMng_->Load();
}

void SettingScene::Init(void)
{
	cursor_->Init();
	sliderUIMng_->Init();
	menuMng_->Init();
	ChangeSetting(SETTING_STATE::NORMAL);
}

void SettingScene::NormalUpdate(void)
{
	cursor_->Update();
	updateSetting_();

	//更新はアクション中のみ
	CollisionManager2D::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager2D::GetInstance().Sweep();
}

void SettingScene::NormalDraw(void)
{
	menuMng_->DrawFormat(std::vector<int>{ volume_[static_cast<int>(VOLUME_TYPE::BGM)]
		, volume_[static_cast<int>(VOLUME_TYPE::SE)]
		, volume_[static_cast<int>(VOLUME_TYPE::TEXT_SPD)] });
	sliderUIMng_->Draw();

	cursor_->Draw();
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
	menuMng_->SelectMenu();
	sliderUIMng_->Update();
	menuMng_->Update();


	//割合計算
	for (int i=0;i<static_cast<int>(VOLUME_TYPE::MAX);i++)
	{
		volume_[i] = volumePer_[i] * VOL_MAX;
	}

	const int selectNum = menuMng_->GetSelectMenuNum();
	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK)&&menuMng_->GetSelectMenuNum()==static_cast<int>(SETTING_STATE::EXIT_SETTING))
	{
		ChangeSetting(static_cast<SETTING_STATE>(selectNum));
		return;
	}
	else if (inputMng_.IsKeyKeepPressed(KEY_INPUT_A,0.2f))
	{
		UtilityTemplates::SubIndex(volume_[static_cast<int>(selectNum)],0);
		VolumeRefrect();

	}
	else if (inputMng_.IsKeyKeepPressed(KEY_INPUT_D, 0.2f))
	{
		UtilityTemplates::AddIndex(volume_[static_cast<int>(selectNum)],VOL_MAX);
		VolumeRefrect();
	}

}

void SettingScene::UpdateBGM(void)
{
	//SetNumber(bgmVol_);
	//dataBank_.SetBGMVolume(bgmVol_ / VOL_MAX);
	//ChangeSetting(SETTING_STATE::NORMAL);
}

void SettingScene::UpdateSE(void)
{
	//SetNumber(seVol_);
	//dataBank_.SetBGMVolume(bgmVol_ / VOL_MAX);
	//ChangeSetting(SETTING_STATE::NORMAL);
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
	Vector2F gaugeRightDown = _leftTopPos + (_length * _value);
	DrawBoxAA(_leftTopPos.x, _leftTopPos.y, rightDownPos.x, rightDownPos.y, UtilityCommon::GRAY, true);
	DrawBoxAA(_leftTopPos.x, _leftTopPos.y, gaugeRightDown.x, rightDownPos.y, UtilityCommon::YELLOW, true);

	const float CIRCLE_RADIUS = _length.y;
	constexpr int POSNUM = 8;
	//円の描画
	Vector2F circlePos = { rightDownPos.x * _value , _leftTopPos.y + rightDownPos.y / 2.0f };
	DrawCircleAA(gaugeRightDown.x, _leftTopPos.y+_length.y/2.0f , CIRCLE_RADIUS, POSNUM, UtilityCommon::RED);

}

const SettingScene::VOLUME_TYPE SettingScene::GetVolumeFromString(const std::wstring _str)const
{
	VOLUME_TYPE type = VOLUME_TYPE::MAX;

	if (_str.find(L"BGM") != std::wstring::npos)
	{
		type = VOLUME_TYPE::BGM;
	}
	else if (_str.find(L"SE") != std::wstring::npos)
	{
		type = VOLUME_TYPE::SE;
	}
	else if (_str.find(L"テキスト速度") != std::wstring::npos)
	{
		type = VOLUME_TYPE::TEXT_SPD;
	}
	return type;
}


void SettingScene::VolumeRefrect(void)
{
	const int selectNum = menuMng_->GetSelectMenuNum();
	if (selectNum == static_cast<int>(SETTING_STATE::BGM))
	{
		soundMng_.SetSystemVolume(volume_[static_cast<int>(VOLUME_TYPE::BGM)], static_cast<int>(SoundManager::TYPE::BGM));
	}
	else if (selectNum == static_cast<int>(SETTING_STATE::SE))
	{
		soundMng_.SetSystemVolume(volume_[static_cast<int>(VOLUME_TYPE::SE)], static_cast<int>(SoundManager::TYPE::SE));
	}
	else if (selectNum == static_cast<int>(SETTING_STATE::TEXT_SPD))
	{
		dataBank_.SetTextSpeed(volume_[static_cast<int>(VOLUME_TYPE::TEXT_SPD)]);
	}
}
