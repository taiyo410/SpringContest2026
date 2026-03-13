#pragma once
#include "SceneBase.h"

class InputManager;
class InputManagerS;
class Easing;

class StartScene :
    public SceneBase
{
public:

    /// @brief コンストラクタ
    /// @param  
    StartScene(void);

    /// @brief デストラクタ
    /// @param  
    ~StartScene(void)override;

    /// @brief ロード
    /// @param  
    void Load(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void)override;

    //更新関数
    void NormalUpdate(void) override;

    //描画関数
    void NormalDraw(void) override;



private:

    static constexpr float BLEND_TIME = 2.0f;

    std::unique_ptr<Easing>easing_;

    int stringAlpha_;

    float blendCnt_;
    /// @brief シーン遷移後
    /// @param  
    void OnSceneEnter(void)override;

};

