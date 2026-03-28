#pragma once
#include<memory>
#include<set>
#include"Geometry2D/Geometry2D.h"

class ObjectBase2D;

class Collider2D
{
public:

	//当たり判定タグ
	enum class TAG
	{
		CURSOR,			//カーソル
		DAIMYO,			//大名
		TITLE_MENU,		//タイトルのメニュー
		SLIDER_BUTTON	//スライダーのボタン
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_owner">所有者</param>
	/// <param name="_myTag">自身のタグ</param>
	/// <param name="_geo">形状情報</param>
	/// <param name="_noHitTags">当たらないタグ達</param>
	Collider2D(ObjectBase2D& _owner, const TAG _myTag, std::unique_ptr<Geometry2D>_geo, const std::set<TAG>_noHitTags);

	//デストラクタ
	~Collider2D(void);

	//衝突用タグの取得
	inline const TAG GetTag(void)const { return myTag_; }

	//当たり判定の形状を取得
	inline Geometry2D& GetGeometry(void)const { return *geometry_; }

	//衝突させないタグの取得
	inline const std::set<TAG>& GetNoHitTags(void)const { return noHitTags_; }

	//所持者を取得
	inline const ObjectBase2D& GetOwner(void)const { return owner_; }

	//当たったかの判定の取得
	inline const bool IsHit(void)const { return isHit_; }

	//当たってない
	inline void NotHit(void) { isHit_ = false; }

	//終了判定の取得
	inline const bool IsDead(void)const { return isDead_; }

	//終了処理(所持者の解放時に置く)
	inline void Kill(void) { isDead_ = true; };

	//当たった処理
	void OnHit(std::weak_ptr<Collider2D> _partner);
	
private:

	//持ち主
	ObjectBase2D& owner_;

	//タグ
	TAG myTag_;

	//判定しないタグ
	std::set<TAG> noHitTags_;

	//形状情報
	std::unique_ptr<Geometry2D> geometry_;

	//当たったかどうか
	bool isHit_;

	//終了判定
	bool isDead_;
};

