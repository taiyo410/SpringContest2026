#pragma once
#include <unordered_map>
#include <memory>
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"

class FontController;
class MenuManager;
class Cursor;
class YesNoScene 
{
public:

    enum class YES_NO_STATE
    {
        YES,
        NO
    };

    /// @brief コンストラクタ
    /// @param _question 
    YesNoScene(Vector2F _centerPos,Vector2F _size);

    /// @brief デストラクタ
    /// @param  
    ~YesNoScene(void);

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
    void Draw(const std::wstring& _str);

    /// @brief YES,NOの状態取得
    /// @param  
    /// @return 
    const YES_NO_STATE GetState(void)const { return state_; }


private:

    static constexpr int FONT_SIZE = 40;

    int fontHandle_;
    std::unique_ptr<FontController>fontController_;

	//中心座標
    Vector2F centerPos_;
    //サイズ
    Vector2F size_;
    //質問
    std::wstring questionStr_;
    //メニューマネージャ
    std::unique_ptr<MenuManager>menuMng_;
    //状態
    YES_NO_STATE state_;
    //YESNO座標
    std::unordered_map<YES_NO_STATE, Vector2>yesNoPos_;
    std::unordered_map<YES_NO_STATE, std::wstring>yesNoStr_;
};

