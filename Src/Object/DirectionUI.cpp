#include "../pch.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "DirectionUI.h"

DirectionUI::DirectionUI(void):
	resMng_(ResourceManager::GetInstance()),
	skipGaugePer_(0.0f)
{
}

DirectionUI::~DirectionUI(void)
{
}

void DirectionUI::Load(void)
{

}

void DirectionUI::Init(void)
{

}

void DirectionUI::Update(void)
{

}

void DirectionUI::Draw(void)
{

}

void DirectionUI::SetSkipGaugePer(const float _skipPer)
{
	skipGaugePer_ = _skipPer;
}

void DirectionUI::UpdateIntensiveLineAnim(void)
{

}
