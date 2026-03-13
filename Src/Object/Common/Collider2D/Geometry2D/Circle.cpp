#include "../Pch.h"
#include "BoxGeo.h"
#include "Circle.h"

Circle::Circle(const Vector2F& _pos, const Vector2F& _movedPos, const float _radius)
	: Geometry2D(_pos,_movedPos),
	radius_(_radius)
{
}

Circle::~Circle(void)
{
}

void Circle::Draw(unsigned const int _color)
{
	DrawCircle(pos_.x, pos_.y, _color, true);
}

const bool Circle::IsHit(Geometry2D& _partnerGeo)
{
	return _partnerGeo.IsHit(*this);
}

const bool Circle::IsHit(Circle& _partnerGeo)
{
	//相手情報
	Vector2F pPos = _partnerGeo.GetPos();
	float pRadius = _partnerGeo.GetRadius();
	
	//距離
	Vector2F diff = pPos - pos_;

	//距離の二乗
	float distSq = diff.x * diff.x + diff.y * diff.y;

	//半径の合計
	float totalRadius = radius_ + pRadius;

	//距離と半径の比較
	if (distSq < totalRadius * totalRadius)
	{
		//距離
		float dist = std::sqrtf(distSq);

		//めり込み量
		float depth = totalRadius - dist;
		hitResult_.depth = depth;

		//当たった方向
		Vector2F normal = diff / dist;
		hitResult_.normal = normal;

		//衝突点
		hitResult_.point = pos_ + normal * radius_;

		//相手側も
		HitResult partnerResult;
		partnerResult.depth = depth;
		partnerResult.normal = normal * -1.0f;
		partnerResult.point = pPos - normal * pRadius;
		_partnerGeo.SetHitResult(partnerResult);

		return true;
	}

	return false;
}

const bool Circle::IsHit(BoxGeo& _partnerGeo)
{
	//相手情報
	Vector2F pPos = _partnerGeo.GetPos();
	BoxGeo::OBB_2D pObb = _partnerGeo.GetObb();

	//Boxの範囲
	float minX = pPos.x + pObb.vMin.x;
	float minY = pPos.y + pObb.vMin.y;
	float maxX = pPos.x + pObb.vMax.x;
	float maxY = pPos.y + pObb.vMax.y;

	//円の中心から一番近い点を見る
	float closestX = std::clamp(pos_.x, minX, maxX);
	float closestY = std::clamp(pos_.y, minY, maxY);

	//距離の二乗
	float diffX = pos_.x - closestX;
	float diffY = pos_.y - closestY;
	float distSq = diffX * diffX + diffY * diffY;

	//比較
	if (distSq < radius_ * radius_)
	{
		//距離
		float dist = std::sqrtf(distSq);

		//円の中心が矩形内にいる場合
		if (dist < 0.0001f)
		{
			//各距離を見る
			float left = pos_.x - minX;
			float right = maxX - pos_.x;
			float top = pos_.y - minY;
			float bottom = maxY - pos_.y;

			//どれに近いかを見る
			float minPen = std::min({ left,right,top,bottom });

			//一番近かった方向が法線
			Vector2F normal;
			if (minPen == left)normal = { 1.0,0.0 };
			else if (minPen == right)normal = { -1.0,0.0 };
			else if (minPen == top)normal = { 0.0,1.0 };
			else normal = { 0.0,-1.0 };
			hitResult_.normal = normal;

			//めり込み量
			float depth = radius_ + minPen;
			hitResult_.depth = depth;

			//衝突点
			hitResult_.point = pos_ - normal * radius_;

			//相手側にも
			HitResult partnerResult;
			partnerResult.normal = normal * -1.0f;
			partnerResult.depth = depth;
			partnerResult.point = hitResult_.point;
			_partnerGeo.SetHitResult(partnerResult);

			return true;
		}

		//矩形外の時
		Vector2F normal = { diffX / dist,diffY / dist };
		hitResult_.normal = normal;
		hitResult_.depth = radius_ - dist;
		hitResult_.point = { closestX,closestY };

		//相手側
		HitResult partnerResult;
		partnerResult.normal = normal * -1.0f;
		partnerResult.depth = hitResult_.depth;
		partnerResult.point = hitResult_.point;

		return true;
	}

	return false;
}

void Circle::HitAfter(void)
{
}
