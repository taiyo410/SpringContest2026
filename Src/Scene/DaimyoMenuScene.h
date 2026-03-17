#pragma once
#include "SceneBase.h"

class DaimyoMenuScene : public SceneBase
{
public:

	//コンストラクタ
	DaimyoMenuScene(void);

	//デストラクタ
	~DaimyoMenuScene(void)override;

	//読み込み処理
	void Load(void)override;

	//初期化処理
	void Init(void)override;

private:

	//更新関数
	void LoadingUpdate(void)override;
	void NormalUpdate(void)override;

	//描画関数
	void LoadingDraw(void)override;
	void NormalDraw(void)override;

	//ローディング処理から通常処理へ
	void OnSceneEnter(void)override;

};

