#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityDraw.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Manager/Generic/DataBank.h"
#include "../Manager/Generic/MenuManager.h"
#include "../Manager/Game/CollisionManager2D.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Common/FontController.h"
#include "./StartScene.h"
#include "./SettingScene.h"
#include "./YesNoScene.h"
#include "../Common/Easing.h"
#include "../Common/TextWriter.h"
#include "../Object/Character/Cursor/Cursor.h"

#include "../Object/UI/GaugeController.h"
#include "../Object/UI/ArrowController.h"

#include "TitleScene.h"

TitleScene::TitleScene(void) :
	soundMng_(SoundManager::GetInstance())
{
	CollisionManager2D::CreateInstance();

	//更新関数のセット
	updateFunc_ = [this]() {LoadingUpdate(); };
	//描画関数のセット
	drawFunc_ = [this]() {LoadingDraw(); };
	menuMng_ = std::make_unique<MenuManager>();
	textWtiter_ = std::make_unique<TextWriter>();
	settingScn_ = std::make_shared<SettingScene>();
	cursor_ = std::make_unique<Cursor>();
	yesNoscn_ = std::make_unique<YesNoScene>(Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y), Vector2F(600, 200));
}

TitleScene::~TitleScene(void)
{
	soundMng_.Stop(SoundManager::SRC::TITLE_BGM);
	CollisionManager2D::GetInstance().Destroy();
}

void TitleScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
	//タイトル画像の読み込み
	imgTitleBack = resMng_.Load(ResourceManager::SRC::TITLE_BACK_IMG).handleId_;

	//タイトルロゴの読み込み
	imgTitleLogo = resMng_.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	menuMng_->LoadFont(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE);
	//設定シーン
	settingScn_->Load();
	soundMng_.LoadResource(SoundManager::SRC::TITLE_BGM);
	soundMng_.LoadResource(SoundManager::SRC::MOVE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::DESIDE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::GAME_START_SE);

	ButtonUIManager::GetInstance().Load();
	cursor_->Load();
	yesNoscn_->Load();
}

void TitleScene::Init(void)
{
	titleSelectFuncTable_ = {
		{TITLE_STATE::EASE_MENU,[this]() {ChangeEaseMenu(); }},
		{TITLE_STATE::MENU,[this]() {ChangeTitleMenu(); }},
		{TITLE_STATE::START_GAME,[this]() {ChangeGameStart(); }},
		//{TITLE_STATE::TUTORIAL,[this]() { UpdateTutorial(); }},
		{TITLE_STATE::START_STATE,[this]() {ChangeStart(); }},
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
	easing_ = std::make_unique<Easing>();
	selectState_ = TITLE_STATE::MENU;
	ChangeState(TITLE_STATE::START_STATE);
	selectNum_ = 0;
	easeDistanceCnt_ = 0.0f;
	logoPos_ = { -LOGO_SIZE_X,-LOGO_SIZE_Y };
	logoEaseCnt_ = BUTTON_EASING_TIME;

	int i = 0;
	for (auto& button : buttonStrTable_)
	{
		//イージング演出をするために初期位置は画面外にする
		Vector2 pos = { Application::SCREEN_SIZE_X,static_cast<int>(BUTTON_START_POS_Y + BUTTON_DISTANCE * i) };
		menuMng_->AddMenu(static_cast<int>(button.first), button.second, pos,true);
		i++;
	}
	menuMng_->Init();
	yesNoscn_->Init();
	//soundMng_.Play(SoundManager::SRC::TITLE_BGM, SoundManager::PLAYTYPE::LOOP);
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
	//textWtiter_->Init();
	cursor_->Update();

	updateTitle_();
	menuMng_->Update();
	//更新はアクション中のみ
	CollisionManager2D::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager2D::GetInstance().Sweep();
}

void TitleScene::NormalDraw(void)
{
	drawTitle_();

	cursor_->Draw();
	//gaugeCntl_->Draw();
}

void TitleScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_=[this]() {NormalDraw(); };
}

void TitleScene::UpdateStart(void)
{
	stringAlpha_ = easing_->EaseFunc(0, 255, blendCnt_ / BLEND_TIME, Easing::EASING_TYPE::LERP_COMEBACK);
	blendCnt_ > BLEND_TIME ? blendCnt_ = 0 : blendCnt_ += scnMng_.GetDeltaTime();

	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		ChangeState(TITLE_STATE::EASE_MENU);
		return;
	}
}

void TitleScene::UpdateEase(void)
{
	logoEaseCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	//ロゴ座標のイージング
	logoPos_ = easing_->EaseFunc(START_POS, GOAL_POS, (LOGO_EASING_TIME - logoEaseCnt_) / LOGO_EASING_TIME, Easing::EASING_TYPE::ELASTIC_OUT);

	//メニューの補完
	constexpr int OFFSET = 100;
	menuMng_->UpdateDirection(EASING_DIS_TIME, BUTTON_EASING_TIME, Application::SCREEN_HALF_X - OFFSET);

	if (menuMng_->IsAllDirectEaseEnd())
	{
		ChangeState(TITLE_STATE::MENU);
	}
}

void TitleScene::UpdateMenu(void)
{
	//選択中のボタンをイージングで動かす
	menuMng_->NormalUpdate(SELECT_EASE_DISTANCE, SELECT_EASE_TIME, Easing::EASING_TYPE::COS_BACK);

	//// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	InputManagerS& insS = InputManagerS::GetInstance();

	menuMng_->SelectMenu();
	selectNum_ = menuMng_->GetSelectMenuNum();

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

void TitleScene::DrawMenu(void)
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
	menuMng_->Draw();


	textWtiter_->Draw();

	if (selectState_ == TITLE_STATE::EXIT_MENU)
	{
		DrawExit();
	}
}

void TitleScene::DrawStart(void)
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xff0000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, stringAlpha_);
	UtilityDraw::DrawStringCenterToFontHandle(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, UtilityCommon::WHITE, buttonFontHandle_, L"Push To Click");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void TitleScene::DrawExit(void)
{
	yesNoscn_->Draw(L"本当にゲームを終了しますか？");
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
	yesNoscn_->Update();
	if (InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK))
	{
		if (yesNoscn_->GetState() == YesNoScene::YES_NO_STATE::YES)
		{
			Application::GetInstance().IsGameEnd();
		}
		else
		{
			ChangeState(TITLE_STATE::MENU);
		}
	}
}
void TitleScene::ChangeStart(void)
{
	drawTitle_ = [this]() {DrawStart(); };
	updateTitle_ = [this](){ UpdateStart(); };
}
void TitleScene::ChangeEaseMenu(void)
{
	drawTitle_ = [this]() {DrawMenu(); };
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

