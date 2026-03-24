#include "../Pch.h"
#include "../Utility/Utility2D.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "CollisionManager2D.h"

CollisionManager2D* CollisionManager2D::instance_ = nullptr;

void CollisionManager2D::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new CollisionManager2D();
	}
}

CollisionManager2D& CollisionManager2D::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CreateInstance();
	}

	return *instance_;
}

void CollisionManager2D::AddCollider(const std::shared_ptr<Collider2D> _collider)
{
	colliders_.push_back(_collider);
}

void CollisionManager2D::Sweep(void)
{
	//削除された判定を配列から破棄
	std::erase_if(colliders_, [](std::shared_ptr<Collider2D>& _collider) {return _collider->IsDead(); });
}

void CollisionManager2D::Update(void)
{
	//コライダが一つもないなら処理を飛ばす
	if (colliders_.size() < 1)return;

	//各コライダで一回ずつ判定
	for (int i = 0; i < colliders_.size() - 1; i++)
	{
		for (int j = i + 1; j < colliders_.size(); j++)
		{
			//事前当たり判定
			if (!PreCollision(colliders_[i], colliders_[j]))
			{
				continue;
			}

			//当たり判定
			if (IsCollision(colliders_[i],colliders_[j]))
			{
				//それぞれの当たった処理
				colliders_[i]->OnHit(colliders_[j]);
				colliders_[j]->OnHit(colliders_[i]);

				//当たった後の処理
				colliders_[i]->GetGeometry().HitAfter();
				colliders_[j]->GetGeometry().HitAfter();
			}
		}
	}
}

void CollisionManager2D::Destroy(void)
{
	//コライダの全削除
	colliders_.clear();

	//自身のインスタンス削除
	delete instance_;
	instance_ = nullptr;
}

CollisionManager2D::CollisionManager2D(void)
{
}

CollisionManager2D::~CollisionManager2D(void)
{
}

bool CollisionManager2D::PreCollision(const std::weak_ptr<Collider2D> _col1, const std::weak_ptr<Collider2D> _col2)
{
	//コライダ
	const auto& col1 = _col1.lock();
	const auto& col2 = _col2.lock();

	const auto& geo1 = col1->GetGeometry();
	const auto& geo2 = col2->GetGeometry();

	//事前の距離比較
	if (geo1.GetPreHitRadius() + geo2.GetPreHitRadius() < Utility2D::Distance(geo1.GetPos(), geo2.GetPos()))
	{
		return false;
	}

	//タグの比較
	if (col1->GetNoHitTags().contains(col2->GetTag()) || col2->GetNoHitTags().contains(col1->GetTag()))
	{
		return false;
	}

	return true;
}

bool CollisionManager2D::IsCollision(const std::weak_ptr<Collider2D> _col1, const std::weak_ptr<Collider2D> _col2)
{
	//1つ目の当たり判定形状
	auto& geo1 = _col1.lock()->GetGeometry();

	//2つ目の当たり判定形状
	auto& geo2 = _col2.lock()->GetGeometry();

	//当たり判定
	bool ret = geo1.IsHit(geo2);

	//当たっていないなら
	if (!ret)
	{
		//当たっていないことを返す
		_col1.lock()->NotHit();
		_col2.lock()->NotHit();
	}

	//当たり判定
	return ret;
}
