#include "../pch.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Manager/Resource/ResourceManager.h"
#include "ArrowController.h"

ArrowController::ArrowController(const ResourceManager::SRC _arrowSrc, Vector2F& _startPos, Vector2F& _endPos, float _thick, float& _per,FLOAT4& _col):
	arrowSrc_(_arrowSrc),
	startPos_(_startPos),
	endPos_(_endPos),
	thick_(_thick),
	per_(_per),
	col_(_col)
{
	material_ = std::make_unique<PixelMaterial>(L"GaugePS.cso", GAUGE_CONST_BUF);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}


ArrowController::~ArrowController(void)
{
}

void ArrowController::Load(void)
{
	arrowImg_ = ResourceManager::GetInstance().Load(arrowSrc_).handleId_;
}

void ArrowController::Init(void)
{
	material_->AddTextureBuf(arrowImg_);
	material_->AddConstBuf(col_);
	material_->AddConstBuf(col_);
	material_->AddConstBuf({0.0f,0.0f,0.0f,0.0f});
	material_->AddConstBuf({ per_,per_,0.0f,0.0f });
	MakeArrowGauge();
	//renderer_->MakeVertex(startPos_.x, startPos_.y, endPos_.x + thick_, endPos_.y + thick_);
	//renderer_->MakeVertexQuad({0.0f,0.0f},{100.0f,0.0f},{150.0f,50.0f},{50.0f,50.0f});
}

void ArrowController::Update(void)
{
	//シェーダの更新
	renderer_->SetPos(pos_);
	material_->SetConstBuf(COLER1_BUF_NUM, col_);
	material_->SetConstBuf(COLER2_BUF_NUM, col_);
	material_->SetConstBuf(PERCENT_BUF_NUM, { per_,per_,0.0f,0.0f });
}

void ArrowController::Draw(void)
{
	renderer_->Draw();
}

void ArrowController::Release(void)
{
}

void ArrowController::MakeArrowGauge(void)
{
	//方向を決める
	Vector2F dir = endPos_ - startPos_;

	//長さを求める
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

	//0除算を防ぐ
	if (length < 0.001)return;

	//正規化
	//float inv = 1.0f / length;
	Vector2F norm = dir / length;

	//垂直ベクトルを求める
	Vector2F perp = { -norm.y, norm.x };

	float half = thick_ * 0.5f;

	float w = 0;
	//4頂点を作る
	////Vector2F v0 = { startPos_.x + perp.x * w, startPos_.y + perp.y * w };
	//Vector2F v0 = startPos_ + perp * w * half;
	////Vector2F v1 = { startPos_.x - perp.x * w, startPos_.y - perp.y * w };
	//Vector2F v1 = startPos_ - perp * w * half;
	////Vector2F v2 = { end.x - perp.x * w, end.y - perp.y * w };
	//Vector2F v2 = endPos_ - perp * w * half;
	////Vector2F v3 = { end.x + perp.x * w, end.y + perp.y * w };
	//Vector2F v3 = endPos_ + perp * w * half;

	Vector2F v0 = { startPos_.x + perp.x * half, startPos_.y + perp.y * half };
	Vector2F v1 = { startPos_.x - perp.x * half, startPos_.y - perp.y * half };
	Vector2F v2 = { endPos_.x - perp.x * half, endPos_.y - perp.y * half };
	Vector2F v3 = { endPos_.x + perp.x * half, endPos_.y + perp.y * half };

	renderer_->MakeVertexQuad(v1, v2, v3, v0);

}
