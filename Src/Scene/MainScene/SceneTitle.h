#pragma once
#include "../SceneBase.h"
#include "../../DrawUI/SceneUI/SceneUi.h"

class SoundManager;
class SceneManager;
class Grid;

class SceneTitle : public SceneBase
{
public:

	/// タイトルシーンのUI選択肢数
    static constexpr int PAUSE_UI_COUNT = 2;

	//タイトルシーンのメニュー項目
    enum class MENU_ITEM
    {
        START,          //ゲーム開始
		HOW_TO_PLAY,    //遊び方
		OPTION,         //操作説明
		CREDIT,         //クレジット
		EXIT,           //ゲーム終了
		MAX             //選択肢の数
	};


    /// @brief コンストラクタ
    /// @param  
    SceneTitle(void);

    /// @brief デストラクタ
    /// @param  
    ~SceneTitle(void) = default;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

	/// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief 描画
    /// @param  
    void Draw(void) override;

	/// @brief 解放
	/// @param
    void Release(void) override;

	/// @brief ゲーム終了要求の確認
    /// @param  
    /// @return 
    bool IsExitRequested(void) const;

	/// @brief ロード開始
    /// @param  
    void Load(void) override;

    /// @brief ロード終了
    /// @param  
    void EndLoad(void) override;


private:
    //グリッド
    Grid* grid_;

	// タイトルUI
    std::unique_ptr<SceneUi> uiMain_;

	// メニュー項目と対応する関数のテーブル
	std::unordered_map<MENU_ITEM, std::function<void(void)>>menuFuncTable_;

    //サウンドマネージャ
	SoundManager& soundMng_;

    //シーンマネージャ
	SceneManager& sceneMng_;

	//ロゴ
    int logo_;
	//背景動画
    int movieHandle_;
	//UIの一時停止カウント
    int pauseUiCount_;

    //決定したかどうか
    bool isDecided_;
	//黒背景を表示するかどうか
    bool showBlackBackground_;
    //終了リクエストフラグ
    bool exitRequested_;
    //
    bool isPlay_;
	//遊び方説明ページ
    int howToPlayPage_;

	//デバッグ描画
    void DrawDebug(void);

};
