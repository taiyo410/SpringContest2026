#pragma once

#include <string>
#include <unordered_map>
#include "../Object/Character/UIData/CharacterUIData.h"
#include "../../Template/Singleton.h"
class DataBank :public Singleton<DataBank>
{
	// シングルトンにだけ共有する
	friend class Singleton<DataBank>;

public:	

	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief プレイヤー人数を設定
	/// @param _num プレイヤー人数を設定
	/// @return 正しく設定できればtrue,問題が起きればfalse
	bool SetPlayerNum(int _num);
	
	/// @brief  プレイヤー人数を返す
	/// @param  
	/// @return プレイヤー人数
	int GetPlayerNum(void);

	/// @brief フルスクリーンフラグの取得
	/// @param  
	/// @return フルスクリーンフラグ
	const bool GetIsFullScreen(void) { return isFullScreen_; }

	/// @brief フルスクリーンの設定
	/// @param _isFullScreen フルスクリーンフラグ(true:フルスクリーン　false:ウィンドウ)
	void SetIsFullScreen(const bool _isFullScreen);


private:

	DataBank(void);
	DataBank(const DataBank& instance_) = default;
	~DataBank(void);
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする

	//フルスクリーンの設定
	bool isFullScreen_;
};

