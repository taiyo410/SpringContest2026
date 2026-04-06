#pragma once
#include"Daimyo.h"

class DaimyoOnHit
{
public:

	//コンストラクタ
	DaimyoOnHit(Daimyo& _parent);

	//デストラクタ
	~DaimyoOnHit(void);

	//当たった処理
	void OnHit(const std::weak_ptr<Collider2D> _partner);

private:

	SoundManager& soundMng_;

	//所有者
	Daimyo& parent_;

	//当たり判定管理
	std::unordered_map<Collider2D::TAG, std::function<void(std::weak_ptr<Collider2D>)>> hit_;

	//当たり判定
	void HitCursor(const std::weak_ptr<Collider2D> _partner);
};

