#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Manager/Generic/DataBank.h"
#include "../Manager/Generic/MenuController.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Common/FontController.h"
#include "./SettingScene.h"
#include "../Common/Easing.h"
#include "../Common/TextWriter.h"
#include "TitleScene.h"

TitleScene::TitleScene(void) :
	soundMng_(SoundManager::GetInstance())
{
	//更新関数のセット
	updateFunc_ = [this]() {LoadingUpdate(); };
	//描画関数のセット
	drawFunc_=[this]() {LoadingDraw(); };
	menuController_ = std::make_unique<MenuController>();
	textWtiter_ = std::make_unique<TextWriter>();
	settingScn_ = std::make_shared<SettingScene>();
}

TitleScene::~TitleScene(void)
{
	soundMng_.Stop(SoundManager::SRC::TITLE_BGM);
}

void TitleScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
	//タイトル画像の読み込み
	imgTitleBack = resMng_.Load(ResourceManager::SRC::TITLE_BACK_IMG).handleId_;

	//タイトルロゴの読み込み
	imgTitleLogo = resMng_.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	menuController_->LoadFont(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE);
	//設定シーン
	settingScn_->Load();

	soundMng_.LoadResource(SoundManager::SRC::TITLE_BGM);
	soundMng_.LoadResource(SoundManager::SRC::MOVE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::DESIDE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::GAME_START_SE);

	ButtonUIManager::GetInstance().Load();


	yesNoState_ = YES_NO::NO;
}

void TitleScene::Init(void)
{
	titleSelectFuncTable_ = {
		{TITLE_STATE::EASE_MENU,[this]() {ChangeEaseMenu(); }},
		{TITLE_STATE::MENU,[this]() {ChangeTitleMenu(); }},
		{TITLE_STATE::START_GAME,[this]() {ChangeGameStart(); }},
		//{TITLE_STATE::TUTORIAL,[this]() { UpdateTutorial(); }},
		{TITLE_STATE::EXIT_MENU,[this]() {ChangeExit(); }},
		{TITLE_STATE::SETTING,[this]() {ChangeSetting(); }},
		{TITLE_STATE::EXIT,[this]() { Application::GetInstance().IsGameEnd(); }}
	};

	buttonStrTable_ = {
		{TITLE_BTN::START_GAME,L"ゲームスタート"},
		//{TITLE_BTN::TUTORIAL,L"TUTORIAL"},
		{TITLE_BTN::SCREEN,L"設定"},
		{TITLE_BTN::EXIT,L"ゲーム終了"}
	};

	textWtiter_->AddText(L"あああああああああああああああああああああああああああああああああああああああああああああああああああ");
	yesNoStrTable_ = {
		{YES_NO::YES,L"はい"},
		{YES_NO::NO,L"いいえ"}
	};

	easing_ = std::make_unique<Easing>();
	selectState_ = TITLE_STATE::MENU;
	ChangeState(TITLE_STATE::EASE_MENU);
	selectNum_ = 0;
	easeDistanceCnt_ = 0.0f;
	logoPos_ = { -LOGO_SIZE_X,-LOGO_SIZE_Y };
	logoEaseCnt_ = BUTTON_EASING_TIME;

	int i = 0;
	for (auto& button : buttonStrTable_)
	{
		//イージング演出をするために初期位置は画面外にする
		Vector2 pos = { Application::SCREEN_SIZE_X,static_cast<int>(BUTTON_START_POS_Y + BUTTON_DISTANCE * i) };
		menuController_->AddMenu(static_cast<int>(button.first), button.second, pos);
		i++;
	}

	soundMng_.Play(SoundManager::SRC::TITLE_BGM, SoundManager::PLAYTYPE::LOOP);
}

void TitleScene::PopSceneAfter(void)
{
	ChangeState(TITLE_STATE::MENU);
}

void TitleScene::ChangeState(const TITLE_STATE& _state)
{
	if (selectState_ == _state)return;
	selectState_ = _state;
	titleSelectFuncTable_[selectState_]();
}

void TitleScene::NormalUpdate(void)
{
	textWtiter_->Update();
	updateTitle_();
}

void TitleScene::NormalDraw(void)
{
	DrawExtendGraph(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imgTitleBack,
		true
	);

	//タイトルロゴ
	DrawExtendGraphF(logoPos_.x, logoPos_.y, logoPos_.x + LOGO_SIZE_X, logoPos_.y + LOGO_SIZE_Y, imgTitleLogo, true);
	menuController_->Draw();


	//決定ボタン
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::B_BUTTON_COL_PUSH, DICITION_BTN_POS, DICITION_BTN_SIZE);
	Vector2F strPos = { DICITION_BTN_POS.x + DICITION_BTN_SIZE / 2.0f,DICITION_BTN_POS.y - BTN_STR_OFFSET_X };
	DrawStringToHandle(
		static_cast<int>(strPos.x),
		static_cast<int>(strPos.y),
		L"決定",
		UtilityCommon::WHITE, buttonFontHandle_);

	//戻るボタン
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::A_BUTTON_COL_PUSH, BACK_BTN_POS, DICITION_BTN_SIZE);
	strPos = { BACK_BTN_POS.x + DICITION_BTN_SIZE / 2.0f,BACK_BTN_POS.y - BTN_STR_OFFSET_X };
	DrawStringToHandle(
		static_cast<int>(strPos.x),
		static_cast<int>(strPos.y),
		L"戻る",
		UtilityCommon::WHITE, buttonFontHandle_);


	textWtiter_->Draw();

	if (selectState_ == TITLE_STATE::EXIT_MENU)
	{
		DrawExit();
	}
}

void TitleScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_=[this]() {NormalDraw(); };
}

void TitleScene::UpdateEase(void)
{
	logoEaseCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	//ロゴ座標のいーじんぐ
	logoPos_ = easing_->EaseFunc(START_POS, GOAL_POS, (LOGO_EASING_TIME - logoEaseCnt_) / LOGO_EASING_TIME, Easing::EASING_TYPE::ELASTIC_OUT);

	//メニューの補完
	constexpr int OFFSET = 100;
	menuController_->UpdateDirection(EASING_DIS_TIME, BUTTON_EASING_TIME, Application::SCREEN_HALF_X - OFFSET);

	if (menuController_->IsAllDirectEaseEnd())
	{
		ChangeState(TITLE_STATE::MENU);
	}
}

void TitleScene::UpdateMenu(void)
{
	//選択中のボタンをイージングで動かす
	menuController_->NormalUpdate(SELECT_EASE_DISTANCE, SELECT_EASE_TIME, Easing::EASING_TYPE::COS_BACK);

	//// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	InputManagerS& insS = InputManagerS::GetInstance();

	menuController_->SelectMenu();
	selectNum_ = menuController_->GetSelectMenuNum();

	//OKボタンが押されたら
	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		SoundManager::SRC se;
		//ゲームスタート以外のボタンなら決定音、ゲームスタートならゲームスタート音を鳴らす

		selectNum_ != static_cast<int>(TITLE_BTN::START_GAME) ? se = SoundManager::SRC::DESIDE_BTN_SE : se = SoundManager::SRC::GAME_START_SE;
		soundMng_.Play(se, SoundManager::PLAYTYPE::BACK);
		ChangeState(static_cast<TITLE_STATE>(selectNum_));
	}
}

void TitleScene::UpdateSetting(void)
{
	scnMng_.PushScene(settingScn_);
	return;
}

void TitleScene::UpdateTutorial(void)
{
	//まだ未実装
	ChangeState(TITLE_STATE::MENU);
}

void TitleScene::DrawSetting(void)
{
	std::wstring str = L"";
	bool isFull = DataBank::GetInstance().GetIsFullScreen();
	if (isFull)
	{
		str = L"通常スクリーンにしますか？";
	}
	else
	{
		str = L"フルスクリーンにしますか？";
	}
	menuController_->YesNoDraw(str);
}

void TitleScene::DrawExit(void)
{
	menuController_->YesNoDraw(L"本当にゲームを終了しますか？");
}

void TitleScene::UpdateSelectGame(void)
{
	if (!soundMng_.IsPlay(SoundManager::SRC::GAME_START_SE))
	{
		soundMng_.Play(SoundManager::SRC::GAME_START_SE, SoundManager::PLAYTYPE::BACK);
	}

	//ゲームシーンに遷移
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
}

void TitleScene::UpdateExitMenu(void)
{
	UpdateYesNo();
	InputManagerS& insS = InputManagerS::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		if (menuController_->GetIsYes())
		{
			Application::GetInstance().IsGameEnd();
		}
		else { ChangeState(TITLE_STATE::MENU); }
	}
}
void TitleScene::ChangeEaseMenu(void)
{
	updateTitle_ = [this]() {UpdateEase(); };
}
void TitleScene::ChangeTitleMenu(void)
{
	updateTitle_ = [this]() {UpdateMenu(); };
}
void TitleScene::ChangeSetting(void)
{
	updateTitle_ = [this]() {UpdateSetting(); };
}
void TitleScene::ChangeExit(void)
{
	updateTitle_ = [this]() {UpdateExitMenu(); };
}

void TitleScene::ChangeGameStart(void)
{
	updateTitle_ = [this]() {UpdateSelectGame(); };
}


void TitleScene::UpdateYesNo(void)
{
	InputManagerS& insS = InputManagerS::GetInstance();
	InputManager& ins = InputManager::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::LEFT) || ins.IsTrgDown(KEY_INPUT_A))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->SetYesNoUpdate(true);
	}
	else if (insS.IsTrgDown(INPUT_EVENT::RIGHT) || ins.IsTrgDown(KEY_INPUT_D))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->SetYesNoUpdate(false);
	}
}

void TitleScene::ChangeScreenSize(void)
{
	bool isFull = DataBank::GetInstance().GetIsFullScreen();
	if (isFull)
	{
		DataBank::GetInstance().SetIsFullScreen(false);
	}
	else
	{
		DataBank::GetInstance().SetIsFullScreen(true);
	}
}

