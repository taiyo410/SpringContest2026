#include "../pch.h"
#include "../Object/UI/SliderUIController.h"
#include "SliderUIManager.h"

SliderUIManager::SliderUIManager(void):
	sliderUIs_()
{
}

SliderUIManager::~SliderUIManager(void)
{
	sliderUIs_.clear();
}

void SliderUIManager::AddSliderUI(const Cursor& _cursor, float& _per, Vector2F _leftTopPos, Vector2F _length)
{
	std::unique_ptr sliderUI = std::make_unique<SliderUIController>(_cursor, _per, _leftTopPos, _length);
	sliderUIs_.emplace_back(std::move(sliderUI));
}

void SliderUIManager::Load(void)
{
	for (auto& slider : sliderUIs_)
	{
		slider->Load();
	}
}

void SliderUIManager::Init(void)
{
	for (auto& slider : sliderUIs_)
	{
		slider->Init();
	}
}

void SliderUIManager::Update(void)
{
	for (auto& slider : sliderUIs_)
	{
		slider->Update();
	}
}

void SliderUIManager::Draw(void)
{
	for (auto& slider : sliderUIs_)
	{
		slider->Draw();
	}
}

void SliderUIManager::Release(void)
{
	for (auto& slider : sliderUIs_)
	{
		slider->Release();
	}
}
