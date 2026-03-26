#pragma once
#include <DxLib.h>
#include "../../Manager/Resource/ResourceManager.h"
#include "../../Object/Character/Base/UIBase2D.h"
class ArrowController :
    public UIBase2D
{
public:

    /// @brief コンストラクタ
    /// @param _arrowSrc 矢印リソース
    /// @param _startPos 始点
    /// @param _endPos 終点
    /// @param _thick 太さ
    /// @param _col 色
    ArrowController(ResourceManager::SRC _arrowSrc,Vector2F& _startPos,Vector2F& _endPos,float _thick,float& _per,FLOAT4& _col);

    /// @brief デストラクタ
    /// @param  
    ~ArrowController(void);

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

    /// @brief ヒット処理
    /// @param _partner 
    void OnHit(const std::weak_ptr<Collider2D> _partner)override {}

private:

    //定数バッファ
    static constexpr int GAUGE_CONST_BUF = 3;
    //カラー1の定数バッファ番号
    static constexpr int COLER1_BUF_NUM = 0;
    //カラー2の定数バッファ番号
    static constexpr int COLER2_BUF_NUM = 1;
    //ゲージ割合の定数バッファ番号
    static constexpr int PERCENT_BUF_NUM = 2;

    //矢印に使用する素材
    ResourceManager::SRC arrowSrc_;

    //画像
    int arrowImg_;

    //始点
    Vector2F& startPos_;
    //終点
    Vector2F& endPos_;
    //太さ
    float thick_;
    //色
    FLOAT4 col_;
    //割合(ゲージを作る場合)
    float& per_;

    //アローゲージを作る
    void MakeArrowGauge(void);
};

