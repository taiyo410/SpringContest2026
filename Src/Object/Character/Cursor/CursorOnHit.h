#pragma once
#include<unordered_map>
#include<functional>
#include"Cursor.h"

class CursorOnHit
{
public:

	//コンストラクタ
	CursorOnHit(Cursor& _parent);
	
	//デストラクタ
	~CursorOnHit(void);

	//ヒット処理
	void OnHit(const std::weak_ptr<Collider2D> _partner);

private:

	//所有者
	Cursor& parent_;

	//当たり判定管理
	std::unordered_map<Collider2D::TAG, std::function<void(std::weak_ptr<Collider2D>)>> hit_;

	//対象ごとの当たり判定
	void HitDaimyo(const std::weak_ptr<Collider2D> _partner);
	void HitDaimyoAlternate(const std::weak_ptr<Collider2D> _partner);
	void HitDaimyoEnhancement(const std::weak_ptr<Collider2D> _partner);
	void HitDaimyoDetails(const std::weak_ptr<Collider2D> _partner);
	void HitDissatisfactionBar(const std::weak_ptr<Collider2D> _partner);
};