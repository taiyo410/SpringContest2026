#include "../../Pch.h"
#include "SceneGame.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Decoration/SoundManager.h"
#include "../../Manager/System/TimeManager.h"
#include "SceneScore.h"
#include "../../Manager/System/Loading.h"
#include "../../Object/Charactor/Player/Player.h"
#include "../../Object/Charactor/Daimyo/DaimyoSatsuma.h"
#include "../../Object/Charactor/Daimyo/DaimyoChoshu.h"
#include "../../Object/Charactor/Daimyo/DaimyoHizen.h"
#include "../../Object/Charactor/Daimyo/DaimyoAizu.h"
#include "../../Object/Charactor/Daimyo/DaimyoBase.h"
#include "../../Object/Charactor/Daimyo/DaimyoTosa.h"
#include "../SubScene/PauseScene.h"
#include "../../Manager/System/DataManager.h"
#include "../../DrawUI/Font.h"

// コンストラクタ
SceneGame::SceneGame(void)
{
	// プレイヤーん生成
	player_ = std::make_shared<Player>();

	daimyos_.push_back(std::make_shared<DaimyoSatsuma>());
	daimyos_.push_back(std::make_shared<DaimyoChoshu>());
	daimyos_.push_back(std::make_shared<DaimyoTosa>());
	daimyos_.push_back(std::make_shared<DaimyoHizen>());
	daimyos_.push_back(std::make_shared<DaimyoAizu>());
}

// 読み込み
void SceneGame::Load()
{
	SceneBase::Load();

	// ここで必要なリソースを読み込む
	ResourceManager::GetInstance().InitGame();

	
	Loading::GetInstance()->SetProgress(20.0f);

	//　プレイヤーのリソース読み込み
	if (player_) { player_->InitLoad(); }

	Loading::GetInstance()->SetProgress(45.0f);

	Loading::GetInstance()->SetProgress(60.0f);

	// サウンドの読み込み
	
	Loading::GetInstance()->SetProgress(80.0f);

	//時間カウントリセット
	TimeManager::GetInstance().Reset();

	Loading::GetInstance()->SetProgress(100.0f);
}

// 読み込み終了
void SceneGame::EndLoad()
{
	SceneBase::EndLoad();
}

// 初期化
void SceneGame::Init(void)
{
	// カメラ設定
	auto camera = SceneManager::GetInstance().GetCamera();
	camera->ChangeMode(Camera::MODE::FREE);
	SetMouseDispFlag(TRUE);

	// 全ての藩を初期化
	for (auto& daimyo : daimyos_)
	{
		daimyo->InitLoad();
		daimyo->Init();
	}

	// プレイヤーの初期化
	if (player_) { player_->Init(); }

}

// 更新処理
void SceneGame::Update(void)
{
	auto& sound = SoundManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = TimeManager::GetInstance();
	auto camera = SceneManager::GetInstance().GetCamera();
	auto loader = Loading::GetInstance();

	// --- 追加：Escキーが押されたらポーズシーンを表示 ---
	if (input.IsTrgDown(KEY_INPUT_ESCAPE))
	{
		// PauseSceneを現在のシーンの上に重ねる
		DataManager::GetInstance().SaveFinalRecord();
		SceneManager::GetInstance().PushScene(std::make_shared<PauseScene>());

		// ポーズから戻った時に連打にならないよう、ここで処理を抜けるのが安全です
		return;
	}

	for (auto& daimyo : daimyos_) {
		daimyo->Update();

		// 参勤交代のお金を回収する処理
		if (daimyo->CanTransferToEdo())
		{
			long long gain = daimyo->TransferMoney();
			player_->AddEdoMoney(gain); // ここで江戸の資金が増える！
			DataManager::GetInstance().AddCount();

			player_->AddFrustration(2.0f);
		}

		// 1. 援助成功のチェック
		if (daimyo->popHelpSuccess())
		{
			if (player_->ConsumeEdoMoney(10000000))
			{
				player_->AddFrustration(-10.0f);
				// お金が減ったことが分かるようSEを鳴らすのもアリ
			}
		}

		// 2. 援助失敗のチェック
		if (daimyo->popHelpFailure())
		{
			player_->AddFrustration(20.0f);
		}
	}

	// プレイヤーの更新
	if (player_) { player_->Update(); }

	if (player_->IsGameOver())
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneScore>());
	}

}

// 描画処理
void SceneGame::Draw(void)
{
	auto camera = SceneManager::GetInstance().GetCamera();

	for (auto& daimyo : daimyos_) {
		daimyo->Draw();
	}

	// プレイヤーの描画
	if (player_) { player_->Draw(); }

	// --- 年代のシンプル表示 ---
	int year = DataManager::GetInstance().GetCurrentYear();
	std::string yearText = std::to_string(year) + " 年";

	// 画面横幅1920の中央付近に表示 (フォントサイズ40)
	Font::GetInstance().DrawDefaultText(920, 30, yearText.c_str(), GetColor(255, 255, 255), 40);

#ifdef _DEBUG
	//デバック表示
	DrawDebug();
#endif // _DEBUG
}

// 解放処理
void SceneGame::Release(void)
{
}

// 描画(デバック)
void SceneGame::DrawDebug(void)
{
}