#include "../pch.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Common/FontController.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Common/Easing.h"
#include "StartScene.h"

StartScene::StartScene(void)
{
	//更新関数のセット
	updateFunc_ = [this]() {NormalUpdate(); };
	//描画関数のセット
	drawFunc_ = [this]() {NormalDraw(); };

	easing_ = std::make_unique<Easing>();
}

StartScene::~StartScene(void)
{
	DeleteFontToHandle(buttonFontHandle_); //フォントの削除
}

void StartScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}

void StartScene::Load(void)
{
	//フォントの登録
 	buttonFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
}

void StartScene::Init(void)
{

}

void StartScene::NormalUpdate(void)
{
	stringAlpha_ = easing_->EaseFunc(0, 255, blendCnt_ / BLEND_TIME, Easing::EASING_TYPE::LERP_COMEBACK);
	blendCnt_> BLEND_TIME?blendCnt_=0: blendCnt_ += scnMng_.GetDeltaTime();

	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		return;
	}
}

void StartScene::NormalDraw(void)
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xff0000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)stringAlpha_);
	UtilityDraw::DrawStringCenter(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, UtilityCommon::WHITE, buttonFontHandle_, L"Push To Click");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}
