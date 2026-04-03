#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Object/Character/Cursor/Cursor.h"
#include "../Object/Common/Collider2D/Geometry2D/Circle.h"
#include "../Object/Common/Collider2D/Geometry2D/BoxGeo.h"
#include "SliderUIController.h"

SliderUIController::SliderUIController(const int _sliderNum, const Cursor& _cursor, float _per, Vector2F _leftTopPos, Vector2F _length):
	cursor_(_cursor),
	insS_(InputManagerS::GetInstance()),
	per_(_per),
	leftTopPos_(_leftTopPos),
	length_(_length),
	buttonColor_(UtilityCommon::GREEN),
	rightDownPos_(_leftTopPos + _length),
	isHit_(false),
	circlePos_(),
	circleRadius_(),
	sliderNum_(_sliderNum)
{
	circleRadius_ = length_.y;
}

SliderUIController::~SliderUIController(void)
{
}

void SliderUIController::Load(void)
{
}

void SliderUIController::Init(void)
{
	std::unique_ptr<Geometry2D>geo = std::make_unique<Circle>(circlePos_, circlePos_, circleRadius_ + 10.0f, circleRadius_);
	geo = std::make_unique<BoxGeo>(leftTopPos_, leftTopPos_,length_.x+30.0f,Vector2F(0.0f,0.0f), length_);
	MakeCollider(Collider2D::TAG::SLIDER_BUTTON, std::move(geo), { Collider2D::TAG::SLIDER_BUTTON,Collider2D::TAG::SLIDER_BAR });
	geo = std::make_unique<BoxGeo>(leftTopPos_, leftTopPos_, length_.x + 30.0f, Vector2F(0.0f, 0.0f), length_);
	MakeCollider(Collider2D::TAG::SLIDER_BAR, std::move(geo), { Collider2D::TAG::SLIDER_BUTTON,Collider2D::TAG::SLIDER_BAR });

	Vector2F rightDownPos = GetRightDownPos();
	circlePos_ = { rightDownPos.x ,  leftTopPos_.y + length_.y / 2.0f };
}

void SliderUIController::Update(void)
{
	buttonColor_ = UtilityCommon::GREEN;
	isHit_ = false;

	//if (isSetting_)
	//{
	//	if (!insS_.IsPressed(INPUT_EVENT::OK))return;
	//	PercentReflection();
	//}

	const float rightDownPosX = GetRightDownPos().x;
	if (per_ > 1.0f)
	{
		per_ = 1.0f;
		circlePos_.x = rightDownPosX;
	}
	else if (per_ < 0.0f)
	{
		per_ = 0.0f;
		circlePos_.x = leftTopPos_.x;
	}
}

void SliderUIController::Draw(void)
{
	//ƒQ[ƒW˜g‚Ì•`‰æ
	Vector2F rightDownPos = GetRightDownPos();
	DrawBoxAA(leftTopPos_.x, leftTopPos_.y, rightDownPos.x, rightDownPos.y, UtilityCommon::GRAY, true);

	//ƒQ[ƒW‚Ì•`‰æ
	Vector2F gaugeRightDown = leftTopPos_ + (length_ * per_);
	DrawBoxAA(leftTopPos_.x, leftTopPos_.y, gaugeRightDown.x, rightDownPos.y, UtilityCommon::YELLOW, true);

	//‰~‚Ì•`‰æ
	const float CIRCLE_RADIUS = length_.y;
	constexpr int POSNUM = 8;
	DrawCircleAA(circlePos_.x, circlePos_.y, circleRadius_, POSNUM, buttonColor_);
}

void SliderUIController::Release(void)
{
}

void SliderUIController::OnHit(std::weak_ptr<Collider2D> _partner)
{
	const bool buttonHit = colliders_[SLIDER_BUTTON_COLLIDER_NUM]->IsHit();
	const bool barHit = colliders_[SLIDER_BAR_COLLIDER_NUM]->IsHit();

	if (colliders_[SLIDER_BUTTON_COLLIDER_NUM]->IsHit())
	{
		buttonColor_ = UtilityCommon::RED;
	}
	
	isHit_ = true;
}

void SliderUIController::PercentReflection(void)
{
	float cursorPosX = cursor_.GetPos().x;
	float circleLength = circlePos_.x - leftTopPos_.x;
	circlePos_.x = cursor_.GetPos().x;
	if (cursorPosX > GetRightDownPos().x)
	{
		circlePos_.x = GetRightDownPos().x;
	}
	else if (cursorPosX < leftTopPos_.x)
	{
		circlePos_.x = leftTopPos_.x;
	}	
	per_ = circleLength / length_.x;
}
