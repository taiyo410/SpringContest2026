#include "../pch.h"
#include "../Object/UI/SliderUIController.h"
#include "SliderUIManager.h"

SliderUIManager::SliderUIManager(void):
	sliderUIs_(),
	sliderNumCnt_()
{
}

SliderUIManager::~SliderUIManager(void)
{
	sliderUIs_.clear();
}

void SliderUIManager::AddSliderUI(const Cursor& _cursor, float _per, Vector2F _leftTopPos, Vector2F _length)
{
	std::unique_ptr sliderUI = std::make_unique<SliderUIController>(sliderNumCnt_,_cursor, _per, _leftTopPos, _length);
	sliderUIs_.emplace_back(std::move(sliderUI));
	sliderNumCnt_++;
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
	//auto itr = std::find_if(sliderUIs_.begin(), sliderUIs_.end(), [](const std::unique_ptr<SliderUIController>& slider) { return slider->GetIsHitSlider(); });

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

std::vector<std::unique_ptr<SliderUIController>>::const_iterator SliderUIManager::GetIsHitSlider(void)const
{
	auto itr = std::find_if(sliderUIs_.begin(), sliderUIs_.end(), [](const std::unique_ptr<SliderUIController>& slider) { return slider->GetIsHit(); });
	return itr;
}

const std::vector<float>SliderUIManager::GetSliderPercent(void)
{
	std::vector<float>percent;
	for(auto& slider : sliderUIs_)
	{
		float per = slider->GetPercent();
		percent.emplace_back(per);
	}
	return percent;
}
