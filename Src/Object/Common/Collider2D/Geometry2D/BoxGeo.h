#pragma once
#include "Geometry2D.h"

class BoxGeo : public Geometry2D
{
public:

	//バウンディングボックス
	struct OBB_2D
	{
		Vector2F vMin;
		Vector2F vMax;
		Vector2F axis[2];
	};

	//コンストラクタ
	BoxGeo(const Vector2F _pos, const Vector2F _movedPos, const Vector2F _min, Vector2F _max);
	BoxGeo(const Vector2F _pos, const Vector2F _movedPos, const Vector2F _halfSize);

	//デストラクタ
	~BoxGeo(void)override;

	//描画
	void Draw(unsigned const int _color = 0xffffff)override;

	//当たり判定
	const bool IsHit(Geometry2D& _partnerGeo)override;
	const bool IsHit(Circle& _partnerGeo)override;
	const bool IsHit(BoxGeo& _partnerGeo)override;

	//ヒット後処理
	void HitAfter(void)override;

	//箱情報取得
	const OBB_2D GetObb(void) { return obb_; }

private:

	//箱情報
	OBB_2D obb_;
};

