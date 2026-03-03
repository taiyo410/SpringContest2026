#pragma once
#include <map>
#include <functional>
#include <memory>
#include "../Base/CharacterBase.h"

#define DEBUG_ON
class AnimationController;
class InputController;
class Camera;
class Geometry;
class ModelMaterial;
class ModelRenderer;

class EffectController;

class Player :public CharacterBase
{
public:

	/// @brief コンストラクタ
	/// @param  
	Player(void);

	// デストラクタ
	~Player(void)override;
	
	/// @brief 読み込み
	/// @param  
	void Load(void)override;
	
	/// @brief 初期化
	/// @param  
	void Init(void)override;
	
	/// @brief 描画
	/// @param  
	void Draw(void)override;

	/// @brief 2D関連の描画
	/// @param  
	void Draw2D(void)override;

	/// @brief 当たった時の処理
	/// @param _hitCol 相手のコライダ
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;


private:

	//***********************************
	//アニメーション関連
	//***********************************

#ifdef _DEBUG
	//デバッグ描画
	void DrawDebug(void);
#endif // _DEBUG

	/// @brief コライダの形状を作成
	/// @param  
	void MakeColliderGeometry(void)override;

	//演出時の更新
	void UpdateNormal(void)override;
};

