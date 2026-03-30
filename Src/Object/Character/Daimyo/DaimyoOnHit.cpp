#include "../pch.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "DaimyoOnHit.h"

DaimyoOnHit::DaimyoOnHit(Daimyo& _parent)
	: parent_(_parent)
{
	//タグごとの当たり判定
	hit_.emplace(Collider2D::TAG::CURSOR, [this](const std::weak_ptr<Collider2D> _partner) {HitCursor(_partner); });
	hit_.emplace(Collider2D::TAG::CHOICE_ALTERNATE, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::CHOICE_ENHANCEMENT, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::CHOICE_DETAILS, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_SAFETY, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_NORMAL, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_DENGER, [](const std::weak_ptr<Collider2D> _partner) {});
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
	if (input.IsTrgMouseLeft())
	{
		for (const auto& myCol : parent_.GetColliders())
		{
			if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::DAIMYO)
			{
				//状態遷移
				parent_.ChangeState(Daimyo::STATE::SELECT_DIRECTION);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_ALTERNATE)
			{
				//お金が足りているか
				if (parent_.GetMoney() >= Daimyo::FUNDS_MIN)
				{
					//状態遷移
					parent_.ChangeState(Daimyo::STATE::SELECT_ALTERNATE);
				}
				else
				{
					//状態遷移
					parent_.ChangeState(Daimyo::STATE::NO_MONEY);
				}
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_ENHANCEMENT)
			{
				//状態遷移
				parent_.ChangeState(Daimyo::STATE::SELECT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_DETAILS)
			{
				//状態遷移
				parent_.ChangeState(Daimyo::STATE::SELECT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_SAFETY)
			{
				//難易度設定
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::SAFETY);

				//状態遷移
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_NORMAL)
			{
				//難易度設定
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::NORMAL);

				//状態遷移
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_DENGER)
			{
				//難易度設定
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::DENGER);

				//状態遷移
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);
			}
		}

		//クリックで戻さない
		parent_.ProhibitedBack();
	}
}