#pragma once
#include "../SceneBase.h"

class Player;
class DaimyoBase;

class SceneGame : public SceneBase
{
public:
	// 制限時間（秒）
	static constexpr float LIMIT_TIME = 999.0f; // 5分

	// コンストラクタ
	SceneGame(void);

	// デストラクタ
	~SceneGame(void) = default;

	// 読み込み
	void Load(void) override;

	// 読み込み終了
	void EndLoad(void) override;

	// 初期化
	void Init(void) override;

	// 更新処理
	void Update(void) override;

	// 描画処理
	void Draw(void) override;

	// 解放処理
	void Release(void) override;

private:

	// 描画(デバック)
	void DrawDebug(void);

	// プレイヤー
	std::shared_ptr<Player> player_;

	// 大名
	std::vector<std::shared_ptr<DaimyoBase>> daimyos_;
};