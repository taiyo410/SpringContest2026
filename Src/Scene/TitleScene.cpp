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
	
	// タイトル背景
	imgSky_ = resMng_.Load(ResourceManager::SRC::TITLE_SKY).handleId_;

	imgMountain_ = resMng_.Load(ResourceManager::SRC::TITLE_MOUNTAIN).handleId_;

	imgCloud_ = resMng_.Load(ResourceManager::SRC::TITLE_CLOUD).handleId_;

	imgRoad_ = resMng_.Load(ResourceManager::SRC::TITLE_ROAD).handleId_;

	imgKago_ = resMng_.Load(ResourceManager::SRC::TITLE_KAGO).handleId_;

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
	// スクロール用変数の初期化
	scrollCloudX_ = 0.0f;
	scrollRoadX_ = 0.0f;

	// ロゴスタンプ演出用変数の初期化
	logoWaitTimer_ = 0.0f;
	logoStampEaseCnt_ = LOGO_STAMP_TIME;
	isLogoAppeared_ = false;
	currentLogoScale_ = 3.0f;
	currentLogoAlpha_ = 0;

	// プロローグ演出用変数の初期化
	isPrologue_ = false;
	prologueTimer_ = 0.0f;

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

	prologueText_ = L"1635年。天下を平定し、江戸幕府を開いた徳川家康は、\n諸大名の統制と財力の削圧を目的とし、武家諸法度を改訂。\n\n「参勤交代」の制を定めた。\n\n参勤交代を実行し、\n各地域の大名に反乱を起こされることを阻止しよう！";
	prologueTextDispCnt_ = 0.0f;
	prologueTextDispLen_ = 0;
	
	yesNoStrTable_ = {
		{YES_NO::YES,L"はい"},
		{YES_NO::NO,L"いいえ"}
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
		menuMng_->AddMenu(static_cast<int>(button.first), button.second, pos, true);
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
	// 背景のスクロール処理
	// 雲は0.5倍サイズにするため、ループの判定幅も0.5倍に変更
	float cloudW = Application::SCREEN_SIZE_X * 0.5f;

	scrollCloudX_ -= CLOUD_SPEED;

	if (scrollCloudX_ <= -cloudW) scrollCloudX_ += cloudW;

	scrollRoadX_ -= ROAD_SPEED;
	if (scrollRoadX_ <= -Application::SCREEN_SIZE_X * 2.0f) scrollRoadX_ += Application::SCREEN_SIZE_X * 2.0f;

	// 14秒後のロゴスタンプ演出処理
	if (selectState_ != TITLE_STATE::START_STATE) {
		if (!isLogoAppeared_) {
				logoWaitTimer_ += scnMng_.GetDeltaTime();
				if(logoWaitTimer_ >= LOGO_WAIT_TIME) {
					isLogoAppeared_ = true;
			}
		}
		else {
			if(logoStampEaseCnt_ > 0.0f) {
				logoStampEaseCnt_ -= scnMng_.GetDeltaTime();
				if (logoStampEaseCnt_ < 0.0f) logoStampEaseCnt_ = 0.0f;

				// 演出時間に応じたイージング
				float t = (LOGO_STAMP_TIME - logoStampEaseCnt_) / LOGO_STAMP_TIME;
				currentLogoScale_ = easing_->EaseFunc(3.0f, 1.0f, t, Easing::EASING_TYPE::QUAD_IN);
				currentLogoAlpha_ = easing_->EaseFunc(0.0f, 255.0f, t, Easing::EASING_TYPE::LERP);
			}
		}
	}

	//textWtiter_->Init();
	cursor_->Update();

	updateTitle_();
	
	// テキスト描画中 : ボタン入力を受け付けないようにする
	if (selectState_ != TITLE_STATE::START_STATE) {
		menuMng_->Update();
	}

	//更新はアクション中のみ
	CollisionManager2D::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager2D::GetInstance().Sweep();
}

void TitleScene::NormalDraw(void)
{
	// スタートシーンとタイトルシーンの共通背景として描画
	DrawBackground();

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

	//if (inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	//{
	//	soundMng_.Play(SoundManager::SRC::TITLE_BGM, SoundManager::PLAYTYPE::LOOP);

	//	ChangeState(TITLE_STATE::EASE_MENU);
	//	return;
	//}

	if (!isPrologue_)
	{
		if(inputMngS_.IsTrgDown(INPUT_EVENT::OK))
		{
			// クリックされたらプロローグ演出へ移行
			soundMng_.Play(SoundManager::SRC::TITLE_BGM, SoundManager::PLAYTYPE::LOOP);
			isPrologue_ = true;
			prologueTimer_ = 0.0f;
		}
	}
	else
	{
		// プロローグ中の更新処理
		prologueTimer_ += scnMng_.GetDeltaTime();
		
		// 自作のテキスト表示更新処理
		prologueTextDispCnt_ += 1.0f;
		if(prologueTextDispCnt_ >= PROLOGUE_TEXT_SPD)
		{
			prologueTextDispCnt_ = 0.0f;
			if(prologueTextDispLen_ < prologueText_.length())
			{
				prologueTextDispLen_++;
			}
		}

		InputManager& ins = InputManager::GetInstance();

		// Shiftキーでスキップ、または14秒経過でタイトルシーンへ移行
		if(prologueTimer_ >= PROLOGUE_TIME || ins.IsNew(KEY_INPUT_LSHIFT) || ins.IsNew(KEY_INPUT_RSHIFT))
		{
			// タイトルシーンへ移行した瞬間にBGMを再生
			ChangeState(TITLE_STATE::EASE_MENU);
			return;
		}
	}
}

void TitleScene::UpdateEase(void)
{
	//logoEaseCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	////ロゴ座標のイージング
	//logoPos_ = easing_->EaseFunc(START_POS, GOAL_POS, (LOGO_EASING_TIME - logoEaseCnt_) / LOGO_EASING_TIME, Easing::EASING_TYPE::ELASTIC_OUT);

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

void TitleScene::DrawBackground(void)
{
	int w = Application::SCREEN_SIZE_X;
	int h = Application::SCREEN_SIZE_Y;

	// 1. sky (スクロール無し)
	DrawExtendGraphF(0, 0, w, h, imgSky_, true);

	// 2. cloud (スクロールあり、0.5倍サイズ)
	float cloudW = static_cast<int>(w * 0.5f);
	float cloudH = static_cast<int>(h * 0.5f);

	int cX = static_cast<int>(scrollCloudX_);

	// 1枚目(通常)	
	DrawExtendGraph(cX, 0, cX + cloudW, cloudH, imgCloud_, true);
	
	// 2枚目(通常)
	DrawExtendGraph(cX + cloudW - 1, 0, cX + cloudW * 2 - 1, cloudH, imgCloud_, true);

	// 3枚目(通常)
	DrawExtendGraph(cX + cloudW * 2 -2, 0, cX + cloudW * 3 - 2, cloudH, imgCloud_, true);

	// mountain (スクロール無し)
	DrawExtendGraphF(0, 0, w, h, imgMountain_, true);
	
	// 4. road (スクロール有り)
	int rX = static_cast<int>(scrollRoadX_);

	// 道の裏技(通常描画と反転描画を交互に行う)
	// 1枚目(通常)	
	DrawExtendGraphF(rX, 0, rX + w, h, imgRoad_, true);
	
	// 2枚目(左右反転) : DrawModiGraphを使って頂点座標をクロスさせることで反転します
	DrawModiGraph(rX + w * 2, 0, rX + w, 0, rX + w, h, rX + w * 2, h, imgRoad_, true);

	// 3枚目(通常)	
	DrawExtendGraphF(rX + w * 2,0, rX + w * 3, h, imgRoad_, true);

	// 5. kago (スクロール無し)
	DrawExtendGraphF(KAGO_POS_X, KAGO_POS_Y, KAGO_POS_X + KAGO_SIZE_X, KAGO_POS_Y + KAGO_SIZE_Y, imgKago_, true);

}

void TitleScene::DrawMenu(void)
{
	// タイトルロゴ
	if (isLogoAppeared_) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, currentLogoAlpha_);
		float cx = Application::SCREEN_HALF_X;
		float cy = Application::SCREEN_HALF_Y - 100.0f;
		float helfW = (LOGO_SIZE_X / 2.0f) * currentLogoScale_;
		float helfH = (LOGO_SIZE_Y / 2.0f) * currentLogoScale_;
		DrawExtendGraphF(cx - helfW, cy - helfH, cx + helfW, cy + helfH, imgTitleLogo, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawExtendGraph(
	//	0,
	//	0,
	//	Application::SCREEN_SIZE_X,
	//	Application::SCREEN_SIZE_Y,
	//	imgTitleBack,
	//	true
	//);

	//タイトルロゴ
	//DrawExtendGraphF(logoPos_.x, logoPos_.y, logoPos_.x + LOGO_SIZE_X, logoPos_.y + LOGO_SIZE_Y, imgTitleLogo, true);
	menuMng_->Draw();


	//textWtiter_->Draw();

	if (selectState_ == TITLE_STATE::EXIT_MENU)
	{
		DrawExit();
	}
}

void TitleScene::DrawStart(void)
{
	//DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xff0000, true);

	if (!isPrologue_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, stringAlpha_);
		UtilityDraw::DrawStringCenterToFontHandle(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 100, UtilityCommon::WHITE, buttonFontHandle_, L"Push To Click");
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		// 画面全体を薄い黒色で覆う
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150); // アルファ値で薄くする
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// テキストを描画する
		//textWtiter_->Draw(100, 200, buttonFontHandle_);

		// 「参勤交代」だけを赤くする自作テキスト描画処理
		int curX = 100;
		int curY = 200;
		for (int i = 0; i < prologueTextDispLen_ && i < prologueText_.length(); ) {
			if (i + 4 <= prologueText_.length() && prologueText_.substr(i, 4) == L"参勤交代") {
				int drawLen = std::min(4, prologueTextDispLen_ - i);
				for(int j = 0; j < drawLen; j++) {
					std::wstring c = prologueText_.substr(i + j, 1);
					DrawStringToHandle(curX, curY, c.c_str(), UtilityCommon::RED, buttonFontHandle_);
					curX += GetDrawStringWidthToHandle(c.c_str(), c.length(), buttonFontHandle_);
				}
				i += drawLen;
				if (drawLen < 4)break;
			}
			else {
				std::wstring c = prologueText_.substr(i, 1);
				if (c == L"\n") {
					curX = 100;
					curY += GetFontSizeToHandle(buttonFontHandle_) + 10;	// 改行+少しの行間
				}
				else {
					DrawStringToHandle(curX, curY, c.c_str(), UtilityCommon::WHITE, buttonFontHandle_);
					curX += GetDrawStringWidthToHandle(c.c_str(), c.length(), buttonFontHandle_);
				}
				i++;
			}
		}

		// スキップの案内を点滅で表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, stringAlpha_);
		
		
		DrawStringToHandle(Application::SCREEN_SIZE_X - 350, Application::SCREEN_SIZE_Y - 40, L"Press Shift to Skip", UtilityCommon::WHITE, buttonFontHandle_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

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

