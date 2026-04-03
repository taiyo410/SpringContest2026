#include "../pch.h"
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h" 
#include "../Manager/Generic/InputManager.h"      
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Game/CollisionManager2D.h"
#include "../Manager/Game/GameRuleManager.h"		
#include "../Object/Character/Cursor/Cursor.h"
#include "../Common/TextWriter.h"     // パスを修正
#include "../Common/FontController.h" // パスを修正
#include "../Common/Easing.h"         // パスを修正
#include "GameOverScene.h"

GameOverScene::GameOverScene(void) :
	soundMng_(SoundManager::GetInstance())
{

	CollisionManager2D::CreateInstance();
	cursor_ = std::make_unique<Cursor>();

	updateFunc_ = std::bind(&GameOverScene::LoadingUpdate, this);
	drawFunc_ = [this]() {LoadingDraw(); };
}

GameOverScene::~GameOverScene(void)
{
	soundMng_.Stop(SoundManager::SRC::GAME_OVER_BGM);
	CollisionManager2D::GetInstance().Destroy();

	GameRuleManager::GetInstance().Destroy();		//ゲームシーンで作ったゲームルールマネージャをここで消す
}

void GameOverScene::Load(void)
{
	easing_ = std::make_unique<Easing>();

	buttonFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, FONT_SIZE, 0);
	gameOverFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_APRIL_GOTHIC, 80, 0);
	menuFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_BOKUTATI, 40, 0);
	messageFontHandle_ = fontController_->GetFontHandle(FontManager::FONT_BOKUTATI, 40, 0);

	imgGameOver_ = resMng_.GetInstance().Load(ResourceManager::SRC::GAME_OVER_IMG).handleId_;

	soundMng_.LoadResource(SoundManager::SRC::GAME_OVER_BGM);
	soundMng_.LoadResource(SoundManager::SRC::MOVE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::DESIDE_BTN_SE);

	textWriter_ = std::make_unique<TextWriter>();
	textWriter_->AddText(L"西暦1682年\n各地域の大名に反乱を起こされ、\n家康の家系は滅びました・・・");

	cursor_->Load();
}

void GameOverScene::Init(void)
{
	soundMng_.Play(SoundManager::SRC::GAME_OVER_BGM, SoundManager::PLAYTYPE::LOOP);
	SoundManager::GetInstance().SetSystemVolume(BGM_VOL, static_cast<int>(SoundManager::TYPE::BGM));

	textEndTimer_ = 0.0f;
	imgAlpha_ = 255;
	isAnimationEnd_ = false;
	oldMouseClick_ = false;

	int mx, my;
	GetMousePoint(&mx, &my);
	lastMousePos_ = { mx, my };

	menuBtns_.clear();
	int textStartX = Application::SCREEN_HALF_X - 220;

	MENU_BTN btn1;
	btn1.str = L"もう一度天下を平定する";
	btn1.startPos = { textStartX, Application::SCREEN_SIZE_Y * 7 / 10 };
	btn1.curPos = btn1.startPos;
	btn1.easeCnt = 0.0f;
	menuBtns_.push_back(btn1);

	MENU_BTN btn2;
	btn2.str = L"タイトルに戻る";
	btn2.startPos = { textStartX, Application::SCREEN_SIZE_Y * 8 / 10 };
	btn2.curPos = btn2.startPos;
	btn2.easeCnt = 0.0f;
	menuBtns_.push_back(btn2);

	selectNum_ = -1;
	cursor_->Init();
}

void GameOverScene::NormalUpdate(void)
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
					Vector2 sizeInt = UtilityCommon::GetStringSizeToHandle(menuFontHandle_, menuBtns_[i].str);
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

					if (insS.IsTrgDown(INPUT_EVENT::UP) || ins.IsTrgDown(KEY_INPUT_W))
					{
						if (selectNum_ <= 0) selectNum_ = static_cast<int>(menuBtns_.size()) - 1;
						else selectNum_--;
						soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
					}
					else if (insS.IsTrgDown(INPUT_EVENT::DOWN) || ins.IsTrgDown(KEY_INPUT_S))
					{
						if (selectNum_ == -1 || selectNum_ >= static_cast<int>(menuBtns_.size()) - 1) selectNum_ = 0;
						else selectNum_++;
						soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
					}
				}
				lastMousePos_ = mousePos;

				for (int i = 0; i < static_cast<int>(menuBtns_.size()); ++i)
				{
					auto& btn = menuBtns_[i];
					if (i == selectNum_)
					{
						btn.easeCnt += SceneManager::GetInstance().GetDeltaTime();
						if (btn.easeCnt >= 0.4f) btn.easeCnt = 0.4f;
					}
					else
					{
						btn.easeCnt -= SceneManager::GetInstance().GetDeltaTime();
						if (btn.easeCnt <= 0.0f) btn.easeCnt = 0.0f;
					}

					Vector2 goalPos;
					goalPos.x = btn.startPos.x + 30;
					goalPos.y = btn.startPos.y;
					btn.curPos = easing_->EaseFunc(btn.startPos, goalPos, btn.easeCnt / 0.4f, Easing::EASING_TYPE::OUT_BACK);
				}

				bool currentMouseClick = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
				bool mouseClickTrg = currentMouseClick && !oldMouseClick_;
				oldMouseClick_ = currentMouseClick;

				if ((InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK) || mouseClickTrg) && selectNum_ != -1)
				{
					soundMng_.Play(SoundManager::SRC::DESIDE_BTN_SE, SoundManager::PLAYTYPE::BACK);

					if (selectNum_ == 0)
					{
						SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
					}
					else
					{
						SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
					}
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

void GameOverScene::NormalDraw(void)
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(50, 50, 50), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, imgAlpha_);
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgGameOver_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (isAnimationEnd_)
	{
		UtilityDraw::DrawStringCenter(Application::SCREEN_HALF_X, Application::SCREEN_SIZE_Y * 3 / 10, UtilityCommon::WHITE, gameOverFontHandle_, L"GameOver");

		for (int i = 0; i < static_cast<int>(menuBtns_.size()); ++i)
		{
			unsigned int color = (i == selectNum_) ? UtilityCommon::RED : UtilityCommon::WHITE;
			DrawFormatStringToHandle(menuBtns_[i].curPos.x, menuBtns_[i].curPos.y, color, menuFontHandle_, L"%ls", menuBtns_[i].str.c_str());
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

void GameOverScene::OnSceneEnter(void)
{
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}