#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Common/Easing.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene(void):
	soundMng_(SoundManager::GetInstance()),
	easeCnt_(0.0f),
	strYPos_(SceneBase::BACK_TITLE_STRING_POS.y)
{
	//更新関数のセット
	updateFunc_ = [this]() {LoadingUpdate(); };
	//描画関数のセット
	drawFunc_=[this]() {LoadingDraw(); };
}

GameClearScene::~GameClearScene(void)
{
	soundMng_.Stop(SoundManager::SRC::GAME_CLEAR_BGM);
}

void GameClearScene::Load(void)
{
	imgGameClear_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_CLEAR_IMG).handleId_;
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
}

void GameClearScene::Init(void)
{
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FIXED_POINT);

	easing_ = std::make_unique<Easing>();

}

void GameClearScene::NormalUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	InputManagerS& insS = InputManagerS::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	else if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameClearScene::NormalDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0xff0000,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		L"GameClearScene"
	);

	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgGameClear_, true);

	DrawFormatStringToHandle(
		SceneBase::BACK_TITLE_STRING_POS.x,
		static_cast<int>(strYPos_),
		UtilityCommon::WHITE,
		buttonFontHandle_,
		L"'Bボタンまたはスペースキー'でタイトルに戻る"
	);

}

void GameClearScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = std::bind(&GameClearScene::NormalUpdate, this);
	updateFunc_ = [this]() {NormalUpdate(); };
	//drawFunc_ = std::bind(&GameClearScene::NormalDraw, this);
	drawFunc_=[this]() {NormalDraw(); };
}
