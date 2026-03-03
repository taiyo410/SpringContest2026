#include "../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Object/Common/Geometry/Capsule.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../../../Utility/Utility3D.h"
#include "CharacterOnHitBase.h"

namespace
{
	using TAG_PRIORITY = ObjectBase::TAG_PRIORITY;
	using TAG = Collider::TAG;
}

CharacterOnHitBase::CharacterOnHitBase(CharacterBase& _chara, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam):
	charaObj_(_chara),
	colParam_(_colParam)
{
}

CharacterOnHitBase::~CharacterOnHitBase(void)
{
}

void CharacterOnHitBase::OnHitUpdate(const std::weak_ptr<Collider> _hitCol)
{
	for (const auto tag : _hitCol.lock()->GetTags())
	{
		colUpdates_[tag](_hitCol);
	}
}

void CharacterOnHitBase::InitHit(void)
{
	isHitTarget_ = false;
}


void CharacterOnHitBase::CollNone(void)
{
}

void CharacterOnHitBase::CollStage(const std::weak_ptr<Collider> _hitCol)
{

}

void CharacterOnHitBase::HitModelCommon(const std::weak_ptr<Collider> _hitCol)
{

}

