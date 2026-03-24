#pragma once
#include "./SceneBase.h"

class InputManager;
class InputManagerS;
class SoundManager;
class MenuController;
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

    //ボタン間の間隔
    static constexpr int BUTTON_OFFSET = 100;

    //ボタンX座標
    static constexpr Vector2 BUTTON_POS = { 200,200 };

    //音の大きさの最大値(描画用)
    static constexpr int VOL_MAX = 100.0f;

    //現在の状態
    SETTING_STATE state_;

    //メニューコントローラー
    std::unique_ptr<MenuController>menuController_;

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

    ////BGMの大きさ
    //int bgmVol_;
    ////SEの大きさ
    //int seVol_;
    ////テキスト速度
    //int textSpd_;
    //設定関連
    int volume_[static_cast<int>(VOLUME_TYPE::MAX)];

    /// @brief 状態遷移
    /// @param _state 遷移したい状態
    void ChangeSetting(const SETTING_STATE _state);

    //遷移系
    void ChangeSettingNormal(void);
    void ChangeBGM(void);
    void ChangeSE(void);
    void ChangeTextSpeed(void);
    void ChangeExitSetting(void);

    //更新系
    void UpdateSettingNormal(void);
    void UpdateBGM(void);
    void UpdateSE(void);
    void UpdateTextSpeed(void);
    void UpdateExitSetting(void);

    /// @brief 音量やBGMの調整
    /// @param _num 
    void SetNumber(int& _num);

    //スライダーの描画
    void DrawSliderUI(Vector2F _leftTopPos, Vector2F _length, float _value);

    /// @brief 文字列から音量をそれぞれ返す
    /// @param _str 調べたい文字列
    /// @return 
    const float GetVolumeFromString(const std::wstring _str)const;

    //ボリューム関連の反映
    void VolumeRefrect(void);
};


