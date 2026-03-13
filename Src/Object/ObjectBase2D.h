#pragma once
#include"../Common/Vector2F.h"

class Collider2D;

class ObjectBase2D
{
public:

	//コンストラクタ
	ObjectBase2D(void);

	//デストラクタ
	virtual ~ObjectBase2D(void);

	//読み込み(ループ中一度のみ)
	virtual void Load(void) = 0;

	//初期化
	virtual void Init(void) = 0;

	//更新
	virtual void Update(void) = 0;

	//描画
	virtual void Draw(void) = 0;

	//解放
	virtual void Release(void) = 0;

	//ヒット処理
	virtual void OnHit(const std::weak_ptr<Collider2D> _partner);

	//当たり判定の削除
	void Sweep(void);

	//座標の取得
	const Vector2F& GetPos(void) { return pos_; };

protected:

	//画像ID
	int imageId_;

	//座標
	Vector2F pos_;

	//コライダ
	std::vector<std::shared_ptr<Collider2D>> colliders_;

	/// <summary>
	/// コライダ作成
	/// </summary>
	/// <param name="_tag">自身のタグ</param>
	/// <param name="_geo">形状情報</param>
	/// <param name="_noHitTags">当たり判定をしないタグたち</param>
	void MakeCollider(Collider2D::TAG _tag, std::unique_ptr<Geometry2D> _geo, std::set<Collider2D::TAG> _noHitTags);

	//該当タグで当たり判定削除
	void DeleteColliderAtTag(Collider2D::TAG _tag);

	//全コライダの削除
	void DeleteAllColliders(void);
};

