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
#include "../Object/UI/SliderUIController.h"

#include "../Scene/YesNoScene.h"

#include "SettingScene.h"

SettingScene::SettingScene(void):
	state_(SETTING_STATE::MAX),
	soundMng_(SoundManager::GetInstance()),
	dataBank_(DataBank::GetInstance()),
	volume_(VOL_MAX, VOL_MAX, VOL_MAX),
	volumePer_(1.0f,1.0f,1.0f),
	questionStr_(L"")
{
	//更新関数のセット
	updateFunc_ = [this]() {NormalUpdate(); };
	//描画関数のセット
	drawFunc_ = [this]() {NormalDraw(); };

	buttonStrTable_ = {
		{SETTING_BTN::BGM,L"BGM(%d)"},
		{SETTING_BTN::SE,L"SE(%d)"},
		{SETTING_BTN::TEXT_SPD,L"テキスト速度(%d)"},
		{SETTING_BTN::SCREEN_SETTING,L"スクリーン変更"},
		{SETTING_BTN::EXIT_SETTING,L"タイトルメニューへ"}
	};
	
	changeSetting_ = {
		{SETTING_STATE::NORMAL,[this]() {ChangeSettingNormal(); }},
		{SETTING_STATE::BGM,[this]() {ChangeBGM(); }},
		{SETTING_STATE::SE,[this]() {ChangeSE(); }},
		{SETTING_STATE::TEXT_SPD,[this]() {ChangeTextSpeed(); }},
		{SETTING_STATE::SCREEN_SET,[this]() {ChangeScreenSetting(); }},
		{SETTING_STATE::EXIT_SETTING,[this]() {ChangeExitSetting(); }}
	};

	menuMng_ = std::make_unique<MenuManager>();
	cursor_ = std::make_unique<Cursor>();
	sliderUIMng_ = std::make_unique<SliderUIManager>();
	yesNoScn_ = std::make_unique<YesNoScene>(Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y), YES_NO_SIZE);
	int i = 0;
	for (auto& button : buttonStrTable_)
	{
		//イージング演出をするために初期位置は画面外にする
		//bool isMakeCollider = !UtilityCommon::IsHasFormat(button.second);	//タイトルメニューへはコライダを作らない
		Vector2 pos = { BUTTON_POS.x,BUTTON_POS.y + BUTTON_OFFSET * i};
		//menuMng_->AddMenu(static_cast<int>(button.first), button.second, pos, isMakeCollider);
		menuMng_->AddMenu(static_cast<int>(button.first), button.second, pos, true);
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

	yesNoScn_->Load();
}

void SettingScene::Init(void)
{
	cursor_->Init();
	sliderUIMng_->Init();
	menuMng_->Init();
	yesNoScn_->Init();
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
	DrawBox(100, 100, Application::SCREEN_SIZE_X-100, Application::SCREEN_SIZE_Y - 100, UtilityCommon::PINK, true);

	menuMng_->DrawFormat(std::vector<int>{ volume_[static_cast<int>(VOLUME_TYPE::BGM)]
		, volume_[static_cast<int>(VOLUME_TYPE::SE)]
		, volume_[static_cast<int>(VOLUME_TYPE::TEXT_SPD)] });
	sliderUIMng_->Draw();

	if (state_ == SETTING_STATE::SCREEN_SET)
	{
		yesNoScn_->Draw(questionStr_);
	}

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
	questionStr_ = L"タイトルメニューに戻りますか？";
	updateSetting_ = [this]() {UpdateExitSetting(); };
}

void SettingScene::ChangeScreenSetting(void)
{
	int i= dataBank_.GetIsFullScreen();

	if (dataBank_.GetIsFullScreen())
	{
		questionStr_ = L"ウィンドウモードに変更しますか？";
	}
	else
	{
		questionStr_ = L"フルスクリーンに変更しますか？";
	}
	yesNoScn_->Init();
	updateSetting_ = [this]() {UpdateScreenSetting(); };
}

void SettingScene::UpdateSettingNormal(void)
{
	//選択中のボタンをイージングで動かす
	menuMng_->NormalUpdate(SELECT_EASE_DISTANCE, SELECT_EASE_TIME, Easing::EASING_TYPE::COS_BACK);
	menuMng_->SelectMenu();
	menuMng_->Update();


	//割合計算
	for (int i=0;i<static_cast<int>(VOLUME_TYPE::MAX);i++)
	{
		volumePer_[i] = sliderUIMng_->GetSliderPercent()[i];
		volume_[i] = volumePer_[i] * VOL_MAX;
	}
	VolumeRefrect();

	auto hitItr = sliderUIMng_->GetIsHitSlider();
	if (sliderUIMng_->GetActiveSliderNum() != -1)
	{
		menuMng_->SetSelectMenuNum(sliderUIMng_->GetActiveSliderNum());
	}
	else if (hitItr != sliderUIMng_->GetSliderUIsEnd())
	{
		menuMng_->SetSelectMenuNum((*hitItr)->GetSliderNum());
	}
	sliderUIMng_->Update();

	const int selectNum = menuMng_->GetSelectMenuNum();
	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK)&& IsQuestionScene())
	{
		ChangeSetting(static_cast<SETTING_STATE>(selectNum));
		return;
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

void SettingScene::UpdateScreenSetting(void)
{
	yesNoScn_->Update();
	if (InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK))
	{
		if (yesNoScn_->GetState() == YesNoScene::YES_NO_STATE::YES)
		{
			dataBank_.GetIsFullScreen() ? dataBank_.SetIsFullScreen(false) : dataBank_.SetIsFullScreen(true);
		}
		ChangeSetting(SETTING_STATE::NORMAL);

	}
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

const bool SettingScene::IsQuestionScene(void) const
{
	const int selectNum=menuMng_->GetSelectMenuNum();
	return selectNum == static_cast<int>(SETTING_STATE::SCREEN_SET)
		|| selectNum == static_cast<int>(SETTING_STATE::EXIT_SETTING);
}
