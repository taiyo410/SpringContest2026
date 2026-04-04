#pragma once

class Vector2;

struct Vector2F
{
	//各座標
	float x;
	float y;

	//Vector2F同士の比較
	
	/// @brief Vector2F同士の比較
	/// @param value1 ベクトル1
	/// @param value2 ベクトル2
	/// @return 
	static bool IsVector2F(const Vector2F value1, const Vector2F value2);

	/// @brief Vector2F同士の値が同じか比較
	/// @param value1 ベクトル1
	/// @param value2 ベクトル2
	/// @return 
	static bool IsSameVector2F(const Vector2F value1, const Vector2F value2);

	/// @brief ベクトルの長さを求める
	/// @param value 求めたいベクトル
	/// @return 
	const float Length(const Vector2F& value);

	/// @brief ベクトルの長さを求める
	/// @param  
	/// @return 
	const float Length(void)const;

	/// @brief 正規化
	/// @param  
	/// @return 正規化したベクトルを返す
	const Vector2F Normalized(Vector2F _vec)const;
	const Vector2F Normalized(void)const;

	//垂直ベクトルを求める
	const Vector2F Perpendicular(void)const;
	const Vector2F Perpendicular(const Vector2F _vec)const;

	//垂直方向のみを求める
	const Vector2F Vertical(void)const;
	const Vector2F Vertical(const Vector2F _vec)const;

	//Vector2F同士の演算
	const Vector2F operator+(const Vector2F _value)const;
	void operator+=(const Vector2F _value);
	const Vector2F operator-(const Vector2F _value)const;
	void operator-=(const Vector2F _value);
	const Vector2F operator*(const Vector2F _value)const;
	void operator*=(const Vector2F _value);
	const Vector2F operator/(const Vector2F _value)const;
	void operator/=(const Vector2F _value);

	//Vector2への変換
	const Vector2 ToVector2(void)const;

	//一つの数での演算
	template<typename T>
	const Vector2F operator+(const T _value)const
	{
		return{ x + _value,y + _value };
	}
	template<typename T>
	void operator+=(const T _value)
	{
		x += _value;
		y += _value;

	}
	template<typename T>
	const Vector2F operator-(const T _value)const
	{
		return { static_cast<float>(x - _value),static_cast<float>(y - _value) };
	}
	template<typename T>
	void operator-=(const T _value)
	{
		x -= _value;
		y -= _value;
	}
	template<typename T>
	const Vector2F operator*(const T _value)const
	{
		return { static_cast<float>(x * _value),static_cast<float>(y * _value) };
	}
	template<typename T>
	void operator*=(const T _value)
	{
		x *= _value;
		y *= _value;
	}
	template<typename T>
	const Vector2F operator/(const T _value)const
	{
		return { static_cast<float>(x / _value),static_cast<float>(y / _value) };
	}
	template<typename T>
	void operator/=(const T _value)
	{
		x /= _value;
		y /= _value;
	}


};
