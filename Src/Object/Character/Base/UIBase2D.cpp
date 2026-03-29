#include "../pch.h"
#include "../Common/Easing.h"
#include "../../Manager/Resource/ResourceManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "UIBase2D.h"

UIBase2D::UIBase2D(void):
	resMng_(ResourceManager::GetInstance()),
	easing_(nullptr),
	material_(nullptr),
	renderer_(nullptr)
{
}

UIBase2D::~UIBase2D(void)
{
}
