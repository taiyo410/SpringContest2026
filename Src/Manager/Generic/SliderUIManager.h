#pragma once
#include <vector>
#include <memory>
#include "../Common/Vector2F.h"

class Cursor;
class SliderUIController;

class SliderUIManager
{
public:

	/// @brief コンストラクタ
	/// @param  
	SliderUIManager(void);

	/// @brief デストラクタ
	/// @param  
	~SliderUIManager(void);

	/// @brief スライダーUIの追加
	/// @param _cursor カーソルクラス
	/// @param _per 割合の参照
	/// @param _leftTopPos 右上座標 
	/// @param _length スライダーの長さ
	void AddSliderUI(const Cursor& _cursor, float& _per, Vector2F _leftTopPos, Vector2F _length);

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief 解放
	/// @param  
	void Release(void);

private:

	//スライダ
	std::vector<std::unique_ptr<SliderUIController>>sliderUIs_;
};

