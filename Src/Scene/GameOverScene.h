#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"
#include "SceneBase.h"

class FontContrroeller;
class TextWriter;
class MenuManager; // 追加: MenuManagerの前方宣言
class Cursor;      // 追加: Cursorの前方宣言

class GameOverScene :
    public SceneBase
{
public:

	/// @brief コンストラクタ
	/// @param  
	GameOverScene(void);

	/// @brief デストラクタ
	/// @param  
	~GameOverScene(void)override;

	/// @brief 読み込み
	/// @param  
	void Load(void) override;
	
	/// @brief 初期化処理
	/// @param  
	void Init(void) override;

private:

	// 手動メニュー管理用構造体 (TitleSceneのBTNを模倣)
	struct MENU_BTN
	{
		std::wstring str;
		Vector2 startPos;
		Vector2 curPos;
		float easeCnt;
	};

	//BGMの大きさ
	static constexpr int BGM_VOL = 80;

	//演出用定数
	static constexpr float WAIT_TIME = 4.0f; // 待機時間（秒）
	static constexpr int TARGET_ALPHA = 80;  // 最終的な画像のアルファ値（薄さ）
	static constexpr int FADE_SPEED = 2;     // 画像が薄くなるスピード

	//ゲームオーバー画面
	int imgGameOver_;

	//サウンド
	SoundManager& soundMng_;

	// フォントコントローラー
	std::unique_ptr<FontController> fontController_;

	std::unique_ptr<Easing> easing_; // アニメーション用

	// フォントハンドル
	int buttonFontHandle_;
	int gameOverFontHandle_;
	int messageFontHandle_;
	int menuFontHandle_;

	// テキストライター
	std::unique_ptr<TextWriter> textWriter_;

	// メニューとカーソル管理
	std::unique_ptr<Cursor> cursor_;

	std::vector<MENU_BTN> menuBtns_; // メニューリスト
	int selectNum_;                  // 選択番号（何も選んでいない時は -1）


	//演出用・UI用変数
	float textEndTimer_;
	int imgAlpha_;	//更新関数
	bool isAnimationEnd_; // 演出が完了したかどうかのフラグ
	bool oldMouseClick_; // 追加: 左クリックの1フレーム前の状態を保存

	Vector2 lastMousePos_; // マウスが動いたかを判定するための変数

	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void OnSceneEnter(void) override;
};

