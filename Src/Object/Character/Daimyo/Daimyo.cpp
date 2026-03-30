#include "../pch.h"
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Utility/Utility2D.h"
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/BoxGeo.h"
#include "../Object/Character/Daimyo/DaimyoOnHit.h"
#include "Daimyo.h"

Daimyo::Daimyo(const DaimyoImport _import)
	: import_(_import)
{
	state_ = STATE::STANDBY;
	nextState_ = STATE::STANDBY;
	money_ = 0.0f;
	dissatisfaction_ = 0;
	alternateInfo_ = {};
	cnt_ = 0.0;
	isBackMenu_ = false;

	easing_ = std::make_unique<Easing>();
	//更新
	update_.emplace(STATE::STANDBY, [this](void) {UpdateStandby(); });
	update_.emplace(STATE::NORMAL, [this](void) {UpdateNormal(); });
	update_.emplace(STATE::SELECT_DIRECTION, [this](void) {UpdateSelectDirection(); });
	update_.emplace(STATE::DELETE_SELECT_DIRECTION, [this](void) {UpdateDeleteSelectDirection(); });
	update_.emplace(STATE::SELECT, [this](void) {UpdateSelect(); });
	update_.emplace(STATE::SELECT_ALTERNATE, [this](void) {UpdateSelectAlternate(); });
	update_.emplace(STATE::NO_MONEY, [this](void) {UpdateNoMoney(); });
	update_.emplace(STATE::ACTION_ALTERNATE, [this](void) {UpdateActionAlternate(); });
	update_.emplace(STATE::RESULT_ALTERNATE, [this](void) {UpdateResultAlternate(); });
	update_.emplace(STATE::ENHANCEMENT, [this](void) {UpdateEnhancement(); });
	update_.emplace(STATE::DETAILS, [this](void) {UpdateDetails(); });

	//描画
	draw_.emplace(STATE::STANDBY, [this](void) {DrawStandby(); });
	draw_.emplace(STATE::NORMAL, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::SELECT_DIRECTION, [this](void) {DrawSelectDirection(); });
	draw_.emplace(STATE::DELETE_SELECT_DIRECTION, [this](void) {DrawSelectDirection(); });
	draw_.emplace(STATE::SELECT, [this](void) {DrawSelect(); });
	draw_.emplace(STATE::SELECT_ALTERNATE, [this](void) {DrawSelectAlternate(); });
	draw_.emplace(STATE::NO_MONEY, [this](void) {DrawNoMoney(); });
	draw_.emplace(STATE::ACTION_ALTERNATE, [this](void) {DrawActionAlternate(); });
	draw_.emplace(STATE::RESULT_ALTERNATE, [this](void) {DrawResultAlternate(); });
	draw_.emplace(STATE::ENHANCEMENT, [this](void) {DrawEnhancement(); });
	draw_.emplace(STATE::DETAILS, [this](void) {DrawDetails(); });

	//コライダ生成
	changeSetting_.emplace(STATE::STANDBY, [this](void) {});
	changeSetting_.emplace(STATE::NORMAL, [this](void) {CreateCastleCol(); });
	changeSetting_.emplace(STATE::SELECT_DIRECTION, [this](void) {InitSelectDirection(); });
	changeSetting_.emplace(STATE::DELETE_SELECT_DIRECTION, [this](void) {DeleteSelectDirection(); });
	changeSetting_.emplace(STATE::SELECT, [this](void) {CreateSelectCol(); });
	changeSetting_.emplace(STATE::SELECT_ALTERNATE, [this](void) {CreateAlternateCol(); });
	changeSetting_.emplace(STATE::NO_MONEY, [this](void) {DeleteAllColliders(); });
	changeSetting_.emplace(STATE::ACTION_ALTERNATE, [this](void) {DeleteAllColliders(); });
	changeSetting_.emplace(STATE::RESULT_ALTERNATE, [this](void) {ResultAlternate(); });
	changeSetting_.emplace(STATE::ENHANCEMENT, [this](void) {});
	changeSetting_.emplace(STATE::DETAILS, [this](void) {});

	//難易度設定
	settingDiff_.emplace(ALTERNATE_DIFF::SAFETY, [this](void) {SettingSafety(); });
	settingDiff_.emplace(ALTERNATE_DIFF::NORMAL, [this](void) {SettingNormal(); });
	settingDiff_.emplace(ALTERNATE_DIFF::DENGER, [this](void) {SettingDenger(); });
}

Daimyo::~Daimyo(void)
{
}

void Daimyo::Load(void)
{
	//初期化
	Init();

	//画像ID
	imageId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CASTLE).handleId_;
	
	//城の当たり判定生成
	CreateCastleCol();
}

void Daimyo::Init(void)
{
	//当たり判定
	onHit_ = std::make_unique<DaimyoOnHit>(*this);

	//初期化
	pos_ = import_.pos;
	selectPos_.emplace(SELECT::SELECT_ALTERNATE, pos_ + ALTERNATE_LOCAL_POS);
	selectPos_.emplace(SELECT::ENHANCEMENT, pos_ + ENHANCEMENT_LOCAL_POS);
	selectPos_.emplace(SELECT::DETAILS, pos_ + DETAILS_LOCAL_POS);

	alternateMenuPos_.emplace(ALTERNATE_DIFF::SAFETY, Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - ALTERNATE_MENU_LOCAL_POS ));
	alternateMenuPos_.emplace(ALTERNATE_DIFF::NORMAL, Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y));
	alternateMenuPos_.emplace(ALTERNATE_DIFF::DENGER, Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + ALTERNATE_MENU_LOCAL_POS));

	state_ = STATE::STANDBY;
	nextState_ = STATE::NORMAL;
	money_ = 0.0f;
	dissatisfaction_ = 0;
	alternateInfo_ = {};
	cnt_ = 0.0f;
	isBackMenu_ = false;
}

void Daimyo::Update(void)
{
	//状態ごとの更新
	update_[state_]();
}

void Daimyo::Draw(void)
{
	//状態ごとの描画
	draw_[state_]();
}

void Daimyo::Release(void)
{
}

void Daimyo::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	onHit_->OnHit(_partner);
}

void Daimyo::ChangeState(const STATE _state)
{
	nextState_ = _state;
	state_ = STATE::STANDBY;
}

void Daimyo::SetAlternateDiff(const ALTERNATE_DIFF _diff)
{
	settingDiff_[_diff]();
}

void Daimyo::CreateCastleCol(void)
{
	//コライダの初期化
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(pos_, pos_, Utility2D::Distance(import_.hitBoxMin, import_.hitBoxMax), import_.hitBoxMin, import_.hitBoxMax);
	MakeCollider(Collider2D::TAG::DAIMYO, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::CHOICE_ALTERNATE,Collider2D::TAG::CHOICE_ENHANCEMENT,Collider2D::TAG::CHOICE_DETAILS });
}

void Daimyo::InitSelectDirection(void)
{
	easingCnt_ = 0.0f;
	blendAlpha_ = UtilityCommon::ALPHA_MIN;
	startAlpha_ = UtilityCommon::ALPHA_MIN;
	goalAlpha_ = UtilityCommon::ALPHA_MAX;
	selectGoalPos_[SELECT::SELECT_ALTERNATE] = pos_ + ALTERNATE_LOCAL_POS;
	selectGoalPos_[SELECT::ENHANCEMENT] = pos_ + ENHANCEMENT_LOCAL_POS;
	selectGoalPos_[SELECT::DETAILS] = pos_ + DETAILS_LOCAL_POS;
	for (auto& select : selectPos_)
	{
		//選択肢の座標を城の座標に初期化
		select.second = pos_;
		selectStartPos_[select.first] = pos_;
	}
}

void Daimyo::DeleteSelectDirection(void)
{
	//コライダの初期化
	DeleteAllColliders();
	//選択肢の座標を城の座標に初期化
	easingCnt_ = 0.0f;
	startAlpha_ = UtilityCommon::ALPHA_MAX;
	goalAlpha_ = UtilityCommon::ALPHA_MIN;
	for (auto& selectGoal : selectGoalPos_)
	{
		selectStartPos_[selectGoal.first] = selectPos_[selectGoal.first];
		selectGoal.second = pos_;
	}
}

void Daimyo::EasingSelectDirection(void)
{
	Vector2F alternate = pos_ + ALTERNATE_LOCAL_POS;
	Vector2F enhancement = pos_ + ENHANCEMENT_LOCAL_POS;
	Vector2F details = pos_ + DETAILS_LOCAL_POS;

	////選択肢の座標をイージングで動かす
	//selectPos_[SELECT::SELECT_ALTERNATE] = easing_->EaseFunc(pos_, selectGoalPos_[SELECT::SELECT_ALTERNATE], easingCnt_ / EASEING_TIME, Easing::EASING_TYPE::LERP);
	//selectPos_[SELECT::ENHANCEMENT] = easing_->EaseFunc(pos_, selectGoalPos_[SELECT::ENHANCEMENT], easingCnt_/ EASEING_TIME, Easing::EASING_TYPE::LERP);
	//selectPos_[SELECT::DETAILS] = easing_->EaseFunc(pos_, selectGoalPos_[SELECT::DETAILS], easingCnt_/ EASEING_TIME, Easing::EASING_TYPE::LERP);

	for (auto& pos : selectPos_)
	{
		pos.second = easing_->EaseFunc(selectStartPos_[pos.first], selectGoalPos_[pos.first], easingCnt_ / EASEING_TIME, Easing::EASING_TYPE::QUAD_OUT);
	}

	Easing::EASING_TYPE type = startAlpha_ ==UtilityCommon::ALPHA_MIN ? Easing::EASING_TYPE::QUAD_IN : Easing::EASING_TYPE::QUAD_OUT;
	blendAlpha_ = easing_->EaseFunc(startAlpha_, goalAlpha_, easingCnt_ / EASEING_TIME, type);
	easingCnt_ += SceneManager::GetInstance().GetDeltaTime();
	if (easingCnt_ >= EASEING_TIME)
	{
		blendAlpha_ = goalAlpha_;
		//選択肢の座標を目的の座標にする
		for (auto& pos : selectPos_)
		{
			pos.second = selectGoalPos_[pos.first];
		}
	}
}

void Daimyo::CreateSelectCol(void)
{
	//コライダの初期化
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(selectPos_[SELECT::SELECT_ALTERNATE], selectPos_[SELECT::SELECT_ALTERNATE], SELECT_PRE_RADIUS, SELECT_MIN, SELECT_MAX);
	MakeCollider(Collider2D::TAG::CHOICE_ALTERNATE, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::CHOICE_ALTERNATE,Collider2D::TAG::CHOICE_ENHANCEMENT,Collider2D::TAG::CHOICE_DETAILS });

	geo = std::make_unique<BoxGeo>(selectPos_[SELECT::ENHANCEMENT], selectPos_[SELECT::ENHANCEMENT], SELECT_PRE_RADIUS, SELECT_MIN, SELECT_MAX);
	MakeCollider(Collider2D::TAG::CHOICE_ENHANCEMENT, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::CHOICE_ALTERNATE,Collider2D::TAG::CHOICE_ENHANCEMENT,Collider2D::TAG::CHOICE_DETAILS });

	geo = std::make_unique<BoxGeo>(selectPos_[SELECT::DETAILS], selectPos_[SELECT::DETAILS], SELECT_PRE_RADIUS, SELECT_MIN, SELECT_MAX);
	MakeCollider(Collider2D::TAG::CHOICE_DETAILS, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::CHOICE_ALTERNATE,Collider2D::TAG::CHOICE_ENHANCEMENT,Collider2D::TAG::CHOICE_DETAILS });
}

void Daimyo::CreateAlternateCol(void)
{
	//コライダの初期化
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(alternateMenuPos_[ALTERNATE_DIFF::SAFETY], alternateMenuPos_[ALTERNATE_DIFF::SAFETY], ALTERNATE_PRE_RADIUS, ALTERNATE_MENU_MIN, ALTERNATE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ALTERNATE_SAFETY, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });

	geo = std::make_unique<BoxGeo>(alternateMenuPos_[ALTERNATE_DIFF::NORMAL], alternateMenuPos_[ALTERNATE_DIFF::NORMAL], ALTERNATE_PRE_RADIUS, ALTERNATE_MENU_MIN, ALTERNATE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ALTERNATE_NORMAL, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });

	geo = std::make_unique<BoxGeo>(alternateMenuPos_[ALTERNATE_DIFF::DENGER], alternateMenuPos_[ALTERNATE_DIFF::DENGER], ALTERNATE_PRE_RADIUS, ALTERNATE_MENU_MIN, ALTERNATE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ALTERNATE_DENGER, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });
}

void Daimyo::SettingSafety(void)
{
	alternateInfo_.probability = SUCCESS_SAFETY;
	alternateInfo_.income = INCOME_SAFETY;
	alternateInfo_.confiscation = CONFISCATION_SAFETY;
	alternateInfo_.requiredTime = REQUIRED_TIME_SAFETY;
}

void Daimyo::SettingNormal(void)
{
	alternateInfo_.probability = SUCCESS_NORMAL;
	alternateInfo_.income = INCOME_NORMAL;
	alternateInfo_.confiscation = CONFISCATION_NORMAL;
	alternateInfo_.requiredTime = REQUIRED_TIME_NORMAL;
}

void Daimyo::SettingDenger(void)
{
	alternateInfo_.probability = SUCCESS_DENGER;
	alternateInfo_.income = INCOME_DENGER;
	alternateInfo_.confiscation = CONFISCATION_DENGER;
	alternateInfo_.requiredTime = REQUIRED_TIME_DENGER;
}

void Daimyo::ResultAlternate(void)
{
	//収益
	int income = alternateInfo_.income;
	int dissatisfaction = SUCCESS_DISSATISFACTION;

	//大名のお金が減る
	money_ -= FUNDS_MIN;

	//ランダムの値(%)
	int rand = UtilityCommon::GetRandomValue(1, 100);
	if (alternateInfo_.probability < rand)
	{
		//失敗
		income *= alternateInfo_.confiscation;
		dissatisfaction = FAILED_DISSATISFACTION;
	}

	//お金を増やす
	auto& gameMng = GameRuleManager::GetInstance();
	gameMng.AddMoney(income);

	//不満度を増やす
	dissatisfaction_ += dissatisfaction;

	//不満度が上限に達したら
	if (dissatisfaction_ >= DISSATISFACTION_MAX)
	{
		//全体不満度を上昇
		gameMng.AddDissatisfaction(ADD_ALL_DISSATISFACTION);

		//自分の不満度はリセット
		dissatisfaction_ = 0;
	}
}

void Daimyo::UpdateStandby(void)
{
	//コライダの変更
	state_ = nextState_;
	changeSetting_[state_]();
}

void Daimyo::UpdateNormal(void)
{
	//お金を増やす
	money_ += SceneManager::GetInstance().GetDeltaTime() * import_.accumulationSpeed_;
}

void Daimyo::UpdateSelectDirection(void)
{
	EasingSelectDirection();
	//イージングが終わったら
	if (easingCnt_ >= EASEING_TIME)
	{
		//選択状態に移行
		ChangeState(STATE::SELECT);
	}
}

void Daimyo::UpdateDeleteSelectDirection(void)
{
	EasingSelectDirection();
	//イージングが終わったら
	if (easingCnt_ >= EASEING_TIME)
	{
		//選択状態に移行
		ChangeState(STATE::NORMAL);
	}
}

void Daimyo::UpdateSelect(void)
{
	//お金の上昇
	UpdateNormal();

	//入力
	const auto& input = InputManager::GetInstance();

	//項目を選択せずに左クリック
	if (isBackMenu_ && input.IsTrgMouseLeft())
	{
		//通常に戻る
		ChangeState(STATE::DELETE_SELECT_DIRECTION);
	}

	//クリックで戻る
	isBackMenu_ = true;
}

void Daimyo::UpdateSelectAlternate(void)
{
	//お金の上昇
	UpdateNormal();

	//入力
	const auto& input = InputManager::GetInstance();

	//項目を選択せずに左クリック
	if (isBackMenu_ && input.IsTrgMouseLeft())
	{
		//通常に戻る
		ChangeState(STATE::NORMAL);
	}
	//クリックで戻る
	isBackMenu_ = true;
}

void Daimyo::UpdateNoMoney(void)
{
	//お金の上昇
	UpdateNormal();
	
	//入力
	const auto& input = InputManager::GetInstance();

	//項目を選択せずに左クリック
	if (isBackMenu_ && input.IsTrgMouseLeft())
	{
		//通常に戻る
		ChangeState(STATE::NORMAL);
	}
	//クリックで戻る
	isBackMenu_ = true;
}

void Daimyo::UpdateActionAlternate(void)
{
	if (alternateInfo_.requiredTime < cnt_)
	{
		//結果
		ChangeState(STATE::RESULT_ALTERNATE);

		//カウンタの初期化
		cnt_ = 0.0f;

		//参勤交代終了
		return;
	}

	//カウンタ
	cnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void Daimyo::UpdateResultAlternate(void)
{
	//お金の上昇
	UpdateNormal();

	//入力
	const auto& input = InputManager::GetInstance();

	//項目を選択せずに左クリック
	if (isBackMenu_ && input.IsTrgMouseLeft())
	{
		//通常に戻る
		ChangeState(STATE::NORMAL);
	}

	//クリックで戻る
	isBackMenu_ = true;
}

void Daimyo::UpdateEnhancement(void)
{
	//お金の上昇
	UpdateNormal();

	//入力
	const auto& input = InputManager::GetInstance();

	//項目を選択せずに左クリック
	if (isBackMenu_ && input.IsTrgMouseLeft())
	{
		//通常に戻る
		ChangeState(STATE::NORMAL);
	}

	//クリックで戻る
	isBackMenu_ = true;
}

void Daimyo::UpdateDetails(void)
{
	//お金の上昇
	UpdateNormal();

	//入力
	const auto& input = InputManager::GetInstance();

	//項目を選択せずに左クリック
	if (isBackMenu_ && input.IsTrgMouseLeft())
	{
		//通常に戻る
		ChangeState(STATE::NORMAL);
	}

	//クリックで戻る
	isBackMenu_ = true;
}

void Daimyo::DrawStandby(void)
{
	DrawNormal();
}

void Daimyo::DrawNormal(void)
{
	for (auto& col : colliders_)
	{
		col.get()->GetGeometry().Draw(import_.color);
	}

	//名前
	DrawFormatString(pos_.x, pos_.y, 0xffffff, L"%ls", UtilityCommon::GetWStringFromString(import_.name).c_str());

	//城画像
	DrawExtendGraph(pos_.x + import_.hitBoxMin.x, pos_.y + import_.hitBoxMin.y, pos_.x + import_.hitBoxMax.x, pos_.y + import_.hitBoxMax.y, imageId_, true);

	//所持金
	DrawFormatString(pos_.x, pos_.y + 50, 0x00ff00, L"%.2f", money_);

	//不満度
	DrawFormatString(pos_.x, pos_.y + 66, 0xff0000, L"%d", dissatisfaction_);
}

void Daimyo::DrawSelect(void)
{
	//通常描画
	DrawNormal();

	Vector2F alternate = pos_ + ALTERNATE_LOCAL_POS;
	Vector2F enhancement = pos_ + ENHANCEMENT_LOCAL_POS;
	Vector2F details = pos_ + DETAILS_LOCAL_POS;

	////名前
	//DrawStringF(alternate.x, alternate.y, L"alternate", 0x0);
	//DrawStringF(enhancement.x, enhancement.y, L"enhancement", 0x0);
	//DrawStringF(details.x, details.y, L"details", 0x0);
	UtilityDraw::DrawStringCenter(selectPos_[SELECT::SELECT_ALTERNATE].x, selectPos_[SELECT::SELECT_ALTERNATE].y, 0x0, L"alternate");
	UtilityDraw::DrawStringCenter(selectPos_[SELECT::ENHANCEMENT].x, selectPos_[SELECT::ENHANCEMENT].y, 0x0, L"enhancement");
	UtilityDraw::DrawStringCenter(selectPos_[SELECT::DETAILS].x, selectPos_[SELECT::DETAILS].y, 0x0,L"details");


}

void Daimyo::DrawSelectDirection(void)
{
	//通常描画
	DrawNormal();
	Vector2F alternate = pos_ + ALTERNATE_LOCAL_POS;
	Vector2F enhancement = pos_ + ENHANCEMENT_LOCAL_POS;
	Vector2F details = pos_ + DETAILS_LOCAL_POS;
	////名前
	//DrawStringF(alternate.x, alternate.y, L"alternate", 0x0);
	//DrawStringF(enhancement.x, enhancement.y, L"enhancement", 0x0);
	//DrawStringF(details.x, details.y, L"details", 0x0);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendAlpha_);
	UtilityDraw::DrawStringCenter(selectPos_[SELECT::SELECT_ALTERNATE].x, selectPos_[SELECT::SELECT_ALTERNATE].y, 0x0, L"alternate");
	UtilityDraw::DrawStringCenter(selectPos_[SELECT::ENHANCEMENT].x, selectPos_[SELECT::ENHANCEMENT].y, 0x0, L"enhancement");
	UtilityDraw::DrawStringCenter(selectPos_[SELECT::DETAILS].x, selectPos_[SELECT::DETAILS].y, 0x0, L"details");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}

void Daimyo::DrawSelectAlternate(void)
{
	//通常描画
	DrawNormal();
}

void Daimyo::DrawNoMoney(void)
{
	DrawNormal();
}

void Daimyo::DrawActionAlternate(void)
{
	//通常描画
	DrawNormal();

	DrawFormatString(0, 0, 0xffffff, L"%.2f", cnt_);
}

void Daimyo::DrawResultAlternate(void)
{
	//通常描画
	DrawNormal();
}

void Daimyo::DrawEnhancement(void)
{
	//通常描画
	DrawNormal();
}

void Daimyo::DrawDetails(void)
{
	//通常描画
	DrawNormal();
}
