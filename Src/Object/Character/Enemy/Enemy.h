#pragma once
#include<memory>
#include "../Base/CharacterBase.h"
#include"../../../Common/Vector2.h"

class CardDeck;
class EnemyInput;
class ActionController;
class AnimationController;
class PixelMaterial;
class PixelRenderer;

class Enemy :public CharacterBase
{
public:

	/// @brief コンストラクタ
	/// @param  
	Enemy(void);

	/// @brief デストラクタ
	/// @param  
	~Enemy(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 描画
	/// @param  
	void Draw(void) override;

	/// @brief 2D描画
	/// @param  
	void Draw2D(void) override;

	/// <summary>
	/// オブジェクトに当たった時の処理
	/// </summary>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	//コライダ作成
	void MakeColliderGeometry(void)override;

	//通常更新
	void UpdateNormal(void)override;
#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

};

