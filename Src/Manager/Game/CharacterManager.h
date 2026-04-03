#pragma once
#include<vector>
#include "../Object/UI/ArrowController.h"
#include "../../Template/Singleton.h"
#include "../Object/Character/Daimyo/DaimyoImport.h"

class Cursor;
class ArrowController;
class Daimyo;

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

	//大名のお金が上限を超えたか
	const bool IsMoneyMax(void);

private:

	static constexpr Vector2F EDO_POS = { 784.0f,359.0f};

	//矢印ゲージ
	std::unique_ptr<ArrowController>arrow_;
	float arrowPer_;
	FLOAT4 arrowCol_;
	Vector2F arrowStartPos_;
	Vector2F arrowEndPos_;


	//カーソル
	std::unique_ptr<Cursor> cursor_;

	//大名
	std::vector<std::unique_ptr<Daimyo>> daimyo_;

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

