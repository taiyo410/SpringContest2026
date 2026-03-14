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
	/// @param  
	void Draw(void);
private:

	static constexpr float TEXT_SPD = 0.001f;

	//一連の流れで描画するテキスト配列
	std::list<std::wstring>texts_;

	//現在描画中のテキスト
	std::wstring disPlayStr_;

	//テキスト間隔カウント
	float disCnt_;

	//現在描画中の文字数
	int currentStrNum_;

	
};

