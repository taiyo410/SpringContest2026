#pragma once
#include "../CharactorBase.h"

// @brief プレイヤー（大目付）クラス
// @details
// 江戸の資金を管理し、ゲーム内で操作されるキャラクター
class Player : public CharactorBase
{
public:
    // @brief コンストラクタ
    Player(void);

    // @brief デストラクタ
    virtual ~Player(void) override;

    // --- CharactorBase から継承した必須関数 ---

    // @brief リソースの読み込み処理
    virtual void InitLoad(void) override;

    // @brief 初期化処理
    virtual void Init(void) override;

    // @brief 毎フレーム更新処理
    virtual void Update(void) override;

    // @brief 描画処理
    virtual void Draw(void) const override;

    // @brief 江戸の現金を取得する
    long long GetEdoMoney(void) const { return edoMoney_; }

    // @brief 江戸の現金を加算する
    void AddEdoMoney(long long amount) { edoMoney_ += amount; }

    // @brief 江戸の現金を消費する
    // @param amount 消費する金額
    // @return 消費に成功した場合 true
    bool ConsumeEdoMoney(long long amount);

    void AddFrustration(float amount);

    float GetFrustration(void) const { return frustration_; }

    bool IsGameOver(void) const { return frustration_ >= 100.0f; }

private:
    static constexpr int FONT_SIZE_MONEY = 40;   // フォントサイズ

    long long edoMoney_;    // 江戸の現金（long longで大きな桁に対応）

    float frustration_;

};
