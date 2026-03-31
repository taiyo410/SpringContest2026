#include "../pch.h"
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../../Common/TextWriter.h"
#include "../../Common/FontController.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void):
	soundMng_(SoundManager::GetInstance())
{
	//更新関数のセット
	updateFunc_ = std::bind(&GameOverScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_=[this]() {LoadingDraw(); };
}

GameOverScene::~GameOverScene(void)
{
	soundMng_.Stop(SoundManager::SRC::GAME_OVER_BGM);
}

void GameOverScene::Load(void)
{
	// フォントコントローラーの作成
	fontController_ = std::make_unique<FontController>();

	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
	
	// メッセージ用フォントの登録
	messageFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_BOKUTATI, 40, 0);

	imgGameOver_ = resMng_.GetInstance().Load(ResourceManager::SRC::GAME_OVER_IMG).handleId_;

	//BGMロード
	soundMng_.GetInstance().LoadResource(SoundManager::SRC::GAME_OVER_BGM);

	// テキストライターの生成とテキストの追加
	textWriter_ = std::make_unique<TextWriter>();
	textWriter_->AddText(L"西暦1682年\n各地域の大名に反乱を起こされ、\n家康の家系は滅びました…");

}

void GameOverScene::Init(void)
{
	//BGM再生
	soundMng_.GetInstance().Play(SoundManager::SRC::GAME_OVER_BGM, SoundManager::PLAYTYPE::LOOP);
	SoundManager::GetInstance().SetSystemVolume(BGM_VOL, static_cast<int>(SoundManager::TYPE::BGM));

}

void GameOverScene::NormalUpdate(void)
{
	// 文字送りの更新
	textWriter_->Update();
	
	if (InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::NormalDraw(void)
{
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgGameOver_, true);

	// 文字送りの描画(画面中央付近に、専用フォントで描画する)
	int messagePosX = 200;
	int messagePosY = Application::SCREEN_HALF_Y - 50;
	textWriter_->Draw(messagePosX, messagePosY, messageFontHandle_);

	DrawFormatStringToHandle(
		BACK_TITLE_STRING_POS.x,
		BACK_TITLE_STRING_POS.y,
		UtilityCommon::WHITE,
		buttonFontHandle_,
		L"'Bボタンまたはスペースキー'でタイトルに戻る"
	);
}

void GameOverScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}
