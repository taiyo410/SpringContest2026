#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "TextWriter.h"

TextWriter::TextWriter(void) :
	currentStrNum_(0),
	disCnt_(0.0f)
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
	if (texts_.empty() || currentStrNum_ >= texts_.front().size()) return;

	disCnt_ += 1.0f; // フレームごとにカウントアップ
	if (disCnt_ >= TEXT_SPD)
	{
		disCnt_ = 0.0f;
		currentStrNum_++;
	}
}

// 修正：引数を受け取って指定の座標とフォントで描画するように変更
void TextWriter::Draw(int _x, int _y, int _fontHandle)
{
	if (texts_.empty()) return;

	std::wstring displayStr = texts_.front().substr(0, currentStrNum_);

	if (_fontHandle != -1)
	{
		DrawFormatStringToHandle(_x, _y, UtilityCommon::WHITE, _fontHandle, L"%ls", displayStr.c_str());
	}
	else
	{
		DrawFormatString(_x, _y, UtilityCommon::WHITE, L"%ls", displayStr.c_str());
	}
}

const bool TextWriter::IsEnd(void) const
{
	if (texts_.empty()) return true;
	return currentStrNum_ >= texts_.front().size();
}

void TextWriter::ForceComplete(void)
{
	if (!texts_.empty())
	{
		currentStrNum_ = static_cast<int>(texts_.front().size());
	}
}