#include "../Pch.h"
#include "ObjectBase.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Collider/ColliderBase.h"

// コンストラクタ
ObjectBase::ObjectBase(void)
{
}

// デストラクタ
ObjectBase::~ObjectBase(void)
{
}

void ObjectBase::InitLoad(void)
{
}

// 初期化処理
void ObjectBase::Init(void)
{
	InitTransform();
	InitCollider();
}

// 描画処理
void ObjectBase::Draw(void) const
{
#ifdef _DEBUG

	// デバック用全コライダの描画
	for (const auto& own : ownColliders_)
	{
		if (own.second)
		{
			own.second->Draw();
		}
	}
#endif 

}

// 解放処理
void ObjectBase::Release(void)
{
	// 所有コライダの自動解放
	for (auto& own : ownColliders_)
	{
		delete own.second;
	}

	ownColliders_.clear();
	hitColliders_.clear();
}

const ColliderBase* ObjectBase::GetOwnCollider(int key) const
{
	if (ownColliders_.count(key) == 0) return nullptr;
	return ownColliders_.at(key);
}

void ObjectBase::AddHitCollider(const ColliderBase* hitCollider)
{
	if (!hitCollider) return;

	// 重複チェック
	for (const auto& c : hitColliders_)
	{
		if (c == hitCollider) return;
	}
	hitColliders_.emplace_back(hitCollider);
}

void ObjectBase::ClearHitCollider(void)
{
	hitColliders_.clear();
}
