#include "../../../Pch.h"
#include "DaimyoBase.h"
#include "../../../Manager/System/TimeManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../DrawUI/Font.h"
#include "../../../Application.h"

// 静的変数の実体化（最初は誰も選択していない）
bool DaimyoBase::IsAnySelecting = false;

DaimyoBase::DaimyoBase(void)
    : money_(0)
    , accumulationTimer_(0.0f)
    , color_(GetColor(255, 255, 255))
    , sankinState_(SankinState::Normal)
    , sankinTimer_(0.0f)
    , selectedRoute_(0)
    , accumulationSpeed_(1.0f)
    , resultMessage_("")
    , lastGain_(0)
    , resultDisplayTimer_(0.0f)
    , isGainTransferred_(false)
    , isRequestingHelp_(false)
    , helpTimer_(0.0f)
    , nextEventTimer_(0.0f)
    , helpFailureFlag_(false)
    , helpSuccessFlag_(false)
{
}

DaimyoBase::~DaimyoBase(void) {}

void DaimyoBase::InitLoad(void)
{
    // 将来的に画像を使う場合はここで LoadGraph などを行う
}

void DaimyoBase::Init(void)
{
    money_ = 0;
    accumulationTimer_ = 0.0f;
    sankinState_ = SankinState::Normal;
    sankinTimer_ = 0.0f;
    IsAnySelecting = false;
}

void DaimyoBase::Update(void)
{
    float deltaTime = TimeManager::GetInstance().GetDeltaTime();
    InputManager& input = InputManager::GetInstance();
    Vector2 mPos = input.GetMousePos();
    bool isClick = input.IsTrgMouseLeft(); // 押しっぱなし防止のクリック判定

    // --- 1. 援助イベントの発生判定 (蓄財中のみ) ---
    if (sankinState_ == SankinState::Normal && !isRequestingHelp_)
    {
        nextEventTimer_ -= deltaTime;
        if (nextEventTimer_ <= 0.0f) 
        {
            // ランダムに発生 (例: 10% の確率)
            if (GetRand(100) < 20)
            {
                isRequestingHelp_ = true;
                helpTimer_ = 0.0f;
            }
            nextEventTimer_ = 10.0f + static_cast<float>(GetRand(10));
        }
    }

    // 援助要請中の処理
    if (isRequestingHelp_) 
    {
        helpTimer_ += deltaTime;

        // マウスが重なっているか
        bool isHover = (mPos.x >= trans_.pos.x && mPos.x <= trans_.pos.x + 64 &&
            mPos.y >= trans_.pos.y && mPos.y <= trans_.pos.y + 64);

        // 成功
        if (isHover && input.IsTrgMouseRight())
        {
            helpSuccessFlag_ = true;
            isRequestingHelp_ = false;
            helpTimer_ = 0.0f;
        }
        // 失敗
        else if (helpTimer_ >= HELP_LIMIT) 
        {
            isRequestingHelp_ = false;
            helpFailureFlag_ = true;
        }
    }

    // 1. 通常状態（お金が貯まる）
    if (sankinState_ == SankinState::Normal)
    {
        accumulationTimer_ += deltaTime * accumulationSpeed_;
        if (accumulationTimer_ >= ACCUMULATION_INTERVAL)
        {
            money_ += ADD_AMOUNT;
            accumulationTimer_ -= ACCUMULATION_INTERVAL;
        }

        // 参勤交代開始判定：1000万以上 且つ 他の大名が選択中でない 且つ クリック
        if (!IsAnySelecting && money_ >= 10000000 && isClick)
        {
            int castleSize = 64;
            if (mPos.x >= trans_.pos.x && mPos.x <= trans_.pos.x + castleSize &&
                mPos.y >= trans_.pos.y && mPos.y <= trans_.pos.y + castleSize)
            {
                sankinState_ = SankinState::Selecting;
                IsAnySelecting = true; // 他の大名のクリックをブロック
            }
        }
    }
    // 2. ルート選択中
    else if (sankinState_ == SankinState::Selecting)
    {
        if (isClick)
        {
            for (int i = 0; i < 3; i++)
            {
                // routeRectsに保存されたボタンの範囲内かチェック
                if (mPos.x >= routeRects[i].x && mPos.x <= routeRects[i].x + routeRects[i].w &&
                    mPos.y >= routeRects[i].y && mPos.y <= routeRects[i].y + routeRects[i].h)
                {
                    if (i == 0) StartSankin(1, 10.0f); // 普通
                    if (i == 1) StartSankin(2, 5.0f);  // 危険
                    if (i == 2) StartSankin(3, 15.0f); // 安全

                    IsAnySelecting = false; // 選択完了したのでブロック解除
                    break;
                }
            }
        }
    }
    // 3. 参勤交代中（江戸へ移動中）
    else if (sankinState_ == SankinState::Traveling)
    {
        sankinTimer_ -= deltaTime;
        if (sankinTimer_ <= 0)
        {
            FinishSankin();
        }
    }
    else if (sankinState_ == SankinState::Result) 
    {
        resultDisplayTimer_ -= deltaTime;
        if (resultDisplayTimer_ <= 0) 
        {
            sankinState_ = SankinState::Normal;
            isGainTransferred_ = false; 
            lastGain_ = 0;              
        }
    }

    CharactorBase::Update();
}

void DaimyoBase::Draw(void) const
{
    CharactorBase::Draw();
    int x = static_cast<int>(trans_.pos.x);
    int y = static_cast<int>(trans_.pos.y);
    int castleSize = 64;

    // --- お城と蓄財ゲージの描画 ---
    DrawBox(x, y, x + castleSize, y + castleSize, color_, TRUE);

    int gaugeWidth = 120; int gaugeHeight = 15;
    int gaugeX = x - (gaugeWidth - castleSize) / 2;
    int gaugeY = y - 25;

    DrawBox(gaugeX - 2, gaugeY - 2, gaugeX + gaugeWidth + 2, gaugeY + gaugeHeight + 2, GetColor(50, 50, 50), TRUE);
    DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(0, 0, 0), TRUE);

    float ratio = static_cast<float>(money_) / REBELLION_THRESHOLD;
    if (ratio > 1.0f) ratio = 1.0f;
    unsigned int gaugeColor = (IsRebellion()) ? GetColor(255, 0, 0) : GetColor(255, 255, 0);
    DrawBox(gaugeX, gaugeY, gaugeX + static_cast<int>(gaugeWidth * ratio), gaugeY + gaugeHeight, gaugeColor, TRUE);

    std::string moneyStr = std::to_string(money_ / 10000) + " / 6000man";
    Font::GetInstance().DrawDefaultText(gaugeX + 5, gaugeY - 20, moneyStr.c_str(), GetColor(255, 255, 255), 18);

    if (sankinState_ == SankinState::Selecting)
    {
        // 画面サイズ 1920x1080 の中央を計算
        int screenCenterX = Application::SCREEN_SIZE_X / 2;
        int screenCenterY = Application::SCREEN_SIZE_Y / 2;

        int uiW = 600;  // 横幅を大きく
        int uiH = 80;   // 1項目の高さ
        int spacing = 20; // 項目間の隙間
        int totalH = (uiH * 3) + (spacing * 2);

        int startX = screenCenterX - (uiW / 2);
        int startY = screenCenterY - (totalH / 2);

        const char* titles[] = { "普通の道 (10秒)", "危険な近道 (5秒)", "安全な道 (15秒)" };

        for (int i = 0; i < 3; i++)
        {
            int ry = startY + (i * (uiH + spacing));

            // 外枠と背景
            DrawBox(startX, ry, startX + uiW, ry + uiH, GetColor(30, 30, 30), TRUE);  // 暗いグレー
            DrawBox(startX, ry, startX + uiW, ry + uiH, GetColor(255, 255, 255), FALSE); // 白枠

            // マウスオーバー時に色を変える演出を入れるとより「ボタン」らしくなります
            Vector2 mPos = InputManager::GetInstance().GetMousePos();
            if (mPos.x >= startX && mPos.x <= startX + uiW && mPos.y >= ry && mPos.y <= ry + uiH) {
                DrawBox(startX + 2, ry + 2, startX + uiW - 2, ry + uiH - 2, GetColor(100, 100, 0), FALSE);
            }

            // テキストを中央寄せで描画
            int textW = Font::GetInstance().GetDefaultTextWidth(titles[i]);
            Font::GetInstance().DrawDefaultText(
                startX + (uiW / 2) - (textW / 2),
                ry + (uiH / 2) - 15,
                titles[i], GetColor(255, 255, 255), 32 // フォントサイズを32に大きく
            );

            // クリック判定用の座標を保存
            routeRects[i] = { startX, ry, uiW, uiH };
        }
    }

    // --- 移動中のカウントダウン表示 ---
    if (sankinState_ == SankinState::Traveling)
    {
        std::string t = "Sankin: " + std::to_string((int)sankinTimer_ + 1) + "s";
        Font::GetInstance().DrawDefaultText(x, y - 50, t.c_str(), GetColor(0, 255, 255), 20);
    }

    if (sankinState_ == SankinState::Result)
    {
        int resW = 800;  // リザルトはさらに大きく
        int resH = 400;
        int rx = (Application::SCREEN_SIZE_X - resW) / 2;
        int ry = (Application::SCREEN_SIZE_Y - resH) / 2;

        // 背景（豪華に金枠など）
        DrawBox(rx, ry, rx + resW, ry + resH, GetColor(10, 10, 20), TRUE);
        DrawBox(rx, ry, rx + resW, ry + resH, GetColor(255, 200, 0), FALSE); // 金色の枠

        // メッセージ表示（改行対応が必要な場合はDrawFormatString等を使うか、分割して描画）
        // \n を含んでいるので DrawDefaultText が対応していればそのまま、してなければ分割します
        Font::GetInstance().DrawDefaultText(rx + 40, ry + 60, resultMessage_.c_str(), GetColor(255, 255, 255), 36);

        // 入手金額をド派手に
        std::string gainStr = "入手金額: " + std::to_string(lastGain_) + " 円";
        unsigned int moneyColor = (lastGain_ == 0) ? GetColor(255, 0, 0) : GetColor(0, 255, 0);

        int moneyW = Font::GetInstance().GetDefaultTextWidth(gainStr.c_str());
        Font::GetInstance().DrawDefaultText(
            rx + (resW / 2) - (moneyW / 2),
            ry + 280,
            gainStr.c_str(), moneyColor, 50 // 金額は特大サイズ
        );
    }

    if (isRequestingHelp_) 
    {
        // 赤色ゲージ (20秒で満タンになる)
        int barW = 80;
        int barH = 12;
        int barX = x + (64 / 2) - (barW / 2);
        int barY = y + 70; // お城(64px)の少し下

        // 背景
        DrawBox(barX, barY, barX + barW, barY + barH, GetColor(60, 0, 0), TRUE);

        // 赤ゲージ
        float ratio = helpTimer_ / HELP_LIMIT;
        DrawBox(barX, barY, barX + static_cast<int>(barW * ratio), barY + barH, GetColor(255, 0, 0), TRUE);

        // 枠とテキスト
        DrawBox(barX, barY, barX + barW, barY + barH, GetColor(255, 255, 255), FALSE);
    }
}

void DaimyoBase::StartSankin(int route, float duration)
{
    selectedRoute_ = route;
    sankinTimer_ = duration;
    sankinState_ = SankinState::Traveling;
}

void DaimyoBase::FinishSankin()
{
    int dice = GetRand(100);
    resultMessage_ = "";

    if (selectedRoute_ == 1) 
    { // 普通の道
        if (dice < 30) {
            resultMessage_ = "川が氾濫した！\n荷物の一部が流された...";
            lastGain_ = 2500000;
        }
        else 
        {
            resultMessage_ = "無事に到着した。\n予定通りの献上だ。";
            lastGain_ = 5000000;
        }
    }
    else if (selectedRoute_ == 2) 
    { // 危険な近道
        if (dice < 50) 
        {
            resultMessage_ = "山賊に襲われた！\nお金を全損した...";
            lastGain_ = 0;
        }
        else 
        {
            resultMessage_ = "険しい道を突破した！\nボーナスを受け取った。";
            lastGain_ = 8000000;
        }
    }
    else if (selectedRoute_ == 3)
    { // 安全な道
        resultMessage_ = "非常に穏やかな旅だった。\n皆無事に到着した。";
        lastGain_ = 3000000;
    }

    // プレイヤーにお金を渡す (SceneGame経由で呼ぶのが理想ですが、一旦概念として)
    // player->AddEdoMoney(lastGain_); 

    money_ -= SANKIN_COST;
    sankinState_ = SankinState::Result; // 結果表示モードへ
    resultDisplayTimer_ = 3.0f;         // 3秒間表示する
}

void DaimyoBase::ReduceMoney(long long amount)
{
    money_ -= amount;
    if (money_ < 0) money_ = 0;
}

long long DaimyoBase::TransferMoney(void)
{
    isGainTransferred_ = true; // 送金済みフラグを立てる
    return lastGain_;          // 金額を返すが、0にはしない（表示用）
}

bool DaimyoBase::popHelpSuccess(void)
{
    if (helpSuccessFlag_) { helpSuccessFlag_ = false; return true; } return false;
}

bool DaimyoBase::popHelpFailure(void)
{
    if (helpFailureFlag_) { helpFailureFlag_ = false; return true; } return false;
}
