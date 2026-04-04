#include "../pch.h"
#include "CursorOnHit.h"

CursorOnHit::CursorOnHit(Cursor& _parent)
	: parent_(_parent)
{
	hit_.emplace(Collider2D::TAG::DAIMYO, [this](const std::weak_ptr<Collider2D> _partner) {HitDaimyo(_partner); });
	hit_.emplace(Collider2D::TAG::TITLE_MENU, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::SLIDER_BUTTON, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::SLIDER_BAR, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::CHOICE_ALTERNATE, [this](const std::weak_ptr<Collider2D> _partner) {HitDaimyoAlternate(_partner); });
	hit_.emplace(Collider2D::TAG::CHOICE_ENHANCEMENT, [this](const std::weak_ptr<Collider2D> _partner) {HitDaimyoEnhancement(_partner); });
	hit_.emplace(Collider2D::TAG::CHOICE_DETAILS, [this](const std::weak_ptr<Collider2D> _partner) {HitDaimyoDetails(_partner); });
	hit_.emplace(Collider2D::TAG::ALTERNATE_SAFETY, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_NORMAL, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ALTERNATE_DENGER, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_TIME, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_PROBABILITY, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::ENHANCEMENT_INCOME, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::DISSATISFACTION_RECOVERY, [this](const std::weak_ptr<Collider2D> _partner) {HitDissatisfactionBar(_partner); });
	hit_.emplace(Collider2D::TAG::DISSATISFACTION_RECOVERY_YES, [this](const std::weak_ptr<Collider2D> _partner) {});
	hit_.emplace(Collider2D::TAG::DISSATISFACTION_RECOVERY_NO, [this](const std::weak_ptr<Collider2D> _partner) {});
}

CursorOnHit::~CursorOnHit(void)
{
}

void CursorOnHit::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	//コライダ
	const auto& col = _partner.lock();

	//タグごとで分ける
	hit_[col->GetTag()](_partner);
}

void CursorOnHit::HitDaimyo(const std::weak_ptr<Collider2D> _partner)
{
	parent_.ChangeExplan(Cursor::EXPLAN::DAIMYO);
}

void CursorOnHit::HitDaimyoAlternate(const std::weak_ptr<Collider2D> _partner)
{
	parent_.ChangeExplan(Cursor::EXPLAN::DAIMYO_ALTERNATE);
}

void CursorOnHit::HitDaimyoEnhancement(const std::weak_ptr<Collider2D> _partner)
{
	parent_.ChangeExplan(Cursor::EXPLAN::ENHANCEMENT);
}

void CursorOnHit::HitDaimyoDetails(const std::weak_ptr<Collider2D> _partner)
{
	parent_.ChangeExplan(Cursor::EXPLAN::DETAILS);
}

void CursorOnHit::HitDissatisfactionBar(const std::weak_ptr<Collider2D> _partner)
{
	parent_.ChangeExplan(Cursor::EXPLAN::DISSATISFACTION);
}
