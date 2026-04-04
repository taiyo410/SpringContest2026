#include "../pch.h"
#include "../Common/Vector2.h"
#include "../Common/FontController.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
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
	explanImg_.emplace(EXPLAN::NO_IMAGE, -1);
	explanImg_.emplace(EXPLAN::DAIMYO, res.Load(ResourceManager::SRC::CASTLE).handleId_);
	explanImg_.emplace(EXPLAN::DAIMYO_ALTERNATE, res.Load(ResourceManager::SRC::CASTLE).handleId_);
	explanImg_.emplace(EXPLAN::ENHANCEMENT, res.Load(ResourceManager::SRC::CASTLE).handleId_);
	explanImg_.emplace(EXPLAN::DETAILS, res.Load(ResourceManager::SRC::CASTLE).handleId_);
	explanImg_.emplace(EXPLAN::DISSATISFACTION, res.Load(ResourceManager::SRC::CASTLE).handleId_);

	//説明文の登録
	explanStr_.emplace(EXPLAN::NO_IMAGE, L"");
	explanStr_.emplace(EXPLAN::DAIMYO, L"参勤交代の実行\n参勤交代の強化\n現在の詳細を見る\nの3つの項目を選べる");
	explanStr_.emplace(EXPLAN::DAIMYO_ALTERNATE, L"参勤交代をする\n難易度によって\n時間、収入、成功率\nが変化する");
	explanStr_.emplace(EXPLAN::ENHANCEMENT, L"参勤交代の\n時間、収入、成功率\nを強化する\n強化にはお金がかかる");
	explanStr_.emplace(EXPLAN::DETAILS, L"現在の強化状況が分かる");
	explanStr_.emplace(EXPLAN::DISSATISFACTION, L"不満度を下げる\nお金が1000万円かかる");

	//当たり判定
	std::unique_ptr<Geometry2D>geo = std::make_unique<Circle>(pos_, pos_, 15,10);
	MakeCollider(Collider2D::TAG::CURSOR, std::move(geo), { Collider2D::TAG::CURSOR });

	//ヒット処理
	onHit_ = std::make_unique<CursorOnHit>(*this);

	//フォント
	font_ = std::make_unique<FontController>();
	fontHandle_ = font_->GetFontHandle(FontManager::FONT_BOKUTATI , FONT_SIZE, FONT_TICKNESS);
}

void Cursor::Init(void)
{
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
}

void Cursor::Draw(void)
{
	for (auto& col : colliders_)
	{
		col.get()->GetGeometry().Draw(0);
	}

	//説明画像
	if (explanType_ != EXPLAN::NO_IMAGE) 
	{
		//吹き出し
		DrawRotaGraph(speechPos_.x, speechPos_.y, 1.0, 0.0, speechBubbleImg_, true);

		//説明文
		UtilityDraw::DrawStringCenterToFontHandle(speechPos_.x, speechPos_.y, 0x0, fontHandle_, explanStr_[explanType_]);
	}

	DrawFormatString(0, 0, 0xffffff, L"Cursor Pos:(%f,%f)", pos_.x, pos_.y);
}

void Cursor::Release(void)
{
}

void Cursor::OnHit(const std::weak_ptr<Collider2D> _partner)
{
	onHit_->OnHit(_partner);
}

void Cursor::ChangeExplan(const EXPLAN _type)
{
	explanType_ = _type;
}
