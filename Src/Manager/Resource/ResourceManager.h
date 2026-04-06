#pragma once
#include <map>
#include <string>
#include "../../Template/Singleton.h"
#include "Resource.h"

class ResourceManager : public Singleton<ResourceManager>
{

	friend class Singleton<ResourceManager>;

public:

	//リソース
	enum class SRC
	{
		NONE,
		TEST,

		//画像
		MAP,		//マップ
		CASTLE,		//城
		GAUGE,
		ARROW_GAUGE,		//矢印ゲージ

		// タイトルシーン用追加素材
		TITLE_SKY,				//空
		TITLE_MOUNTAIN,			//山
		TITLE_CLOUD,			//雲
		TITLE_ROAD,				//道
		TITLE_KAGO,				//籠

		TITLE_BACK_IMG,		//タイトル背景
		TITLE_LOGO,			//タイトルロゴ
		GAME_OVER_IMG,		//ゲームオーバー
		GAME_CLEAR_IMG,		//ゲームクリア

		//複数画像
		CONTROLLER_UI_IMGS,

		//ピクセルシェーダ

		//エフェクト
	};
	
	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 解放(シーン切替時に一旦解放)
	/// @param  
	void SceneChangeRelease(void);
	
	/// @brief リソースの完全解放
	/// @param  
	void Release(void);
	
	/// @brief リソースのロード
	/// @param src 読み込むリソース
	/// @return リソース
	const ResourceData& Load(SRC src);
	
	/// @brief リソースの複製ロード(モデル用)
	/// @param src 複製したいリソース
	/// @return 複製したリソース
	int LoadModelDuplicate(SRC src);

private:
	//カード番号画像
	static constexpr int CARD_NO_X = 5;	//横の数
	static constexpr int CARD_NO_Y = 2;	//縦の数
	static constexpr int CARD_NO_SIZE_X = 180;
	static constexpr int CARD_NO_SIZE_Y = 222;

	//コントローラーUI画像
	static constexpr int CONTROLLER_UI_NO_X = 10;
	static constexpr int CONTROLLER_UI_NO_Y = 10;
	static constexpr int CONTROLLER_UI_SIZE_X = 128;
	static constexpr int CONTROLLER_UI_SIZE_Y = 128;

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<ResourceData>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, ResourceData&> loadedMap_;

	ResourceData dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	~ResourceManager(void);

	// 内部ロード
	ResourceData& _Load(SRC src);

};
