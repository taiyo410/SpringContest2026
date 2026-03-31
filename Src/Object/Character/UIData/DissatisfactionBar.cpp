#include "../pch.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/BoxGeo.h"
#include "DissatisfactionBar.h"

DissatisfactionBar::DissatisfactionBar(void)
{
}

DissatisfactionBar::~DissatisfactionBar(void)
{
}

void DissatisfactionBar::Load(void)
{
	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(DIS_POS, DIS_POS, DIS_RADIUS, DIS_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY, std::move(geo), {});
}

void DissatisfactionBar::Init(void)
{
}

void DissatisfactionBar::Update(void)
{
}

void DissatisfactionBar::Draw(void)
{
	//不満度
	DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y - DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, 0x666666, true);
	if (dissatisfaction_ > 0)
		DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y - (DIS_BOX_SIZE.y * 2) * (static_cast<float>(dissatisfaction_) / static_cast<float>(DISSATISFACTION_MAX)), 0xff00ff, true);
	DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y - DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, 0x0, false);
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
	if (input.IsTrgMouseLeft() && partner->GetTag() == Collider2D::TAG::CURSOR && gameMng.HasEnoughMoney(NECESSARY_MONEY))
	{
		//下げる
		dissatisfaction_ -= SUB_VALUE;

		//お金の消費
		gameMng.SubMoney(NECESSARY_MONEY);
	}
}
