#include "../Manager/Generic/SceneManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Common/Easing.h"
#include "MenuController.h"

MenuController::MenuController(const int _menuNum, const std::wstring _menu, const Vector2 _pos, int& _fontHandle) :
	scnMng_(SceneManager::GetInstance()),
	fontHandle_(_fontHandle),
	menuNum_(_menuNum),
	btnStr_(_menu),
	startPos_(_pos),
	curPos_(startPos_),
	directionEaseCnt_(),
	isEase_(false),
	isEndDirectEase_(false),
	color_(UtilityCommon::WHITE),
	isHit_(false)
{
	easing_ = std::make_unique<Easing>();
}

MenuController::~MenuController(void)
{
}

void MenuController::Load(void)
{
}

void MenuController::Init(void)
{
}

void MenuController::Update(void)
{
}

void MenuController::Draw(void)
{
	//選択中のメニューはサイズイージングして赤色で描画
	DrawStringToHandle(
		curPos_.x, curPos_.y, btnStr_.c_str(), color_, fontHandle_);
}

void MenuController::DrawCenter(void)
{
	UtilityDraw::DrawStringCenter(curPos_.x, curPos_.y, color_, fontHandle_, btnStr_.c_str());
}

void MenuController::Release(void)
{
}

void MenuController::UpdateDirection(const float _goalPosX,const float _easeTime)
{
	//イージング中のメニューの座標を更新
	if (isEase_)
	{
		Vector2 goalPos = { _goalPosX,curPos_.y };
		curPos_ = easing_->EaseFunc(startPos_, goalPos,directionEaseCnt_ / _easeTime, Easing::EASING_TYPE::OUT_BACK);
		directionEaseCnt_ += SceneManager::GetInstance().GetDeltaTime();
		if (directionEaseCnt_ >= _easeTime)
		{
			isEase_ = false;
			isEndDirectEase_ = true;
			curPos_ = goalPos;
			startPos_ = goalPos;
		}
	}
}

void MenuController::SelectUpdate(const float _selectNum, const float _currentCnt, const Vector2 _localPos, const float _easeTime, const Easing::EASING_TYPE _easeType)
{
	//選択中のメニュー左右に動かす
	if (_selectNum == menuNum_)
	{
		curPos_ = easing_->EaseFunc(startPos_, startPos_ + _localPos, _currentCnt / _easeTime, _easeType);
		color_ = SELECT_COL;
	}
	else
	{
		curPos_ = startPos_;
		color_ = UNSELECT_COL;
	}
}

void MenuController::SetEase(void)
{
	isEase_ = true;
}

void MenuController::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	isHit_ = true;
}

const bool MenuController::IsHasFormat(void) const
{
	if (btnStr_.find(L"%d") != std::wstring::npos || btnStr_.find(L"%f") != std::wstring::npos)
	{
		return true;
	}
	return false;
}
