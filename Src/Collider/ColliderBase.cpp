#include "../Pch.h"
#include "ColliderBase.h"

// コンストラクタ
ColliderBase::ColliderBase(SHAPE shape, TAG tag, const Transform* follow)
	: shape_(shape)
	, tag_(tag)
	, follow_(follow)
	, isValid_(true)
{
}

// デストラクタ
ColliderBase::~ColliderBase(void)
{
}

// 描画
void ColliderBase::Draw(void)
{
	int color = isValid_ ? COLOR_VALID : COLOR_INVALID;
	DrawDebug(color);
}

// ★ 2D用回転座標計算の実装
VECTOR ColliderBase::GetRotPos2D(const VECTOR& localOffset) const
{
	if (!follow_) return localOffset;

	// 2D回転（Z軸の回転角）を取得
	// Transformのrot.zがラジアン単位であることを想定
	float angle = follow_->rot.z;

	float sinA = sinf(angle);
	float cosA = cosf(angle);

	// Z軸周りの回転
	VECTOR rotated;
	rotated.x = localOffset.x * cosA - localOffset.y * sinA;
	rotated.y = localOffset.x * sinA + localOffset.y * cosA;
	rotated.z = 0;

	// 親の座標（follow_->pos）を足してワールド座標へ
	return VAdd(follow_->pos, rotated);
}

// 追従先の取得
const Transform* ColliderBase::GetFollow(void) const
{
	return follow_;
}

// 追従先の設定
void ColliderBase::SetFollow(Transform* follow)
{
	follow_ = follow;
}

// 形状の取得
ColliderBase::SHAPE ColliderBase::GetShape(void) const
{
	return shape_;
}

// 衝突種別の取得
ColliderBase::TAG ColliderBase::GetTag(void) const
{
	return tag_;
}

// 有効フラグの取得
bool ColliderBase::IsValid(void) const
{
	return isValid_;
}

// 有効フラグの設定
void ColliderBase::SetValid(bool valid)
{
	isValid_ = valid;
}