#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/BoxGeo.h"
#include "Daimyo.h"

Daimyo::Daimyo(const DaimyoImport _import)
	: import_(_import)
{
}

Daimyo::~Daimyo(void)
{
}

void Daimyo::Load(void)
{
	//初期化
	Init();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(pos_, pos_, import_.hitBoxMin, import_.hitBoxMax);
	MakeCollider(Collider2D::TAG::DAIMYO, std::move(geo), {});
}

void Daimyo::Init(void)
{
	//初期化
	pos_ = import_.pos;
}

void Daimyo::Update(void)
{
}

void Daimyo::Draw(void)
{
	for (auto& col : colliders_)
	{
		col.get()->GetGeometry().Draw(import_.color);
	}

	DrawFormatString(pos_.x, pos_.y, 0xffffff, L"%ls", UtilityCommon::GetWStringFromString(import_.name).c_str());
}

void Daimyo::Release(void)
{
}

void Daimyo::OnHit(const std::weak_ptr<Collider2D> _partner)
{
}
