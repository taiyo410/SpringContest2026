#pragma once
#include"../Object/ObjectBase2D.h"

class DissatisfactionBar : public ObjectBase2D
{
public:

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

	//総不満度
	int dissatisfaction_;
};

