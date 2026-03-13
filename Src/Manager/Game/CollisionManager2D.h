#pragma once

#include"../Object/Common/Collider2D/Collider2D.h"
#include"../Object/ObjectBase2D.h"

class CollisionManager2D
{
public:

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static CollisionManager2D& GetInstance(void);

	//コライダの追加
	void AddCollider(const std::shared_ptr<Collider2D> _collider);

	//必要なくなったコライダの削除(更新の最後に置く)
	void Sweep(void);

	//更新
	void Update(void);

	//解放
	void Destroy(void);

private:

	//静的インスタンス
	static CollisionManager2D* instance_;

	//当たり判定の格納
	std::vector<std::shared_ptr<Collider2D>>colliders_;

	//当たり判定する範囲
	std::map<Collider2D::TAG, float> hitRange_;

	//当たり判定の頻度
	float updateFrequency_;

	//コンストラクタ
	CollisionManager2D(void);
	
	//デストラクタ
	~CollisionManager2D(void);

	//コピー禁止
	CollisionManager2D(CollisionManager2D& _copy) = delete;
	CollisionManager2D& operator= (CollisionManager2D& _copy) = delete;
};

