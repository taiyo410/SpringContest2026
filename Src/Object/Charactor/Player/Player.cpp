#include "../../../Pch.h"
#include "../../../DrawUI/Font.h"
#include "Player.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/InputManager.h"



Player::Player(void)
    : edoMoney_(0)
    , frustration_(0.0f)
    , CharactorBase()
{
}

Player::~Player(void)
{
}

void Player::InitLoad(void)
{
    // プレイヤー関連の画像が必要ならここでロード

   
}

void Player::Init(void)
{
    // ActorBase::Init() や CharactorBase::Init() を呼び出す
    // transform_.pos 等の初期化

    // 初期資金の設定（例：5000万両）
    edoMoney_ = 50000000;
    frustration_ = 0.0f;
}

void Player::Update(void)
{
    // CharactorBaseの基本更新（アニメーション等）
    CharactorBase::Update();

    // プレイヤー自身の入力検知や状態更新があればここに記述
}

void Player::Draw(void) const
{
    CharactorBase::Draw();
    std::string moneyText = (const char*)"江戸の現金: " + std::to_string(edoMoney_) + (const char*)" 円";

    Font::GetInstance().DrawDefaultText(
        1300, 50,
        moneyText.c_str(),
        GetColor(255, 255, 0),
        FONT_SIZE_MONEY,
        Font::FONT_TYPE_ANTIALIASING_EDGE
    );

    // --- 天下の不満度ゲージ (左端・特大縦型・文字内包) ---
    int barW = 60;
    int barH = 900;
    int barX = 40;
    int barY = (1080 - barH) / 2;

    // 1. 背景（重厚な黒紫）
    DrawBox(barX, barY, barX + barW, barY + barH, GetColor(10, 0, 20), TRUE);

    // 2. ゲージ本体（下から上へ）
    float ratio = frustration_ / 100.0f;
    int currentHeight = static_cast<int>(barH * ratio);

    unsigned int purpleColor = (frustration_ >= 80.0f) ? GetColor(220, 0, 180) : GetColor(140, 0, 255);
    DrawBox(barX, (barY + barH) - currentHeight, barX + barW, barY + barH, purpleColor, TRUE);

    // 3. 枠線
    DrawBox(barX, barY, barX + barW, barY + barH, GetColor(180, 180, 180), FALSE);

    // 4. 【修正】ゲージの中に文字を縦に配置
    const char* labels[] = { "不", "満", "度" };
    int fontSize = 35; // ゲージ幅60に対して余裕のあるサイズ
    int startTextY = barY + 30; // ゲージの上端から少し下
    int stepY = 50; // 文字ごとの間隔

    for (int i = 0; i < 3; i++)
    {
        // 文字を中央に寄せるためのオフセット計算
        int textX = barX + (barW / 2) - (fontSize / 2);
        int textY = startTextY + (i * stepY);

        // 文字の視認性を上げるために、液体の色と重なっても見えるよう影（フチ）をつける
        Font::GetInstance().DrawDefaultText(textX + 2, textY + 2, labels[i], GetColor(0, 0, 0), fontSize);
        Font::GetInstance().DrawDefaultText(textX, textY, labels[i], GetColor(255, 255, 255), fontSize);
    }

    // 5. 数値表示 (ゲージのすぐ右側に大きく)
    std::string fStr = std::to_string((int)frustration_) + "%";
    unsigned int textColor = (frustration_ >= 80.0f) ? GetColor(255, 50, 50) : GetColor(255, 255, 255);
    Font::GetInstance().DrawDefaultText(barX + barW + 15, (barY + barH) - currentHeight - 25, fStr.c_str(), textColor, 48);
}

bool Player::ConsumeEdoMoney(long long amount)
{
    if (edoMoney_ >= amount)
    {
        edoMoney_ -= amount;
        return true;
    }
    // 資金不足
    return false;
}

void Player::AddFrustration(float amount)
{
    frustration_ += amount;
    if (frustration_ > 100.0f) frustration_ = 100.0f;
    if (frustration_ < 0.0f)   frustration_ = 0.0f;
}
