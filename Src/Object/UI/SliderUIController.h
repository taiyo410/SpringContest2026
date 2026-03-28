#pragma once
#include "../Manager/Generic/InputManagerS.h"
#include "..//Character/Base/UIBase2D.h"

class Cursor;

class SliderUIController :
    public UIBase2D
{

public:
    /// @brief コンストラクタ
    /// @param  
    SliderUIController(const Cursor& _cursor,float& _per,Vector2F _leftTopPos,Vector2F _length);

    /// @brief デストラクタ
    /// @param  
    ~SliderUIController(void)override;

    /// @brief ロード
    /// @param  
    void Load(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void)override;

    /// @brief 更新
    /// @param  
    void Update(void)override;

    /// @brief 描画
    /// @param  
    void Draw(void)override;
    
    /// @brief 解放
    /// @param  
    void Release(void)override;

    /// @brief 当たった時の処理
    /// @param _partner 
    void OnHit(std::weak_ptr<Collider2D>_partner)override;

    /// @brief ボタン割合の取得
    /// @param  
    /// @return 
    const float GetPercent(void)const { return per_; }

private:
    const Cursor& cursor_;

    //入力
    InputManagerS& insS_;

    //割合
    float& per_;

    //ボタンの座標
    Vector2F circlePos_;

    //ボタンの半径
    float circleRadius_;

    //左上座標
    Vector2F leftTopPos_;

    //長さ
    Vector2F length_;

    //ボタンの色
    unsigned int buttonColor_;

    //右下座標の取得
    const Vector2F GetRightDownPos(void)const { return leftTopPos_ + length_; }

    /// @brief 割合が0~1にあるか
    /// @param  
    /// @return true:範囲外：false;範囲内
    bool IsOutOfRangePercent(void) { return per_ < 0.0f || per_ > 1.0f; }
};

