#include "../../../Pch.h"
#include "DaimyoSatsuma.h"

void DaimyoSatsuma::Init()
{
    DaimyoBase::Init();
    
    // 座標設定 (x, y, z)
    trans_.pos = { 150.0f, 200.0f, 0.0f };
    
    // 藩のイメージカラー（赤）
    color_ = GetColor(255, 0, 0);

    accumulationSpeed_ = 1.2f;
}
