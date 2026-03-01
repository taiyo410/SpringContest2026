#pragma once
#include<DxLib.h>
#include"../Lib/nlohmann/json.hpp"
#include<iostream>

struct DaimyoImport
{
	//名前
	std::string name = "";

	//座標
	VECTOR pos = { 0.0f,0.0f,0.0f };

	//色
	unsigned int color = 0;

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
		VECTOR pos;
		pos.x = _j["position"].value("x", 0.0f);
		pos.y = _j["position"].value("y", 0.0f);
		pos.z = _j["position"].value("z", 0.0f);
		
		_data.pos = pos;
	}

	//色
	if (_j.contains("color"))
	{
		//RGB
		int red, green, blue;
		red = _j["color"].value("red", 0);
		green = _j["color"].value("green", 0);
		blue = _j["color"].value("blue", 0);
	
		_data.color = GetColor(red, green, blue);
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
