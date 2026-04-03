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
}

DaimyoOnHit::~DaimyoOnHit(void)
{
}

void DaimyoOnHit::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	auto myCol = _partner.lock();

	// 【修正4】マウスの「左クリックした瞬間」を安全に取得
	static bool oldMouseClick = false;
	bool currentMouseClick = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool mouseClickTrg = currentMouseClick && !oldMouseClick;
	oldMouseClick = currentMouseClick;

	if (myCol)
	{
		hit_[myCol->GetTag()](_partner);

		if (parent_.GetState() == Daimyo::STATE::SELECT)
		{
			// 右クリックで戻る処理は削除
		}

		if (mouseClickTrg)
		{
			if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::DAIMYO)
			{
				parent_.ChangeState(Daimyo::STATE::SELECT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_ALTERNATE)
			{
				if (parent_.GetMoney() >= 50.0f) // 仮で定数
				{
					parent_.ChangeState(Daimyo::STATE::SELECT_ALTERNATE);
				}
				else
				{
					parent_.ChangeState(Daimyo::STATE::NO_MONEY);
				}
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_ENHANCEMENT)
			{
				parent_.ChangeState(Daimyo::STATE::SELECT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::CHOICE_DETAILS)
			{
				parent_.ChangeState(Daimyo::STATE::SELECT);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_SAFETY)
			{
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::SAFETY);
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);

				// 参勤交代を実行する度に1年経過
				GameRuleManager::GetInstance().AddYear(1);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_NORMAL)
			{
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::NORMAL);
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);

				GameRuleManager::GetInstance().AddYear(1);
			}
			else if (myCol->IsHit() && myCol->GetTag() == Collider2D::TAG::ALTERNATE_DENGER)
			{
				parent_.SetAlternateDiff(Daimyo::ALTERNATE_DIFF::DENGER);
				parent_.ChangeState(Daimyo::STATE::ACTION_ALTERNATE);

				GameRuleManager::GetInstance().AddYear(1);
			}
		}
	}
}

void DaimyoOnHit::HitCursor(const std::weak_ptr<Collider2D> _partner)
{
}