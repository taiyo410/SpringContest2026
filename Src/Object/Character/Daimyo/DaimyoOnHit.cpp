#include "../pch.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "DaimyoOnHit.h"

DaimyoOnHit::DaimyoOnHit(Daimyo& _parent)
	: parent_(_parent)
{
	//É^ÉOÇ≤Ç∆ÇÃìñÇΩÇËîªíË
	hit_.emplace(Collider2D::TAG::CURSOR, [this](const std::weak_ptr<Collider2D> _partner) {HitCursor(_partner); });
	hit_.emplace(Collider2D::TAG::CHOICE_ALTERNATE, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::CHOICE_ENHANCEMENT, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::CHOICE_DETAILS, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_SAFETY, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_NORMAL, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_DENGER, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_TIME, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_PROBABILITY, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_INCOME, [](const std::weak_ptr<Collider2D> _partner) {});
}

DaimyoOnHit::~DaimyoOnHit(void)
{
}

void DaimyoOnHit::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	//ÉRÉâÉCÉ_
	const auto& col = _partner.lock();

	//É^ÉOÇ≤Ç∆Ç≈ï™ÇØÇÈ
	hit_[col->GetTag()](_partner);
}

void DaimyoOnHit::HitCursor(const std::weak_ptr<Collider2D> _partner)
{
	//ì¸óÕ
	const auto& input = InputManager::GetInstance();

	//ç∂ÉNÉäÉbÉN
	if (input.IsTrgMouseLeft())
	{
		for (const auto& myCol : parent_.GetColliders())
		{
			if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::DAIMYO)
			{
				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::SELECT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_ALTERNATE)
			{
				//Ç®ã‡Ç™ë´ÇËÇƒÇ¢ÇÈÇ©
				if (parent_.GetMoney() >= Daimyo::FUNDS_MIN)
				{
					//èÛë‘ëJà⁄
					parent_.ChangeState(Daimyo::STATE::SELECT_ALTERNATE);
				}
				else
				{
					//èÛë‘ëJà⁄
					parent_.ChangeState(Daimyo::STATE::NO_MONEY);
				}
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_ENHANCEMENT)
			{
				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::ENHANCEMENT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_DETAILS)
			{
				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::SELECT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_SAFETY)
			{
				//ìÔà’ìxê›íË
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::SAFETY);

				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_NORMAL)
			{
				//ìÔà’ìxê›íË
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::NORMAL);

				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_DENGER)
			{
				//ìÔà’ìxê›íË
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::DENGER);

				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ENHANCEMENT_TIME)
			{
				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::NORMAL);

				//ã≠âª
				parent_.Enhancement(Daimyo::ENHANCEMENT_TYPE::TIME);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ENHANCEMENT_PROBABILITY)
			{
				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::NORMAL);

				//ã≠âª
				parent_.Enhancement(Daimyo::ENHANCEMENT_TYPE::PROBABILITY);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ENHANCEMENT_INCOME)
			{
				//èÛë‘ëJà⁄
				parent_.ChangeState(Daimyo::STATE::NORMAL);

				//ã≠âª
				parent_.Enhancement(Daimyo::ENHANCEMENT_TYPE::INCOME);
			}
		}

		//ÉNÉäÉbÉNÇ≈ñﬂÇ≥Ç»Ç¢
		parent_.ProhibitedBack();
	}
}