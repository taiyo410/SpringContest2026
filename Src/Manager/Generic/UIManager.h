#pragma once
#include <unordered_map>
#include "../Object/Character/UIData/CharacterUIData.h"
#include "../Common/Vector2F.h"
#include "../Template/Singleton.h"

class DirectionUI;

class UIManager :public Singleton<UIManager>
{
	friend class Singleton<UIManager>;

public:

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 更新
	/// @param 
	void Update(void);
	
	//演出時のUI更新
	void DirectionUpdate(void);

	/// @brief UIの描画
	/// @param  
	void Draw(void);
	
	/// @brief 演出時のUI描画
	/// @param  
	void DirectionDraw(void);
private:

	//フォント
	int fontHandle_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	UIManager(void);

	// デストラクタも同様
	~UIManager(void) = default;

};

