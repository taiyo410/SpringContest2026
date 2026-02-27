#include "../../../Pch.h"
#include "DaimyoChoshu.h"

void DaimyoChoshu::Init(void)
{
	DaimyoBase::Init();

	trans_.pos = { 150.0f, 350.0f, 0.0f };
	
	// 藩のイメージカラー（青）
	color_ = GetColor(0, 0, 255);

	accumulationSpeed_ = 2.0f;
}
