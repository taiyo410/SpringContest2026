#pragma once
#include "./SceneBase.h"

class InputManager;
class InputManagerS;
class SoundManager;
class MenuManager;
class Cursor;
class SliderUIManager;
class YesNoScene;
class DataBank;

class SettingScene:
    public SceneBase
{
public:

    enum class SETTING_BTN
    {
        BGM,
        SE,
        TEXT_SPD,
        SCREEN_SETTING,
        EXIT_SETTING,
        MAX
    };

    enum class VOLUME_TYPE
    {
        BGM,
        SE,
        TEXT_SPD,
        MAX
    };

    enum class SETTING_STATE
    {
        BGM,            //BGM
        SE,             //SE
        TEXT_SPD,       //テキスト速度
        SCREEN_SET,     //スクリーンの設定
        EXIT_SETTING,   //設定画面を抜ける
        NORMAL,         //メニュー選択
        MAX             //最大数
    };

    /// @brief コンストラクタ
    /// @param  
    SettingScene(void);

    /// @brief デストラクタ
    /// @param  
    ~SettingScene(void)override;

    /// @brief ロード
    /// @param  
    void Load(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void)override;

	/// @brief 更新
	/// @param  
	void NormalUpdate(void)override;

	/// @brief 描画
	/// @param  
	void NormalDraw(void)override;

private:

    //選択中ボタンのイージングで動かせる距離
    static constexpr Vector2 SELECT_EASE_DISTANCE = { 20,0 };

    //はいいいえのサイズ
	static constexpr Vector2F YES_NO_SIZE = { 600,200 };

    //選択中ボタンのイージング時間
    static constexpr float SELECT_EASE_TIME = 0.5f;

    //ボタン間の間隔
    static constexpr int BUTTON_OFFSET = 100;

    //ボタンX座標
    static constexpr Vector2 BUTTON_POS = { 200,200 };

    //音の大きさの最大値(描画用)
    static constexpr int VOL_MAX = 100.0f;

    //現在の状態
    SETTING_STATE state_;

    //メニューコントローラー
    std::unique_ptr<MenuManager>menuMng_;

    //カーソル
    std::unique_ptr<Cursor>cursor_;

    //スクリーンやゲーム終了のはいいいえ
    std::unique_ptr<YesNoScene>yesNoScn_;

    //スライダー
    std::unique_ptr<SliderUIManager>sliderUIMng_;

    //文字列格納
    std::unordered_map<SETTING_BTN, std::wstring>buttonStrTable_;

    //状態遷移系のテーブル
    std::unordered_map<SETTING_STATE, std::function<void(void)>>changeSetting_;

    //更新
    std::function<void(void)>updateSetting_;

    //サウンド
    SoundManager& soundMng_;

    //データバンク
    DataBank& dataBank_;

    //設定関連
    //int volume_[static_cast<int>(VOLUME_TYPE::MAX)];
    int volume_[static_cast<int>(VOLUME_TYPE::MAX)];
    float volumePer_[static_cast<int>(VOLUME_TYPE::MAX)];

	//質問の文字列
    std::wstring questionStr_;

    /// @brief 状態遷移
    /// @param _state 遷移したい状態
    void ChangeSetting(const SETTING_STATE _state);

    //遷移系
    void ChangeSettingNormal(void);
    void ChangeBGM(void);
    void ChangeSE(void);
    void ChangeTextSpeed(void);
    void ChangeExitSetting(void);
    void ChangeScreenSetting(void);

    //更新系
    void UpdateSettingNormal(void);
    void UpdateBGM(void);
    void UpdateSE(void);
    void UpdateTextSpeed(void);
    void UpdateExitSetting(void);
    void UpdateScreenSetting(void);

    /// @brief 音量やBGMの調整
    /// @param _num 
    void SetNumber(int& _num);

    /// @brief スライダーの描画
    /// @param _leftTopPos 左上
    /// @param _length 長さ
    /// @param _value 割合(0~1)
    void DrawSliderUI(Vector2F _leftTopPos, Vector2F _length, float _value);

    /// @brief 文字列から音量をそれぞれ返す
    /// @param _str 調べたい文字列
    /// @return 
    const VOLUME_TYPE GetVolumeFromString(const std::wstring _str)const;

    //ボリューム関連の反映
    void VolumeRefrect(void);

	//質問シーンを出すかどうか
    const bool IsQuestionScene(void)const;
};


