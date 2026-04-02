#pragma once
#include <list>
#include <string>
class TextWriter
{
public:

	/// @brief コンストラクタ
	/// @param  
	TextWriter(void);

	/// @brief デストラクタ
	/// @param  
	~TextWriter(void);

	/// @brief 使用するテキストの追加
	/// @param _text 
	void AddText(const std::wstring _text);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param _x 描画するX座標
	/// @param _y 描画するY座標
	/// @param _fontHandle フォントハンドル(-1でデフォルト)
	void Draw(int _x = 0, int _y = 0, int _fontHandle = -1);

	/// @brief テキスト表示が完了したか
	/// @return true: 完了している
	const bool IsEnd(void) const;

	/// @brief テキストを強制的に全表示させる
	void ForceComplete(void); 

private:

	static constexpr int TEXT_SPD = 3.0f;

	//一連の流れで描画するテキスト配列
	std::list<std::wstring>texts_;

	//現在描画中のテキスト
	std::wstring disPlayStr_;

	//テキスト速度
	int textSpd_;

	//テキスト間隔カウント
	int disCnt_;

	//現在描画中の文字数
	int currentStrNum_;

	
};

