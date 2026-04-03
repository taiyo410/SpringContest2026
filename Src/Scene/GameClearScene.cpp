#include "../pch.h"
#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Manager/Game/CollisionManager2D.h" 
#include "../Object/Character/Cursor/Cursor.h"  
#include "../Common/Easing.h"         // パスを修正
#include "../Common/TextWriter.h"     // パスを修正
#include "../Common/FontController.h" // パスを修正
#include "GameClearScene.h"

GameClearScene::GameClearScene(void) :
	soundMng_(SoundManager::GetInstance())
{

	CollisionManager2D::CreateInstance();
	cursor_ = std::make_unique<Cursor>();

	updateFunc_ = [this]() {LoadingUpdate(); };
	drawFunc_ = [this]() {LoadingDraw(); };
}

GameClearScene::~GameClearScene(void)
{
	soundMng_.Stop(SoundManager::SRC::GAME_CLEAR_BGM);
	CollisionManager2D::GetInstance().Destroy();

	GameRuleManager::GetInstance().Destroy();		//ゲームシーンで作ったゲームルールマネージャをここで消す
}

void GameClearScene::Load(void)
{
	easing_ = std::make_unique<Easing>();

	buttonFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
	gameClearFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, 80, 0);
	messageFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_BOKUTATI, 40, 0);

	imgGameClear_ = resMng_.GetInstance().Load(ResourceManager::SRC::GAME_CLEAR_IMG).handleId_;

	soundMng_.LoadResource(SoundManager::SRC::GAME_CLEAR_BGM);
	soundMng_.LoadResource(SoundManager::SRC::MOVE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::DESIDE_BTN_SE);

	textWriter_ = std::make_unique<TextWriter>();
	cursor_->Load();
}

void GameClearScene::Init(void)
{
	soundMng_.Play(SoundManager::SRC::GAME_CLEAR_BGM, SoundManager::PLAYTYPE::LOOP);
	SoundManager::GetInstance().SetSystemVolume(BGM_VOL, static_cast<int>(SoundManager::TYPE::BGM));

	textEndTimer_ = 0.0f;
	imgAlpha_ = 255;
	isAnimationEnd_ = false;
	oldMouseClick_ = false;

	// マウス座標を整数(int)で取得してVector2に格納
	int mx, my;
	GetMousePoint(&mx, &my);
	lastMousePos_ = { mx, my };

	elapsedYear_ = GameRuleManager::GetInstance().GetElapsedYear();
	currentYear_ = GameRuleManager::START_YEAR + elapsedYear_;

	std::wstring msg = L"西暦" + std::to_wstring(currentYear_) + L"年\n"
		+ L"貴方は参勤交代を全うし、\n"
		+ L"この" + std::to_wstring(elapsedYear_) + L"年間、平和を保ち続けました";
	textWriter_->AddText(msg);

	menuBtns_.clear();
	int textStartX = Application::SCREEN_HALF_X - 220;

	MENU_BTN btn1;
	btn1.str = L"タイトルに戻る";
	btn1.startPos = { textStartX, Application::SCREEN_SIZE_Y * 8 / 10 };
	btn1.curPos = btn1.startPos;
	btn1.easeCnt = 0.0f;
	menuBtns_.push_back(btn1);

	selectNum_ = -1;
	cursor_->Init();
}

void GameClearScene::NormalUpdate(void)
{
	cursor_->Update();
	textWriter_->Update();

	CollisionManager2D::GetInstance().Update();
	CollisionManager2D::GetInstance().Sweep();

	if (textWriter_->IsEnd())
	{
		textEndTimer_ += SceneManager::GetInstance().GetDeltaTime();

		if (textEndTimer_ >= WAIT_TIME)
		{
			imgAlpha_ -= FADE_SPEED;
			if (imgAlpha_ <= TARGET_ALPHA)
			{
				imgAlpha_ = TARGET_ALPHA;
				isAnimationEnd_ = true;
			}

			if (isAnimationEnd_)
			{
				int mx, my;
				GetMousePoint(&mx, &my);
				Vector2 mousePos = { mx, my };

				int hoverNum = -1;
				for (int i = 0; i < static_cast<int>(menuBtns_.size()); ++i)
				{
					Vector2 sizeInt = UtilityCommon::GetStringSizeToHandle(messageFontHandle_, menuBtns_[i].str);
					int paddingX = 30;
					int paddingY = 15;

					int left = menuBtns_[i].startPos.x - paddingX;
					int right = menuBtns_[i].startPos.x + sizeInt.x + paddingX;
					int top = menuBtns_[i].startPos.y - paddingY;
					int bottom = menuBtns_[i].startPos.y + sizeInt.y + paddingY;

					if (mousePos.x >= left && mousePos.x <= right && mousePos.y >= top && mousePos.y <= bottom)
					{
						hoverNum = i;
						break;
					}
				}

				if (mousePos.x != lastMousePos_.x || mousePos.y != lastMousePos_.y)
				{
					if (hoverNum != selectNum_)
					{
						if (hoverNum != -1)
						{
							soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
						}
						selectNum_ = hoverNum;
					}
				}
				else
				{
					InputManager& ins = InputManager::GetInstance();
					InputManagerS& insS = InputManagerS::GetInstance();

					if (insS.IsTrgDown(INPUT_EVENT::UP) || ins.IsTrgDown(KEY_INPUT_W) ||
						insS.IsTrgDown(INPUT_EVENT::DOWN) || ins.IsTrgDown(KEY_INPUT_S))
					{
						if (selectNum_ == -1)
						{
							selectNum_ = 0;
							soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
						}
					}
				}
				lastMousePos_ = mousePos;

				for (int i = 0; i < static_cast<int>(menuBtns_.size()); ++i)
				{
					auto& btn = menuBtns_[i];
					if (i == selectNum_)
					{
						btn.easeCnt += SceneManager::GetInstance().GetDeltaTime();
						if (btn.easeCnt >= 0.4f) btn.easeCnt = 0.0f;
					}
					else
					{
						//btn.easeCnt -= SceneManager::GetInstance().GetDeltaTime();
						//if (btn.easeCnt <= 0.0f)
						btn.easeCnt = 0.0f;
					}

					Vector2 goalPos;
					goalPos.x = btn.startPos.x + 30;
					goalPos.y = btn.startPos.y;
					btn.curPos = easing_->EaseFunc(btn.startPos, goalPos, btn.easeCnt / 0.4f, Easing::EASING_TYPE::COS_BACK);
				}

				bool currentMouseClick = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
				bool mouseClickTrg = currentMouseClick && !oldMouseClick_;
				oldMouseClick_ = currentMouseClick;

				if ((InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK) || mouseClickTrg) && selectNum_ == 0)
				{
					soundMng_.Play(SoundManager::SRC::DESIDE_BTN_SE, SoundManager::PLAYTYPE::BACK);
					SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
				}
			}
		}
	}
	else
	{
		if (InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK))
		{
			textWriter_->ForceComplete();
		}
	}
}

void GameClearScene::NormalDraw(void)
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(50, 50, 50), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, imgAlpha_);
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgGameClear_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (isAnimationEnd_)
	{
		UtilityDraw::DrawStringCenterToFontHandle(Application::SCREEN_HALF_X, Application::SCREEN_SIZE_Y * 3 / 10, UtilityCommon::WHITE, gameClearFontHandle_, L"GameClear");

		int textStartX = Application::SCREEN_HALF_X - 220;
		int statsPosY = Application::SCREEN_HALF_Y - 30;

		int money = GameRuleManager::GetInstance().GetNowMoney() * GameRuleManager::UNITS;
		int dissatisfaction = GameRuleManager::GetInstance().GetDissatisfaction();

		DrawFormatStringToHandle(textStartX, statsPosY, UtilityCommon::WHITE, messageFontHandle_, L"西暦%d年", currentYear_);
		DrawFormatStringToHandle(textStartX, statsPosY + 50, UtilityCommon::WHITE, messageFontHandle_, L"所持金 : %d", money);
		DrawFormatStringToHandle(textStartX, statsPosY + 100, UtilityCommon::WHITE, messageFontHandle_, L"不満度 : %d", dissatisfaction);

		for (int i = 0; i < static_cast<int>(menuBtns_.size()); ++i)
		{
			unsigned int color = (i == selectNum_) ? UtilityCommon::RED : UtilityCommon::WHITE;
			DrawFormatStringToHandle(menuBtns_[i].curPos.x, menuBtns_[i].curPos.y, color, messageFontHandle_, L"%ls", menuBtns_[i].str.c_str());
		}
	}
	else
	{
		int messagePosX = Application::SCREEN_HALF_X - 220;
		int messagePosY = Application::SCREEN_HALF_Y - 50;
		textWriter_->Draw(messagePosX, messagePosY, messageFontHandle_);
	}

	cursor_->Draw();
}

void GameClearScene::OnSceneEnter(void)
{
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}