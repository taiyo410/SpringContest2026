#include "../pch.h"
#include "../Common/Vector2.h"
#include "../Common/FontController.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Game/GameRuleManager.h"
#include "../Object/Common/Collider2D/Collider2D.h"
#include "../Object/Common/Collider2D/Geometry2D/Circle.h"
#include "CursorOnHit.h"
#include "Cursor.h"

Cursor::Cursor(void)
{
}

Cursor::~Cursor(void)
{
}

void Cursor::Load(void)
{
	//説明画像の登録
	auto& res = ResourceManager::GetInstance();

	speechBubbleImg_ = res.Load(ResourceManager::SRC::SPEECH_BUBBLE).handleId_;
	imageId_ = res.Load(ResourceManager::SRC::CURSOR).handleId_;

	//説明文の登録
	explanStr_.emplace(EXPLAN::NO_IMAGE, L"");
	explanStr_.emplace(EXPLAN::DAIMYO, L"参勤交代の実行\n参勤交代の強化\nの2つの項目を選べる");
	explanStr_.emplace(EXPLAN::DAIMYO_ALTERNATE, L"参勤交代をする\n難易度によって\n時間、収入、成功率\nが変化する");
	explanStr_.emplace(EXPLAN::ENHANCEMENT, L"参勤交代の\n時間、収入、成功率\nを強化する\n強化にはお金がかかる");
	explanStr_.emplace(EXPLAN::ENHANCE_TIME, L"実行時間を減らす\n一回につき10％減");
	explanStr_.emplace(EXPLAN::ENHANCE_PROBABILITY, L"成功率を上げる\n一回につき10％増");
	explanStr_.emplace(EXPLAN::ENHANCE_INCOME, L"収入を上げる\n一回につき10％増");
	explanStr_.emplace(EXPLAN::ENHANCE_MAX, L"もう最大強化やで");
	explanStr_.emplace(EXPLAN::DETAILS, L"現在の強化状況が分かる");
	explanStr_.emplace(EXPLAN::DISSATISFACTION, L"不満度を下げる\nお金が1000万円かかる");

	preChange_.emplace(EXPLAN::NO_IMAGE, [](const int _value) {});
	preChange_.emplace(EXPLAN::DAIMYO, [this](const int _value) {});
	preChange_.emplace(EXPLAN::DAIMYO_ALTERNATE, [this](const int _value) {});
	preChange_.emplace(EXPLAN::ENHANCEMENT, [this](const int _value) {});
	preChange_.emplace(EXPLAN::ENHANCE_TIME, [this](const int _value) {PreChangeEnhanceTime(_value); });
	preChange_.emplace(EXPLAN::ENHANCE_PROBABILITY, [this](const int _value) {PreChangeEnhanceProbability(_value); });
	preChange_.emplace(EXPLAN::ENHANCE_INCOME, [this](const int _value) {PreChangeEnhanceIncome(_value); });
	preChange_.emplace(EXPLAN::ENHANCE_MAX, [this](const int _value) {});
	preChange_.emplace(EXPLAN::DETAILS, [this](const int _value) {});
	preChange_.emplace(EXPLAN::DISSATISFACTION, [this](const int _value) {});

	//当たり判定
	std::unique_ptr<Geometry2D>geo = std::make_unique<Circle>(pos_, pos_, 25,20);
	MakeCollider(Collider2D::TAG::CURSOR, std::move(geo), { Collider2D::TAG::CURSOR });

	//ヒット処理
	onHit_ = std::make_unique<CursorOnHit>(*this);

	//フォント
	font_ = std::make_unique<FontController>();
	fontHandle_ = font_->GetFontHandle(FontManager::FONT_BOKUTATI , FONT_SIZE, FONT_TICKNESS);
}

void Cursor::Init(void)
{
	isExplanImg_ = true;
}

void Cursor::Update(void)
{
	//カーソル座標
	Vector2 pos;
	GetMousePoint(&pos.x, &pos.y);
	pos_ = pos.ToVector2F();

	speechPos_ = pos_ + SPEECH_LOCAL_POS;

	//説明画像の初期化
	explanType_ = EXPLAN::NO_IMAGE;

	if (InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::EXPLAIN))
	{
		isExplanImg_ ? isExplanImg_ = false : isExplanImg_ = true;
	}
}

void Cursor::Draw(void)
{
	//for (auto& col : colliders_)
	//{
	//	col.get()->GetGeometry().Draw(0);
	//}

	//カーソル画像
	DrawRotaGraph(pos_.x, pos_.y, 0.15, UtilityCommon::Deg2RadD(-30.0), imageId_, true);

	//説明画像
	if (explanType_ != EXPLAN::NO_IMAGE&&isExplanImg_) 
	{
		//吹き出し
		DrawRotaGraph(speechPos_.x, speechPos_.y, 1.0, 0.0, speechBubbleImg_, true);

		//説明文
		UtilityDraw::DrawStringCenterToFontHandle(speechPos_.x, speechPos_.y, 0x0, fontHandle_, explanStr_[explanType_]);
	}

	//DrawFormatString(0, 0, 0xffffff, L"Cursor Pos:(%f,%f)", pos_.x, pos_.y);
}

void Cursor::Release(void)
{
}

void Cursor::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	onHit_->OnHit(_partner);
}

void Cursor::ChangeExplan(const EXPLAN _type, const int _value)
{
	preChange_[_type](_value);
	explanType_ = _type;
}

void Cursor::PreChangeEnhanceTime(const int _value)
{
	explanStr_[EXPLAN::ENHANCE_TIME] = L"実行時間を減らす\n一回につき10％減";
	std::wstringstream ss;
	ss.imbue(std::locale(""));
	ss << _value * GameRuleManager::UNITS;
	std::wstring str = L"\n" + ss.str();
	str += L"円必要";
	explanStr_[EXPLAN::ENHANCE_TIME] += str;
}

void Cursor::PreChangeEnhanceProbability(const int _value)
{
	explanStr_[EXPLAN::ENHANCE_PROBABILITY] = L"成功率を上げる\n一回につき10％増";
	std::wstringstream ss;
	ss.imbue(std::locale(""));
	ss << _value * GameRuleManager::UNITS;
	std::wstring str = L"\n" + ss.str();
	str += L"円必要";
	explanStr_[EXPLAN::ENHANCE_PROBABILITY] += str;
}

void Cursor::PreChangeEnhanceIncome(const int _value)
{
	explanStr_[EXPLAN::ENHANCE_INCOME] = L"収入を上げる\n一回につき10％増";
	std::wstringstream ss;
	ss.imbue(std::locale(""));
	ss << _value * GameRuleManager::UNITS;
	std::wstring str = L"\n" + ss.str();
	str += L"円必要";
	explanStr_[EXPLAN::ENHANCE_INCOME] += str;
}