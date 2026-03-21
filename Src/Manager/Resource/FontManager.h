#pragma once
#include <string>
#include <unordered_map>
#include "../Template/Singleton.h"

class FontManager
{

public:

	/// @brief コンストラクタ
	/// @param  
	FontManager(void);

	/// @brief デストラクタ
	/// @param  
	~FontManager(void) = default;

	enum class FONT_TYPE
	{
		DOT,
		BOKUTATI,
		APRIL_GOTHIC_ONE,
		MAX
	};


	//フォント種類
	static constexpr int FONT_TYPES = static_cast<int>(FONT_TYPE::MAX);

	//フォントネーム(cppで設定。プロパティ→詳細のタイトルを入れること)
	static constexpr wchar_t FONT_DOT[] = L"ベストテンDOT";
	static constexpr wchar_t FONT_BOKUTATI[] = L"ぼくたちのゴシック";
	static constexpr wchar_t FONT_APRIL_GOTHIC[] = L"April Gothic one Regular";

	//const std::wstring FontManager::FONT_DOT = L"ベストテンDOT";
	//const std::wstring FontManager::FONT_BOKUTATI = L"ぼくたちのゴシック";
	//const std::wstring FontManager::FONT_APRIL_GOTHIC = L"April Gothic one Regular";


	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 解放処理
	/// @param  
	void Release(void);

private:

	//登録用パス
	std::wstring fontPath_[FONT_TYPES];

};

