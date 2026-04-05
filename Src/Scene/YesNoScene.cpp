#include "../pch.h"
#include "../Utility/UtilityDraw.h"
#include "../Object/Character/Cursor/Cursor.h"
#include "../Manager/Generic/MenuManager.h"
#include "../Manager/Game/CollisionManager2D.h"
#include "../Manager/Resource/FontManager.h"
#include "../Common/FontController.h"
#include "YesNoScene.h"

YesNoScene::YesNoScene(Vector2F _centerPos, Vector2F _size):
	centerPos_(_centerPos),
	size_(_size),
	state_(YES_NO_STATE::NO)
{
	fontController_ = std::make_unique<FontController>();
	fontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);

	menuMng_ = std::make_unique<MenuManager>();
	yesNoStr_.emplace(YES_NO_STATE::YES, L"‚Í‚¢");
	yesNoStr_.emplace(YES_NO_STATE::NO, L"‚¢‚¢‚¦");
	yesNoPos_.emplace(YES_NO_STATE::YES, Vector2( centerPos_.x - size_.x / 2,centerPos_.y ));
	yesNoPos_.emplace(YES_NO_STATE::NO, Vector2(centerPos_.x + size_.x / 2,centerPos_.y ));

	for (auto& str : yesNoStr_)
	{
		menuMng_->AddMenu(static_cast<int>(str.first), str.second, yesNoPos_[str.first], true);
	}
}

YesNoScene::~YesNoScene(void)
{
}

void YesNoScene::Load(void)
{
	menuMng_->LoadFont(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE);
}

void YesNoScene::Init(void)
{
	menuMng_->Init();
	state_ = YES_NO_STATE::NO;
}

void YesNoScene::Update(void)
{

	menuMng_->NormalUpdate();
	menuMng_->SelectMenu();
	menuMng_->Update();

	if (menuMng_->GetSelectMenuNum() == static_cast<int>(YES_NO_STATE::YES))
	{
		state_ = YES_NO_STATE::YES;
	}
	else
	{
		state_ = YES_NO_STATE::NO;
	}
}

void YesNoScene::Draw(const std::wstring& _str)
{
	DrawBox(centerPos_.x - size_.x, centerPos_.y - size_.y, centerPos_.x + size_.x, centerPos_.y + size_.y , UtilityCommon::GREEN, true);

	UtilityDraw::DrawStringCenterToFontHandle(
		static_cast<int>(centerPos_.x),
		static_cast<int>(centerPos_.y - (size_.y / 2 + 20)),
		UtilityCommon::BLACK,
		fontHandle_,
		_str);
	
	menuMng_->Draw();
}
