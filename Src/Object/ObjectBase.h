#pragma once
#include "Common/Transform.h"

class ColliderBase;

class ObjectBase
{
public:

	// コンストラクタ
	ObjectBase(void);

	// デストラクタ
	virtual ~ObjectBase(void);

	// 初期化処理
	virtual void Init(void);

	// 更新処理
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) const;

	// 解放処理
	virtual void Release(void);

	// 座標・回転・大きさの情報取得(固定)
	const Transform& GetTransform(void) const { return trans_; }

	// 座標・回転・大きさの情報取得(変動)
	Transform& GetTransform(void) {	return trans_;}

	// 座標の取得(固定)
	const VECTOR& GetPos(void) const { return trans_.pos; }

	// 座標の設定
	void SetPos(const VECTOR& pos);

	// 角度の取得
	const VECTOR& GetAngle(void) const { return trans_.rot; }

	// 角度の設定
	void SetAngle(void);
	
	const std::map<int, ColliderBase*>& GetOwnColliders(void) const { return ownColliders_; }
	
	const ColliderBase* GetOwnCollider(int key) const;

	void AddHitCollider(const ColliderBase* hitCollider);

	void ClearHitCollider(void);

protected:

	// 基本情報
	Transform trans_;

	// 衝突判定関連
	std::map<int, ColliderBase*> ownColliders_;
	std::vector<const ColliderBase*> hitColliders_;

	// リソースのロード
	virtual void InitLoad(void);

	// 座標・回転・大きさの初期化
	virtual void InitTransform(void){}

	// 衝突判定の初期化
	virtual void InitCollider(void){}

};

