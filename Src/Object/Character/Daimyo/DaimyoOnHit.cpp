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

	// 【修正】参勤交代を実行した際に、1年経過させる処理をラムダ式内に追加
	hit_.emplace(Collider2D::TAG::ALTERNATE_SAFETY, [this](const std::weak_ptr<Collider2D> _partner) {
		// ※ラムダ式の中身は OnHit() の内部処理に依存するため、ここでは空のままにして下部の OnHit で処理します
		});
	hit_.emplace(Collider2D::TAG::ALTERNATE_NORMAL, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_DENGER, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_TIME, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_PROBABILITY, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_INCOME, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::DISSATISFACTION_RECOVERY, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::DISSATISFACTION_RECOVERY_YES, [](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::DISSATISFACTION_RECOVERY_NO, [](const std::weak_ptr<Collider2D> _partner) {});
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
				parent_.ChangeState(Daimyo::STATE::ENHANCEMENT);
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
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);

				// 参勤交代を実行する度に1年経過
				GameRuleManager::GetInstance().AddYear(1);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_NORMAL)
			{
				//難易度設定
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::NORMAL);
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);

				GameRuleManager::GetInstance().AddYear(1);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_DENGER)
			{
				//難易度設定
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::DENGER);
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);

				//状態遷移
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ENHANCEMENT_TIME)
			{
				//状態遷移
				parent_.ChangeState(Daimyo::STATE::NORMAL);

				//強化
				parent_.Enhancement(Daimyo::ENHANCEMENT_TYPE::TIME);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ENHANCEMENT_PROBABILITY)
			{
				//状態遷移
				parent_.ChangeState(Daimyo::STATE::NORMAL);

				//強化
				parent_.Enhancement(Daimyo::ENHANCEMENT_TYPE::PROBABILITY);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ENHANCEMENT_INCOME)
			{
				//状態遷移
				parent_.ChangeState(Daimyo::STATE::NORMAL);

				//強化
				parent_.Enhancement(Daimyo::ENHANCEMENT_TYPE::INCOME);
			}
		}

		//一年経過
		GameRuleManager::GetInstance().AddYear(1);
	
		//クリックで戻さない
		parent_.ProhibitedBack();

	}

}

