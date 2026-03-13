#include "GameScene.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Manager/Game/CollisionManager2D.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/DataBank.h"

#include "PauseScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updateFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);

	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	CharacterManager::CreateInstance();
	CollisionManager2D::CreateInstance();
	UIManager::CreateInstance();

}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	CollisionManager2D::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	SoundManager::GetInstance().Release();
	UIManager::GetInstance().Destroy();
}

void GameScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);


	//ポーズ画面のリソース
	pauseScene_ = std::make_shared<PauseScene>();
	pauseScene_->Load();

	UIManager::GetInstance().Load();
	ButtonUIManager::GetInstance().Load();

	CharacterManager::GetInstance().Load();

	//UIManager::GetInstance().Load();
}

void GameScene::Init(void)
{
	changeUpdate_ = {
		{UPDATE_PHASE::NONE,[this]() {ChangeNone(); }},
		{UPDATE_PHASE::FADE,[this]() {ChangeFade(); }},
		{UPDATE_PHASE::NORMAL,[this]() {ChangeNormal(); }},
		{UPDATE_PHASE::SLOW,[this]() {ChangeSlow(); }}
	};
	updatePhase_ = UPDATE_PHASE::NONE;

	CharacterManager::GetInstance().Init();
	UIManager::GetInstance().Init();
}

void GameScene::NoneUpdate(void)
{
	//何もしない
}

void GameScene::FadeUpdate(void)
{
	scnMng_.Fade();
	if (scnMng_.GetIsEndFade())
	{
		ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
	}
}

void GameScene::NormalUpdate(void)
{
	//ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_P))
	{
		scnMng_.PushScene(pauseScene_);
		return;
	}

	//キャラクターの更新
	CharacterManager::GetInstance().Update();

	UIManager::GetInstance().Update();

	//更新はアクション中のみ
	CollisionManager2D::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager2D::GetInstance().Sweep();



#ifdef _DEBUG
	//デバッグ処理
	DebagUpdate();
#endif // _DEBUG


}

void GameScene::NormalDraw(void)
{
	CharacterManager::GetInstance().Draw();

	//UIなどの描画
	CharacterManager::GetInstance().Draw2D();

	UIManager::GetInstance().Draw();

	//UI2DManager::GetInstance().Draw();
#ifdef _DEBUG
	//デバッグ処理
	DebagDraw();
#endif // _DEBUG

}

void GameScene::DirectionDraw(void)
{
	CharacterManager::GetInstance().Draw();

	if (updatePhase_ == UPDATE_PHASE::DIRECTION)
	{
		UIManager::GetInstance().DirectionDraw();
	}

}


void GameScene::ChangeUpdatePhase(const UPDATE_PHASE _phase)
{
	if (updatePhase_ == _phase)return;
	updatePhase_ = _phase;
	changeUpdate_[updatePhase_]();
}

void GameScene::ChangeNone(void)
{
}

void GameScene::ChangeFade(void)
{
	scnMng_.StartFadeIn();
	updateFunc_ = [this]() {FadeUpdate(); };
}

void GameScene::ChangeNormal(void)
{
	CharacterManager::GetInstance().ChangeCharacterNormalUpdate();
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}

void GameScene::ChangeSlow(void)
{
	CharacterManager::GetInstance().ChangeCharacterNormalUpdate();
	updateFunc_ = [this]() {SlowUpdate(); };
}



void GameScene::SlowUpdate(void)
{
	if (--slowFrame_ > 0)return;
	slowFrame_ = FRAME_PER_UPDATE;
	NormalUpdate();
}

void GameScene::OnSceneEnter(void)
{
	//演出状態へ移行
	ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
}

#ifdef _DEBUG
void GameScene::DebagUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME_CLEAR);
	}
	frame_++;  
}

void GameScene::DebagDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x00ff00,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		L"GameScene"
	);

	constexpr float r = 40.0f;
	float angle = DX_PI_F * 2.0f * static_cast<float>(frame_ % 360) / 60.0f;


	//円運動を描画
	DrawCircleAA(
		320+cos(angle) * r, 
		240+sin(angle) * r,
		r, 
		32, 
		0xff8888, 
		true);
}
#endif // _DEBUG