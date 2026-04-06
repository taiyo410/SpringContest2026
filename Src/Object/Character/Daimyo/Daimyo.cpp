#include "../pch.h"
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Utility/Utility2D.h"
#include "../Common/Easing.h"
#include "../Common/FontController.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/BoxGeo.h"
#include "../Object/Character/Daimyo/DaimyoOnHit.h"
#include "../Object/UI/ArrowController.h"
#include "../Object/UI/GaugeController.h"
#include "Daimyo.h"

Daimyo::Daimyo(const DaimyoImport _import)
	: import_(_import),
	soundMng_(SoundManager::GetInstance())
{
	state_ = STATE::STANDBY;
	nextState_ = STATE::STANDBY;
	money_ = 0.0f;
	alternateInfo_ = {};
	cnt_ = 0.0;
	isSuccess_ = false;
	isBackMenu_ = false;
	alternatePer_ = 0.0f;
	edoPos_ = EDO_POS;
	arrowPos_ = import_.pos;
	alternateColor_ = import_.color;
	easing_ = std::make_unique<Easing>();
	arrow_ = std::make_unique<ArrowController>(ResourceManager::SRC::ARROW_GAUGE
		, arrowPos_, edoPos_
		, ARROW_THICK, alternatePer_
		, alternateColor_,EDO_COL,Vector2F(20.0f,5.0f),Vector2F(-50.0f, -5.0f));

	dissatisfaction_ = 0;

	float gaugeX = import_.pos.x + import_.hitBoxMin.x;
	float gaugeY = import_.pos.y - import_.hitBoxMin.y+10.0f;
	moneyGaugePos_ = { gaugeX ,gaugeY };
	moneyGaugeSize_ = Vector2F(100.0f, 20.0f);
	moneyGaugeCol_ = {1.0f,1.0f,0.0f,1.0f};
	moneyPer_ = 0.0f;
	moneyGauge_ = std::make_unique<GaugeController>(ResourceManager::SRC::GAUGE, moneyGaugePos_, moneyGaugeSize_, moneyPer_, moneyGaugeCol_, moneyGaugeCol_);
	moneyGaugeColCnt_ = 0.0f;

	fontController_ = std::make_unique<FontController>();
	enhancementMarkAlphaCnt_ = 0.0f;
	enhancementCnt_[ENHANCEMENT_TYPE::TIME] = 0;
	enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY] = 0;
	enhancementCnt_[ENHANCEMENT_TYPE::INCOME] = 0;
	enhancementStr_.emplace(ENHANCEMENT_TYPE::TIME, L"時間");
	enhancementStr_.emplace(ENHANCEMENT_TYPE::PROBABILITY, L"成功率");
	enhancementStr_.emplace(ENHANCEMENT_TYPE::INCOME, L"収入");



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
	draw_.emplace(STATE::SELECT_DIRECTION, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::DELETE_SELECT_DIRECTION, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::SELECT, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::SELECT_ALTERNATE, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::NO_MONEY, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::ACTION_ALTERNATE, [this](void) {DrawActionAlternate(); });
	draw_.emplace(STATE::RESULT_ALTERNATE, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::ENHANCEMENT, [this](void) {DrawNormal(); });
	draw_.emplace(STATE::DETAILS, [this](void) {DrawNormal(); });

	drawAfter_.emplace(STATE::STANDBY, [this](void) {});
	drawAfter_.emplace(STATE::NORMAL, [this](void) {});
	drawAfter_.emplace(STATE::SELECT_DIRECTION, [this](void) {DrawSelectDirection(); });
	drawAfter_.emplace(STATE::DELETE_SELECT_DIRECTION, [this](void) {DrawSelectDirection(); });
	drawAfter_.emplace(STATE::SELECT, [this](void) {DrawSelect(); });
	drawAfter_.emplace(STATE::SELECT_ALTERNATE, [this](void) {DrawSelectAlternate(); });
	drawAfter_.emplace(STATE::NO_MONEY, [this](void) {DrawNoMoney(); });
	drawAfter_.emplace(STATE::ACTION_ALTERNATE, [this](void) {});
	drawAfter_.emplace(STATE::RESULT_ALTERNATE, [this](void) {DrawResultAlternate(); });
	drawAfter_.emplace(STATE::ENHANCEMENT, [this](void) {DrawEnhancement(); });
	drawAfter_.emplace(STATE::DETAILS, [this](void) {DrawDetails(); });

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
	changeSetting_.emplace(STATE::ENHANCEMENT, [this](void) {CreateEnhancementCol(); });
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
	arrow_->Load();

	moneyGauge_->Load();
	//画像ID
	imageId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CASTLE).handleId_;
	selectMenuImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SELECT_MENU).handleId_;

	//フォント
	font_ = std::make_unique<FontController>();
	selectFontHandle_ = font_->GetFontHandle(FontManager::FONT_BOKUTATI, SELECT_FONT_SIZE, FONT_TICKNESS);
	alternateFontHandle_ = font_->GetFontHandle(FontManager::FONT_BOKUTATI, ALTERNATE_FONT_SIZE, FONT_TICKNESS, DX_FONTTYPE_EDGE);
	alternateExplanFontHandle_ = font_->GetFontHandle(FontManager::FONT_BOKUTATI, ALTERNATE_EXPLAN_FONT_SIZE, FONT_TICKNESS);
	selectStr_.emplace(SELECT::SELECT_ALTERNATE, L"参勤交代");
	selectStr_.emplace(SELECT::ENHANCEMENT, L"強化");
	selectStr_.emplace(SELECT::DETAILS, L"詳細");
	alternateStr_.emplace(ALTERNATE_DIFF::SAFETY, L"安全な道");
	alternateStr_.emplace(ALTERNATE_DIFF::NORMAL, L"普通の道");
	alternateStr_.emplace(ALTERNATE_DIFF::DENGER, L"危険な道");

	alternateFailedStr_.emplace_back(L"ざけんなや...\n隊列乱すな...\nドブカスが...");
	alternateFailedStr_.emplace_back(L"列があかんわ...");

	soundMng_.LoadResource(SoundManager::SRC::GAME_BGM);
	soundMng_.LoadResource(SoundManager::SRC::ALTERNATE_SE);
	soundMng_.LoadResource(SoundManager::SRC::ALTERNATE_START);
	soundMng_.LoadResource(SoundManager::SRC::ALTERNATE_FAIL);
	soundMng_.LoadResource(SoundManager::SRC::ALTERNATE_SUCCESS);
	soundMng_.LoadResource(SoundManager::SRC::ENHANCEMENT);

	alternateSuccessStr_.emplace_back(L"今回はドブカスおらん\nかったな。成功や。");
	alternateSuccessStr_.emplace_back(L"参勤交代成功！\nこれで民も安心だ！");
	alternateSuccessStr_.emplace_back(L"今回はスムーズに\n行けたわ");

	kagoImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::KAGO).handleId_;
	speechBalloonImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SPEECH_BUBBLE).handleId_;
	enhancementMarkImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::FAMIRY_CREST).handleId_;
	fontHandle_ = fontController_->GetFontHandle(FontManager::FONT_BOKUTATI, 30, 0);
	//城の当たり判定生成
	CreateCastleCol();
}

void Daimyo::Init(void)
{
	//当たり判定
	onHit_ = std::make_unique<DaimyoOnHit>(*this);

	arrow_->Init();
	moneyGauge_->Init();
	//初期化
	pos_ = import_.pos;
	selectPos_.emplace(SELECT::SELECT_ALTERNATE, pos_ + ALTERNATE_LOCAL_POS);
	selectPos_.emplace(SELECT::ENHANCEMENT, pos_ + ENHANCEMENT_LOCAL_POS);
	selectPos_.emplace(SELECT::DETAILS, pos_ + DETAILS_LOCAL_POS);

	alternateMenuPos_.emplace(ALTERNATE_DIFF::SAFETY, Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - ALTERNATE_MENU_LOCAL_POS ));
	alternateMenuPos_.emplace(ALTERNATE_DIFF::NORMAL, Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y));
	alternateMenuPos_.emplace(ALTERNATE_DIFF::DENGER, Vector2F(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + ALTERNATE_MENU_LOCAL_POS));

	enhancementPos_.emplace(ENHANCEMENT_TYPE::TIME, Vector2F(Application::SCREEN_HALF_X - ENHANCEMENT_MENU_LOCAL_POS_X, Application::SCREEN_HALF_Y - ENHANCEMENT_MENU_LOCAL_POS_Y));
	enhancementPos_.emplace(ENHANCEMENT_TYPE::PROBABILITY, Vector2F(Application::SCREEN_HALF_X - ENHANCEMENT_MENU_LOCAL_POS_X, Application::SCREEN_HALF_Y));
	enhancementPos_.emplace(ENHANCEMENT_TYPE::INCOME, Vector2F(Application::SCREEN_HALF_X - ENHANCEMENT_MENU_LOCAL_POS_X, Application::SCREEN_HALF_Y + ENHANCEMENT_MENU_LOCAL_POS_Y));

	enhancementCol_.emplace(ENHANCEMENT_TYPE::TIME, UtilityCommon::RED);
	enhancementCol_.emplace(ENHANCEMENT_TYPE::PROBABILITY, UtilityCommon::BLUE);
	enhancementCol_.emplace(ENHANCEMENT_TYPE::INCOME, UtilityCommon::GREEN);

	state_ = STATE::STANDBY;
	nextState_ = STATE::NORMAL;
	money_ = 0.0f;
	alternateInfo_ = {};
	cnt_ = 0.0f;
	isBackMenu_ = false;

	enhancementCnt_[ENHANCEMENT_TYPE::TIME] = 0;
	enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY] = 0;
	enhancementCnt_[ENHANCEMENT_TYPE::INCOME] = 0;

	enhancementMarkAlpha_[ENHANCEMENT_TYPE::TIME] = 0;
	enhancementMarkAlpha_[ENHANCEMENT_TYPE::PROBABILITY] = 0;
	enhancementMarkAlpha_[ENHANCEMENT_TYPE::INCOME] = 0;
}

void Daimyo::Update(void)
{
	moneyGauge_->Update();
	//状態ごとの更新
	update_[state_]();
}

void Daimyo::Draw(void)
{
	moneyGauge_->Draw();

	//状態ごとの描画
	draw_[state_]();
}

void Daimyo::DrawAfter(void)
{
	//状態ごとの描画
	drawAfter_[state_]();
}

void Daimyo::Release(void)
{
}

void Daimyo::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	onHit_->OnHit(_partner);
}


void Daimyo::ChangeState(const STATE _nextState)
{
	nextState_ = _nextState;
	state_ = STATE::STANDBY;
}

void Daimyo::SetAlternateDiff(ALTERNATE_DIFF _diff)
{
	//難易度設定
	settingDiff_[_diff]();
}

void Daimyo::Enhancement(ENHANCEMENT_TYPE _type)
{
	//強化カウントの上昇
	enhancementCnt_[_type]++;
}

void Daimyo::UpdateEnhancementMarkAlpha(ENHANCEMENT_TYPE _type)
{
	for (auto& alpha : enhancementMarkAlpha_)
	{
		if (_type != alpha.first)
		{
			enhancementMarkAlpha_[alpha.first] = 0;
			continue;
		}
		enhancementMarkAlpha_[alpha.first] = easing_->EaseFunc(UtilityCommon::ALPHA_MIN, UtilityCommon::ALPHA_MAX, enhancementMarkAlphaCnt_ / 1.0f, Easing::EASING_TYPE::SIN_BACK);
		UtilityCommon::CountUp(enhancementMarkAlphaCnt_, 1.0f, true, SceneManager::GetInstance().GetDeltaTime());
	}

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

	//geo = std::make_unique<BoxGeo>(selectPos_[SELECT::DETAILS], selectPos_[SELECT::DETAILS], SELECT_PRE_RADIUS, SELECT_MIN, SELECT_MAX);
	//MakeCollider(Collider2D::TAG::CHOICE_DETAILS, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::CHOICE_ALTERNATE,Collider2D::TAG::CHOICE_ENHANCEMENT,Collider2D::TAG::CHOICE_DETAILS });
}

void Daimyo::CreateAlternateCol(void)
{
	//コライダの初期化
	DeleteAllColliders();

	alternateColor_ = import_.color;

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(alternateMenuPos_[ALTERNATE_DIFF::SAFETY], alternateMenuPos_[ALTERNATE_DIFF::SAFETY], ALTERNATE_PRE_RADIUS, ALTERNATE_MENU_MIN, ALTERNATE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ALTERNATE_SAFETY, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });

	geo = std::make_unique<BoxGeo>(alternateMenuPos_[ALTERNATE_DIFF::NORMAL], alternateMenuPos_[ALTERNATE_DIFF::NORMAL], ALTERNATE_PRE_RADIUS, ALTERNATE_MENU_MIN, ALTERNATE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ALTERNATE_NORMAL, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });

	geo = std::make_unique<BoxGeo>(alternateMenuPos_[ALTERNATE_DIFF::DENGER], alternateMenuPos_[ALTERNATE_DIFF::DENGER], ALTERNATE_PRE_RADIUS, ALTERNATE_MENU_MIN, ALTERNATE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ALTERNATE_DENGER, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });
}

void Daimyo::CreateEnhancementCol(void)
{
	//コライダの初期化
	DeleteAllColliders();

	//当たり判定
	std::unique_ptr<Geometry2D> geo = std::make_unique<BoxGeo>(enhancementPos_[ENHANCEMENT_TYPE::TIME], enhancementPos_[ENHANCEMENT_TYPE::TIME], ALTERNATE_PRE_RADIUS, ENHANCE_MENU_MIN, ENHANCE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ENHANCEMENT_TIME, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });

	geo = std::make_unique<BoxGeo>(enhancementPos_[ENHANCEMENT_TYPE::INCOME], enhancementPos_[ENHANCEMENT_TYPE::INCOME], ALTERNATE_PRE_RADIUS, ENHANCE_MENU_MIN, ENHANCE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ENHANCEMENT_INCOME, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });

	geo = std::make_unique<BoxGeo>(enhancementPos_[ENHANCEMENT_TYPE::PROBABILITY], enhancementPos_[ENHANCEMENT_TYPE::PROBABILITY], ALTERNATE_PRE_RADIUS, ENHANCE_MENU_MIN, ENHANCE_MENU_MAX);
	MakeCollider(Collider2D::TAG::ENHANCEMENT_PROBABILITY, std::move(geo), { Collider2D::TAG::DAIMYO,Collider2D::TAG::ALTERNATE_SAFETY,Collider2D::TAG::ALTERNATE_NORMAL,Collider2D::TAG::ALTERNATE_DENGER });
}

void Daimyo::SettingSafety(void)
{
	alternateInfo_.probability = SUCCESS_SAFETY + (SUCCESS_SAFETY / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY];
	alternateInfo_.income = INCOME_SAFETY + (INCOME_SAFETY / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::INCOME];
	alternateInfo_.confiscation = CONFISCATION_SAFETY;
	alternateInfo_.requiredTime = REQUIRED_TIME_SAFETY - (REQUIRED_TIME_SAFETY / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::TIME];
}

void Daimyo::SettingNormal(void)
{
	alternateInfo_.probability = SUCCESS_NORMAL + (SUCCESS_NORMAL / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY];
	alternateInfo_.income = INCOME_NORMAL + (INCOME_NORMAL / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::INCOME];
	alternateInfo_.confiscation = CONFISCATION_NORMAL;
	alternateInfo_.requiredTime = REQUIRED_TIME_NORMAL - (REQUIRED_TIME_NORMAL / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::TIME];
}

void Daimyo::SettingDenger(void)
{
	alternateInfo_.probability = SUCCESS_DENGER + (SUCCESS_DENGER / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY];
	alternateInfo_.income = INCOME_DENGER + (INCOME_DENGER / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::INCOME];
	alternateInfo_.confiscation = CONFISCATION_DENGER;
	alternateInfo_.requiredTime = REQUIRED_TIME_DENGER - (REQUIRED_TIME_DENGER / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::TIME];
}

void Daimyo::ResultAlternate(void)
{
	//インスタンス
	auto& gameMng = GameRuleManager::GetInstance();

	//収益
	int income = alternateInfo_.income;
	isSuccess_ = true;

	//大名のお金が減る
	money_ -= FUNDS_MIN;

	//ランダムの値(%)
	int rand = UtilityCommon::GetRandomValue(1, 100);
	if (alternateInfo_.probability < rand)
	{
		//失敗
		RandomAlternateFailedStr(alternateFailedStr_);
		//失敗分の収益倍率
		income *= alternateInfo_.confiscation;
		isSuccess_ = false;

		//全体不満度を上昇
		gameMng.AddDissatisfaction(FAILED_DISSATISFACTION);

		dissatisfactionUp_ = FAILED_DISSATISFACTION;

		//SE再生
		soundMng_.Play(SoundManager::SRC::ALTERNATE_FAIL,SoundManager::PLAYTYPE::BACK);

	}
	else
	{
		//失敗
		RandomAlternateFailedStr(alternateSuccessStr_);

		//全体不満度を上昇
		gameMng.AddDissatisfaction(SUCCESS_DISSATISFACTION);

		dissatisfactionUp_ = SUCCESS_DISSATISFACTION;

		//SE再生
		soundMng_.Play(SoundManager::SRC::ALTERNATE_SUCCESS, SoundManager::PLAYTYPE::BACK);
	}
	alternateResultCnt_ = ALTERNATE_RESULT_TIME;
	income_ = income*GameRuleManager::UNITS;
	//お金を増やす
	gameMng.AddMoney(income);
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
	const auto& rule = GameRuleManager::GetInstance();
	const int year = rule.GetElapsedYear();
	float time = SceneManager::GetInstance().GetDeltaTime() * import_.accumulationSpeed_;

	//経過年数によって倍率変化
	if (PHASE_2_YEAR < year)
	{
		time *= PHASE_2_MONEY_MULTI;
	}
	else if (PHASE_3_YEAR < year)
	{
		time *= PHASE_3_MONEY_MULTI;
	}

	money_ += time;
	if (money_ > FUNDS_MIN)
	{
		const FLOAT4 yellow = UtilityCommon::GetColorF(UtilityCommon::YELLOW);
		const FLOAT4 green = UtilityCommon::GetColorF(UtilityCommon::ORANGE);
		moneyGaugeCol_ = easing_->EaseFunc(yellow, green, moneyGaugeColCnt_ / 0.5f, Easing::EASING_TYPE::COS_BACK);
		moneyGaugeColCnt_ += SceneManager::GetInstance().GetDeltaTime();
		if (moneyGaugeColCnt_ > 0.5f)
		{
			moneyGaugeColCnt_ = 0.0f;
		}
	}

	moneyPer_ = money_ / static_cast<float>(FUNDS_MAX);

}

void Daimyo::UpdateSelectDirection(void)
{
	//お金の上昇
	UpdateNormal();

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
	//お金の上昇
	UpdateNormal();

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

		soundMng_.Stop(SoundManager::SRC::ALTERNATE_SE);

		//カウンタの初期化
		cnt_ = 0.0f;

		//参勤交代終了
		return;
	}
	alternatePer_ = cnt_ / alternateInfo_.requiredTime;

	KagoUpdate();

	arrow_->Update();

	if (!soundMng_.IsPlay(SoundManager::SRC::ALTERNATE_SE))
	{
		soundMng_.Play(SoundManager::SRC::ALTERNATE_SE, SoundManager::PLAYTYPE::LOOP);
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
		soundMng_.Play(SoundManager::SRC::ENHANCEMENT, SoundManager::PLAYTYPE::BACK);
		//通常に戻る
		ChangeState(STATE::NORMAL);
	}

	//クリックで戻る
	isBackMenu_ = true;
}

void Daimyo::UpdateEnhancementDirction(void)
{
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
}

void Daimyo::DrawNormal(void)
{
	for (auto& col : colliders_)
	{
		col.get()->GetGeometry().Draw();
	}

	//名前
	DrawFormatString(pos_.x, pos_.y, 0xffffff, L"%ls", UtilityCommon::GetWStringFromString(import_.name).c_str());

	//城画像
	DrawExtendGraph(pos_.x + import_.hitBoxMin.x, pos_.y + import_.hitBoxMin.y, pos_.x + import_.hitBoxMax.x, pos_.y + import_.hitBoxMax.y, imageId_, true);

	//所持金
	DrawFormatString(pos_.x, pos_.y + 50, 0x00ff00, L"%.2f", money_);

	if (alternateResultCnt_ > 0.0f)
	{
		Vector2F baloonPos = { pos_.x + import_.hitBoxMax.x, pos_.y + import_.hitBoxMin.y };
		int LOCAL_POS_Y = 40;
		float SCALE = 1.1f;
		baloonPos += Vector2F(BALOON_SIZE.x * SCALE / 2-60, -BALOON_SIZE.y * SCALE / 2+50);
		DrawRotaGraph(baloonPos.x, baloonPos.y, SCALE, 0.0f, speechBalloonImg_, true);

		std::wstring resultStr = isSuccess_ ? alternateSuccessStr_[alternateFailedNum_] : alternateFailedStr_[alternateFailedNum_];
		//UtilityDraw::DrawStringCenterToFontHandle(baloonPos.x, baloonPos.y-LOCAL_POS_Y, 0x0, alternateExplanFontHandle_, resultStr);
		UtilityDraw::DrawFormatStringCenterToFontHandle(baloonPos.x, baloonPos.y, 0x0, alternateExplanFontHandle_, (resultStr + L"\n収入：%d\n不満ゲージ：%d").c_str(),
			income_, dissatisfactionUp_);
		UtilityCommon::CountDown(alternateResultCnt_, 0.0f, false, SceneManager::GetInstance().GetDeltaTime());
	}


}

void Daimyo::DrawSelect(void)
{
	Vector2F alternate = pos_ + ALTERNATE_LOCAL_POS;
	Vector2F enhancement = pos_ + ENHANCEMENT_LOCAL_POS;
	Vector2F details = pos_ + DETAILS_LOCAL_POS;

	////名前
	//DrawStringF(alternate.x, alternate.y, L"alternate", 0x0);
	//DrawStringF(enhancement.x, enhancement.y, L"enhancement", 0x0);
	//DrawStringF(details.x, details.y, L"details", 0x0);

	//選択肢
	DrawExtendGraph(selectPos_[SELECT::SELECT_ALTERNATE].x + SELECT_MIN.x
		, selectPos_[SELECT::SELECT_ALTERNATE].y + SELECT_MIN.y
		, selectPos_[SELECT::SELECT_ALTERNATE].x + SELECT_MAX.x
		, selectPos_[SELECT::SELECT_ALTERNATE].y + SELECT_MAX.y
		,selectMenuImg_, true);
	DrawExtendGraph(selectPos_[SELECT::ENHANCEMENT].x + SELECT_MIN.x
		, selectPos_[SELECT::ENHANCEMENT].y + SELECT_MIN.y
		, selectPos_[SELECT::ENHANCEMENT].x + SELECT_MAX.x
		, selectPos_[SELECT::ENHANCEMENT].y + SELECT_MAX.y
		,selectMenuImg_, true);
	UtilityDraw::DrawStringCenterToFontHandle(selectPos_[SELECT::SELECT_ALTERNATE].x, selectPos_[SELECT::SELECT_ALTERNATE].y, 0x0, selectFontHandle_, selectStr_[SELECT::SELECT_ALTERNATE]);
	UtilityDraw::DrawStringCenterToFontHandle(selectPos_[SELECT::ENHANCEMENT].x, selectPos_[SELECT::ENHANCEMENT].y, 0x0, selectFontHandle_, selectStr_[SELECT::ENHANCEMENT]);
	//UtilityDraw::DrawStringCenterToFontHandle(selectPos_[SELECT::DETAILS].x, selectPos_[SELECT::DETAILS].y, 0x0, selectFontHandle_, selectStr_[SELECT::DETAILS]);
}

void Daimyo::DrawSelectDirection(void)
{
	Vector2F alternate = pos_ + ALTERNATE_LOCAL_POS;
	Vector2F enhancement = pos_ + ENHANCEMENT_LOCAL_POS;
	Vector2F details = pos_ + DETAILS_LOCAL_POS;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendAlpha_);
	//選択肢
	DrawExtendGraph(selectPos_[SELECT::SELECT_ALTERNATE].x + SELECT_MIN.x
		, selectPos_[SELECT::SELECT_ALTERNATE].y + SELECT_MIN.y
		, selectPos_[SELECT::SELECT_ALTERNATE].x + SELECT_MAX.x
		, selectPos_[SELECT::SELECT_ALTERNATE].y + SELECT_MAX.y
		, selectMenuImg_, true);
	DrawExtendGraph(selectPos_[SELECT::ENHANCEMENT].x + SELECT_MIN.x
		, selectPos_[SELECT::ENHANCEMENT].y + SELECT_MIN.y
		, selectPos_[SELECT::ENHANCEMENT].x + SELECT_MAX.x
		, selectPos_[SELECT::ENHANCEMENT].y + SELECT_MAX.y
		, selectMenuImg_, true);
	//DrawExtendGraph(selectPos_[SELECT::DETAILS].x + SELECT_MIN.x
	//	, selectPos_[SELECT::DETAILS].y + SELECT_MIN.y
	//	, selectPos_[SELECT::DETAILS].x + SELECT_MAX.x
	//	, selectPos_[SELECT::DETAILS].y + SELECT_MAX.y
	//	, selectMenuImg_, true);
	//UtilityDraw::DrawStringCenter(selectPos_[SELECT::SELECT_ALTERNATE].x, selectPos_[SELECT::SELECT_ALTERNATE].y, 0x0, L"alternate");
	//UtilityDraw::DrawStringCenter(selectPos_[SELECT::ENHANCEMENT].x, selectPos_[SELECT::ENHANCEMENT].y, 0x0, L"enhancement");
	//UtilityDraw::DrawStringCenter(selectPos_[SELECT::DETAILS].x, selectPos_[SELECT::DETAILS].y, 0x0, L"details");
	UtilityDraw::DrawStringCenterToFontHandle(selectPos_[SELECT::SELECT_ALTERNATE].x, selectPos_[SELECT::SELECT_ALTERNATE].y, 0x0, selectFontHandle_, L"参勤交代");
	UtilityDraw::DrawStringCenterToFontHandle(selectPos_[SELECT::ENHANCEMENT].x, selectPos_[SELECT::ENHANCEMENT].y, 0x0, selectFontHandle_, L"強化");
	//UtilityDraw::DrawStringCenterToFontHandle(selectPos_[SELECT::DETAILS].x, selectPos_[SELECT::DETAILS].y, 0x0, selectFontHandle_, L"詳細");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}

void Daimyo::DrawSelectAlternate(void)
{
	for (auto pos : alternateMenuPos_)
	{
		//選択肢
		DrawRotaGraph(pos.second.x, pos.second.y, 1.0, 0.0, selectMenuImg_, true);
	}

	//詳細
	std::wstring timeStr = L"実行時間：";
	std::wstring probabilityStr = L"\n成功率：";
	std::wstring incomeStr = L"\n収入：";
	wchar_t buf[32];
	float timeValue = REQUIRED_TIME_SAFETY - (REQUIRED_TIME_SAFETY / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::TIME];
	float probabilityValue = SUCCESS_SAFETY + (SUCCESS_SAFETY / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY];
	if (probabilityValue > 100.0f)probabilityValue = 100.0f;
	float incomeValue = INCOME_SAFETY + (INCOME_SAFETY / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::INCOME];
	incomeValue *= GameRuleManager::UNITS;

	//安全な道
	swprintf(buf,32,L"%.1f", timeValue);
	std::wstring time = timeStr + buf + L"秒";
	swprintf(buf, 32, L"%.1f", probabilityValue);
	std::wstring probability = probabilityStr + buf + L"％";
	//swprintf(buf, 32, L"%.1f", incomeValue);
	std::wstring income = incomeStr + std::to_wstring(static_cast<int>(incomeValue)) + L"円";
	UtilityDraw::DrawStringCenterToFontHandle(alternateMenuPos_[ALTERNATE_DIFF::SAFETY].x
		, alternateMenuPos_[ALTERNATE_DIFF::SAFETY].y + FONT_ALTERNATE_LOCAL_POS_Y
		, UtilityCommon::GREEN
		,alternateFontHandle_
		,alternateStr_[ALTERNATE_DIFF::SAFETY]);
	UtilityDraw::DrawStringCenterToFontHandle(alternateMenuPos_[ALTERNATE_DIFF::SAFETY].x
		, alternateMenuPos_[ALTERNATE_DIFF::SAFETY].y + FONT_ALTERNATE_EXPLAN_LOCAL_POS_Y
		, UtilityCommon::BLACK
		, alternateExplanFontHandle_
		, time + probability + income);
	

	//普通の道
	timeValue = REQUIRED_TIME_NORMAL - (REQUIRED_TIME_NORMAL / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::TIME];
	probabilityValue = SUCCESS_NORMAL + (SUCCESS_NORMAL / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY];
	if (probabilityValue > 100.0f)probabilityValue = 100.0f;
	incomeValue = INCOME_NORMAL + (INCOME_NORMAL / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::INCOME];
	incomeValue *= GameRuleManager::UNITS;

	swprintf(buf, 32, L"%.1f", timeValue);
	time = timeStr + buf + L"秒";
	swprintf(buf, 32, L"%.1f", probabilityValue);
	probability = probabilityStr + buf + L"％";
	//swprintf(buf, 32, L"%.1f", incomeValue);
	income = incomeStr + std::to_wstring(static_cast<int>(incomeValue)) + L"円";
	UtilityDraw::DrawStringCenterToFontHandle(alternateMenuPos_[ALTERNATE_DIFF::NORMAL].x
		, alternateMenuPos_[ALTERNATE_DIFF::NORMAL].y + FONT_ALTERNATE_LOCAL_POS_Y
		, UtilityCommon::YELLOW
		,alternateFontHandle_
		,alternateStr_[ALTERNATE_DIFF::NORMAL]);
	UtilityDraw::DrawStringCenterToFontHandle(alternateMenuPos_[ALTERNATE_DIFF::NORMAL].x
		, alternateMenuPos_[ALTERNATE_DIFF::NORMAL].y + FONT_ALTERNATE_EXPLAN_LOCAL_POS_Y
		, UtilityCommon::BLACK
		, alternateExplanFontHandle_
		, time + probability + income);

	//危険な道
	timeValue = REQUIRED_TIME_DENGER - (REQUIRED_TIME_DENGER / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::TIME];
	probabilityValue = SUCCESS_DENGER + (SUCCESS_DENGER / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::PROBABILITY];
	if (probabilityValue > 100.0f)probabilityValue = 100.0f;
	incomeValue = INCOME_DENGER + (INCOME_DENGER / ENHANCE_PER) * enhancementCnt_[ENHANCEMENT_TYPE::INCOME];
	incomeValue *= GameRuleManager::UNITS;

	swprintf(buf, 32, L"%.1f", timeValue);
	time = timeStr + buf + L"秒";
	swprintf(buf, 32, L"%.1f", probabilityValue);
	probability = probabilityStr + buf + L"％";
	//swprintf(buf, 32, L"%.1f", incomeValue);
	income = incomeStr + std::to_wstring(static_cast<int>(incomeValue)) + L"円";
	UtilityDraw::DrawStringCenterToFontHandle(alternateMenuPos_[ALTERNATE_DIFF::DENGER].x
		, alternateMenuPos_[ALTERNATE_DIFF::DENGER].y + FONT_ALTERNATE_LOCAL_POS_Y
		, UtilityCommon::RED
		,alternateFontHandle_
		,alternateStr_[ALTERNATE_DIFF::DENGER]);
	UtilityDraw::DrawStringCenterToFontHandle(alternateMenuPos_[ALTERNATE_DIFF::DENGER].x
		, alternateMenuPos_[ALTERNATE_DIFF::DENGER].y + FONT_ALTERNATE_EXPLAN_LOCAL_POS_Y
		, UtilityCommon::BLACK
		, alternateExplanFontHandle_
		, time + probability + income);
}

void Daimyo::DrawNoMoney(void)
{
	DrawString(pos_.x + 50, pos_.y, L"NoMoney", 0xffffff);
}

void Daimyo::DrawActionAlternate(void)
{
	DrawNormal();

	DrawFormatString(0, 0, 0xffffff, L"%.2f", cnt_);

	arrow_->Draw();

	DrawKago();
}

void Daimyo::DrawResultAlternate(void)
{
	DrawString(pos_.x + 50, pos_.y, isSuccess_ ? L"Success" : L"Failure", 0xffffff);
}

void Daimyo::DrawEnhancement(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(ENHANCEMENT_MENU_LOCAL_BOX_POS, ENHANCEMENT_MENU_LOCAL_BOX_POS,
		 Application::SCREEN_SIZE_X- ENHANCEMENT_MENU_LOCAL_BOX_POS,Application::SCREEN_SIZE_Y- ENHANCEMENT_MENU_LOCAL_BOX_POS,0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	for (auto& enhancePos : enhancementPos_)
	{
		DrawBox(enhancePos.second.x+ ENHANCE_MENU_MIN.x, enhancePos.second.y + ENHANCE_MENU_MIN.y,
			enhancePos.second.x+ ENHANCE_MENU_MAX.x, enhancePos.second.y + ENHANCE_MENU_MAX.y, 0xffffff, true);

		Vector2 boxCenter = { 530,enhancePos.second.y };
		DrawBox(boxCenter.x - 530 / 2, enhancePos.second.y + ENHANCE_MENU_MIN.y, boxCenter.x + 530 / 2, enhancePos.second.y + ENHANCE_MENU_MAX.y, enhancementCol_[enhancePos.first], true);


		//DrawBox(enhancePos.x+)
		UtilityDraw::DrawStringCenterToFontHandle(enhancePos.second.x, enhancePos.second.y, 0x0, fontHandle_, enhancementStr_[enhancePos.first]);

		//ここで強化項目の家紋を描画
		float posX = enhancePos.second.x + ENHANCE_MENU_MAX.x + ENHANCE_MARK_OFFSET;
		for (int i = 0; i < enhancementCnt_[enhancePos.first]; i++)
		{
			DrawRotaGraphF(posX, enhancePos.second.y,
				ENHANCE_MARK_SCL, 0.0f, enhancementMarkImg_, true);

			posX += ENHANCE_MARK_SIZE + ENHANCE_MARK_OFFSET;
		}

		//強化予測でのマークを描画
		posX = enhancePos.second.x + ENHANCE_MENU_MAX.x + ENHANCE_MARK_OFFSET;
		for (int i = 0; i < enhancementCnt_[enhancePos.first] + 1; i++)
		{
			//if(enhancementCnt_>ENHANCE_MAX)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, enhancementMarkAlpha_[enhancePos.first]);
			DrawRotaGraphF(posX, enhancePos.second.y,
				ENHANCE_MARK_SCL, 0.0f, enhancementMarkImg_, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			posX += ENHANCE_MARK_SIZE + ENHANCE_MARK_OFFSET;
		}

	}
}

void Daimyo::DrawDetails(void)
{
}

void Daimyo::DrawKago(void)
{
	Vector2F dir = EDO_POS - import_.pos;
	DrawRotaGraph(kagoCenterPos_.x, kagoCenterPos_.y, 0.1f, atan2f(dir.y, dir.x), kagoImage_, true);
}

void Daimyo::KagoUpdate(void)
{
	////駕籠の座標をイージングで動かす
	kagoCenterPos_ = easing_->EaseFunc(import_.pos, EDO_POS, cnt_ / alternateInfo_.requiredTime, Easing::EASING_TYPE::LERP);

	kagoCenterPos_ += kagoCenterPos_.Vertical() + Vector2F(0.0f, easing_->EaseFunc(KAGO_VERTICAL_LOCAL_START_POS, KAGO_VERTICAL_LOCAL_GOAL_POS
		, kagoVerticalLocalCnt_/ KAGO_VERTICAL_LOCAL_CNT,Easing::EASING_TYPE::COS_BACK));
	kagoVerticalLocalCnt_ += SceneManager::GetInstance().GetDeltaTime();
	if(kagoVerticalLocalCnt_> KAGO_VERTICAL_LOCAL_CNT)
	{
		kagoVerticalLocalCnt_ = 0.0f;
	}
}

void Daimyo::AlternateResultEffect(void)
{
	std::wstring resultStr = isSuccess_ ? L"無事に江戸へ到達！": GetRandomAlternateResultStr();
}

const std::wstring Daimyo::GetRandomAlternateResultStr(void) const
{
	return alternateFailedStr_[alternateFailedNum_];
}

void Daimyo::RandomAlternateFailedStr(const std::vector<std::wstring>_str)
{
	alternateFailedNum_ = UtilityCommon::GetRandomValue(0, static_cast<int>(_str.size() - 1));
}
