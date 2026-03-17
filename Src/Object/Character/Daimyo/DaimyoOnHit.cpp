#include "../pch.h"
#include "../Manager/Generic/InputManager.h"
#include "DaimyoOnHit.h"

DaimyoOnHit::DaimyoOnHit(Daimyo& _parent)
	: parent_(_parent)
{
	//タグごとの当たり判定
	hit_.emplace(Collider2D::TAG::CURSOR, [this](const std::weak_ptr<Collider2D> _partner) {HitCursor(_partner); });
}

DaimyoOnHit::~DaimyoOnHit(void)
{
}

void DaimyoOnHit::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	//コライダ
	const auto& col = _partner.lock();

	//タグごとで分ける
	hit_[col->GetTag()](_partner);
}

void DaimyoOnHit::HitCursor(const std::weak_ptr<Collider2D> _partner)
{
	//入力
	const auto& input = InputManager::GetInstance();

	//左クリック
	if (input.IsClickMouseLeft())
	{
		int a = 0;
	}
}