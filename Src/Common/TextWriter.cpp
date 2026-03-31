#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "TextWriter.h"

TextWriter::TextWriter(void):
	currentStrNum_(),
	textSpd_()
{
}

TextWriter::~TextWriter(void)
{
}

void TextWriter::AddText(const std::wstring _text)
{
	texts_.push_back(_text);
}

void TextWriter::Update(void)
{
	if (currentStrNum_ >= texts_.front().size())return;

	if (disCnt_ > TEXT_SPD)
	{
		disCnt_ = 0.0f;
		currentStrNum_++;
	}
	disCnt_ > TEXT_SPD ? disCnt_ = 0 : disCnt_ ++;
}

void TextWriter::Draw(int _x, int _y, int _fontHandle)
{
	if (texts_.empty()) return;

	// フォントハンドルが指定されている場合は、ハンドルを使って描画
	if (_fontHandle != -1)
	{
		DrawFormatStringToHandle(_x, _y, UtilityCommon::WHITE, _fontHandle, texts_.front().substr(0, currentStrNum_).c_str());
	}
	else
	{
		// 指定がない場合はデフォルトの描画
		DrawFormatString(_x, _y, UtilityCommon::WHITE, texts_.front().substr(0, currentStrNum_).c_str());
	}
}
