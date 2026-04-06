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
	// 初期化
	isShowDescriptionText_ = true;
	oldRightClick_ = false;
	descFontHandle_ = -1;

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
	// フォントハンドルの削除
	if (descFontHandle_ != -1)
	{
		DeleteFontToHandle(descFontHandle_);
	}

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

	// 説明テキスト用のフォントハンドルを作成
	descFontHandle_ = CreateFontToHandle(FontManager::FONT_BOKUTATI, 24, 3, DX_FONTTYPE_EDGE);
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

	////ポーズ
	//InputManagerS& insS = InputManagerS::GetInstance();
	//if (insS.IsTrgDown(INPUT_EVENT::PAUSE))
	//{
	//	SceneManager::GetInstance().PushScene(pauseScene_);
	//}

	// 右クリックによる説明テキストの表示切り替え処理
	bool currentRightClick = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;
	if (currentRightClick && !oldRightClick_)
	{
		// 押された瞬間にフラグを反転させる
		isShowDescriptionText_ = !isShowDescriptionText_;
	}
	oldRightClick_ = currentRightClick;

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

	GameRuleManager::GetInstance().DrawSelect();

	if (updatePhase_ == UPDATE_PHASE::DIRECTION)
	{
		UIManager::GetInstance().DirectionDraw();
	}

	cursor_->Draw();

	// 説明テキストの描画
	const TCHAR* baseText = TEXT("右クリックで説明テキスト");
	const TCHAR* stateText = isShowDescriptionText_ ? TEXT("非表示") : TEXT("表示");

	// 作成した縁取り付きフォントハンドルを使って文字の描画幅を計算
	int baseTextW = GetDrawStringWidthToHandle(baseText, -1, descFontHandle_);
	int stateTextW = GetDrawStringWidthToHandle(baseText, -1, descFontHandle_);
	int totalW = baseTextW + stateTextW;

	// 描画開始の座標を計算
	int startX = Application::SCREEN_SIZE_X - totalW + 200;
	int startY = Application::SCREEN_SIZE_Y - 30;

	// 前半部分を白色で描画
	DrawStringToHandle(startX, startY, baseText, GetColor(255, 255, 255), descFontHandle_);

	// 後半部分(表示/非表示)を赤色で、前半の幅分だけずらして描画
	DrawStringToHandle(startX + baseTextW, startY, stateText, GetColor(255, 50, 50), descFontHandle_);
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