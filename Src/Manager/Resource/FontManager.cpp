#include "FontManager.h"
#include <DxLib.h>
#include "../Application.h"

FontManager::FontManager(void)
{
	//初期化
	for (int i = 0; i < static_cast<int>(FONT_TYPE::MAX); i++)
	{
		fontPath_[i] = L"null";
	}
}

void FontManager::Init(void)
{
	//フォント登録を行う
	// フォントパス初期化
	fontPath_[static_cast<int>(FONT_TYPE::DOT)] = L"FontDot.otf";
	fontPath_[static_cast<int>(FONT_TYPE::BOKUTATI)] = L"bokutachi.otf";
	fontPath_[static_cast<int>(FONT_TYPE::APRIL_GOTHIC_ONE)] = L"AprilGothicOne-R.ttf";

	// フォントをシステムに登録
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (AddFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}



void FontManager::Release(void)
{

	// フォント登録解除
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (RemoveFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== 0)
		{
			return;
		}
		fontPath_[i].clear();

	}

}