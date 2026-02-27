#pragma once
#include "../SceneBase.h"
#include "../../DrawUI/SceneUI/PauseMenu.h"

/// ポーズ（一時停止）シーン
class PauseScene : public SceneBase
{
public:
    PauseScene(void);
    virtual ~PauseScene(void) override;

    virtual void Init(void) override;
    virtual void Update(void) override;
    virtual void Draw(void) override;
    virtual void Release(void) override;

    // ロード関連（ポーズ画面で重い読み込みはしない想定ですが必須なので実装）
    virtual void Load(void) override;
    virtual void EndLoad(void) override;

private:
    std::unique_ptr<PauseMenu> pauseMenu_;
};