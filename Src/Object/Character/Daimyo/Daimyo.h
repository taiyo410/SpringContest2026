#pragma once
#include<unordered_map>
#include<string>
#include "../Base/CharacterBase2D.h"
#include "DaimyoImport.h"

class ArrowController;
class GaugeController;
class DaimyoOnHit;
class Easing;
class FontController;

class Daimyo : public CharacterBase2D
{
public:
	
	//状態
	enum class STATE
	{
		STANDBY,			//遷移待機
		NORMAL,				//通常
		SELECT_DIRECTION,	//演出
		DELETE_SELECT_DIRECTION,	//演出
		SELECT,				//選択
		SELECT_ALTERNATE,	//参勤(選択)
		NO_MONEY,			//お金が足りない
		ACTION_ALTERNATE,	//参勤(実行中)
		RESULT_ALTERNATE,	//参勤(結果)
		ENHANCEMENT,		//強化
		DETAILS,			//詳細
	};

	//選択肢
	enum class SELECT
	{
		SELECT_ALTERNATE,	//参勤(選択)
		ENHANCEMENT,		//強化
		DETAILS,			//詳細
	};

	//参勤の選択肢
	enum class ALTERNATE_DIFF
	{
		SAFETY,		//安全
		NORMAL,		//普通
		DENGER,		//危険
	};

	//強化の種類
	enum class ENHANCEMENT_TYPE
	{
		TIME,			//時間
		PROBABILITY,	//成功率
		INCOME			//収入
	};

	//参勤の情報
	struct AlternateInfo
	{
		float probability = 0.0f;	//成功確率
		float income = 0.0f;		//収益
		float confiscation = 0.0f;	//没収率
		float requiredTime = 0.0f;	//所要時間
	};

	//事前当たり判定用
	static constexpr float SELECT_PRE_RADIUS = 100.0f;
	static constexpr Vector2F SELECT_MIN = {-50.0f,-20.0f};
	static constexpr Vector2F SELECT_MAX = {50.0f,20.0f};
	static constexpr float ALTERNATE_PRE_RADIUS = 200.0f;
	static constexpr Vector2F ALTERNATE_MENU_MIN = {-200.0f,-75.0f};
	static constexpr Vector2F ALTERNATE_MENU_MAX = {200.0f,75.0f};

	//選択肢との相対座標
	static constexpr Vector2F ALTERNATE_LOCAL_POS = { 50.0f, -80.0f };
	static constexpr Vector2F ENHANCEMENT_LOCAL_POS = { 120.0f, -20.0f };
	static constexpr Vector2F DETAILS_LOCAL_POS = { 120.0f, 50.0f };
	static constexpr float ALTERNATE_MENU_LOCAL_POS = 220.0f;

	//難易度ごとの所要時間
	static constexpr float REQUIRED_TIME_SAFETY = 15.0f;
	static constexpr float REQUIRED_TIME_NORMAL = 10.0f;
	static constexpr float REQUIRED_TIME_DENGER = 5.0f;

	//難易度ごとの成功確率(%)
	static constexpr int SUCCESS_SAFETY = 100;
	static constexpr int SUCCESS_NORMAL = 70;
	static constexpr int SUCCESS_DENGER = 50;

	//難易度ごとの収入(100000単位)
	static constexpr float INCOME_SAFETY = 3.0f;
	static constexpr float INCOME_NORMAL = 5.0f;
	static constexpr float INCOME_DENGER = 8.0f;

	//難易度ごとの失敗の没収割合
	static constexpr float CONFISCATION_SAFETY = 0.5f;
	static constexpr float CONFISCATION_NORMAL = 0.5f;
	static constexpr float CONFISCATION_DENGER = 0.0f;

	//各項目の強化倍率(%)
	static constexpr float ENHANCE_PER = 10.0f;

	//参勤交代に行ける最低資金
	static constexpr int FUNDS_MIN = 10;

	//所持できる資金の上限
	static constexpr int FUNDS_MAX = 50;

	//不満度上昇値
	static constexpr int SUCCESS_DISSATISFACTION = 2;
	static constexpr int FAILED_DISSATISFACTION = 6;

	//コンストラクタ
	Daimyo(const DaimyoImport _import);

	//デストラクタ
	~Daimyo(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//ヒット処理
	void OnHit(const std::weak_ptr<Collider2D> _partner)override;

	//項目を戻ることを禁ずる
	void ProhibitedBack(void) { isBackMenu_ = false; }

	//お金が上限に達したか
	const bool IsMoneyMax(void)const { return money_ >= FUNDS_MAX; }

	// 【修正3】DaimyoOnHit.cppから呼び出される関数の宣言を追加
	const STATE GetState(void) const { return state_; }
	
	//状態遷移
	void ChangeState(const STATE _nextState);
	
	//所持金
	const float GetMoney(void) const { return money_; }
	
	//難易度設定
	void SetAlternateDiff(ALTERNATE_DIFF _diff);

	//強化
	void Enhancement(ENHANCEMENT_TYPE _type);

private:

	static constexpr float EASEING_TIME = 0.2f;

	static constexpr Vector2F EDO_POS = { 784.0f,359.0f };
	//江戸の色
	static constexpr FLOAT4 EDO_COL = { 1.0f, 0.647f, 0.0f, 1.0f };

	static constexpr float ARROW_THICK = 100.0f;

	static constexpr int SELECT_FONT_SIZE = 16;
	static constexpr int ALTERNATE_FONT_SIZE = 30;
	static constexpr int ALTERNATE_EXPLAN_FONT_SIZE = 24;
	static constexpr int FONT_TICKNESS = 24;

	static constexpr float FONT_ALTERNATE_LOCAL_POS_Y = -52.0f;
	static constexpr float FONT_ALTERNATE_EXPLAN_LOCAL_POS_Y = 16.0f;

	//選択肢画像
	int selectMenuImg_;

	//フォント
	std::unique_ptr<FontController> font_;
	int selectFontHandle_;
	std::unordered_map<SELECT, std::wstring> selectStr_;
	int alternateFontHandle_;
	int alternateExplanFontHandle_;
	std::unordered_map<ALTERNATE_DIFF, std::wstring> alternateStr_;

	//状態
	STATE state_;
	STATE nextState_;

	//当たり判定
	std::unique_ptr<DaimyoOnHit> onHit_;

	//インポート情報
	DaimyoImport import_;

	Vector2F edoPos_;

	//所持金
	float money_;

	//不満度
	int dissatisfaction_;

	//参勤の成功失敗
	bool isSuccess_;

	//参勤の情報
	AlternateInfo alternateInfo_;

	//強化回数
	std::unordered_map<ENHANCEMENT_TYPE, int> enhancementCnt_;

	//参勤交代の時間
	float cnt_;
	float alternatePer_;
	FLOAT4 alternateColor_;
	//イージング
	std::unique_ptr<Easing> easing_;

	////矢印ゲージ
	std::unique_ptr<ArrowController>arrow_;
	//イージングカウント
	float easingCnt_;
	//アルファ値
	int blendAlpha_;
	int startAlpha_;
	int goalAlpha_;

	//お金ゲージ
	std::unique_ptr<GaugeController>moneyGauge_;
	//不満度ゲージ
	std::unique_ptr<GaugeController>dissatisfactionGauge_;
	Vector2F moneyGaugePos_;
	Vector2F moneyGaugeSize_;
	float moneyPer_;
	FLOAT4 moneyGaugeCol_;
	float moneyGaugeColCnt_;

	//不満度割合
	float dissatisfactionPer_;
	FLOAT4 dissatisfactionGaugeCol_;
	Vector2F dissatisfactionGaugePos_;
	//選択肢座標
	std::unordered_map<SELECT,Vector2F> selectPos_;
	std::unordered_map<SELECT,Vector2F> selectGoalPos_;
	std::unordered_map<SELECT,Vector2F> selectStartPos_;

	//参勤難易度
	std::unordered_map<ALTERNATE_DIFF,Vector2F> alternateMenuPos_;

	//状態ごとの更新
	std::unordered_map<STATE,std::function<void(void)>>update_;

	//状態ごとの描画
	std::unordered_map<STATE, std::function<void(void)>>draw_;

	//状態ごとの設定
	std::unordered_map<STATE, std::function<void(void)>>changeSetting_;

	//難易度ごとの設定
	std::unordered_map<ALTERNATE_DIFF, std::function<void(void)>>settingDiff_;

	//戻るか
	bool isBackMenu_;

	//更新
	void UpdateStandby(void);
	void UpdateNormal(void);
	void UpdateSelectDirection(void);
	void UpdateDeleteSelectDirection(void);
	void UpdateSelect(void);
	void UpdateSelectAlternate(void);
	void UpdateNoMoney(void);
	void UpdateActionAlternate(void);
	void UpdateResultAlternate(void);
	void UpdateEnhancement(void);
	void UpdateDetails(void);

	//描画
	void DrawStandby(void);
	void DrawNormal(void);
	void DrawSelect(void);
	void DrawSelectDirection(void);
	void DrawSelectAlternate(void);
	void DrawNoMoney(void);
	void DrawActionAlternate(void);
	void DrawResultAlternate(void);
	void DrawEnhancement(void);
	void DrawDetails(void);

	//城コライダの生成
	void CreateCastleCol(void);

	//選択肢の出てくる演出の初期化
	void InitSelectDirection(void);

	//選択肢を消す時の演出の初期化
	void DeleteSelectDirection(void);

	//選択肢についてのイージング
	void EasingSelectDirection(void);

	//項目コライダの生成
	void CreateSelectCol(void);

	//参勤項目コライダの生成
	void CreateAlternateCol(void);

	//強化項目コライダの生成
	void CreateEnhancementCol(void);

	//難易度ごとの設定
	void SettingSafety(void);
	void SettingNormal(void);
	void SettingDenger(void);

	//参勤交代の結果
	void ResultAlternate(void);
};

