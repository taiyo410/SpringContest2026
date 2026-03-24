#pragma once
#include "../../ObjectBase2D.h"

class CharacterBase2D : public ObjectBase2D
{
public:

	//コンストラクタ
	CharacterBase2D(void);
	
	//デストラクタ
	virtual ~CharacterBase2D(void)override;

	//読み込み
	virtual void Load(void)override;

	//初期化
	virtual void Init(void)override;

	//更新
	virtual void Update(void)override;

	//描画
	virtual void Draw(void)override;

	//解放
	virtual void Release(void)override;

	//ヒット処理
	virtual void OnHit(const std::weak_ptr<Collider2D> _partner)override;

protected:
};

