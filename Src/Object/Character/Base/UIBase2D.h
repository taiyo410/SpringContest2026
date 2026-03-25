#pragma once
#include<memory>
#include "../Common/Vector2F.h"
#include "../Object/ObjectBase2D.h"


class Easing;
class ResourceManager;
class PixelRenderer;
class PixelMaterial;

class UIBase2D:public ObjectBase2D
{
public:

	/// @brief コンストラクタ
	/// @param  
	UIBase2D(void);

	/// @brief デストラクタ
	/// @param  
	virtual ~UIBase2D(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void)override = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void)override = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void)override = 0;

	/// @brief 描画
	/// @param  
	virtual void Draw(void)override = 0;

	/// @brief 解放
	/// @param  
	virtual void Release(void)override = 0;
protected:
	//イージング
	std::unique_ptr<Easing>easing_;
	//レンダラー
	std::unique_ptr<PixelRenderer>renderer_;
	//マテリアル
	std::unique_ptr<PixelMaterial>material_;

	//リソースマネージャ
	ResourceManager& resMng_;
private:
};

