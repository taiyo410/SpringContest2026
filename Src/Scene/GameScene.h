#pragma once
#include <memory>
#include <map>
#include <functional>
#include "SceneBase.h"

class PauseScene;
class PixelMaterial;
class PixelRenderer;
class CharacterBase;
class SkyDome;
class Player;
class Enemy;
class Stage;
class CardSystem;

class DrawUI3D;

class GameScene : public SceneBase
{

public:
	
	enum class UPDATE_PHASE   
	{
		NONE,
		FADE,
		NORMAL,
		DIRECTION,
		CLEAR_DIRECTION,
		OVER_DIRECTION,
		SLOW

	};

	/// @brief コンストラクタ
	/// @param  
	GameScene(void);

	/// @brief デストラクタ
	/// @param  
	~GameScene(void)override;

	/// @brief 読み込み
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

private:



	//スローの時、何フレームに1回更新するか
	static constexpr int FRAME_PER_UPDATE = 2;

	//ポストエフェクト用スクリーン
	int postEffectScreen_;

	//マップ
	int mapImage_;

	int frame_;

	//スローカウンタ(フレーム)
	int slowFrame_;

	//更新フェーズ
	UPDATE_PHASE updatePhase_;
	std::map<UPDATE_PHASE, std::function<void(void)>>changeUpdate_;

	//ポーズ画面
	std::shared_ptr<PauseScene> pauseScene_;

	// 【修正】LoadingUpdate と LoadingDraw の宣言を追加
	void LoadingUpdate(void) override;
	void LoadingDraw(void) override;

	//更新関数
	void NoneUpdate(void);				//何もしない
	void FadeUpdate(void);				//フェード
	void NormalUpdate(void) override;	//通常
	void SlowUpdate(void);				//スロー
	//描画関数
	void NormalDraw(void) override;		//通常
	void DirectionDraw(void);			//演出時の描画

	/// @brief 更新フェーズの変更
	/// @param _phase 
	void ChangeUpdatePhase(const UPDATE_PHASE _phase);

	//各状態の遷移処理
	void ChangeNone(void);
	void ChangeFade(void);
	void ChangeNormal(void);
	void ChangeSlow(void);
	//処理の変更
	void OnSceneEnter(void) override;
	
#ifdef _DEBUG
	//デバッグ処理
	void DebagUpdate(void);
	void DebagDraw(void);
#endif // _DEBUG



};