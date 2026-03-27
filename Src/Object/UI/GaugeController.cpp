#include "../pch.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "GaugeController.h"

GaugeController::GaugeController(ResourceManager::SRC _maskSrc, Vector2F& _pos, Vector2F& _size, float& _per, FLOAT4& _col1, FLOAT4& _col2):
	maskSrc_(_maskSrc),
	pos_(_pos),
	size_(_size),
	per_(_per),
	col1_(_col1),
	col2_(_col2)
{
	material_ = std::make_unique<PixelMaterial>(L"GaugePS.cso", GAUGE_CONST_BUF);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

GaugeController::~GaugeController(void)
{
}

void GaugeController::Init(void)
{
	material_->AddTextureBuf(maskImg_);
	material_->AddConstBuf(col1_);
	material_->AddConstBuf(col2_);
	material_->AddConstBuf(GAUGE_OVER_COLOR);
	material_->AddConstBuf({ per_,per_,0.0f,0.0f });
	renderer_->MakeSquareVertex(pos_, size_);
}

void GaugeController::Load(void)
{
	maskImg_ = resMng_.Load(maskSrc_).handleId_;
}

void GaugeController::Update(void)
{
	//シェーダの更新
	renderer_->SetPos(pos_);
	material_->SetConstBuf(COLER1_BUF_NUM,col1_);
	material_->SetConstBuf(COLER2_BUF_NUM,col2_);
	material_->SetConstBuf(PERCENT_BUF_NUM, { per_,per_,0.0f,0.0f });
}

void GaugeController::Draw(void)
{
	renderer_->Draw();
}

void GaugeController::Release(void)
{
}
