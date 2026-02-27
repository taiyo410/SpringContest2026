#include "../../../Pch.h"
#include "DaimyoHizen.h"

void DaimyoHizen::Init(void)
{
	DaimyoBase::Init();

	trans_.pos = { 400.0f, 200.0f, 0.0f };

	// 藩のイメージカラー（黄）
	color_ = GetColor(255, 255, 0);
}
