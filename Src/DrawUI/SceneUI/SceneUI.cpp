#include "../../Pch.h"
#include "SceneUi.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../DrawUI/Font.h"
#include "../Common/Easing.h"
#include "../../Application.h"

SceneUi::SceneUi(void)
    : frameCount_(0), isBlinking_(false), currentIndex_(0)
{
	easing_ = std::make_unique<Easing>();
}

SceneUi::~SceneUi(void)
{
}

void SceneUi::Draw(int baseYOverride)
{
    DrawFont(baseYOverride); // 上のオーバーロード呼ぶ
}


void SceneUi::FontBlinking(void)
{
    frameCount_++;
    isBlinking_ = (frameCount_ / blinkInterval_) % 2 ? true : false;
}

void SceneUi::DrawFont(int baseYOverride)
{
    int baseY = baseYOverride;   // 呼び出し側から指定されたY座標
    int spacing = 80;
    easingTime_ >= EASING_DURATION ? easingTime_ = 0 : easingTime_ += SceneManager::GetInstance().GetDeltaTime();
    for (size_t i = 0; i < fontList_.size(); ++i)
    {
        const auto& font = fontList_[i];
        int textWidth = GetDrawStringWidth(font.message.c_str(), (int)font.message.length());

        int xPos = (Application::SCREEN_SIZE_X / 2);
        int yPos = baseY + (int)i * spacing;
        int color = 0;
        if (static_cast<int>(i) == currentIndex_)
        {
            color = GetColor(255, 255, 0);
            fontSize_ = easing_->EaseFunc(42, 68, easingTime_ / EASING_DURATION, Easing::EASING_TYPE::COS_BACK);
        }
        else
        {
            color =  GetColor(170, 170, 170);
            fontSize_ = 42;
        }
        //DrawRotaStringFToHandle(xPos, yPos, fontSize_, fontSize_, xPos, yPos, 0.0f, color, Font::FONT_TYPE_ANTIALIASING_EDGE, GetColor(0,0,0),false,font.message.c_str());
        //Font::GetInstance().DrawDefaultText(xPos, yPos, font.message.c_str(), color, fontSize_, Font::FONT_TYPE_ANTIALIASING_EDGE);
        Font::GetInstance().DrawTextCentered(xPos, yPos, font.message.c_str(), color, fontSize_, Font::FONT_TYPE_ANTIALIASING_EDGE);
    }
}

void SceneUi::AddCharctor(const char* _char)
{
    fontList_.push_back(FontData{ _char });
}

void SceneUi::SetCurrentIndex(int index)
{
    if (index >= 0 && index < static_cast<int>(fontList_.size()))
    {
        currentIndex_ = index;
    }
}

int SceneUi::GetCurrentIndex(void) const
{
    return currentIndex_;
}

int SceneUi::GetMaxIndex(void) const
{
    return static_cast<int>(fontList_.size());
}
