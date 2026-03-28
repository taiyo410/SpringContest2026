#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Common/FontController.h"
#include "../Application.h"
#include "../Common/Easing.h"
#include "MenuController.h"
#include "MenuManager.h"

MenuManager::MenuManager(void) :
	ins_(InputManager::GetInstance()),
	insS_(InputManagerS::GetInstance()),
	soundMng_(SoundManager::GetInstance()),
	selectMenuNum_(0),
	disSpawnCnt_(0.0f),
	sizeEaseCnt_(0.0f),
	isAllDirectEaseEnd_(false),
	yesNoStrTable_{ L"はい",L"いいえ" },
	yesNoState_(YES_NO::NO)
{
	easing_ = std::make_unique<Easing>();
	fontController_ = std::make_unique<FontController>();
}

MenuManager::~MenuManager(void)
{
}

void MenuManager::LoadFont(const std::wstring _fontType, const int _size)
{
	defaultFontHandle_ = _fontType;
	defaultFontSize_ = _size;

	fontHandle_ = fontController_->GetFontHandle(_fontType, _size, 0);

}

void MenuManager::AddMenu(const int _arrayNum, const std::wstring _menu, const Vector2 _pos)
{
	//情報の代入
	BTN_INFO info;
	info.btnStr = _menu;
	info.startPos = _pos;
	//info.curPos = _pos;
	std::unique_ptr menu = std::make_unique<MenuController>(_arrayNum, _menu, _pos,fontHandle_);
	menuList_.emplace_back(std::move(menu));
}

void MenuManager::Init(void)
{
	for (auto& menu : menuList_)
	{
		menu->Init();
	}
}

void MenuManager::Update(void)
{
	for (auto& menu : menuList_)
	{
		menu->Update();
	}
}

const int MenuManager::GetSizeEasingFontHandle(const int _arrayNum, const int _startSize, const int _goalSize, const float _easeTime, Easing::EASING_TYPE _easeType)
{
	int size = easing_->EaseFunc(_startSize, _goalSize, sizeEaseCnt_ / _easeTime, _easeType);
	if (sizeEaseCnt_ >= _easeTime)
	{
		sizeEaseCnt_ = 0.0f;
	}
	sizeEaseCnt_ += SceneManager::GetInstance().GetDeltaTime();

	auto& dynamicFont = sizeEasingFontHandleTable_;
	auto it = dynamicFont.find(size);
	int dynamicFontHandle = -1;

	//同じサイズのフォントがすでにあるかをチェック
	if (it != dynamicFont.end())
	{
		dynamicFontHandle = it->second;
	}
	else
	{
		int newFontHandle = CreateFontToHandle(defaultFontHandle_.c_str(), size, 0);
		dynamicFont[size] = newFontHandle;
		dynamicFontHandle = newFontHandle;
	}
	return dynamicFontHandle;
}

void MenuManager::UpdateDirection(const float _disSpawn, const float _easeTime, const int _goalPosX)
{

	//すべてのメニューが演出イージングが終わっているかをチェック
	isAllDirectEaseEnd_ = std::all_of(menuList_.begin(), menuList_.end(), [](const auto& menu) { return menu->GetIsEndDirectionEase(); });

	//すべてのメニューが演出イージングが終わっている場合は、これ以上更新しない
	if (isAllDirectEaseEnd_)return;

	//間隔カウントが一定時間を超えたらイージング開始
	for (auto& menu : menuList_)
	{
		//////イージング中またはイージングが終わっているものはスキップ
		if (menu->GetIsEase())continue;
		if (disSpawnCnt_ > _disSpawn)
		{
			disSpawnCnt_ = 0.0f;
			menu->SetEase();
		}
	}

	//イージング中のメニューの座標を更新
	for (auto& menu : menuList_)
	{
		//if (menu.second.isEase)
		//{
		//	Vector2 goalPos = { _goalPosX,menu.second.curPos.y };
		//	menu.second.curPos = easing_->EaseFunc(menu.second.startPos, goalPos, menu.second.directionEaseCnt / _easeTime, Easing::EASING_TYPE::OUT_BACK);
		//	menu.second.directionEaseCnt += SceneManager::GetInstance().GetDeltaTime();
		//	if (menu.second.directionEaseCnt >= _easeTime)
		//	{
		//		menu.second.isEase = false;
		//		menu.second.isEndDirectEase = true;
		//		menu.second.curPos = goalPos;
		//		menu.second.startPos = goalPos;
		//	}
		//}
		menu->UpdateDirection(_goalPosX, _easeTime);
	}
	//if (disSpawnCnt_ > _disSpawn)
	//{
	//	disSpawnCnt_ = 0.0f;
	//}
	disSpawnCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void MenuManager::NormalUpdate(const Vector2 _localPos, const float _easeTime, const Easing::EASING_TYPE _easeType)
{
	for (auto& menu : menuList_)
		{
		//	//選択中のメニュー左右に動かす
		//	if (menu.first == selectMenuNum_)
		//	{
		//		menu.second.curPos = easing_->EaseFunc(menu.second.startPos, menu.second.startPos + _localPos, sizeEaseCnt_ / _easeTime, _easeType);
		//	}
		//	else
		//	{
		//		menu.second.curPos = menu.second.startPos;
		//	}
		menu->SelectUpdate(selectMenuNum_, sizeEaseCnt_, _localPos, _easeTime, _easeType);
	}
	if (sizeEaseCnt_ >= _easeTime)
	{
		sizeEaseCnt_ = 0.0f;
	}
	sizeEaseCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void MenuManager::SetYesNoUpdate(const bool _isYes)
{
	_isYes ? yesNoState_ = YES_NO::YES : yesNoState_ = YES_NO::NO;
}


void MenuManager::Draw(void)
{
	unsigned int color = UtilityCommon::WHITE;
	for (auto& menu : menuList_)
	{
		////選択中のメニューはサイズイージングして赤色で描画
		//color = DecideColor(menu.first);
		//DrawStringToHandle(
		//	menu.second.curPos.x, menu.second.curPos.y, menu.second.btnStr.c_str(), color, fontHandle_);
		menu->Draw();
	}
}

void MenuManager::DrawCenter(void)
{
	unsigned int color = UtilityCommon::WHITE;
	for (auto& menu : menuList_)
	{
		////選択中のメニューはサイズイージングして赤色で描画
		//color = DecideColor(menu.first);
		//UtilityDraw::DrawStringCenter(menu.second.curPos.x, menu.second.curPos.y, color, fontHandle_, menu.second.btnStr.c_str());
		menu->DrawCenter();
	}
}


void MenuManager::YesNoDraw(const std::wstring _questionStr)
{
	const Vector2 startPos = { Application::SCREEN_HALF_X - (CHECK_EXIT_MENU_SIZE_X / 2)
						,Application::SCREEN_HALF_Y - (CHECK_EXIT_MENU_SIZE_Y / 2) };
	const Vector2 endPos = { Application::SCREEN_HALF_X + (CHECK_EXIT_MENU_SIZE_X / 2),
							Application::SCREEN_HALF_Y + (CHECK_EXIT_MENU_SIZE_Y / 2) };

	//メニュ背景
	DrawBox(startPos.x,
		startPos.y,
		endPos.x,
		endPos.y,
		0x00ff00,
		true
	);

	//質問の文字列描画
	DrawFormatStringToHandle(
		startPos.x + QUESTION_OFFSET,
		startPos.y + QUESTION_OFFSET,
		UtilityCommon::BLACK,
		fontHandle_,
		_questionStr.c_str()
	);

	int size = static_cast<int>(yesNoStrTable_->size());
	for (int i = 0; i < size; i++)
	{
		unsigned int btnCol = UtilityCommon::WHITE;
		//選択中の文字は赤色で描画
		yesNoState_ == static_cast<YES_NO>(i) ? btnCol = UtilityCommon::RED : btnCol = UtilityCommon::WHITE;

		DrawFormatStringToHandle(
			startPos.x + YES_NO_DISTANCE_X + i * YES_NO_DISTANCE_Y,
			startPos.y + YES_NO_DISTANCE_Y,
			btnCol,
			fontHandle_,
			yesNoStrTable_[i].c_str()
		);
	};
}

void MenuManager::SelectMenu(const SoundManager::SRC _src)
{
	if (insS_.IsTrgDown(INPUT_EVENT::UP) || ins_.IsTrgDown(KEY_INPUT_W))
	{
		soundMng_.Play(_src, SoundManager::PLAYTYPE::BACK);
		SubSelectMenuNum();
	}
	else if (insS_.IsTrgDown(INPUT_EVENT::DOWN) || ins_.IsTrgDown(KEY_INPUT_S))
	{
		soundMng_.Play(_src, SoundManager::PLAYTYPE::BACK);
		AddSelectMenuNum();
	}

	auto itr = std::find_if(menuList_.begin(), menuList_.end(), [](auto& menu) {return menu->GetIsHit(); });
	if (itr != menuList_.end())
	{
		selectMenuNum_ = (*itr)->GetMenuNum();
	}

}

void MenuManager::AddSelectMenuNum(void)
{
	selectMenuNum_++;
	if (selectMenuNum_ >= static_cast<int>(menuList_.size()))
	{
		selectMenuNum_ = 0;
	}
}

void MenuManager::SubSelectMenuNum(void)
{
	selectMenuNum_--;
	if (selectMenuNum_ < 0)
	{
		selectMenuNum_ = static_cast<int>(menuList_.size()) - 1;
	}
}



//const bool MenuManager::IsHasFormat(const std::wstring _str)const
//{
//	if (_str.find(L"%d") != std::wstring::npos || _str.find(L"%f") != std::wstring::npos)
//	{
//		return true;
//	}
//	return false;
//}

const Vector2 MenuManager::GetMenuCenterPos(const std::wstring _str)const
{
	int w, h;
	GetDrawStringSizeToHandle(
		&w,
		&h,
		NULL,
		_str.c_str(),
		static_cast<int>(wcslen(_str.c_str())),
		fontHandle_);
	int centerX = w / 2;
	int centerY = h / 2;
	return { centerX,centerY };
}

unsigned int MenuManager::DecideColor(const int btnNum)
{
	if (btnNum == selectMenuNum_)
	{
		return SELECT_COL;
	}
	return UNSELECT_COL;
}



