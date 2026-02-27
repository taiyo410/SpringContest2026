#include "../../Pch.h"
#include "SceneScore.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Decoration/SoundManager.h"
#include "../../Manager/System/Loading.h"
#include "SceneTitle.h"
#include "../../Manager/System/TimeManager.h"
#include "../../Manager/System/DataManager.h"
#include "../../Application.h"
#include "../../DrawUI/Font.h"

SceneScore::SceneScore(void)
{
}

void SceneScore::Load(void)
{
    SceneBase::Load();
    // 時間カウントリセット
    TimeManager::GetInstance().Reset();

    ResourceManager::GetInstance().InitGameClear();

    Loading::GetInstance()->SetProgress(25.0f);

    //SoundManager::GetInstance().Add(SoundManager::TYPE::BGM, SoundManager::SOUND::BGM_SCORE, ResourceManager::GetInstance().Load(ResourceManager::SRC::BGM_SCORE).handleId_);

    Loading::GetInstance()->SetProgress(45.0f);

    Loading::GetInstance()->SetProgress(60.0f);

    Loading::GetInstance()->SetProgress(80.0f);

    Loading::GetInstance()->SetProgress(100.0f);

    // サウンドのリソース読み込み
    
}

void SceneScore::EndLoad(void)
{
    SceneBase::EndLoad();
}

void SceneScore::Init(void)
{
    // サウンド
    auto& res = ResourceManager::GetInstance();

    // 初期BGM
    //SoundManager::GetInstance().Play(SoundManager::SOUND::BGM_SCORE);

}

void SceneScore::Update(void)
{
    auto& sound = SoundManager::GetInstance();
    auto& input = InputManager::GetInstance();
    auto& sceneMgr = SceneManager::GetInstance();

    if (input.IsClickMouseLeft())
    {
        // 決定音
        sound.Play(SoundManager::SOUND::SE_PUSH);
        // BGM停止
        sound.Stop(SoundManager::SOUND::BGM_TITLE);

        auto newScene = std::make_shared<SceneTitle>();
        sceneMgr.ChangeScene(newScene);

        return;
    }
}

void SceneScore::Draw(void)
{
    const int SCREEN_W = Application::SCREEN_SIZE_X;
    const int SCREEN_H = Application::SCREEN_SIZE_Y;

    // 中央から少し左に基準点をずらす（-100の部分を調整して位置を決めてください）
    const int BASE_X = (SCREEN_W / 2) - 100;
    const int CENTER_Y = SCREEN_H / 2;

    // 背景
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
    DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int finalYear = DataManager::GetInstance().GetFinalRecord();

    // --- 1. 年代の表示（BASE_X基準で中央寄せ） ---
    unsigned int goldColor = GetColor(255, 215, 0);
    std::string yearText = "西暦 " + std::to_string(finalYear) + " 年";
    std::string periodText = "（寛永より続く幕府の記録）";

    int yearW = Font::GetInstance().GetDefaultTextWidth(yearText.c_str());
    Font::GetInstance().DrawDefaultText(BASE_X - (yearW / 2), CENTER_Y - 200, yearText.c_str(), goldColor, 64);

    int periodW = Font::GetInstance().GetDefaultTextWidth(periodText.c_str());
    Font::GetInstance().DrawDefaultText(BASE_X - (periodW / 2), CENTER_Y - 130, periodText.c_str(), GetColor(200, 200, 200), 24);

    // --- 2. 結末の表示 ---
    std::string resultMsg = (finalYear <= 1867) ?
        "諸大名の反乱によって、徳川の治世は終焉を迎えた。" :
        "幕府は荒波を乗り越え、新たなる時代へとその血脈を繋いだ。";

    unsigned int msgColor = (finalYear <= 1867) ? GetColor(255, 50, 50) : GetColor(100, 255, 100);

    int msgW = Font::GetInstance().GetDefaultTextWidth(resultMsg.c_str());
    Font::GetInstance().DrawDefaultText(BASE_X - (msgW / 2), CENTER_Y, resultMsg.c_str(), msgColor, 36);

    // --- 3. サブメッセージ ---
    if (finalYear <= 1867) {
        std::string subMsg = "天下の不満を抑えきれず、泰平の世は潰え去ったのである...";
        int subW = Font::GetInstance().GetDefaultTextWidth(subMsg.c_str());
        Font::GetInstance().DrawDefaultText(BASE_X - (subW / 2), CENTER_Y + 60, subMsg.c_str(), GetColor(180, 180, 180), 24);
    }

    // 操作説明
    const char* pushSpace = "左クリックでタイトルへ";
    int pushW = Font::GetInstance().GetDefaultTextWidth(pushSpace);
    // 操作説明だけは画面の本当の中央（SCREEN_W / 2）でもいいかもしれません
    Font::GetInstance().DrawDefaultText((SCREEN_W / 2) - (pushW / 2), SCREEN_H - 100, pushSpace, GetColor(120, 120, 120), 24);
}

void SceneScore::Release(void)
{
}

void SceneScore::DrawDebug(void)
{
#ifdef _DEBUG
#endif
}