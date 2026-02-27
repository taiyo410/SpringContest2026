#include "../../Pch.h"
#include "PauseScene.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/InputManager.h"

PauseScene::PauseScene(void)
{
    pauseMenu_ = std::make_unique<PauseMenu>();
}

PauseScene::~PauseScene(void)
{
    Release();
}

void PauseScene::Init(void)
{
    SceneBase::Init();
    // シーンが始まったらメニューを表示状態にする
    if (pauseMenu_) {
        pauseMenu_->Show();
    }
}

void PauseScene::Update(void)
{
    if (pauseMenu_) {
        pauseMenu_->Update();

        // メニューが非表示になった（「続ける」が押された等）なら、このシーンを終わらせる
        if (!pauseMenu_->IsVisible()) {
            SceneManager::GetInstance().PopScene();
        }
    }

    // ESCキーでも戻れるようにする場合
    if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_ESCAPE)) {
        SceneManager::GetInstance().PopScene();
    }
}

void PauseScene::Draw(void)
{
    // 背景を暗くする演出（これがあるとポーズっぽくなります）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // メニュー本体の描画
    if (pauseMenu_) {
        pauseMenu_->Draw();
    }
}

void PauseScene::Release(void)
{
    if (pauseMenu_) {
        pauseMenu_->Release();
    }
}

void PauseScene::Load(void) { SceneBase::Load(); }
void PauseScene::EndLoad(void) { SceneBase::EndLoad(); }