#include "../pch.h"
#include "../Common/Vector2.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/Circle.h"
#include "Cursor.h"

Cursor::Cursor(void)
{
}

Cursor::~Cursor(void)
{
}

void Cursor::Load(void)
{
	//当たり判定
	std::unique_ptr<Geometry2D>geo = std::make_unique<Circle>(pos_, pos_, 15,10);
	MakeCollider(Collider2D::TAG::CURSOR, std::move(geo), { Collider2D::TAG::CURSOR });
}

void Cursor::Init(void)
{
}

void Cursor::Update(void)
{
	//カーソル座標
	Vector2 pos;
	GetMousePoint(&pos.x, &pos.y);
	pos_ = pos.ToVector2F();
}

void Cursor::Draw(void)
{
	for (auto& col : colliders_)
	{
		col.get()->GetGeometry().Draw(0);
	}
	DrawFormatString(0, 0, 0xffffff, L"Cursor Pos:(%f,%f)", pos_.x, pos_.y);
}

void Cursor::Release(void)
{
}

void Cursor::OnHit(const std::weak_ptr<Collider2D> _partner)
{
}
