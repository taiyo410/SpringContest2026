#pragma once
#include "../../Collider/ColliderBase.h"

class ObjectBase;

// 衝突結果の情報
struct CollisionInfo
{
    const ColliderBase* myCollider;      // 自分のコライダ
    const ColliderBase* hitCollider;     // 衝突相手のコライダ
    VECTOR hitPosition;                  // 衝突位置
    VECTOR hitNormal;                    // 衝突面の法線
    float penetration;                   // めり込み量
    bool isValid;                        // 衝突が有効か
};


// 全体の衝突を管理するコントローラー
class CollisionController
{
public:
    // シングルトンインスタンスの生成
    static void CreateInstance(void);

    // シングルトンインスタンスの取得
    static CollisionController& GetInstance(void);

    // シングルトンインスタンスの削除
    static void Destroy(void);

    // 初期化
    void Init(void);

    // 更新（全ての衝突判定を実行）
    void Update(void);

    // ユニットの登録
    void RegisterObject(ObjectBase* obj);

    // ユニットの登録解除
    void UnregisterObject(ObjectBase* obj);

    // 全ユニットのクリア
    void Clear(void);

private:
    // コンストラクタ
    CollisionController(void);

    // デストラクタ
    ~CollisionController(void);

    // コピー禁止
    CollisionController(const CollisionController&) = delete;
    CollisionController& operator=(const CollisionController&) = delete;

    // ムーブ禁止
    CollisionController(CollisionController&&) = delete;
    CollisionController& operator=(CollisionController&&) = delete;

    // シングルトンインスタンス
    static CollisionController* instance_;

    // 登録されたユニット
    std::vector<ObjectBase*> objects_;

    static constexpr float UPDATE_INTERVAL = 0.016f;

    float updateTimer_;

    void UpdateCollisionPairs(void);

    // 2D向けの衝突判定関数
    bool CheckCircleVsCircle(const ColliderBase* colA, const ColliderBase* colB, CollisionInfo& outInfo);

    bool IsCollisionEnabled(ColliderBase::TAG tagA, ColliderBase::TAG tagB);
};