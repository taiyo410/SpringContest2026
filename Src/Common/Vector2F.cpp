#include <cmath>
#include "Vector2.h"
#include "Vector2F.h"


////デフォルトコンストラクタ
//Vector2F::Vector2F(void)
//{
//	x = 0.0F;
//	y = 0.0F;
//}
//
//
////コンストラクタ
//Vector2F::Vector2F(float ix, float iy)
//{
//	x = ix;
//	y = iy;
//}
//
//
////デストラクタ
//Vector2F::~Vector2F(void)
//{
//
//}



//
////ベクトル線を作る
//Vector2F CreateVectorLine(Vector2 value1, Vector2 value2)
//{
//	return Vector2F(static_cast<float>(value1.x - value2.x),
//		static_cast<float>(value1.y - value2.y));
//}

//void Vector2F::ToVector2(void)
//{
//	x = static_cast<int>(round(x));
//	y = static_cast<int>(round(y));
//}

//Vector2F同士の比較
bool Vector2F::IsVector2F(Vector2F value1, Vector2F value2)
{
	if (value1.x < value2.x &&
		value1.y < value2.y)
	{
		return true;
	}

	return false;
}

bool Vector2F::IsSameVector2F(const Vector2F value1, const Vector2F value2)
{
	if (value1.x == value2.x && value1.y == value2.y)
	{
		return true;
	}
	return false;
}

const float Vector2F::Length(const Vector2F& value)
{
	return sqrtf(value.x * value.x + value.y * value.y);
}

const float Vector2F::Length(void)const
{
	return sqrtf(x * x + y * y);
}

const Vector2F Vector2F::Normalized(Vector2F _vec)const
{
	float length = _vec.Length();
	if (length == 0.0f)
	{
		//ゼロベクトルを正規化しようとした場合は、ゼロベクトルを返す
		return Vector2F(0.0f, 0.0f);
	}
	return Vector2F(_vec.x / length, _vec.y / length);
}

const Vector2F Vector2F::Normalized(void) const
{
	float length = Length();
	if (length == 0.0f)
	{
		//ゼロベクトルを正規化しようとした場合は、ゼロベクトルを返す
		return Vector2F(0.0f, 0.0f);
	}
	return Vector2F(x / length, y / length);
}

const Vector2F Vector2F::Perpendicular(void) const
{
	//反時計回りに90度回転させたベクトルを返す
	return Vector2F(-y, x);
}

const Vector2F Vector2F::Perpendicular(const Vector2F _vec) const
{
	//反時計回りに90度回転させたベクトルを返す
	return Vector2F(-_vec.y, _vec.x);
}

const Vector2F Vector2F::Vertical(void) const
{
	Vector2F norm= Vector2F(x,y).Normalized();
	return Vector2F(-norm.y, norm.x);
}

const Vector2F Vector2F::Vertical(const Vector2F _vec) const
{
	Vector2F norm = _vec.Normalized();
	return Vector2F(-norm.y, norm.x);
}


const Vector2F Vector2F::operator+(const Vector2F _value)const
{
	return { x + _value.x,y + _value.y };
}
void Vector2F::operator+=(const Vector2F _value)
{
	x += _value.x;
	y += _value.y;
}
const Vector2F Vector2F::operator-(const Vector2F _value)const
{
	return { x - _value.x,y - _value.y };
}
void Vector2F::operator-=(const Vector2F _value)
{
	x -= _value.x;
	y -= _value.y;
}
const Vector2F Vector2F::operator*(const Vector2F _value)const
{
	return { x * _value.x,y * _value.y };
}
void Vector2F::operator*=(const Vector2F _value)
{
	x*= _value.x;
	y*= _value.y;
}
const Vector2F Vector2F::operator/(const Vector2F _value)const
{
	return { x / _value.x,y / _value.y };
}
void Vector2F::operator/=(const Vector2F _value)
{
	x /= _value.x;
	y /= _value.y;
}

const Vector2 Vector2F::ToVector2(void) const
{
	Vector2 ret;

	ret.x = static_cast<int>(x);
	ret.y = static_cast<int>(y);

	return ret;
}