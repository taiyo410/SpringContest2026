#include "../../Pch.h"
#include "CharactorBase.h"

CharactorBase::CharactorBase(void)
    : imageHandle_(-1)
    , isLoaded_(false)
    , isActive_(true)
    , animeTimer_(0.0f)
    , animeFrame_(0)
{
}

CharactorBase::~CharactorBase(void)
{
}

void CharactorBase::Init(void)
{
    ObjectBase::Init();
    isActive_ = true;
    animeTimer_ = 0.0f;
    animeFrame_ = 0;
}

void CharactorBase::Update(void)
{
    if (!isActive_) return;
    // 共通のアニメーション更新処理など
}

void CharactorBase::Draw(void) const
{
    if (!isActive_ || !isLoaded_) return;
}