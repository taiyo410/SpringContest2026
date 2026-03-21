#include "../pch.h"
#include "FontController.h"

FontController::FontController(void):
	fontStatus_()
{
}

FontController::~FontController(void)
{
	for (auto& font : fontStatus_)
	{
		DeleteFontToHandle(font.handle);
	}
	fontStatus_.clear();
}

const int FontController::GetFontHandle(const std::wstring& _fontPass, const int _size, const int thick, const int _fontType)
{
	FONT_STATUS fontStatus = { _fontPass,_size,thick,_fontType };
	auto itr = std::find_if(fontStatus_.begin(),
		fontStatus_.end(),
		[&fontStatus](const FONT_STATUS& status) {return status == fontStatus; }
	);
	//見つからなければ配列に挿入し、作ったハンドルを返す
	if (itr == fontStatus_.end())
	{
		fontStatus.handle = CreateFontToHandle(fontStatus.pass.c_str(), fontStatus.size, fontStatus.thick, fontStatus.type);
		fontStatus_.emplace_back(fontStatus);
		return fontStatus.handle;
	}

	//見つかれば配列内のハンドルを返す
	return (*itr).handle;
}