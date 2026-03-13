#pragma once
#include "../Base/CharacterBase2D.h"

class Daimyo : public CharacterBase2D
{
public:

	//コンストラクタ
	Daimyo(void);

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

protected:
};

