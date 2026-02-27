#pragma once
#include "../CharactorBase.h"

class DaimyoBase : public CharactorBase
{
public:

	enum class SankinState {
		Normal,      // 通常（蓄財中）
		Selecting,   // 参勤交代のルート選択中
		Traveling,   // 江戸へ移動中
		Result
	};

	//@brief コンストラクタ
	DaimyoBase(void);

	//@brief デストラクタ
	virtual ~DaimyoBase(void) override;

	//@brief リソースのロード
	virtual void InitLoad(void) override;

	//@brief 初期化処理
	virtual void Init(void) override;

	//@brief 更新処理
	virtual void Update(void) override;

	//@brief 描画処理
	virtual void Draw(void) const override;

	//@brief 現金を消費する
	void ReduceMoney(long long amount);

	//@brief 謀反（上限到達）判定
	bool IsRebellion(void) const { return money_ >= REBELLION_THRESHOLD; }

	// @brief 座標を直接設定する
	void SetPosition(float x, float y) { trans_.pos = { x, y, 0.0f }; }

	//  @brief江戸に送金可能かチェック
	bool CanTransferToEdo(void) const { return (sankinState_ == SankinState::Result && !isGainTransferred_); }

	long long TransferMoney(void);

	// 援助成功フラグ
	bool popHelpSuccess(void);

	// 援助失敗フラグ
	bool popHelpFailure(void);

protected:

	// --- ゲームバランス調整用定数 ---
	static constexpr float ACCUMULATION_INTERVAL = 5.0f;       // 加算間隔（秒）
	static constexpr long long ADD_AMOUNT = 3000000;           // 加算額（300万）
	static constexpr long long REBELLION_THRESHOLD = 60000000; // 謀反ライン（6000万）
	static constexpr long long SANKIN_COST = 10000000;         // 参勤交代に必要な金額
	static constexpr float HELP_LIMIT = 20.0f;                 // 制限時間

	bool isRequestingHelp_;    // 援助要請中か
	float helpTimer_;           // 援助制限時間のカウント (0～20秒)
	bool helpSuccessFlag_;
	bool helpFailureFlag_;
	

	// イベント発生用のランダムタイマー
	float nextEventTimer_;

	long long money_;           // 現在の蓄財額
	float accumulationTimer_;   // 5秒カウント用タイマー
	unsigned int color_;        // 四角形描画用の色
	float accumulationSpeed_;

	std::string resultMessage_; // 「山賊に襲われた！」などの文言
	long long lastGain_;    // 今回手に入れた金額
	float resultDisplayTimer_; // リザルトを表示しておく時間
	bool isGainTransferred_;

	// 参勤交代用メンバ
	SankinState sankinState_;   // 現在の状態
	float sankinTimer_;         // 参勤交代の残り時間
	int selectedRoute_;         // 選択されたルート(1～3)

	// 選択肢のクリック判定用（Draw時に座標を保存しUpdateで使用）
	struct Rect { int x, y, w, h; };
	mutable Rect routeRects[3];

	// 全大名で共有：誰かが選択肢を開いていたら true にし、他の大名のクリックを無効化する
	static bool IsAnySelecting;

	void StartSankin(int route, float duration);
	void FinishSankin(void);
};

