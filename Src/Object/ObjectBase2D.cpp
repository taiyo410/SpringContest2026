#include "../pch.h"
#include"../Manager/Game/CollisionManager2D.h"
#include"../Object/Common/Collider2D/Collider2D.h"
#include "ObjectBase2D.h"

ObjectBase2D::ObjectBase2D(void)
{
	imageId_ = -1;
	pos_ = {};
}

ObjectBase2D::~ObjectBase2D(void)
{
	colliders_.clear();
}

void ObjectBase2D::OnHit(const std::weak_ptr<Collider2D> _partner)
{
}

void ObjectBase2D::Sweep(void)
{
	//削除された判定を配列から破棄
	std::erase_if(colliders_, [](std::shared_ptr<Collider2D>& _collider) {return _collider->IsDead(); });
}

void ObjectBase2D::MakeCollider(Collider2D::TAG _tag, std::unique_ptr<Geometry2D> _geo, std::set<Collider2D::TAG> _noHitTags)
{
	std::shared_ptr<Collider2D> col = std::make_shared<Collider2D>(*this, _tag, std::move(_geo), _noHitTags);
	colliders_.push_back(col);
	CollisionManager2D::GetInstance().AddCollider(col);
}

void ObjectBase2D::DeleteColliderAtTag(Collider2D::TAG _tag)
{
	for (auto& collider : colliders_)
	{
		//そのコライダが存在しない　又は　指定タグと違う
		if (collider == nullptr || collider->GetTag() != _tag)continue;
		collider->Kill();
	}

	//削除
	std::erase_if(colliders_, [](std::shared_ptr<Collider2D>& _collider) {return _collider->IsDead(); });
}

void ObjectBase2D::DeleteAllColliders(void)
{
	for (auto& col : colliders_)
	{
		//当たり判定が存在しないならスキップ
		if (col == nullptr)continue;

		//コライダの削除
		col->Kill();
	}

	//当たり判定情報の削除
	colliders_.clear();
}
