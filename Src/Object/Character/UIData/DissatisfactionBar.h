#pragma once
#include<unordered_map>
#include<functional>
#include<string>
#include"../Object/ObjectBase2D.h"

class SoundManager;
class FontController;

class DissatisfactionBar : public ObjectBase2D
{
public:

	//状態
	enum class STATE
	{
		STANDBY,
		NORMAL,
		DIERCTION_IN,
		DIERCTION_OUT,
		SELECT,
	};

	//最大不満度
	static constexpr int DISSATISFACTION_MAX = 100;

	//下げるときの要求金
	static constexpr int NECESSARY_MONEY = 100;

	//不満度を下げる量
	static constexpr int SUB_VALUE = 20;

	//コンストラクタ
	DissatisfactionBar(void);

	//デストラクタ
	~DissatisfactionBar(void)override;

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

	//不満度を高める
	void AddDissatisfaction(const int _value) { dissatisfaction_ += _value; }

	//不満度の取得
	const int GetDissatisfaction(void)const { return dissatisfaction_ ; }

	//不満度が上限に達したか
	const bool IsDissatisfactionMax(void)const { return dissatisfaction_ >= DISSATISFACTION_MAX; }

private:

	//不満度の座標
	static constexpr Vector2F DIS_POS = { 60.0f, 300.0f };
	static constexpr Vector2F DIS_BOX_SIZE = { 40.0f, 200.0f };
	static constexpr float DIS_RADIUS = 300.0f;
	static constexpr float SELECT_LOCAL_POS_Y = 150.0f;
	static constexpr Vector2F SELECT_BOX_SIZE = { 200.0f,50.0f };

	static constexpr float DRAW_TIME = 2.0f;

	//サウンド
	SoundManager& soundMng_;

	//総不満度
	int dissatisfaction_;

	//状態
	STATE preState_;
	STATE state_;

	//選択肢の座標
	Vector2F yesPos_;
	Vector2F noPos_;

	//不満度ゲージを消費する際の情報
	float easeCnt_;
	int alpha_;
	float scl_;
	float drawCnt_;
	int mituguImg_;

	//画像
	int selectMenuImg_;

	//フォント
	std::unique_ptr<FontController> font_;
	int fontHandle_;

	std::unique_ptr<Easing>easing_;

	//更新
	std::unordered_map<STATE, std::function<void(void)>> update_;

	//描画
	std::unordered_map<STATE, std::function<void(void)>> draw_;

	//状態遷移前
	std::unordered_map<STATE, std::function<void(void)>> preChange_;

	//状態遷移
	void ChangeState(const STATE _state);

	//状態遷移の前処理
	void PreChangeStateNormal(void);
	void PreChangeStateSelect(void);
	void PreChangeStateDirectionIn(void);
	void PreChangeStateDirectionOut(void);
	//更新
	void UpdateStandBy(void);
	void UpdateNormal(void);
	void UpdateSelect(void);
	void UpdateDirectionIn(void);
	void UpdateDirectionOut(void);

	//描画
	void DrawNormal(void);
	void DrawSelect(void);
	void DrawDirectrion(void);
};