#pragma once
#include "../Template/Singleton.h"

class FontController;

class GameRuleManager : public Singleton<GameRuleManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<GameRuleManager>;

public:

	//基本単位
	static constexpr int UNITS = 100000;

	//初期所持金
	static constexpr int INITIAL_FUNDS = 50;

	//フォントサイズ
	static constexpr int FONT_SIZE = 20;

	//最大不満度
	static constexpr int DISSATISFACTION_MAX = 100;

	//開始年数 (追加)
	static constexpr int START_YEAR = 1682;

	//クリアとなる経過年数 (追加)
	static constexpr int CLEAR_YEAR_LIMIT = 240;

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief 解放
	/// @param  
	void Release(void);

	//所持金を増やす(100000単位)
	void AddMoney(const int _value) { nowMoney_ += _value; }

	//所持金を減らす(100000単位)
	void SubMoney(const int _value) { nowMoney_ -= _value; }

	//所持金の取得(100000単位)
	const int GetNowMoney(void)const { return nowMoney_; }

	//所持金が足りているかの比較(100000単位)
	const bool HasEnoughMoney(int _requestVal)const { return nowMoney_ >= _requestVal; }

	//不満度を増やす
	void AddDissatisfaction(const int _value) { dissatisfaction_ += _value; }

	//不満度を減らす
	void SubDissatisfaction(const int _value) { dissatisfaction_ -= _value; }

	//不満度の取得
	const int GetDissatisfaction(void)const { return dissatisfaction_; }

	// === 年数管理機能 (追加) ===
	//経過年数を取得
	const int GetElapsedYear(void)const { return elapsedYear_; }

	//経過年数を進める（※ゲーム内の適切なタイミングでこの関数を呼んでください）
	void AddYear(const int _year) { elapsedYear_ += _year; }

	//ゲームクリア条件を満たしているか
	const bool IsGameClear(void)const { return elapsedYear_ >= CLEAR_YEAR_LIMIT; }

private:

	//フォント
	std::unique_ptr<FontController>font_;

	int moneyFont_;

	//所持金
	int nowMoney_;

	//不満度
	int dissatisfaction_;

	//経過年数 (追加)
	int elapsedYear_;

	//コンストラクタ
	GameRuleManager(void);

	//デストラクタ
	~GameRuleManager(void)override;

};

