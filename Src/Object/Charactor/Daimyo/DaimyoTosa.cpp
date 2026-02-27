#include "../../../Pch.h"
#include "DaimyoTosa.h"

void DaimyoTosa::Init(void)
{
	DaimyoBase::Init();

	trans_.pos = { 150.0f, 500.0f, 0.0f };

	// 藩のイメージカラー（緑）
	color_ = GetColor(0, 255, 0);
}
