#pragma once
#include<memory>
#include "../Common/Easing.h"
#include "SceneBase.h"

class GaugeController;
class ArrowController;
class TextWriter;
class StartScene;
class SettingScene;
class SoundManager;
class Easing;
class MenuManager;
class Cursor;
class TitleScene : public SceneBase
{

public:
	enum class TITLE_STATE
	{
		//ボタンメニュ
		START_GAME,
		//TUTORIAL,
		SETTING,
		EXIT_MENU,
		SCREEN_MENU,
		//その他
		START_STATE,
		EASE_MENU,
		MENU,

		EXIT,
		MAX
	};

	enum class TITLE_BTN
	{
		START_GAME,
		//TUTORIAL,
		SCREEN,
		EXIT,
		MAX
	};

	enum class YES_NO
	{
		YES,
		NO
	};

	struct BTN
	{
		std::wstring btnStr;
		TITLE_BTN btnType;		//何のボタンか
		Vector2F startPos;		//イージング前の座標
		Vector2F curPos = startPos;//現在座標
		float easeCnt;			//イージング時間
		bool isEase = false;	//イージング中か
	};

	//イージング時間
	static constexpr float BUTTON_EASING_TIME = 1.0f;

	//タイトルロゴイージング時間
	static constexpr float LOGO_EASING_TIME = 1.5f;

	//次のボタンのイージングまでの間隔時間
	static constexpr float EASING_DIS_TIME = 0.1f;

	/// @brief コンストラクタ
	/// @param  
	TitleScene(void);

	/// @brief デストラクタ
	/// @param  
	~TitleScene(void)override;

	/// @brief 読み込み処理
	/// @param  
	void Load(void) override;

	/// @brief 初期化処理
	/// @param  
	void Init(void) override;

	/// @brief ポップ後の初期化
	/// @param  
	void PopSceneAfter(void)override;

private:


	//メニュー座標の間隔
	static constexpr float BUTTON_DISTANCE = SceneBase::FONT_SIZE + 30;

	//選択中ボタンのイージングで動かせる距離
	static constexpr Vector2 SELECT_EASE_DISTANCE = { 20,0 };
	//選択中ボタンのイージング時間
	static constexpr float SELECT_EASE_TIME = 0.5f;

	//ロゴサイズ
	static constexpr float LOGO_SCALE = 0.5f;
	static constexpr float LOGO_SIZE_X = 986.0f * LOGO_SCALE;
	static constexpr float LOGO_SIZE_Y = 759.0f * LOGO_SCALE;

	//ロゴイージング初期座標
	static constexpr Vector2F START_POS = { -LOGO_SIZE_X,-LOGO_SIZE_Y };
	static constexpr Vector2F GOAL_POS = { 0.0f,Application::SCREEN_HALF_Y - LOGO_SIZE_Y / 2.0f };

	//メニュー始まりの座標
	static constexpr float BUTTON_START_POS_X = 700.0f;
	//static constexpr float BUTTON_START_POS_Y = Application::SCREEN_HALF_Y - LOGO_SIZE_Y / 2.0f;
	static constexpr float BUTTON_START_POS_Y = Application::SCREEN_HALF_Y + 130.0f;

	//ゲーム終了確認メニューの大きさ
	static constexpr int CHECK_EXIT_MENU_SIZE_X = 600;
	static constexpr int CHECK_EXIT_MENU_SIZE_Y = 200;

	//終了するかの文字の確認メニューとの間隔
	static constexpr int QUESTION_OFFSET = 30;

	//はい、いいえの文字間隔
	static constexpr int YES_NO_DISTANCE_X = 70;
	static constexpr int YES_NO_DISTANCE_Y = 100;

	//決定ボタン座標
	static constexpr float DICITION_BTN_SIZE = 64;
	static constexpr Vector2F DICITION_BTN_POS = { Application::SCREEN_SIZE_X - 300.0f, Application::SCREEN_SIZE_Y - DICITION_BTN_SIZE - 30 };
	//戻るボタン座標
	static constexpr Vector2F BACK_BTN_POS = { DICITION_BTN_POS.x + 150.0f, Application::SCREEN_SIZE_Y - DICITION_BTN_SIZE - 30 };
	//ボタンの説明文字列座標オフセット
	static constexpr float BTN_STR_OFFSET_X = 16.0f;

	//ブレンド時間
	static constexpr float BLEND_TIME = 2.0f;

	//イージング
	std::unique_ptr<Easing>easing_;
	//テキスト
	std::unique_ptr<TextWriter>textWtiter_;

	//各選択肢の更新
	std::function<void(void)>updateTitle_;
	std::unordered_map<TITLE_STATE, std::function<void(void)>> titleSelectFuncTable_;
	std::function<void(void)>drawTitle_;

	//文字列格納
	std::unordered_map<TITLE_BTN, std::wstring>buttonStrTable_;
	//YES,NOの文字列
	std::unordered_map<YES_NO, std::wstring>yesNoStrTable_;
	//メニューマネージャ
	std::unique_ptr<MenuManager>menuMng_;
	//設定シーン
	std::shared_ptr<SceneBase>settingScn_;
	//カーソル
	std::unique_ptr<Cursor>cursor_;
	//サウンド
	SoundManager& soundMng_;



	//タイトルロゴ座標
	Vector2F logoPos_;

	//セレクト中のボタン
	TITLE_BTN selectBtn_;
	TITLE_STATE selectState_;

	// タイトル背景関連
	int imgSky_;
	int imgMountain_;
	int imgCloud_;
	int imgRoad_;
	int imgKago_;

	//タイトルロゴ
	int imgTitleLogo;

	// 背景スクロール・描画用変数群
	float scrollCloudX_;
	float scrollRoadX_;
	static constexpr float CLOUD_SPEED = 0.5f;
	static constexpr float ROAD_SPEED = 3.0f;

	// 駕籠の描画設定
	static constexpr float KAGO_SIZE_X = 500.0f;
	static constexpr float KAGO_SIZE_Y = 320.0f;
	static constexpr float KAGO_POS_X = -40.0f;
	static constexpr float KAGO_POS_Y = 380.0f;

	// 14病後のロゴ演出用変数
	float logoWaitTimer_;
	float logoStampEaseCnt_;
	bool isLogoAppeared_;
	float currentLogoScale_;
	int currentLogoAlpha_;
	static constexpr float LOGO_WAIT_TIME = 0.0f;
	static constexpr float LOGO_STAMP_TIME = 0.2f;	// スタンプ演出にかかる時間

	// プロローグ演出用変数
	bool isPrologue_;
	float prologueTimer_;
	static constexpr float PROLOGUE_TIME = 14.0f;	// スタートシーンから移行する時間

	// プロローグの自作テキスト表示用
	std::wstring prologueText_;
	float prologueTextDispCnt_;
	int prologueTextDispLen_;
	static constexpr float PROLOGUE_TEXT_SPD = 6.0f;	// テキスト描画速度

	//タイトル背景
	//int imgTitleBack;

	//現在選んでいるボタン
	int selectNum_;

	//イージング開始の間隔カウント
	float easeDistanceCnt_;
	//タイトルイージングカウント
	float logoEaseCnt_;

	//はいの選択肢を選んでいるか
	YES_NO yesNoState_;

	//サイズ確認(デバッグ用)
	int fontSize_;
	int thick_;

	//プッシュボタンの文字列のアルファ値
	int stringAlpha_;
	//アルファ値のブレンドカウント
	float blendCnt_;

	//状態遷移
	void ChangeState(const TITLE_STATE& _state);

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void OnSceneEnter(void) override;

	//更新系
	void UpdateStart(void);		//スタートシーン
	void UpdateEase(void);		//初めのイージング処理
	void UpdateMenu(void);		//メニュー処理
	void UpdateSetting(void);	//スクリーンの設定
	void UpdateTutorial(void);	//チュートリアル

	//描画系
	void DrawBackground(void);	// 背景の描画まとめ
	void DrawMenu(void);
	void DrawStart(void);
	void DrawSetting(void);
	void DrawExit(void);

	//選択処理
	void UpdateSelectGame(void);

	//ゲーム終了確認メニュー
	void UpdateExitMenu(void);

	//状態遷移
	void ChangeStart(void);
	void ChangeEaseMenu(void);
	void ChangeTitleMenu(void);
	void ChangeSetting(void);
	void ChangeExit(void);
	void ChangeGameStart(void);

	//はいいいえ更新
	void UpdateYesNo(void);

	//スクリーンの大きさを変更
	void ChangeScreenSize(void);
};
