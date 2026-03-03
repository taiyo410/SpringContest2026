#include "Collider2D.h"
#include "../Pch.h"

Collider2D::Collider2D(ObjectBase& _owner, TAG _myTag, std::unique_ptr<Geometry2D>_geo, std::set<TAG>_noHitTags)
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
}
