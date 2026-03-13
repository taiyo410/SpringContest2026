#pragma once
#include "./SceneBase.h"

class InputManager;
class InputManagerS;
class MenuController;
class SettingScene :
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
    static constexpr int BUTTON_OFFSAET = 50;

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
};

