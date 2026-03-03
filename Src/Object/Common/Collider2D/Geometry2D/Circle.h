#pragma once
#include "Geometry2D.h"

class Circle : public Geometry2D
{
public:

	//コンストラクタ
	Circle(const Vector2F _pos, const Vector2F _movedPos, const float _radius);

	//デストラクタ
	~Circle(void)override;

	//描画
	void Draw(unsigned const int _color = 0xffffff)override;

	//当たり判定
	const bool IsHit(Geometry2D& _partnerGeo)override;
	const bool IsHit(Circle& _partnerGeo)override;
	const bool IsHit(BoxGeo& _partnerGeo)override;

	//ヒット後処理
	void HitAfter(void)override;

	//半径の取得
	const float GetRadius(void) { return radius_; }

private:

	//半径
	float radius_;	
};

