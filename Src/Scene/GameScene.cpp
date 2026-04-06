#include "GameScene.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h" // 【修正】これを追加しました
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Manager/Game/CollisionManager2D.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/DataBank.h"
#include "../Object/Character/Cursor/Cursor.h"

#include "PauseScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updateFunc_ = [this]() {LoadingUpdate(); };
	//描画関数のセット
	drawFunc_ = [this]() {LoadingDraw(); };
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	GameRuleManager::CreateInstance();
	CharacterManager::CreateInstance();
	CollisionManager2D::CreateInstance();
	UIManager::CreateInstance();
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	CollisionManager2D::GetInstance().Destroy();
	cursor_->Release();
	CharacterManager::GetInstance().Destroy();
	SoundManager::GetInstance().Release();
	UIManager::GetInstance().Destroy();
}

void GameScene::Load(void)
{
	GameRuleManager::GetInstance().Load();
	CharacterManager::GetInstance().Load();
	UIManager::GetInstance().Load();

	//カーソル
	CreateCursor();

	//ポーズ
	pauseScene_ = std::make_shared<PauseScene>();
	pauseScene_->Load();

	//マップ
	mapBackImage_ = resMng_.Load(ResourceManager::SRC::MAP_BACK).handleId_;
	mapImage_ = resMng_.Load(ResourceManager::SRC::MAP).handleId_;
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::GAME_BGM);
	//フェーズ遷移用
	changeUpdate_.emplace(UPDATE_PHASE::NONE, [this]() {ChangeNone(); });
	changeUpdate_.emplace(UPDATE_PHASE::FADE, [this]() {ChangeFade(); });
	changeUpdate_.emplace(UPDATE_PHASE::NORMAL, [this]() {ChangeNormal(); });
	changeUpdate_.emplace(UPDATE_PHASE::SLOW, [this]() {ChangeSlow(); });
}

void GameScene::Init(void)
{
	GameRuleManager::GetInstance().Init();
	CharacterManager::GetInstance().Init();
	CollisionManager2D::GetInstance().Update();
	UIManager::GetInstance().Init();

	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, SoundManager::PLAYTYPE::LOOP);
	//デバッグ用
	frame_ = 0;
	slowFrame_ = FRAME_PER_UPDATE;
}
//
//void GameScene::LoadingUpdate(void)
//{
//}

void GameScene::NoneUpdate(void)
{
}

void GameScene::FadeUpdate(void)
{
}

void GameScene::NormalUpdate(void)
{
	// === クリア判定の監視 ===
	if (GameRuleManager::GetInstance().IsGameClear())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME_CLEAR);
		return;
	}
	else if (GameRuleManager::GetInstance().IsGameOver())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME_OVER);
		return;
	}

#ifdef _DEBUG
	DebagUpdate();
#endif // _DEBUG

	GameRuleManager::GetInstance().Update();
	CharacterManager::GetInstance().Update();
	
	cursor_->Update();
	cursor_->Sweep();

	UIManager::GetInstance().Update();
	CollisionManager2D::GetInstance().Update();
	CollisionManager2D::GetInstance().Sweep();

	//ポーズ
	InputManagerS& insS = InputManagerS::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::PAUSE))
	{
		SceneManager::GetInstance().CreateScene(pauseScene_);
	}

}


void GameScene::NormalDraw(void)
{
#ifdef _DEBUG
	DebagDraw();
#endif // _DEBUG

	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, mapBackImage_, true);
	DrawRotaGraph(Application::SCREEN_HALF_X, 380, 0.6, 0.0, mapImage_, true);

	GameRuleManager::GetInstance().Draw();

	CharacterManager::GetInstance().Draw();

	if (updatePhase_ == UPDATE_PHASE::DIRECTION)
	{
		UIManager::GetInstance().DirectionDraw();
	}

	cursor_->Draw();
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
	CharacterManager::GetInstance().Update();
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}

void GameScene::ChangeSlow(void)
{
	CharacterManager::GetInstance().Update();
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

void GameScene::CreateCursor(void)
{
	//生成
	cursor_ = std::make_unique<Cursor>();

	//読み込みと初期化
	cursor_->Load();
	cursor_->Init();
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
	//printfDx("GameScene\n");
}
#endif // _DEBUG