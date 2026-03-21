#pragma once
#include <vector>
#include <string>
class FontController
{
public:

	struct FONT_STATUS
	{
		std::wstring pass = L"";		//フォントパス	
		int size;					//フォントの大きさ
		int thick;					//太さ
		int type;				//フォントタイプ
		int handle = -1;				//フォントハンドル

		bool operator==(const FONT_STATUS& _status)const
		{
			return pass == _status.pass
				&& size == _status.size
				&& thick == _status.thick
				&& type == _status.type;
		}
	};

	/// @brief コンストラクタ
	/// @param  
	FontController(void);

	/// @brief デストラクタ
	/// @param  
	~FontController(void);

	/// @brief フォントハンドルの取得
	/// @param _size フォントの大きさ
	/// @param thick フォントの太さ
	/// @param _fontType フォントタイプ(入力なしでデフォルトのフォントが入っている)
	/// @return 
	const int GetFontHandle(const std::wstring& _fontPass, const int _size, const int thick, const int _fontType = -1);
private:
	//フォントハンドル
	std::vector<FONT_STATUS>fontStatus_;
};

