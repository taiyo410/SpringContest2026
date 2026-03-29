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
	const bool HasEnoughMoney(int _request)const { return nowMoney_ >= _request; }

	//不満度を高める
	void AddDissatisfaction(const int _value) { dissatisfaction_ += _value; }

	//ゲームオーバー判定
	const bool IsGameOver(void);

private:

	//現在の所持金
	int nowMoney_;

	//総不満度
	int dissatisfaction_;

	//フォント
	std::unique_ptr<FontController>font_;
	int moneyFont_;

	//コンストラクタ
	GameRuleManager(void);

	//デストラクタ
	~GameRuleManager(void)override;

};

