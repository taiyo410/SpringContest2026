#include "../pch.h"
#include "../Application.h"
#include "../Common/FontController.h"
#include "../Common/Easing.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/BoxGeo.h""
#include "DissatisfactionBar.h"

DissatisfactionBar::DissatisfactionBar(void):
	soundMng_(SoundManager::GetInstance())
{
	dissatisfaction_ = 0;
	preState_ = STATE::STANDBY;
	state_ = STATE::STANDBY;
	selectMenuImg_ = -1;

	easing_ = std::make_unique<Easing>();

	drawCnt_ = 0.0f;
	scl_ = 0.1f;
	alpha_ = 0;

	update_.emplace(STATE::STANDBY, [this](void) {UpdateStandBy(); });
	update_.emplace(STATE::NORMAL, [this](void) {UpdateNormal(); });
	update_.emplace(STATE::SELECT, [this](void) {UpdateSelect(); });
	update_.emplace(STATE::DIERCTION_IN, [this](void) {UpdateDirectionIn(); });
	update_.emplace(STATE::DIERCTION_OUT, [this](void) {UpdateDirectionOut(); });

	draw_.emplace(STATE::STANDBY, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::NORMAL, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::SELECT, [this](void) {DrawSelect(); });
	draw_.emplace(STATE::DIERCTION_IN, [this](void) {DrawDirectrion(); });
	draw_.emplace(STATE::DIERCTION_OUT, [this](void) {DrawDirectrion(); });

	preChange_.emplace(STATE::STANDBY, [this](void) {});
	preChange_.emplace(STATE::NORMAL, [this](void) {PreChangeStateNormal(); });
	preChange_.emplace(STATE::SELECT, [this](void) {PreChangeStateSelect(); });
	preChange_.emplace(STATE::DIERCTION_IN, [this](void) {PreChangeStateDirectionIn(); });
	preChange_.emplace(STATE::DIERCTION_OUT, [this](void) {PreChangeStateDirectionOut(); });
}

DissatisfactionBar::~DissatisfactionBar(void)
{
}

void DissatisfactionBar::Load(void)
{
	soundMng_.LoadResource(SoundManager::SRC::DISSATISFACTION_DOWN);
	font_ = std::make_unique<FontController>();
	fontHandle_ = font_->GetFontHandle(FontManager::FONT_BOKUTATI, 24, 0);
	selectMenuImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SELECT_MENU).handleId_;
	mituguImg_= ResourceManager::GetInstance().Load(ResourceManager::SRC::DISSATISFACTION).handleId_;
	soundMng_.SetSoundVolumeSRC(SoundManager::SRC::DISSATISFACTION_DOWN, 300);
}

void DissatisfactionBar::Init(void)
{
	dissatisfaction_ = 0;
	preState_ = STATE::NORMAL;
	state_ = STATE::STANDBY;

	yesPos_ = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - SELECT_LOCAL_POS_Y };
	noPos_ = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + SELECT_LOCAL_POS_Y };

	ChangeState(STATE::NORMAL);
}

void DissatisfactionBar::Update(void)
{
	update_[state_]();
}

void DissatisfactionBar::Draw(void)
{
	draw_[state_]();
}

void DissatisfactionBar::Release(void)
{
}

void DissatisfactionBar::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	//入力
	const auto& input = InputManager::GetInstance();
	const auto& partner = _partner.lock();
	auto& gameMng = GameRuleManager::GetInstance();

	//お金があるなら
	if (input.IsTrgMouseLeft() && partner->GetTag() == Collider2D::TAG::CURSOR)
	{
		for (const auto& myCol : colliders_)
		{
			if (!myCol.get()->IsHit())continue;
			if (myCol->GetTag() == Collider2D::TAG::DISSATISFACTION_RECOVERY&&dissatisfaction_>0)
			{
				//状態遷移
				ChangeState(STATE::SELECT);
			}
			else if (myCol->GetTag() == Collider2D::TAG::DISSATISFACTION_RECOVERY_YES && gameMng.HasEnoughMoney(NECESSARY_MONEY))
			{
				//下げる
				dissatisfaction_ -= SUB_VALUE;
				if (dissatisfaction_ < 0)
				{
					dissatisfaction_ = 0;
				}

				soundMng_.Play(SoundManager::SRC::DISSATISFACTION_DOWN, SoundManager::PLAYTYPE::BACK);

				//お金の消費
				gameMng.SubMoney(NECESSARY_MONEY);
				// スタンプ演出のセットアップ
				drawCnt_ = DRAW_TIME;
				easeCnt_ = 0.2f;
				scl_ = 0.3f;
				alpha_ = 0;
				//状態遷移
				ChangeState(STATE::DIERCTION_IN);
			}
			else if (myCol->GetTag() == Collider2D::TAG::DISSATISFACTION_RECOVERY_NO)
			{
				//状態遷移
				ChangeState(STATE::NORMAL);
			}
		}
	}
}

void DissatisfactionBar::ChangeState(const STATE _state)
{
	preState_ = _state;
	state_ = STATE::STANDBY;
}

void DissatisfactionBar::PreChangeStateNormal(void)
{
	//当たり判定消去
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(DIS_POS, DIS_POS, DIS_RADIUS, DIS_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY, std::move(geo), {});
}

void DissatisfactionBar::PreChangeStateSelect(void)
{
	//当たり判定消去
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(yesPos_, yesPos_, DIS_RADIUS, SELECT_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY_YES, std::move(geo), {});
	
	geo = std::make_unique<BoxGeo>(noPos_, noPos_, DIS_RADIUS, SELECT_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY_NO, std::move(geo), {});
}

void DissatisfactionBar::PreChangeStateDirectionIn(void)
{
	//当たり判定消去
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(DIS_POS, DIS_POS, DIS_RADIUS, DIS_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY, std::move(geo), {});
}

void DissatisfactionBar::PreChangeStateDirectionOut(void)
{
	//当たり判定消去
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(DIS_POS, DIS_POS, DIS_RADIUS, DIS_BOX_SIZE);
	MakeCollider(Collider2D::TAG::DISSATISFACTION_RECOVERY, std::move(geo), {});

	drawCnt_ = DRAW_TIME;
	easeCnt_ = 0.2f;
}


void DissatisfactionBar::UpdateStandBy(void)
{
	state_ = preState_;
	preChange_[state_]();
}

void DissatisfactionBar::UpdateNormal(void)
{
}

void DissatisfactionBar::UpdateSelect(void)
{
}

void DissatisfactionBar::UpdateDirectionIn(void)
{
	if (drawCnt_ <= 0.0f)
	{
		ChangeState(STATE::DIERCTION_OUT);
		return;
	}

	easing_->StampInDirection(easeCnt_, alpha_, scl_);
	drawCnt_ -= SceneManager::GetInstance().GetDeltaTime();
}

void DissatisfactionBar::UpdateDirectionOut(void)
{
	if (easeCnt_ <= 0.0f)
	{
		ChangeState(STATE::NORMAL);
		return;
	}

	easing_->StampOutDirection(easeCnt_, alpha_, scl_);
	drawCnt_ -= SceneManager::GetInstance().GetDeltaTime();
}

void DissatisfactionBar::DrawNormal(void)
{
	//不満度
	DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y - DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, 0x666666, true);
	if (dissatisfaction_ > 0)
		DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y - (DIS_BOX_SIZE.y * 2) * (static_cast<float>(dissatisfaction_) / static_cast<float>(DISSATISFACTION_MAX)), 0xff00ff, true);
	DrawBox(DIS_POS.x - DIS_BOX_SIZE.x, DIS_POS.y - DIS_BOX_SIZE.y, DIS_POS.x + DIS_BOX_SIZE.x, DIS_POS.y + DIS_BOX_SIZE.y, 0x0, false);

}

void DissatisfactionBar::DrawSelect(void)
{
	DrawNormal();

	//はい
	DrawExtendGraph(yesPos_.x - SELECT_BOX_SIZE.x, yesPos_.y - SELECT_BOX_SIZE.y, yesPos_.x + SELECT_BOX_SIZE.x, yesPos_.y + SELECT_BOX_SIZE.y, selectMenuImg_, true);
	UtilityDraw::DrawStringCenterToFontHandle(yesPos_.x, yesPos_.y, 0x0, fontHandle_, L"民にお金をばらまいて不満を下げる\n1000万円必要");

	//いいえ
	DrawExtendGraph(noPos_.x - SELECT_BOX_SIZE.x, noPos_.y - SELECT_BOX_SIZE.y, noPos_.x + SELECT_BOX_SIZE.x, noPos_.y + SELECT_BOX_SIZE.y, selectMenuImg_, true);
	UtilityDraw::DrawStringCenterToFontHandle(noPos_.x, noPos_.y, 0x0, fontHandle_, L"愚民どもに金などやらん");
}

void DissatisfactionBar::DrawDirectrion(void)
{
	DrawNormal();

	// アルファブレンドモードを設定して描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

	// 画面中央から少し左側に描画
	// 画像サイズが大きい場合は、第三引数を0.5や0.6などに下げてスケールを調整してください。
	DrawRotaGraph(Application::SCREEN_HALF_X - 180, Application::SCREEN_HALF_Y - 130, scl_, 0.0, mituguImg_, true);

	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}