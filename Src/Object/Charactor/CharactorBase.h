#pragma once
#include "../ObjectBase.h"

//@brief キャラクターの基底クラス
//@details キャラクターオブジェクトの共通機能を提供する抽象クラス
class CharactorBase : public ObjectBase
{
public:
    //@brief コンストラクタ
	CharactorBase(void);

    //@brief デストラクタ
    virtual ~CharactorBase(void);

    //@brief リソースのロード
    virtual void InitLoad(void) = 0;

    //@brief　初期化処理
    virtual void Init(void) override;

    //@brief 更新処理
    virtual void Update(void) override;

    // @brief 描画処理
    virtual void Draw(void) const override;

protected:
    int imageHandle_;    // キャラクターの基本画像 
    bool isLoaded_;      // ロード完了フラグ
    bool isActive_;      // オブジェクトが有効かどうか（生存フラグ）

    // 共通のアニメーション用変数など（必要に応じて）
    float animeTimer_;
    int animeFrame_;
};