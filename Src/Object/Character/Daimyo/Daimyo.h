#pragma once
#include "../Base/CharacterBase2D.h"
#include "DaimyoImport.h"

class Daimyo : public CharacterBase2D
{
public:

	//コンストラクタ
	Daimyo(const DaimyoImport _import);

	//デストラクタ
	~Daimyo(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//ヒット処理
	void OnHit(const std::weak_ptr<Collider2D> _partner)override;

private:

	//インポート情報
	DaimyoImport import_;
};

