#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Character/Base/CharacterOnHitBase.h"
#include "../Object/Common/EffectController.h"
#include "../Object/Stage.h"
#include "../../../Utility/Utility3D.h"

#include "../Object/ObjectBase.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void) :
	soundMng_(SoundManager::GetInstance()),
	//hpPer_(1.0f),
	//preHpPer_(hpPer_),
	updatePhase_(UPDATE_PHASE::NONE),
	uiMng_(UIManager::GetInstance()),
	hitStopFrame_(HIT_STOP_FRAME)
{
	changeUpdate_ = {
		{UPDATE_PHASE::NONE,[this]() {ChangeUpdateNone(); }},
		{UPDATE_PHASE::NORMAL,[this]() {ChangeUpdateNormal(); }},
		{UPDATE_PHASE::HIT_STOP,[this]() {ChangeUpdateHitStop(); } }
	};

}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::Update(void)
{
	phazeUpdate_();
}

void CharacterBase::MakeAttackCol(const Collider::TAG _charaTag, const Collider::TAG _attackTag, const VECTOR& _atkPos, const float& _radius)
{

}


void CharacterBase::MoveLimit(const VECTOR& _stagePos,const VECTOR& _stageSize)
{

}

void CharacterBase::ChangeUpdatePhase(const UPDATE_PHASE _phase)
{
	if (updatePhase_ == _phase)return;
	updatePhase_ = _phase;
	changeUpdate_[updatePhase_]();
}

void CharacterBase::ChangeDirectToNormal(void)
{
	phazeUpdate_ = [this]() {UpdateNormal(); };
}
const bool CharacterBase::GetIsHitTarget(void) const
{
	return onHit_->GetIsHitTarget();
}



void CharacterBase::UpdateNone(void)
{
	//‰½‚à‚µ‚È‚¢
}

void CharacterBase::UpdateHitStop(void)
{
	if (--hitStopFrame_ > 0)return;
	hitStopFrame_ = HIT_STOP_FRAME;
	ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
}

void CharacterBase::ChangeUpdateNone(void)
{
	phazeUpdate_ = [this]() {UpdateNone(); };
}

void CharacterBase::ChangeUpdateNormal(void)
{
	phazeUpdate_ = [this]() {UpdateNormal(); };
}

void CharacterBase::ChangeUpdateHitStop(void)
{
	phazeUpdate_ = [this]() { UpdateHitStop(); };
}
