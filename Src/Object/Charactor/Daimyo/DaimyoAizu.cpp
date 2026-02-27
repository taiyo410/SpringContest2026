#include "../../../Pch.h"
#include "DaimyoAizu.h"

void DaimyoAizu::Init(void)
{
	DaimyoBase::Init();

	trans_.pos = { 400.0f, 350.0f, 0.0f };

	// 藩のイメージカラー（紫）
	color_ = GetColor(255, 0, 255);

	accumulationSpeed_ = 0.5f;
}
