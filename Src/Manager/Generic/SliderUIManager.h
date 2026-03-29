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
	void AddSliderUI(const Cursor& _cursor, float _per, Vector2F _leftTopPos, Vector2F _length);

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

	/// @brief 当たり判定がヒットしているUIのイテレータの取得
	/// @param  
	/// @return 当たり判定がヒットしているUIのイテレータ
	std::vector<std::unique_ptr<SliderUIController>>::const_iterator GetIsHitSlider(void)const;

	/// @brief スライダーUIのイテレータの終端位置の取得(当たり判定で何も取得できなかった時に使用する)
	/// @param  
	/// @return スライダーUIのイテレータの終端位置
	std::vector<std::unique_ptr<SliderUIController>>::const_iterator GetSliderUIsEnd(void)const { return sliderUIs_.end(); }
	
	/// @brief スライダー割合の取得
	/// @param  
	/// @return スライダー割合
	const std::vector<float>GetSliderPercent(void);

private:

	//スライダ
	std::vector<std::unique_ptr<SliderUIController>>sliderUIs_;

	int sliderNumCnt_;	//スライダー番号カウント
};

