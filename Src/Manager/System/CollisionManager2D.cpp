#include "../Pch.h"
#include "CollisionManager2D.h"

CollisionManager2D* CollisionManager2D::instance_ = nullptr;

void CollisionManager2D::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new CollisionManager2D();
	}
}

CollisionManager2D& CollisionManager2D::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CreateInstance();
	}

	return *instance_;
}

void CollisionManager2D::AddCollider(const std::shared_ptr<Collider2D> _collider)
{
}

void CollisionManager2D::Sweep(void)
{
}

void CollisionManager2D::Update(void)
{
}

void CollisionManager2D::Destroy(void)
{
}

CollisionManager2D::CollisionManager2D(void)
{
}

CollisionManager2D::~CollisionManager2D(void)
{
}
