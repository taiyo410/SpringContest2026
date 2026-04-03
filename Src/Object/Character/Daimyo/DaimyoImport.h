#pragma once
#include<DxLib.h>
#include"../Common/Vector2F.h"
#include"../Lib/nlohmann/json.hpp"
#include<iostream>

struct DaimyoImport
{
	//名前
	std::string name = "";

	//座標
	Vector2F pos = { 0.0f,0.0f };

	//当たり判定サイズ
	Vector2F hitBoxMin = { 0.0f,0.0f };
	Vector2F hitBoxMax = { 0.0f,0.0f };

	//色
	FLOAT4 color = {};

	//蓄積倍率
	float accumulationSpeed_ = 0.0f;
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, DaimyoImport& _data)
{
	//名前
	if (_j.contains("name"))
	{
		_data.name = _j.value("name", "");
	}
	else
	{
		std::cerr << "CharacterのJsonファイルにnameが存在しません" << "\n";
		return;
	}

	//座標
	if (_j.contains("position"))
	{
		auto& p = _j.at("position");
		Vector2F pos;
		pos.x = p.value("x", 0.0f);
		pos.y = p.value("y", 0.0f);
		
		_data.pos = pos;
	}

	//当たり判定サイズ
	if (_j.contains("hitBoxMin"))
	{
		auto& s = _j.at("hitBoxMin");
		Vector2F size;
		size.x = s.value("x", 0.0f);
		size.y = s.value("y", 0.0f);
		
		_data.hitBoxMin = size;
	}
	if (_j.contains("hitBoxMax"))
	{
		auto& s = _j.at("hitBoxMax");
		Vector2F size;
		size.x = s.value("x", 0.0f);
		size.y = s.value("y", 0.0f);

		_data.hitBoxMax = size;
	}

	//色
	if (_j.contains("color"))
	{
		auto& c = _j.at("color");
		//RGB
		int red, green, blue;
		red = c.value("red", 0);
		green = c.value("green", 0);
		blue = c.value("blue", 0);
	
		_data.color = { static_cast<float>(red) / 255.0f, static_cast<float>(green) / 255.0f, static_cast<float>(blue) / 255.0f,0.0f };
	}

	//座標
	if (_j.contains("accumulationSpeed"))
	{
		_data.accumulationSpeed_ = _j.value("accumulationSpeed", 0.0f);
	}
}

inline void FromCsv(std::stringstream& _ss, DaimyoImport& _s)
{
	////格納内容
	//std::string token;

	////インポート

	////名前
	//std::getline(_ss, _data.name, ',');

	////座標
	//std::getline(_ss, token, ',');	_data.position.x = std::stof(token);
	//std::getline(_ss, token, ',');	_data.position.y = std::stof(token);
	//std::getline(_ss, token, ',');	_data.position.z = std::stof(token);

	////大きさ
	//std::getline(_ss, token, ',');	_data.scale.x = std::stof(token);
	//std::getline(_ss, token, ',');	_data.scale.y = std::stof(token);
	//std::getline(_ss, token, ',');	_data.scale.z = std::stof(token);

	////回転
	//std::getline(_ss, token, ',');	_data.quaternion.w = std::stof(token);
	//std::getline(_ss, token, ',');	_data.quaternion.x = std::stof(token);
	//std::getline(_ss, token, ',');	_data.quaternion.y = std::stof(token);
	//std::getline(_ss, token, ',');	_data.quaternion.z = std::stof(token);
}
