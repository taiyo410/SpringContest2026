#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class UtilityDraw
{
public:

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	/// @brief 点線を描画する
	/// @param sPos 始点
	/// @param ePos 終点
	/// @param color 色
	/// @param len 間隔
	static void DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len = 50.0f);

	/// @brief センター座標に文字列を描画する
	/// @param x 
	/// @param y 
	/// @param str 
	/// @param color 
	/// @param fontHandle 
	static void DrawStringCenterToFontHandle(const int _centerX, const int _centerY, unsigned int color, const int fontHandle, const std::wstring _str);
	static void DrawFormatStringCenterToFontHandle(const int _centerX, const int _centerY, unsigned int color, const int fontHandle, const wchar_t* _format, ...);

	/// @brief デバッグ用の文字列描画関数
	/// @param _centerX 中心座標X
	/// @param _centerY 中心座標Y
	/// @param color 色
	/// @param _str 文字列
	static void DrawStringCenter(const int _centerX, const int _centerY, unsigned int color,const std::wstring _str);
	static void DrawStringCenter(const float _centerX, const float _centerY, unsigned int color,const std::wstring _str);
};

