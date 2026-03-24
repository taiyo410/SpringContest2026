#pragma once
#include<vector>
#include "../../Template/Singleton.h"
#include "../Object/Character/Daimyo/DaimyoImport.h"

class CharacterBase2D;

class CharacterManager :public Singleton<CharacterManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<CharacterManager>;
public:

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief 解放
	/// @param  
	void Release(void);

private:

	//キャラクター
	std::vector<std::unique_ptr<CharacterBase2D>> characters_;

	//大名のデータ
	std::vector<DaimyoImport> daimyoImportData_;

	//コンストラクタ
	CharacterManager(void);

	//デストラクタ
	~CharacterManager(void)override;

	//カーソルの生成
	void CreateCursor(void);

	//江戸の生成
	void CreateEdo(void);

	//大名の生成
	void CreateDaimyo(void);
};

