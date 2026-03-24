#include "../Pch.h"
#include "Geometry2D.h"

Geometry2D::~Geometry2D(void)
{
}

void Geometry2D::HitAfter(void)
{
}

Geometry2D::Geometry2D(const Vector2F& _pos, const Vector2F _movedPos, const float _preHitRadius)
	: pos_(_pos),
	movedPos_(_pos),
	preHitRadius_(_preHitRadius)
{
	hitResult_ = {};
}
