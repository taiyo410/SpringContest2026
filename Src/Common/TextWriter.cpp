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

void TextWriter::Draw(void)
{
	DrawFormatString(0, 0, UtilityCommon::WHITE, texts_.front().substr(0, currentStrNum_).c_str());
}
