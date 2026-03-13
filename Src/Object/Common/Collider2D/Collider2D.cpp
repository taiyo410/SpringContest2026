#include "../Pch.h"
#include "../Object/ObjectBase2D.h"
#include "Collider2D.h"

Collider2D::Collider2D(ObjectBase2D& _owner, const TAG _myTag, std::unique_ptr<Geometry2D>_geo, const std::set<TAG>_noHitTags)
	: owner_(_owner),
	myTag_(_myTag),
	geometry_(std::move(_geo)),
	noHitTags_(_noHitTags)
{
	isHit_ = false;
	isDead_ = false;
}

Collider2D::~Collider2D(void)
{
	Kill();
}
