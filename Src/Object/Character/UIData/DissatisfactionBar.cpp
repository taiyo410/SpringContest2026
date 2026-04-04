#include "../pch.h"
#include "../Application.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/BoxGeo.h"
#include "DissatisfactionBar.h"

DissatisfactionBar::DissatisfactionBar(void)
{
	dissatisfaction_ = 0;
	preState_ = STATE::STANDBY;
	state_ = STATE::STANDBY;

	update_.emplace(STATE::STANDBY, [this](void) {UpdateStandBy(); });
	update_.emplace(STATE::NORMAL, [this](void) {UpdateNormal(); });
	update_.emplace(STATE::SELECT, [this](void) {UpdateSelect(); });

	draw_.emplace(STATE::STANDBY, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::NORMAL, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::SELECT, [this](void) {DrawSelect(); });

	preChange_.emplace(STATE::STANDBY, [this](void) {});
	preChange_.emplace(STATE::NORMAL, [this](void) {PreChangeStateNormal(); });
	preChange_.emplace(STATE::SELECT, [this](void) {PreChangeStateSelect(); });
}

DissatisfactionBar::~DissatisfactionBar(void)
{
}

void DissatisfactionBar::Load(void)
{
}

void DissatisfactionBar::Init(void)
{
	dissatisfaction_ = 0;
	preState_ = STATE::NORMAL;
	state_ = STATE::STANDBY;

	ChangeState(STATE::NORMAL);
}

void DissatisfactionBar::Update(void)
{
	update_[state_]();
}

void DissatisfactionBar::Draw(void)
{
	draw_[state_]();
}

void DissatisfactionBar::Release(void)
{
}

void DissatisfactionBar::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	//入力
	const auto& input = InputManager::GetInstance();
	const auto& partner = _partner.lock();
	auto& gameMng = GameRuleManager::GetInstance();

	//お金があるなら
	if (input.IsTrgMouseLeft() && partner->GetTag() == Collider2D::TAG::CURSOR)
	{
		for (const auto& myCol : colliders_)
		{
			if (myCol->GetTag() == Collider2D::TAG::DISSATISFACTION_RECOVERY)
			{
				//状態遷移
				ChangeState(STATE::SELECT);
			}
			else if (myCol->GetTag() == Collider2D::TAG::DISSATISFACTION_RECOVERY_YES && gameMng.HasEnoughMoney(NECESSARY_MONEY))
			{
				//下げる
				dissatisfaction_ -= SUB_VALUE;

				//お金の消費
				gameMng.SubMoney(NECESSARY_MONEY);

				//状態遷移
				ChangeState(STATE::NORMAL);
			}
			else if (myCol->GetTag() == Collider2D::TAG::DISSATISFACTION_RECOVERY_NO)
			{
				//状態遷移
				ChangeState(STATE::NORMAL);
			}
		}
	}
}

void DissatisfactionBar::ChangeState(const STATE _state)
{
	preState_ = _state;
	state_ = STATE::STANDBY;
}

void DissatisfactionBar::PreChangeStateNormal(void)
{
	//当たり判定消去
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(DIS_POS, DIS_POS, DIS_RADIUS, DIS_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY, std::move(geo), {});
}

void DissatisfactionBar::PreChangeStateSelect(void)
{
	//当たり判定消去
	DeleteAllColliders();

	//選択肢位置
	static const Vector2F yesPos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - SELECT_LOCAL_POS_Y};
	static const Vector2F noPos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + SELECT_LOCAL_POS_Y};

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(yesPos, yesPos, DIS_RADIUS, SELECT_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY_YES, std::move(geo), {});
	
	geo = std::make_unique<BoxGeo>(noPos, noPos, DIS_RADIUS, SELECT_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY_NO, std::move(geo), {});
}

void DissatisfactionBar::UpdateStandBy(void)
{
	state_ = preState_;
	preChange_[state_]();
}

void DissatisfactionBar::UpdateNormal(void)
{
}

void DissatisfactionBar::UpdateSelect(void)
{
}

void DissatisfactionBar::DrawNormal(void)
{
	//不満度
	DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y - DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, 0x666666, true);
	if (dissatisfaction_ > 0)
		DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y - (DIS_BOX_SIZE.y * 2) * (static_cast<float>(dissatisfaction_) / static_cast<float>(DISSATISFACTION_MAX)), 0xff00ff, true);
	DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y - DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, 0x0, false);
}

void DissatisfactionBar::DrawSelect(void)
{
	DrawNormal();

	for (const auto& col : colliders_)
	{
		col->GetGeometry().Draw();
	}
}
