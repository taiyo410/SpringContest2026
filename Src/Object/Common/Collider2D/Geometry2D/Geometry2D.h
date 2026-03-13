#pragma once
#include"../Common/Vector2F.h"

class Circle;
class BoxGeo;

class Geometry2D
{
public:

	//ヒット情報
	struct HitResult
	{
		Vector2F point = { 0.0f,0.0f };		//衝突点
		Vector2F normal = { 0.0f,0.0f };	//衝突方向の法線ベクトル
		float depth = 0.0f;					//深度(めり込み量)
	};

	//デストラクタ
	virtual ~Geometry2D(void);

	//描画
	virtual void Draw(unsigned const int _color = 0xffffff) = 0;

	//当たり判定
	virtual const bool IsHit(Geometry2D& _partnerGeo) = 0;
	virtual const bool IsHit(Circle& _partnerGeo) = 0;
	virtual const bool IsHit(BoxGeo& _partnerGeo) = 0;

	//ヒット後処理
	virtual void HitAfter(void);

	//座標の取得
	inline const Vector2F GetPos(void)const { return pos_; }

	//移動後座標の取得
	inline const Vector2F GetMovedPos(void)const { return movedPos_; }

	//ヒット情報の取得
	inline const HitResult GetHitResult(void)const { return hitResult_; }

	//ヒット情報の設定
	inline void SetHitResult (const HitResult _hitResult) { hitResult_ = _hitResult; }

protected:

	//親情報
	const Vector2F& pos_;		//座標
	const Vector2F& movedPos_;	//移動後座標

	//ヒット情報
	HitResult hitResult_;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_movedPos">移動後座標</param>
	Geometry2D(const Vector2F& _pos, const Vector2F _movedPos);
};

