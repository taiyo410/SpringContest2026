#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../Common/Vector2.h"
#include "SceneBase.h"

class Easing;
class TextWriter;
class Cursor;

class GameClearScene :
	public SceneBase
{
public:

	/// @brief コンストラクタ
	/// @param  
	GameClearScene(void);

	/// @brief デストラクタ
	/// @param  
	~GameClearScene(void)override;

	/// @brief 読み込み
	/// @param  
	void Load(void) override;

	/// @brief 初期化処理
	/// @param  
	void Init(void) override;

private:

	// 手動メニュー管理用構造体 【修正】Vector2をVector2Fに変更
	struct MENU_BTN
	{
		std::wstring str;
		Vector2 startPos;
		Vector2 curPos;
		float easeCnt;
	};

	//イージング時間
	static constexpr float EASING_TIME = 1.0f;

	//BGMの大きさ
	static constexpr int BGM_VOL = 80;

	//イージングY座標上限
	static constexpr float BACK_TITLE_STRING_POS_EASE_LIMIT = 40;

	//演出用定数
	static constexpr float WAIT_TIME = 4.0f; // 待機時間（秒）
	static constexpr int TARGET_ALPHA = 80;  // 最終的な画像のアルファ値（薄さ）
	static constexpr int FADE_SPEED = 3;     // 画像が薄くなるスピード

	//ゲームクリア画像
	int imgGameClear_;

	//サウンド
	SoundManager& soundMng_;

	//イージング
	std::unique_ptr<Easing>easing_;

	// フォントハンドル
	int messageFontHandle_;
	int gameClearFontHandle_;

	// テキストライターとカーソル
	std::unique_ptr<TextWriter> textWriter_;
	std::unique_ptr<Cursor> cursor_;

	std::vector<MENU_BTN> menuBtns_; // メニューリスト
	int selectNum_;                  // 選択番号（何も選んでいない時は -1）

	float textEndTimer_;
	int imgAlpha_;
	bool isAnimationEnd_;
	bool oldMouseClick_;
	Vector2 lastMousePos_;

	// 年数管理用変数
	int elapsedYear_;
	int currentYear_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void OnSceneEnter(void) override;
};